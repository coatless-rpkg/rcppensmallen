#include <RcppEnsmallen.h>

// Helpers are given internal linkage so that this file and its Adam
// counterpart each carry their own copy and can be read on their own.
namespace {

// Numerically stable softplus, i.e. log(1 + exp(eta)), computed elementwise.
//
// Writing log(1 + exp(eta)) directly overflows to Inf once eta is larger than
// about 710 in double precision. The identity
//
//     log(1 + exp(eta)) = max(eta, 0) + log1p(exp(-|eta|))
//
// keeps the exponential argument non-positive, so it can never overflow.
inline arma::vec softplus(const arma::vec& eta)
{
    return arma::max(eta, arma::zeros<arma::vec>(eta.n_elem)) +
        arma::log1p(arma::exp(-arma::abs(eta)));
}

// Logistic function, i.e. 1 / (1 + exp(-eta)), computed elementwise.
//
// Unlike the softplus above, this form needs no special handling: a very
// negative eta sends exp(-eta) to Inf and the result to 0, while a very
// positive eta sends exp(-eta) to 0 and the result to 1. Both are correct.
inline arma::vec logistic(const arma::vec& eta)
{
    return 1.0 / (1.0 + arma::exp(-eta));
}

} // namespace

// Define a differentiable objective function that implements Evaluate(),
// Gradient(), and the fused EvaluateWithGradient(). Supplying all three is
// allowed; ensmallen calls the fused form where doing so saves work.
class LogisticRegressionFunction
{
public:
    // Construct the object with the given design matrix, responses, and
    // ridge penalty.
    LogisticRegressionFunction(const arma::mat& X,
                               const arma::vec& y,
                               const double lambda) :
    X(X), y(y), lambda(lambda) { }

    // Return the objective function for model parameters beta.
    double Evaluate(const arma::mat& beta)
    {
        const arma::vec eta = X * beta;
        return arma::accu(softplus(eta) - y % eta) +
            0.5 * lambda * arma::accu(arma::square(beta));
    }

    // Compute the gradient for model parameters beta.
    void Gradient(const arma::mat& beta, arma::mat& g)
    {
        const arma::vec eta = X * beta;
        g = X.t() * (logistic(eta) - y) + lambda * beta;
    }

    // Compute both at once, reusing the single pass over eta.
    double EvaluateWithGradient(const arma::mat& beta, arma::mat& g)
    {
        const arma::vec eta = X * beta;
        g = X.t() * (logistic(eta) - y) + lambda * beta;
        return arma::accu(softplus(eta) - y % eta) +
            0.5 * lambda * arma::accu(arma::square(beta));
    }

private:
    // The design matrix.
    const arma::mat& X;
    // The responses to each data point.
    const arma::vec& y;
    // The ridge penalty applied to every coefficient.
    const double lambda;
};

//' Logistic Regression with L-BFGS
//'
//' Solves logistic regression by minimizing the penalized negative
//' log-likelihood with the L-BFGS optimizer.
//'
//' @param X      A `matrix` that is the Design Matrix for the regression
//'               problem. Include a column of ones for an intercept.
//' @param y      A `vec` of binary responses taking values 0 or 1.
//' @param lambda A `double` giving the ridge penalty. The default of 0
//'               yields the maximum likelihood estimate.
//'
//' @return
//' The estimated \eqn{\beta}{beta} parameter values for the logistic regression.
//' @details
//' Writing \eqn{\eta = \mathbf{X} \beta}{eta = X * beta}, the objective is the
//' penalized negative log-likelihood:
//' \deqn{f(\beta) = \sum_i \left[ \log \left(1 + e^{\eta_i} \right) - y_i \eta_i \right] + \frac{\lambda}{2} \lVert \beta \rVert_2^2}{f(beta) = sum_i [log(1 + exp(eta_i)) - y_i * eta_i] + (lambda / 2) * ||beta||^2}
//' with gradient
//' \deqn{\nabla f(\beta) = \mathbf{X}^{T} \left( \sigma \left( \eta \right) - \mathbf{y} \right) + \lambda \beta}{grad f(beta) = X^T (sigma(eta) - y) + lambda * beta}
//' where \eqn{\sigma}{sigma} is the logistic function.
//'
//' The penalty is applied to every coefficient, including the intercept. This
//' keeps the example self-contained, since the function cannot know which
//' column of `X` holds the intercept. Production code normally leaves the
//' intercept unpenalized.
//'
//' Because the objective is convex, optimization starts from a vector of zeros
//' rather than a random point, which makes the result reproducible.
//' @export
//' @examples
//' # Number of Points
//' n = 1000
//'
//' # Select beta parameters
//' beta = c(-1, 0.7, -0.5, 1.2)
//'
//' # Number of Predictors (including intercept)
//' p = length(beta)
//'
//' # Generate predictors from a normal distribution
//' X_i = matrix(rnorm(n * (p - 1)), ncol = p - 1)
//'
//' # Add an intercept
//' X = cbind(1, X_i)
//'
//' # Generate binary responses
//' y = rbinom(n, size = 1, prob = 1 / (1 + exp(-X %*% beta)))
//'
//' # Run optimization with L-BFGS
//' theta_hat = logistic_reg_lbfgs(X, y)
//'
//' # Compare against glm()
//' cbind(actual = beta,
//'       ensmallen = theta_hat,
//'       glm = glm.fit(X, y, family = binomial())$coefficients)
// [[Rcpp::export]]
arma::vec logistic_reg_lbfgs(const arma::mat& X, const arma::vec& y,
                             const double lambda = 0.0) {

    if (X.n_rows != y.n_elem) {
        Rcpp::stop("`X` must have as many rows as `y` has elements.");
    }
    if (X.n_rows == 0 || X.n_cols == 0) {
        Rcpp::stop("`X` must have at least one row and one column.");
    }
    // Without this check a single NA/NaN/Inf makes the objective non-finite,
    // the optimizer makes no progress, and the untouched starting point is
    // returned as if it were a fitted model.
    if (!X.is_finite()) {
        Rcpp::stop("`X` must not contain missing or infinite values.");
    }
    if (arma::any(y != 0 && y != 1)) {
        Rcpp::stop("`y` must contain only 0 and 1.");
    }
    if (lambda < 0.0) {
        Rcpp::stop("`lambda` must be non-negative.");
    }

    // Construct the objective function.
    LogisticRegressionFunction lrf(X, y, lambda);

    // Create the L-BFGS optimizer with default parameters. The ens::L_BFGS
    // type can be replaced with any ensmallen optimizer that can handle
    // differentiable functions.
    ens::L_BFGS lbfgs;

    // The objective is convex, so a deterministic starting point of zeros
    // reaches the same optimum a random start would.
    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    // Run the optimization.
    lbfgs.Optimize(lrf, beta);

    return beta;
}
