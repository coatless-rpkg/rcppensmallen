# Least Absolute Deviations Regression with Particle Swarm Optimization

Solves least absolute deviations regression by minimizing the sum of
absolute residuals with the local best particle swarm optimizer.

## Usage

``` r
lad_reg_pso(X, y)
```

## Arguments

- X:

  A `matrix` that is the Design Matrix for the regression problem.
  Include a column of ones for an intercept.

- y:

  A `vec` containing the response values.

## Value

The estimated \\\beta\\ parameter values for the least absolute
deviations fit.

## Details

The objective is the sum of absolute residuals: \$\$f(\beta) = \sum_i
\left\lvert y_i - \mathbf{x}\_i^{T} \beta \right\rvert\$\$ Compared with
the residual sum of squares, the absolute value leaves large residuals
with a constant rather than a growing influence, so the fit tolerates
heavy tailed errors and outlying responses.

The price is that the objective has a kink at every \\\beta\\ that makes
a residual zero, and for a design with \\p\\ columns a solution sets
\\p\\ of the residuals to zero. The minimum therefore sits on the kinks,
where no gradient exists.

`LADFunction` in the source of this example supplies

    double Evaluate(const arma::mat& beta);

and no `Gradient()`. That is ensmallen's arbitrary function contract.
Optimizers that need a derivative reject a class shaped this way at
compile time, so fewer optimizers accept it than accept the
differentiable objectives behind
[`lin_reg_lbfgs()`](https://coatless.github.io/rcppensmallen/reference/lin_reg_lbfgs.md)
and
[`logistic_reg_lbfgs()`](https://coatless.github.io/rcppensmallen/reference/logistic_reg_lbfgs.md).

Particle swarm optimization scatters candidate parameter vectors across
the search space and moves each one toward the best point it has seen
and the best point its neighbors have seen. Only the shape of the
starting matrix reaches the optimizer, since the swarm is placed at
random. Those draws come from R's random number generator, so
[`set.seed()`](https://rdrr.io/r/base/Random.html) makes a run
reproducible. Unseeded runs land at slightly different points, and the
spread widens as `X` gains columns.

## Examples

``` r
# Number of Points
n = 400

# Select beta parameters
beta = c(2, -1.5, 0.8)

# Number of Predictors (including intercept)
p = length(beta)

# The swarm is initialized at random, so fix the seed
set.seed(512)

# Generate predictors from a normal distribution
X_i = matrix(rnorm(n * (p - 1)), ncol = p - 1)

# Add an intercept
X = cbind(1, X_i)

# Generate responses with heavy tailed errors
y = X %*% beta + rt(n, df = 2)

# Run optimization with particle swarm optimization
theta_hat = lad_reg_pso(X, y)

# Compare against a derivative free search in base R
nm = optim(rep(0, p), function(b) sum(abs(y - X %*% b)),
           method = "Nelder-Mead",
           control = list(maxit = 20000, reltol = 1e-12))

cbind(actual = beta, ensmallen = theta_hat, optim = nm$par)
#>      actual                optim
#> [1,]    2.0  2.086174  2.0860438
#> [2,]   -1.5 -1.479793 -1.4798253
#> [3,]    0.8  0.805097  0.8048079

# Compare the objective each one reached
obj = function(b) sum(abs(y - X %*% b))
c(ensmallen = obj(theta_hat), optim = obj(nm$par))
#> ensmallen     optim 
#>  657.9162  657.9169 
```
