#include <RcppEnsmallen.h>

// Define an arbitrary objective function by implementing Evaluate() and
// nothing else.
//
// There is deliberately no Gradient() here. The sum of absolute residuals is
// piecewise linear, and it has no derivative wherever a residual is exactly
// zero, which is where its minimum sits. A class shaped this way can only be
// handed to an optimizer that never asks for a derivative, and ensmallen
// checks that pairing at compile time rather than at run time.
class LADFunction
{
public:
    // Construct the object with the given design matrix and responses.
    LADFunction(const arma::mat& X,
                const arma::vec& y) :
    X(X), y(y) { }

    // Return the objective function for model parameters beta.
    double Evaluate(const arma::mat& beta)
    {
        return arma::accu(arma::abs(y - X * beta));
    }

private:
    // The design matrix.
    const arma::mat& X;
    // The responses to each data point.
    const arma::vec& y;
};

//' Least Absolute Deviations Regression with Particle Swarm Optimization
//'
//' Solves least absolute deviations regression by minimizing the sum of
//' absolute residuals with the local best particle swarm optimizer.
//'
//' @param X A `matrix` that is the Design Matrix for the regression problem.
//'          Include a column of ones for an intercept.
//' @param y A `vec` containing the response values.
//'
//' @return
//' The estimated \eqn{\beta}{beta} parameter values for the least absolute
//' deviations fit.
//' @details
//' The objective is the sum of absolute residuals:
//' \deqn{f(\beta) = \sum_i \left\lvert y_i - \mathbf{x}_i^{T} \beta \right\rvert}{f(beta) = sum_i |y_i - x_i^T beta|}
//' Compared with the residual sum of squares, the absolute value leaves large
//' residuals with a constant rather than a growing influence, so the fit
//' tolerates heavy tailed errors and outlying responses.
//'
//' The price is that the objective has a kink at every \eqn{\beta}{beta} that
//' makes a residual zero, and for a design with \eqn{p}{p} columns a solution
//' sets \eqn{p}{p} of the residuals to zero. The minimum therefore sits on the
//' kinks, where no gradient exists.
//'
//' `LADFunction` in the source of this example supplies
//'
//' ```
//' double Evaluate(const arma::mat& beta);
//' ```
//'
//' and no `Gradient()`. That is ensmallen's arbitrary function contract.
//' Optimizers that need a derivative reject a class shaped this way at compile
//' time, so fewer optimizers accept it than accept the differentiable
//' objectives behind [lin_reg_lbfgs()] and [logistic_reg_lbfgs()].
//'
//' Particle swarm optimization scatters candidate parameter vectors across the
//' search space and moves each one toward the best point it has seen and the
//' best point its neighbors have seen. Only the shape of the starting matrix
//' reaches the optimizer, since the swarm is placed at random. Those draws
//' come from R's random number generator, so `set.seed()` makes a run
//' reproducible. Unseeded runs land at slightly different points, and the
//' spread widens as `X` gains columns.
//' @export
//' @examples
//' # Number of Points
//' n = 400
//'
//' # Select beta parameters
//' beta = c(2, -1.5, 0.8)
//'
//' # Number of Predictors (including intercept)
//' p = length(beta)
//'
//' # The swarm is initialized at random, so fix the seed
//' set.seed(512)
//'
//' # Generate predictors from a normal distribution
//' X_i = matrix(rnorm(n * (p - 1)), ncol = p - 1)
//'
//' # Add an intercept
//' X = cbind(1, X_i)
//'
//' # Generate responses with heavy tailed errors
//' y = X %*% beta + rt(n, df = 2)
//'
//' # Run optimization with particle swarm optimization
//' theta_hat = lad_reg_pso(X, y)
//'
//' # Compare against a derivative free search in base R
//' nm = optim(rep(0, p), function(b) sum(abs(y - X %*% b)),
//'            method = "Nelder-Mead",
//'            control = list(maxit = 20000, reltol = 1e-12))
//'
//' cbind(actual = beta, ensmallen = theta_hat, optim = nm$par)
//'
//' # Compare the objective each one reached
//' obj = function(b) sum(abs(y - X %*% b))
//' c(ensmallen = obj(theta_hat), optim = obj(nm$par))
// [[Rcpp::export]]
arma::vec lad_reg_pso(const arma::mat& X, const arma::vec& y) {

    if (X.n_rows != y.n_elem) {
        Rcpp::stop("`X` must have as many rows as `y` has elements.");
    }
    if (X.n_rows == 0 || X.n_cols == 0) {
        Rcpp::stop("`X` must have at least one row and one column.");
    }
    // Without this check a single NA/NaN/Inf makes the objective non-finite
    // for every particle, so no particle looks better than any other and the
    // swarm has nothing to move toward.
    if (!X.is_finite()) {
        Rcpp::stop("`X` must not contain missing or infinite values.");
    }
    // The response is continuous here, so the check is finiteness rather than
    // membership in the two levels a logistic fit requires.
    if (!y.is_finite()) {
        Rcpp::stop("`y` must not contain missing or infinite values.");
    }

    // Construct the objective function.
    LADFunction ladf(X, y);

    // Create the local best particle swarm optimizer with default parameters.
    // The ens::LBestPSO type can be replaced with any ensmallen optimizer that
    // can handle arbitrary functions, meaning ones that supply Evaluate()
    // alone.
    ens::LBestPSO pso;

    // Create a starting point for our optimization. The swarm is placed at
    // random, so only the shape of this matrix is read. The model has p
    // parameters, so the shape is p x 1.
    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    // Run the optimization. The swarm draws through R's random number
    // generator, so set.seed() on the R side pins the result.
    pso.Optimize(ladf, beta);

    return beta;
}
