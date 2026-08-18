## Tests for lin_reg_lbfgs().

library(RcppEnsmallen)

set.seed(11)
n <- 5000
beta <- c(-2, 1.5, 3, 8.2, 6.6)
p <- length(beta)
X <- cbind(1, matrix(rnorm(n * (p - 1)), ncol = p - 1))
y <- X %*% beta + rnorm(n)

fit <- lin_reg_lbfgs(X, y)

## Shape and type of the return value.
expect_true(is.numeric(fit))
expect_equal(dim(fit), c(p, 1L))
expect_true(all(is.finite(fit)))

## lm.fit() solves the same least squares problem in closed form. Unlike the
## logistic functions, lin_reg_lbfgs() starts L-BFGS from a random draw rather
## than from a vector of zeros, so the tolerance is set well wide of the
## agreement actually observed, which is around 1e-11 relative.
expect_equal(as.vector(fit), unname(lm.fit(X, y)$coefficients),
             tolerance = 1e-3)

## The known parameters used to generate the data should be recovered.
expect_true(max(abs(as.vector(fit) - beta)) < 0.1)
