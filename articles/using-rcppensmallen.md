# Solving Linear Regression using Numeric Optimization

Abstract

In this vignette, we describe how to use RcppEnsmallen in a standalone
C++ file.

## Overview

`RcppEnsmallen` package provides an embedded copy of the
[`ensmallen`](https://www.ensmallen.org/docs.html) C++ library of
optimization functions. Optimizers contained within are state of the art
and possess a high level of code quality. Each optimizer must be
accessed through *C++* by implementing the appropriate objective
functions and, then, surfaced into *R* through
[`RcppArmadillo`](https://cran.r-project.org/package=RcppArmadillo).
Alternatively, work has been done by Dirk Schumacher in
[`armacmp`](https://github.com/dirkschumacher/armacmp) to automatically
create the underlying *C++* code from *R*.

**Note:** Optimizers in `RcppEnsmallen` only work with
[`armadillo`](https://arma.sourceforge.net/docs.html) data structures.
Thus, if using
[`Eigen`](https://eigen.tuxfamily.org/index.php?title=Main_Page) through
[`RcppEigen`](https://cran.r-project.org/package=RcppEigen), please
consider the
[`RcppNumerical`](https://cran.r-project.org/package=RcppNumerical)
package.

### Linear Regression

Consider the **Residual Sum of Squares**, also known as **RSS**, defined
as:

$$RSS(\beta) = (\mathbf{y} - \mathbf{X}\beta)^{\top}(\mathbf{y} - \mathbf{X}\beta)$$

The objective function we wish to minimize would be defined as:

$$f(\beta) = \parallel \mathbf{y} - \mathbf{X}\beta \parallel_{2}$$

The gradient is defined as:

$$\frac{\partial RSS}{\partial\beta} = - 2\mathbf{X}^{\top}(\mathbf{y} - \mathbf{X}\beta)$$

#### Two-Step Implementation

When using `ensmallen` to solve this problem, we must create a *C++*
class that computes both the objective function value and its gradient
value either together or separately under member functions named as:

- `Evaluate()`: Value of the objective function under the parameters.
- `Gradient()`: Convergence to the correct value under the given
  parameters.
- `EvaluateWithGradient()`: Perform both steps at the same time.
  (Optional)

In the Linear Regression scenario, we will define each step separately
to emphasize the calculation occurring. Generally, the one step
`EvaluateWithGradient()` function will be faster than the two step
variant. More details on design can be found on [`ensmallen`
documentation page for differentiable
functions](https://www.ensmallen.org/docs.html#differentiable-functions).

Before writing the class, `RcppEnsmallen` requires accessing the library
in a standalone C++ file with the follow include and Rcpp Attribute
declarations:

``` cpp
#include <RcppEnsmallen.h>
// [[Rcpp::depends(RcppEnsmallen)]]
```

The overaching Linear regression class should be constructed as follows:

``` cpp
#include <RcppEnsmallen.h>
// [[Rcpp::depends(RcppEnsmallen)]]

// Define a differentiable objective function by implementing both Evaluate()
// and Gradient() separately.
class LinearRegressionFunction
{
public:
  // Construct the object with the given the design 
  // matrix and responses.
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
```

From there:

1.  Construct a *C++* function that exports into *R*.
2.  Within the function, determine an appropriate optimizer for the
    problem.
3.  Combine the optimizer with the linear regression class to compute
    the solution to the problem.

**Note:** Make sure to have the definition of the Linear Regression
class in the same *C++* file as the exported *C++* function into *R*.

``` cpp
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

  // Run the optimization
  lbfgs.Optimize(lrf, beta);

  return beta;
}
```

#### Verifying Results

Prior to using the new optimizer in mission critical work, compare the
results to methods already implemented in *R*. The best way to achieve
this is to create an oracle model by specifying the parameters known to
generate data and, then, try to recover them. Moreover, if a method is
already implemented in *R* feel free to try to check the result equality
within an appropriate tolerance threshold.

Following with this methodology, data must be generated.

``` r
n <- 1e6
beta <- c(-2, 1.5, 3, 8.2, 6.6)
p <- length(beta)
X <- cbind(1, matrix(rnorm(n), ncol = p - 1))
y <- X %*% beta + rnorm(n / (p - 1))
```

Next, the optimization procedure is used to estimate the parameters of
interest. Under this example, the results of the estimation can be
compared to [`lm()`](https://rdrr.io/r/stats/lm.html). That said,
[`lm()`](https://rdrr.io/r/stats/lm.html) may have more precise results
when compared against the optimizer as it is implemented with a
closed-form solution to linear regression plus the computational is
performed more rigorously.

``` r
coefs_lbfgs <- lin_reg_lbfgs(X, y)
coefs_lm <- lm.fit(X, y)$coefficients
```

|       |     LBFGS |        LM |
|:------|----------:|----------:|
| Beta1 | -1.997963 | -1.997963 |
| Beta2 |  1.501261 |  1.501261 |
| Beta3 |  3.000116 |  3.000116 |
| Beta4 |  8.198827 |  8.198827 |
| Beta5 |  6.602305 |  6.602305 |

Comparison of Estimated Coefficients
