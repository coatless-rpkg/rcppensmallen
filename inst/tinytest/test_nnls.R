## Tests for nnls_auglag().

library(RcppEnsmallen)

## Simulate a problem whose unconstrained solution is infeasible, which is the
## premise the constrained fit exists to handle. Two of the true coefficients
## are exactly zero, and sampling noise pushes their least squares estimates
## below it. This is the data the documented example uses.
n <- 100
beta <- c(2, 0, 1.5, 0)
p <- length(beta)

set.seed(24)
X <- matrix(rnorm(n * p), ncol = p)
y <- as.vector(X %*% beta + rnorm(n))

rss <- function(b, X, y) sum((y - X %*% b)^2)

## The unconstrained fit, which is what the constraints have to overrule.
ols <- unname(qr.solve(X, y))
expect_true(sum(ols < 0) == 2L)
expect_true(min(ols) < -0.2)

## The base R reference: the same objective handed to a solver that takes box
## bounds directly. Its convergence controls are tightened well past the
## defaults so that a disagreement points at the augmented Lagrangian rather
## than at optim() stopping early.
box <- optim(rep(0, p), rss, X = X, y = y,
             method = "L-BFGS-B", lower = rep(0, p),
             control = list(factr = 1e-12, pgtol = 0, maxit = 10000))

fit <- nnls_auglag(X, y)

## --------------------------------------------------------------------------
## nnls_auglag(): shape and type of the return value
## --------------------------------------------------------------------------

expect_true(is.numeric(fit))
expect_equal(dim(fit), c(p, 1L))
expect_true(all(is.finite(fit)))

## --------------------------------------------------------------------------
## nnls_auglag(): the constraints hold
## --------------------------------------------------------------------------

## The bound is imposed by a penalty rather than by projection, so a
## coefficient resting on the boundary comes back as a tiny number that may
## fall on either side of zero. all(fit >= 0) is therefore the wrong
## assertion: it fails on a correct answer. The largest violation seen across
## the randomized sweep at the end of this file was -6.8e-09, and on this data
## it is -4.8e-17.
expect_true(min(fit) > -1e-06)
expect_true(all(round(as.vector(fit), 4) >= 0))

## The two coefficients driven to the boundary are the two that are zero in
## the data generating process, not an arbitrary pair.
expect_equal(which(round(as.vector(fit), 4) == 0), c(2L, 4L))

## --------------------------------------------------------------------------
## nnls_auglag(): agreement with the base R reference
## --------------------------------------------------------------------------

## Observed coefficient agreement is 1.7e-08.
expect_true(max(abs(as.vector(fit) - box$par)) < 1e-05)

## The objective is the tighter measure of the two, and the augmented
## Lagrangian matches the reference on it to 4.3e-14 absolute on a scale of
## 102.7. Written as a one-sided comparison this also rules out the solver
## landing at a feasible but worse point, which a coefficient tolerance alone
## would let through.
expect_true(rss(fit, X, y) <= rss(box$par, X, y) * (1 + 1e-08))

## Constraining the problem cannot lower the objective, since the feasible set
## is a subset of the space the unconstrained fit searches. Observed values are
## 102.72 against 92.60.
expect_true(rss(fit, X, y) > rss(ols, X, y))

## --------------------------------------------------------------------------
## nnls_auglag(): a feasible unconstrained fit is left alone
## --------------------------------------------------------------------------

## When every least squares coefficient is already non-negative no constraint
## is active, and the constrained answer has to be the unconstrained one.
## Without this case the tests above are all satisfied by a solver that pushes
## coefficients toward zero for reasons unrelated to the constraints.
set.seed(101)
n_feas <- 300
X_feas <- matrix(rnorm(n_feas * 3), ncol = 3)
y_feas <- as.vector(X_feas %*% c(3, 2, 5) + rnorm(n_feas, sd = 0.5))
ols_feas <- unname(qr.solve(X_feas, y_feas))

expect_true(min(ols_feas) > 0)
expect_equal(as.vector(nnls_auglag(X_feas, y_feas)), ols_feas,
             tolerance = 1e-06)

## --------------------------------------------------------------------------
## nnls_auglag(): determinism
## --------------------------------------------------------------------------

## No random number is drawn anywhere: the starting point is a fixed vector of
## zeros and the optimizer is deterministic. So repeat calls agree exactly,
## and unlike logistic_reg_adam() or lad_reg_pso() a call leaves R's stream
## where it found it.
expect_identical(nnls_auglag(X, y), nnls_auglag(X, y))

set.seed(7); u_plain <- runif(1)
set.seed(7); invisible(nnls_auglag(X, y)); u_after <- runif(1)
expect_identical(u_plain, u_after)

## --------------------------------------------------------------------------
## nnls_auglag(): a sweep over other shapes
## --------------------------------------------------------------------------

## The single documented data set fixes p = 4 with two active constraints. This
## widens that to a range of sample sizes, column counts and numbers of true
## zeros. Coefficients are compared loosely and the objective tightly, for the
## reason the doc data set shows in reverse: on nearly active coordinates the
## two solvers can differ by 1e-04 while their objectives agree to 1e-08
## relative, and on one such case it was optim() that was the loose party.
for (k in seq_len(8)) {
    set.seed(1000 + k)
    n_k <- sample(60:300, 1)
    p_k <- sample(3:8, 1)
    X_k <- matrix(rnorm(n_k * p_k), ncol = p_k)
    beta_k <- ifelse(runif(p_k) < 0.5, 0, runif(p_k, 0.5, 3))
    y_k <- as.vector(X_k %*% beta_k + rnorm(n_k))

    box_k <- optim(rep(0, p_k), rss, X = X_k, y = y_k,
                   method = "L-BFGS-B", lower = rep(0, p_k),
                   control = list(factr = 1e-12, pgtol = 0, maxit = 10000))
    fit_k <- as.vector(nnls_auglag(X_k, y_k))

    expect_true(min(fit_k) > -1e-06)
    expect_true(rss(fit_k, X_k, y_k) <= box_k$value * (1 + 1e-06))
    expect_true(max(abs(fit_k - box_k$par)) < 1e-03)
}

## --------------------------------------------------------------------------
## nnls_auglag(): input validation
## --------------------------------------------------------------------------

expect_error(nnls_auglag(X[1:10, ], y))
expect_error(nnls_auglag(X[0, , drop = FALSE], y[0]))

## A single non-finite entry makes the objective non-finite, the optimizer
## makes no progress, and without the guard the untouched starting point comes
## back looking like a fit in which every coefficient sits on its bound.
X_na <- X; X_na[1, 2] <- NA
expect_error(nnls_auglag(X_na, y))
X_inf <- X; X_inf[1, 2] <- Inf
expect_error(nnls_auglag(X_inf, y))

## The response is continuous, so the check on y is finiteness rather than the
## membership in {0, 1} that the logistic examples require.
y_na <- y; y_na[3] <- NaN
expect_error(nnls_auglag(X, y_na))
y_inf <- y; y_inf[3] <- Inf
expect_error(nnls_auglag(X, y_inf))

## The guards report the same reasons the other examples report for the same
## mistakes, rather than each example growing its own wording.
err_msg <- function(expr) {
    tryCatch({ expr; "no error" }, error = conditionMessage)
}
expect_identical(err_msg(nnls_auglag(X[1:10, ], y)),
                 err_msg(lad_reg_pso(X[1:10, ], y)))
expect_identical(err_msg(nnls_auglag(X_na, y)),
                 err_msg(lad_reg_pso(X_na, y)))
