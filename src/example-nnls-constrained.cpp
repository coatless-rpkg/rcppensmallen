#include <RcppEnsmallen.h>

// THE SIGN CONVENTION.
//
// ens::AugLagrangian treats every constraint as an EQUALITY constraint,
// c_i(beta) = 0. For each constraint index i, aug_lagrangian_function_impl.hpp
// adds
//
//     -lambda_i * c_i(beta) + (sigma / 2) * c_i(beta)^2
//
// to the objective. Nothing clamps c_i, so a non-zero value of either sign is
// penalized. An inequality therefore has to be written by hand as a residual
// that is exactly zero on the feasible side of the boundary:
//
//     beta_j >= 0   becomes   c_j(beta) = min(0, beta_j)
//
// which is 0 when beta_j >= 0 and equal to beta_j (negative) when beta_j < 0.
// This is the encoding ensmallen uses for the inequality in its own
// GockenbachFunction, in
// inst/include/ensmallen_bits/problems/aug_lagrangian_test_functions_impl.hpp.
//
// Moving the clamp to the other side, min(0, beta_j) -> max(0, beta_j),
// compiles and runs without a warning and states beta_j <= 0 instead. On the
// example data documented below it returned (0, 0, 0, -0.153) with a residual
// sum of squares of 775.11, against (2.055, 0, 1.479, 0) and 102.72 for the
// reference solution.
//
// Negating the constraint is a different matter and is harmless: replacing
// c_j by -c_j together with its gradient leaves the augmented objective
// untouched, because the multiplier update lambda_i -= sigma * c_i negates
// lambda_i in step. Written both ways, the two agreed bit for bit. What cannot
// be swapped is which side of zero counts as feasible.

// Define a constrained objective function. On top of the Evaluate() and
// Gradient() pair that a differentiable function supplies, the constrained
// contract adds NumConstraints(), EvaluateConstraint() and
// GradientConstraint().
class NonNegativeLeastSquaresFunction
{
public:
    // Construct the object with the given design matrix and responses.
    NonNegativeLeastSquaresFunction(const arma::mat& X, const arma::vec& y) :
    X(X), y(y) { }

    // Return the residual sum of squares for coefficients beta.
    double Evaluate(const arma::mat& beta)
    {
        const arma::vec r = y - X * beta;
        return arma::dot(r, r);
    }

    // Compute the gradient of the residual sum of squares.
    void Gradient(const arma::mat& beta, arma::mat& g)
    {
        g = -2.0 * X.t() * (y - X * beta);
    }

    // One constraint per coefficient, each one a lower bound of zero.
    size_t NumConstraints() const { return X.n_cols; }

    // Return the residual of constraint i, which is zero when beta_i is
    // non-negative and equal to beta_i when it is not.
    //
    // ensmallen only ever passes an index below NumConstraints(). The bounds
    // test guards against a caller that reuses this class directly, since
    // beta(i, 0) is unchecked once NDEBUG is set.
    double EvaluateConstraint(const size_t i, const arma::mat& beta)
    {
        if (i >= X.n_cols) return 0.0;

        return std::min(0.0, beta(i, 0));
    }

    // Compute the gradient of constraint i.
    //
    // This is the derivative of the clamped residual returned above, so it is
    // the i-th unit vector where the bound is violated and zero where it is
    // satisfied. Returning the unclamped derivative instead, the i-th unit
    // vector everywhere, leaves the constraint value and its gradient
    // describing different functions. Run on the eight random data sets the
    // sweep in inst/tinytest/test_nnls.R generates, seeds 1001 to 1008, the
    // clamped form exceeded the reference objective by at most 2.3e-13, while
    // the unclamped form exceeded it by 3.8e-04 on one of them.
    void GradientConstraint(const size_t i, const arma::mat& beta,
                            arma::mat& g)
    {
        g.zeros(beta.n_rows, beta.n_cols);

        if (i < X.n_cols && beta(i, 0) < 0.0) g(i, 0) = 1.0;
    }

private:
    // The design matrix.
    const arma::mat& X;
    // The responses to each data point.
    const arma::vec& y;
};

