#include <RcppEnsmallen.h>

// The helpers and the objective are given internal linkage so that this file
// and its L-BFGS counterpart each carry their own copy and can be read on
// their own. The objective below repeats the one in
// example-logistic-regression-lbfgs.cpp verbatim, so internal linkage is also
// what keeps the two definitions from colliding at link time.
namespace {

// Numerically stable softplus, i.e. log(1 + exp(eta)), computed elementwise.
// See example-logistic-regression-lbfgs.cpp for why the naive form overflows.
inline arma::vec softplus(const arma::vec& eta)
{
    return arma::max(eta, arma::zeros<arma::vec>(eta.n_elem)) +
        arma::log1p(arma::exp(-arma::abs(eta)));
}

// Logistic function, i.e. 1 / (1 + exp(-eta)), computed elementwise.
inline arma::vec logistic(const arma::vec& eta)
{
    return 1.0 / (1.0 + arma::exp(-eta));
}

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

} // namespace

// A callback that records every point the optimizer visits.
//
// An ensmallen callback is an ordinary class that implements only the hooks it
// has a use for. ens::Callback inspects the class at compile time and skips
// the hooks it does not find, so there is no base class to derive from and no
// hook that has to be stubbed out. The three used here are declared in
// inst/include/ensmallen_bits/callbacks/traits.hpp as
//
//     void BeginOptimization(OptimizerType&, FunctionType&, MatType&)
//     void StepTaken(OptimizerType&, FunctionType&, const MatType&)
//     void EndOptimization(OptimizerType&, FunctionType&, MatType&)
//
// StepTaken() receives its coordinates by const reference while the other two
// do not, and the difference has to be reproduced here. Adding const to the
// coordinates argument of BeginOptimization() or EndOptimization() stops
// ensmallen from recognising the hook: the class still compiles, no warning is
// issued, and the hook is never called. A signature that drifts from the
// header shows up as missing output rather than as a compiler error.
//
// Optimize() takes its callbacks by forwarding reference and ens::Callback
// then hands them on by reference, but the recorder still stores a reference
// to a matrix the caller owns rather than a matrix of its own. That way the
// recorded path is reachable after Optimize() returns without depending on
// how the callback pack is passed around.
class PathRecorder
{
public:
    // Construct the recorder over a caller-owned matrix. The matrix is
    // expected to have as many rows as there are coefficients and no columns
    // yet; each recorded point is appended as one more column.
    explicit PathRecorder(arma::mat& path) : path(path) { }

    // Record the starting point, before any step has been taken.
    template<typename OptimizerType, typename FunctionType, typename MatType>
    void BeginOptimization(OptimizerType& /* optimizer */,
                           FunctionType& /* function */,
                           MatType& coordinates)
    {
        Record(coordinates);
    }

    // Record the coordinates after each accepted step.
    template<typename OptimizerType, typename FunctionType, typename MatType>
    void StepTaken(OptimizerType& /* optimizer */,
                   FunctionType& /* function */,
                   const MatType& coordinates)
    {
        Record(coordinates);
    }

    // Record the point the optimizer stopped at.
    //
    // ens::L_BFGS calls StepTaken() at the bottom of its iteration, after the
    // convergence tests that break out of the loop. A run that stops because
    // the objective has gone stable therefore ends one accepted step past its
    // last StepTaken() call, and without this hook the final column of the
    // path would not be the answer Optimize() hands back. When the loop
    // instead exits at the top of an iteration, or because a line search
    // failed, the coordinates are unchanged and Record() drops them.
    template<typename OptimizerType, typename FunctionType, typename MatType>
    void EndOptimization(OptimizerType& /* optimizer */,
                         FunctionType& /* function */,
                         MatType& coordinates)
    {
        Record(coordinates);
    }

private:
    // Append the coordinates as one more column, unless they repeat the
    // column already at the end.
    //
    // The comparison is exact rather than tolerance based on purpose. The
    // only duplicate it has to catch is the one EndOptimization() can produce,
    // and in that case no arithmetic has touched the coordinates since the
    // preceding hook fired, so the two columns agree bit for bit.
    void Record(const arma::mat& coordinates)
    {
        const arma::vec point = arma::vectorise(coordinates);

        if (path.n_cols > 0 &&
            arma::all(path.col(path.n_cols - 1) == point))
        {
            return;
        }

        path.insert_cols(path.n_cols, point);
    }

