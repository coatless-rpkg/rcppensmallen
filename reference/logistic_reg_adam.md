# Logistic Regression with Adam

Solves logistic regression with the mini-batch Adam optimizer by
expressing the negative log-likelihood as a separable objective
function.

## Usage

``` r
logistic_reg_adam(
  X,
  y,
  batch_size = 32L,
  max_epochs = 50L,
  step_size = 0.05,
  verbose = FALSE
)
```

## Arguments

- X:

  A `matrix` that is the Design Matrix for the regression problem.
  Include a column of ones for an intercept.

- y:

  A `vec` of binary responses taking values 0 or 1.

- batch_size:

  An `integer` giving the number of observations per mini-batch.

- max_epochs:

  An `integer` giving the number of complete passes to make over the
  data.

- step_size:

  A `double` giving the Adam step size. The default of 0.05 is
  deliberately larger than the 'ensmallen' Adam default of 0.001; see
  Details.

- verbose:

  A `logical` indicating whether to report the loss at the end of each
  epoch through an `ens::PrintLoss` callback.

## Value

The estimated \\\beta\\ parameter values for the logistic regression.

## Details

The objective and gradient match those of
[`logistic_reg_lbfgs()`](https://coatless.github.io/rcppensmallen/reference/logistic_reg_lbfgs.md)
with `lambda = 0`, but are evaluated one mini-batch at a time.

The optimizer's `MaxIterations()` counts *data points visited*, not
batches and not epochs, so `max_epochs` is converted internally by
multiplying by `nrow(X)`.

As of 'ensmallen' 3.10 the SGD family divides the step size by the batch
size, so a step size carried over from 'ensmallen' 2 will behave
differently unless `ENS_OLD_SEPARABLE_STEP_BEHAVIOR` is defined. This is
why `step_size` defaults to 0.05 rather than the 0.001 that `ens::Adam`
itself uses: with a batch size of 32 the effective step is `0.001 / 32`,
roughly `3e-5`, which leaves the optimizer far short of the optimum
after 50 epochs.

Adam shuffles the data between epochs. That shuffle draws on Armadillo's
random number generator, which 'RcppArmadillo' routes to R's own stream,
so the result *is* reproducible under
[`set.seed()`](https://rdrr.io/r/base/Random.html). The flip side is
that each call consumes random numbers from R's stream and therefore
shifts any [`runif()`](https://rdrr.io/r/stats/Uniform.html) or
[`rnorm()`](https://rdrr.io/r/stats/Normal.html) draws that follow it.
Without an explicit seed, successive calls start from different
positions in the stream and so give slightly different answers.

## Examples

``` r
# Number of Points
n = 5000

# Select beta parameters
beta = c(-1, 0.7, -0.5, 1.2)

# Number of Predictors (including intercept)
p = length(beta)

# Generate the design matrix with an intercept
X = cbind(1, matrix(rnorm(n * (p - 1)), ncol = p - 1))

# Generate binary responses
y = rbinom(n, size = 1, prob = 1 / (1 + exp(-X %*% beta)))

# Run optimization with Adam
theta_hat = logistic_reg_adam(X, y)

# Compare against the full-batch L-BFGS solution
cbind(actual = beta,
      adam = theta_hat,
      lbfgs = logistic_reg_lbfgs(X, y))
#>      actual                      
#> [1,]   -1.0 -0.9893675 -0.9897768
#> [2,]    0.7  0.7014957  0.6959250
#> [3,]   -0.5 -0.4562667 -0.4586689
#> [4,]    1.2  1.2423189  1.2408201
```