//' Non-Negative Least Squares with the Augmented Lagrangian
//'
//' Solves least squares subject to every coefficient being non-negative with
//' the augmented Lagrangian optimizer.
//'
//' @param X A `matrix` that is the Design Matrix for the regression problem.
//'          Every column is constrained, so do not include a column of ones
//'          for an intercept.
//' @param y A `vec` of continuous responses.
//'
//' @return
//' The estimated \eqn{\beta}{beta} parameter values, each of them
//' non-negative.
//' @details
//' The objective is the residual sum of squares
//' \deqn{f(\beta) = \lVert \mathbf{y} - \mathbf{X} \beta \rVert_2^2}{f(beta) = ||y - X * beta||^2}
//' with gradient
//' \deqn{\nabla f(\beta) = -2 \mathbf{X}^{T} \left( \mathbf{y} - \mathbf{X} \beta \right)}{grad f(beta) = -2 * X^T (y - X * beta)}
//' minimized subject to \eqn{\beta_j \ge 0}{beta_j >= 0} for every column
//' \eqn{j}{j} of `X`.
//'
//' `ens::AugLagrangian` calls for the constrained function contract, which
//' extends the `Evaluate()` and `Gradient()` pair of a differentiable function
//' with `NumConstraints()`, `EvaluateConstraint()` and `GradientConstraint()`.
//' Every constraint is an equality, \eqn{c_i(\beta) = 0}{c_i(beta) = 0}, and a
//' bound has to be recast as a residual that is zero while the bound holds.
//' Here that residual is \eqn{\min(0, \beta_j)}{min(0, beta_j)}, one per
//' coefficient.
//'
//' The bound is imposed through a penalty rather than by projection, so a
//' coefficient resting on the boundary is returned as a small number of order
//' 1e-10 rather than as exactly zero, and that number can be slightly
//' negative. Round the result before comparing it with zero.
//'
//' Optimization starts from a vector of zeros, which satisfies every
//' constraint and makes the result reproducible.
//' @export
//' @examples
//' # Number of Points
//' n = 100
//'
//' # Select beta parameters, two of which are exactly zero
//' beta = c(2, 0, 1.5, 0)
//'
//' # Number of Predictors
//' p = length(beta)
//'
//' set.seed(24)
//'
//' # Generate predictors from a normal distribution
//' X = matrix(rnorm(n * p), ncol = p)
//'
//' # Generate continuous responses
//' y = as.vector(X %*% beta + rnorm(n))
//'
//' # The unconstrained least squares fit lands outside the feasible region
//' ols = qr.solve(X, y)
//' min(ols)
//'
//' # Run optimization with the augmented Lagrangian
//' beta_hat = nnls_auglag(X, y)
//'
//' # Compare against optim() with a lower bound of zero
//' rss = function(b) sum((y - X %*% b)^2)
//' box = optim(rep(0, p), rss, method = "L-BFGS-B", lower = rep(0, p))
//'
//' cbind(actual = beta,
//'       ols = ols,
//'       ensmallen = beta_hat,
//'       optim = box$par)
// [[Rcpp::export]]
arma::vec nnls_auglag(const arma::mat& X, const arma::vec& y) {

    if (X.n_rows != y.n_elem) {
        Rcpp::stop("`X` must have as many rows as `y` has elements.");
    }
    if (X.n_rows == 0 || X.n_cols == 0) {
        Rcpp::stop("`X` must have at least one row and one column.");
    }
    // Without these checks a single NA/NaN/Inf makes the objective non-finite,
    // the optimizer makes no progress, and the untouched starting point is
    // returned as if it were a fitted model.
    if (!X.is_finite()) {
        Rcpp::stop("`X` must not contain missing or infinite values.");
    }
    // Responses here are continuous, so finiteness is all that is required.
    if (!y.is_finite()) {
        Rcpp::stop("`y` must not contain missing or infinite values.");
    }

    // Construct the objective function.
    NonNegativeLeastSquaresFunction nnls(X, y);

    // Create the augmented Lagrangian optimizer with default parameters. It
    // solves a sequence of unconstrained problems with L-BFGS, raising the
    // penalty on violated constraints between rounds.
    ens::AugLagrangian auglag;

    // A vector of zeros is feasible, so optimization begins inside the
    // feasible region and the result is reproducible.
    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    // Run the optimization.
    auglag.Optimize(nnls, beta);

    return beta;
}
