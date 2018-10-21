#include <RcppEnsmallen.h>

// Define a differentiable objective function by implementing both Evaluate()
// and Gradient() separately.
class LinearRegressionFunction
{
public:
    // Construct the object with the given the design matrix and responses.
    LinearRegressionFunction(const arma::mat& X,
                             const arma::vec& y) :
    X(X), y(y) { }

    // Return the objective function for model parameters beta.
    double Evaluate(const arma::mat& beta)
    {
        return std::pow(arma::norm(y - X * beta), 2.0);
    }

    // Compute the gradient for model parameters beta
    void Gradient(const arma::mat& beta, arma::mat& g)
    {
        g = -2 * X.t() * (y - X * beta);
    }

private:
    // The design matrix.
    const arma::mat& X;
    // The responses to each data point.
    const arma::vec& y;
};

//' Linear Regression with L-BFGS
//'
//' Solves the Linear Regression's Residual Sum of Squares using the L-BFGS
//' optimizer.
//'
//' @param X A `matrix` that is the Design Matrix for the regression problem.
//' @param y A `vec` containing the response values.
//'
//' @return
//' The estimated \eqn{\beta}{beta} parameter values for the linear regression.
//' @details
//' Consider the **Residual Sum of Squares**, also known as **RSS**, defined as:
//' \deqn{RSS\left( \beta \right) = \left( { \mathbf{y} - \mathbf{X} \beta } \right)^{T} \left( \mathbf{y} - \mathbf{X} \beta \right)}{RSS(beta) = (y - X*beta)^T * (y - X*beta)}
//' The objective function is defined as:
//' \deqn{f(\beta) = (y - X\beta)^2}{f(beta) = (y - X * beta)^2}
//' The gradient is defined as:
//' \deqn{\frac{\partial RSS}{\partial \beta} = -2 \mathbf{X}^{T} \left(\mathbf{y} - \mathbf{X} \beta \right)}{dRSS/dbeta = -2 X^T (y - X * beta)}
//' @export
//' @examples
//' # Number of Points
//' n = 1000
//'
//' # Select beta parameters
//' beta = c(-2, 1.5, 3, 8.2, 6.6)
//'
//' # Number of Predictors (including intercept)
//' p = length(beta)
//'
//' # Generate predictors from a normal distribution
//' X_i = matrix(rnorm(n), ncol = p - 1)
//'
//' # Add an intercept
//' X = cbind(1, X_i)
//'
//' # Generate y values
//' y = X%*%beta + rnorm(n / (p - 1))
//'
//' # Run optimization with lbfgs
//' theta_hat = lin_reg_lbfgs(X, y)
//'
//' # Verify parameters were recovered
//' cbind(actual = beta, estimated = theta_hat)
// [[Rcpp::export]]
arma::mat lin_reg_lbfgs(const arma::mat& X, const arma::vec& y) {

    // Construct the first objective function.
    LinearRegressionFunction lrf(X, y);

    // Create the L_BFGS optimizer with default parameters.
    // The ens::L_BFGS type can be replaced with any ensmallen optimizer that can
    // handle differentiable functions.
    ens::L_BFGS lbfgs;

    lbfgs.MaxIterations() = 10;

    // Create a starting point for our optimization randomly.
    // The model has p parameters, so the shape is p x 1.
    arma::mat beta(X.n_cols, 1, arma::fill::randn);

    // Time the routine
    // arma::wall_clock clock;
    // clock.tic();

    // Run the optimization
    lbfgs.Optimize(lrf, beta);

    // End time output
    //Rcpp::Rcout << clock.toc() << std::endl;

    return beta;
}
