# Linear Regression with L-BFGS

Solves the Linear Regression's Residual Sum of Squares using the L-BFGS
optimizer.

## Usage

``` r
lin_reg_lbfgs(X, y)
```

## Arguments

- X:

  A `matrix` that is the Design Matrix for the regression problem.

- y:

  A `vec` containing the response values.

## Value

The estimated \\\beta\\ parameter values for the linear regression.

## Details

Consider the **Residual Sum of Squares**, also known as **RSS**, defined
as: \$\$RSS\left( \beta \right) = \left( { \mathbf{y} - \mathbf{X} \beta
} \right)^{T} \left( \mathbf{y} - \mathbf{X} \beta \right)\$\$ The
objective function is defined as: \$\$f(\beta) = (y - X\beta)^2\$\$ The
gradient is defined as: \$\$\frac{\partial RSS}{\partial \beta} = -2
\mathbf{X}^{T} \left(\mathbf{y} - \mathbf{X} \beta \right)\$\$

## Examples

``` r
# Number of Points
n = 1000

# Select beta parameters
beta = c(-2, 1.5, 3, 8.2, 6.6)

# Number of Predictors (including intercept)
p = length(beta)

# Generate predictors from a normal distribution
X_i = matrix(rnorm(n), ncol = p - 1)

# Add an intercept
X = cbind(1, X_i)

# Generate y values
y = X%*%beta + rnorm(n / (p - 1))

# Run optimization with lbfgs
theta_hat = lin_reg_lbfgs(X, y)

# Verify parameters were recovered
cbind(actual = beta, estimated = theta_hat)
#>      actual          
#> [1,]   -2.0 -2.122497
#> [2,]    1.5  1.526514
#> [3,]    3.0  2.936653
#> [4,]    8.2  8.309844
#> [5,]    6.6  6.726329
```
