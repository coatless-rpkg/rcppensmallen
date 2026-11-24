# Logistic Regression with L-BFGS

Solves logistic regression by minimizing the penalized negative
log-likelihood with the L-BFGS optimizer.

## Usage

``` r
logistic_reg_lbfgs(X, y, lambda = 0)
```

## Arguments

- X:

  A `matrix` that is the Design Matrix for the regression problem.
  Include a column of ones for an intercept.

- y:

  A `vec` of binary responses taking values 0 or 1.

- lambda:

  A `double` giving the ridge penalty. The default of 0 yields the
  maximum likelihood estimate.

## Value

The estimated \\\beta\\ parameter values for the logistic regression.

## Details

Writing \\\eta = \mathbf{X} \beta\\, the objective is the penalized
negative log-likelihood: \$\$f(\beta) = \sum_i \left\[ \log \left(1 +
e^{\eta_i} \right) - y_i \eta_i \right\] + \frac{\lambda}{2} \lVert
\beta \rVert_2^2\$\$ with gradient \$\$\nabla f(\beta) = \mathbf{X}^{T}
\left( \sigma \left( \eta \right) - \mathbf{y} \right) + \lambda
\beta\$\$ where \\\sigma\\ is the logistic function.

The penalty is applied to every coefficient, including the intercept.
This keeps the example self-contained, since the function cannot know
which column of `X` holds the intercept. Production code normally leaves
the intercept unpenalized.

Because the objective is convex, optimization starts from a vector of
zeros rather than a random point, which makes the result reproducible.

## Examples

``` r
# Number of Points
n = 1000

# Select beta parameters
beta = c(-1, 0.7, -0.5, 1.2)

# Number of Predictors (including intercept)
p = length(beta)

# Generate predictors from a normal distribution
X_i = matrix(rnorm(n * (p - 1)), ncol = p - 1)

# Add an intercept
X = cbind(1, X_i)

# Generate binary responses
y = rbinom(n, size = 1, prob = 1 / (1 + exp(-X %*% beta)))

# Run optimization with L-BFGS
theta_hat = logistic_reg_lbfgs(X, y)

# Compare against glm()
cbind(actual = beta,
      ensmallen = theta_hat,
      glm = glm.fit(X, y, family = binomial())$coefficients)
#>      actual                   glm
#> [1,]   -1.0 -1.0205616 -1.0205616
#> [2,]    0.7  0.8031375  0.8031375
#> [3,]   -0.5 -0.5938504 -0.5938504
#> [4,]    1.2  1.2233845  1.2233845
```
