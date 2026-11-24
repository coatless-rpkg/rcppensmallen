#include <RcppEnsmallen.h>

// Helpers are given internal linkage so that this file and its L-BFGS
// counterpart each carry their own copy and can be read on their own.
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

} // namespace

// Define a separable objective function, which is what the mini-batch
// optimizers require. On top of Evaluate() and Gradient() taking a batch
// range, a separable function must report how many data points it holds and
// must be able to shuffle them.
//
// Note that this class stores its own copies of X and y rather than const
// references. Shuffle() reorders the data in place, so the members have to be
// modifiable. That is the one structural difference from the full-batch
// LogisticRegressionFunction used with L-BFGS.
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

//' Logistic Regression with Adam
//'
//' Solves logistic regression with the mini-batch Adam optimizer by expressing
//' the negative log-likelihood as a separable objective function.
//'
//' @param X          A `matrix` that is the Design Matrix for the regression
//'                   problem. Include a column of ones for an intercept.
//' @param y          A `vec` of binary responses taking values 0 or 1.
//' @param batch_size An `integer` giving the number of observations per
//'                   mini-batch.
//' @param max_epochs An `integer` giving the number of complete passes to make
//'                   over the data.
//' @param step_size  A `double` giving the Adam step size. The default of 0.05
//'                   is deliberately larger than the 'ensmallen' Adam default
//'                   of 0.001; see Details.
//' @param verbose    A `logical` indicating whether to report the loss at the
//'                   end of each epoch through an `ens::PrintLoss` callback.
//'
//' @return
//' The estimated \eqn{\beta}{beta} parameter values for the logistic regression.
//' @details
//' The objective and gradient match those of [logistic_reg_lbfgs()] with
//' `lambda = 0`, but are evaluated one mini-batch at a time.
//'
//' The optimizer's `MaxIterations()` counts *data points visited*, not batches
//' and not epochs, so `max_epochs` is converted internally by multiplying by
//' `nrow(X)`.
//'
//' As of 'ensmallen' 3.10 the SGD family divides the step size by the batch
//' size, so a step size carried over from 'ensmallen' 2 will behave
//' differently unless `ENS_OLD_SEPARABLE_STEP_BEHAVIOR` is defined. This is why
//' `step_size` defaults to 0.05 rather than the 0.001 that `ens::Adam` itself
//' uses: with a batch size of 32 the effective step is `0.001 / 32`, roughly
//' `3e-5`, which leaves the optimizer far short of the optimum after 50 epochs.
//'
//' Adam shuffles the data between epochs. That shuffle draws on Armadillo's
//' random number generator, which 'RcppArmadillo' routes to R's own stream, so
//' the result *is* reproducible under [set.seed()]. The flip side is that each
//' call consumes random numbers from R's stream and therefore shifts any
//' [runif()] or [rnorm()] draws that follow it. Without an explicit seed,
//' successive calls start from different positions in the stream and so give
//' slightly different answers.
//' @export
//' @examples
//' # Number of Points
//' n = 5000
//'
//' # Select beta parameters
//' beta = c(-1, 0.7, -0.5, 1.2)
//'
//' # Number of Predictors (including intercept)
//' p = length(beta)
//'
//' # Generate the design matrix with an intercept
//' X = cbind(1, matrix(rnorm(n * (p - 1)), ncol = p - 1))
//'
//' # Generate binary responses
//' y = rbinom(n, size = 1, prob = 1 / (1 + exp(-X %*% beta)))
//'
//' # Run optimization with Adam
//' theta_hat = logistic_reg_adam(X, y)
//'
//' # Compare against the full-batch L-BFGS solution
//' cbind(actual = beta,
//'       adam = theta_hat,
//'       lbfgs = logistic_reg_lbfgs(X, y))
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

    // Construct the separable objective function.
    LogisticRegressionSeparableFunction lrf(X, y);

    // ensmallen counts iterations in data points visited, so one epoch is
    // nrow(X) iterations.
    const size_t max_iterations = max_epochs * X.n_rows;

    // Arguments are: stepSize, batchSize, beta1, beta2, eps, maxIterations,
    // tolerance, and whether to shuffle between epochs.
    ens::Adam optimizer(step_size, batch_size, 0.9, 0.999, 1e-8,
                        max_iterations, 1e-8, true);

    arma::mat beta(X.n_cols, 1, arma::fill::zeros);

    // Callbacks are passed as trailing arguments to Optimize(). PrintLoss
    // writes to arma::get_cout_stream(), which RcppArmadillo maps to
    // Rcpp::Rcout, so the output is safe to emit from an R package.
    if (verbose) {
        optimizer.Optimize(lrf, beta, ens::PrintLoss());
    } else {
        optimizer.Optimize(lrf, beta);
    }

    return beta;
}