    // The accumulated path, owned by the caller.
    arma::mat& path;
};

//' Logistic Regression with L-BFGS, Recording the Optimizer's Path
//'
//' Solves logistic regression as [logistic_reg_lbfgs()] does, and returns
//' every point the optimizer visited on the way rather than the fit alone.
//'
//' @param X      A `matrix` that is the Design Matrix for the regression
//'               problem. Include a column of ones for an intercept.
//' @param y      A `vec` of binary responses taking values 0 or 1.
//' @param lambda A `double` giving the ridge penalty. The default of 0
//'               yields the maximum likelihood estimate.
//'
//' @return
//' A `matrix` with one row per coefficient and one column per recorded point.
//' The first column is the starting point, a vector of zeros, and each column
//' after it holds the coordinates following one accepted step. The last column
//' is the fit, and matches what [logistic_reg_lbfgs()] returns for the same
//' arguments to within the optimizer's tolerance.
//'
//' Coefficients run down the rows and iterations run across the columns, so
//' two coordinates can be traced against each other with
//' `plot(path[2, ], path[4, ])`.
//' @details
//' The objective, its gradient, the starting point and the optimizer settings
//' are those of [logistic_reg_lbfgs()], which documents them. The only
//' addition is an `ens::L_BFGS` callback that copies the coordinates into a
//' matrix each time the optimizer moves.
//'
//' A callback in 'ensmallen' is a plain class that defines whichever of the
//' documented hooks it needs; the library detects them at compile time. This
//' one uses `BeginOptimization()` for the starting point, `StepTaken()` for
//' each accepted step, and `EndOptimization()` for the point the run finished
//' on. Callbacks are passed as trailing arguments to `Optimize()`.
//'
//' The column count is not fixed. L-BFGS stops once the gradient norm falls
//' below its tolerance or the objective stops improving, so the number of
//' steps depends on the data. A point that repeats the previous one is not
//' recorded twice.
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
//' # Record the path L-BFGS takes
//' path = logistic_reg_lbfgs_path(X, y)
//'
//' # One row per coefficient, one column per recorded point
//' dim(path)
//'
//' # The last column is the fit
//' cbind(actual = beta,
//'       path = path[, ncol(path)],
//'       lbfgs = drop(logistic_reg_lbfgs(X, y)))
//'
//' # Trace two coefficients against each other
//' plot(path[2, ], path[4, ], type = "b", pch = 16,
//'      xlab = "beta 2", ylab = "beta 4",
//'      main = "L-BFGS path through two coordinates")
//'
//' # Mark where the run started and where it ended
//' points(path[2, 1], path[4, 1], col = "blue", pch = 16, cex = 1.5)
//' points(path[2, ncol(path)], path[4, ncol(path)],
//'        col = "red", pch = 16, cex = 1.5)
// [[Rcpp::export]]
arma::mat logistic_reg_lbfgs_path(const arma::mat& X, const arma::vec& y,
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

    // Create the L-BFGS optimizer with default parameters, the same ones
    // logistic_reg_lbfgs() uses, so the two functions follow the same route.
    ens::L_BFGS lbfgs;

    // The objective is convex, so a deterministic starting point of zeros
    // reaches the same optimum a random start would.
    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    // The path starts with the right number of rows and no columns, which is
    // what lets the recorder append with insert_cols(). Sizing it here also
    // means an optimizer that never calls a hook leaves behind an empty
    // matrix of the right shape rather than a shapeless one.
    arma::mat path(X.n_cols, 0);

    // Callbacks are passed as trailing arguments to Optimize(). The recorder
    // writes into `path`, which outlives the callback object.
    lbfgs.Optimize(lrf, beta, PathRecorder(path));

    return path;
}
