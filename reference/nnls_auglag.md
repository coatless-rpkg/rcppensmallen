# Non-Negative Least Squares with the Augmented Lagrangian

Solves least squares subject to every coefficient being non-negative
with the augmented Lagrangian optimizer.

## Usage

``` r
nnls_auglag(X, y)
```

## Arguments

- X:

  A `matrix` that is the Design Matrix for the regression problem. Every
  column is constrained, so do not include a column of ones for an
  intercept.

- y:

  A `vec` of continuous responses.

## Value

The estimated \\\beta\\ parameter values, each of them non-negative.

## Details

The objective is the residual sum of squares \$\$f(\beta) = \lVert
\mathbf{y} - \mathbf{X} \beta \rVert_2^2\$\$ with gradient \$\$\nabla
f(\beta) = -2 \mathbf{X}^{T} \left( \mathbf{y} - \mathbf{X} \beta
\right)\$\$ minimized subject to \\\beta_j \ge 0\\ for every column
\\j\\ of `X`.

`ens::AugLagrangian` calls for the constrained function contract, which
extends the `Evaluate()` and `Gradient()` pair of a differentiable
function with `NumConstraints()`, `EvaluateConstraint()` and
`GradientConstraint()`. Every constraint is an equality, \\c_i(\beta) =
0\\, and a bound has to be recast as a residual that is zero while the
bound holds. Here that residual is \\\min(0, \beta_j)\\, one per
coefficient.

The bound is imposed through a penalty rather than by projection, so a
coefficient resting on the boundary is returned as a small number of
order 1e-10 rather than as exactly zero, and that number can be slightly
negative. Round the result before comparing it with zero.

Optimization starts from a vector of zeros, which satisfies every
constraint and makes the result reproducible.

## Examples

``` r
# Number of Points
n = 100

# Select beta parameters, two of which are exactly zero
beta = c(2, 0, 1.5, 0)

# Number of Predictors
p = length(beta)

set.seed(24)

# Generate predictors from a normal distribution
X = matrix(rnorm(n * p), ncol = p)

# Generate continuous responses
y = as.vector(X %*% beta + rnorm(n))

# The unconstrained least squares fit lands outside the feasible region
ols = qr.solve(X, y)
min(ols)
#> [1] -0.2641133

# Run optimization with the augmented Lagrangian
beta_hat = nnls_auglag(X, y)

# Compare against optim() with a lower bound of zero
rss = function(b) sum((y - X %*% b)^2)
box = optim(rep(0, p), rss, method = "L-BFGS-B", lower = rep(0, p))

cbind(actual = beta,
      ols = ols,
      ensmallen = beta_hat,
      optim = box$par)
#>      actual        ols                  optim
#> [1,]    2.0  2.0657368  2.055437e+00 2.055437
#> [2,]    0.0 -0.1195288 -6.464225e-16 0.000000
#> [3,]    1.5  1.5037992  1.479247e+00 1.479247
#> [4,]    0.0 -0.2641133 -1.310395e-15 0.000000
```
