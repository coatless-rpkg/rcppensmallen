// Logistic regression with ensmallen, as a single standalone file.
//
// This is the complete version of the code developed step by step in the
// "Solving Logistic Regression using Numeric Optimization" vignette. Compile
// and load it from R with:
//
//     Rcpp::sourceCpp("logistic-regression.cpp")
//
// The same code is included in this package, split across
// src/example-logistic-regression-lbfgs.cpp,
// src/example-logistic-regression-adam.cpp and
// src/example-logistic-regression-path.cpp, as logistic_reg_lbfgs(),
// logistic_reg_adam() and logistic_reg_lbfgs_path(). The package test suite
// compiles this file and checks that both routes agree.

#include <RcppEnsmallen.h>
// [[Rcpp::depends(RcppEnsmallen)]]

// ---------------------------------------------------------------------------
// Numerically stable helpers
// ---------------------------------------------------------------------------

// Softplus, i.e. log(1 + exp(eta)), computed elementwise.
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

// ---------------------------------------------------------------------------
// Full-batch objective, for L-BFGS
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// Separable objective, for mini-batch optimizers such as Adam
// ---------------------------------------------------------------------------

// Note this class stores copies of X and y rather than const references,
// because Shuffle() reorders the data in place.
class LogisticRegressionSeparableFunction
{
public:
    // Construct the object, copying the design matrix and responses.
    LogisticRegressionSeparableFunction(const arma::mat& X,
                                        const arma::vec& y) :
    X(X), y(y) { }

    // Return the number of separable functions, one per observation.
    size_t NumFunctions() const { return X.n_rows; }

    // Shuffle the order of the observations.
    void Shuffle()
    {
        const arma::uvec ordering =
            arma::shuffle(arma::regspace<arma::uvec>(0, X.n_rows - 1));
        X = X.rows(ordering);
        y = y.elem(ordering);
    }

    // Return the objective summed over the batch [begin, begin + batchSize).
    // Note this is a sum, not a mean; ensmallen expects the former.
    double Evaluate(const arma::mat& beta,
                    const size_t begin,
                    const size_t batchSize)
    {
        const size_t last = begin + batchSize - 1;
        const arma::vec eta = X.rows(begin, last) * beta;
        const arma::vec yBatch = y.subvec(begin, last);

        return arma::accu(softplus(eta) - yBatch % eta);
    }

    // Compute the gradient over the batch [begin, begin + batchSize).
    void Gradient(const arma::mat& beta,
                  const size_t begin,
                  arma::mat& g,
                  const size_t batchSize)
    {
        const size_t last = begin + batchSize - 1;
        const arma::mat XBatch = X.rows(begin, last);
        const arma::vec yBatch = y.subvec(begin, last);

        g = XBatch.t() * (logistic(XBatch * beta) - yBatch);
    }

private:
    // The design matrix, held by value so that Shuffle() can reorder it.
    arma::mat X;
    // The responses to each data point, likewise held by value.
    arma::vec y;
};

// ---------------------------------------------------------------------------
// Callback that records the coordinates the optimizer passes through
// ---------------------------------------------------------------------------

// ens::Callback inspects the class at compile time and calls the hooks it
// finds, so a callback implements only the ones it needs and derives from
// nothing. The three used here are declared in
// ensmallen_bits/callbacks/traits.hpp as
//
//     void BeginOptimization(OptimizerType&, FunctionType&, MatType&)
//     void StepTaken(OptimizerType&, FunctionType&, const MatType&)
//     void EndOptimization(OptimizerType&, FunctionType&, MatType&)
//
// StepTaken() takes its coordinates by const reference while the other two do
// not. Adding const to the coordinates argument of BeginOptimization() or
// EndOptimization() stops ensmallen from recognising the hook: the class still
// compiles, no warning is issued, and the hook is never called.
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
    // instead exits at the top of an iteration, the coordinates are unchanged
    // and Record() drops them.
    template<typename OptimizerType, typename FunctionType, typename MatType>
    void EndOptimization(OptimizerType& /* optimizer */,
                         FunctionType& /* function */,
                         MatType& coordinates)
    {
        Record(coordinates);
    }

private:
    // Append the coordinates as one more column, unless they repeat the
    // column already at the end. The comparison is exact rather than
    // tolerance based: the only duplicate it has to catch is the one
    // EndOptimization() can produce, and there no arithmetic has touched the
    // coordinates since the preceding hook fired.
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

// ---------------------------------------------------------------------------
// Functions exported to R
// ---------------------------------------------------------------------------

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

    LogisticRegressionFunction lrf(X, y, lambda);

    ens::L_BFGS lbfgs;

    // The objective is convex, so a deterministic start of zeros reaches the
    // same optimum a random start would.
    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    lbfgs.Optimize(lrf, beta);

    return beta;
}

// [[Rcpp::export]]
arma::vec logistic_reg_adam(const arma::mat& X, const arma::vec& y,
                            const size_t batch_size = 32,
                            const size_t max_epochs = 50,
                            const double step_size = 0.05,
                            const bool verbose = false) {

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
    if (batch_size == 0 || batch_size > X.n_rows) {
        Rcpp::stop("`batch_size` must be between 1 and `nrow(X)`.");
    }
    if (max_epochs == 0) {
        Rcpp::stop("`max_epochs` must be at least 1.");
    }

    LogisticRegressionSeparableFunction lrf(X, y);

    // ensmallen counts iterations in data points visited, so one epoch is
    // nrow(X) iterations.
    const size_t max_iterations = max_epochs * X.n_rows;

    // Arguments are: stepSize, batchSize, beta1, beta2, eps, maxIterations,
    // tolerance, and whether to shuffle between epochs.
    ens::Adam optimizer(step_size, batch_size, 0.9, 0.999, 1e-8,
                        max_iterations, 1e-8, true);

    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    // Callbacks are passed as trailing arguments to Optimize().
    if (verbose) {
        optimizer.Optimize(lrf, beta, ens::PrintLoss());
    } else {
        optimizer.Optimize(lrf, beta);
    }

    return beta;
}

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

    LogisticRegressionFunction lrf(X, y, lambda);

    // The same optimizer and starting point logistic_reg_lbfgs() uses, so the
    // two runs follow the same route.
    ens::L_BFGS lbfgs;
    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    // The path starts with the right number of rows and no columns, which is
    // what lets the recorder append with insert_cols().
    arma::mat path(X.n_cols, 0);

    // Callbacks are passed as trailing arguments to Optimize(). The recorder
    // writes into `path`, which outlives the callback object.
    lbfgs.Optimize(lrf, beta, PathRecorder(path));

    return path;
}
