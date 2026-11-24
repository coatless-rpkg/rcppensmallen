## Tests for lad_reg_pso().

library(RcppEnsmallen)

obj <- function(b, X, y) sum(abs(y - X %*% b))

## An independent reference for the least absolute deviations fit, built from
## base R alone. Nelder-Mead is the natural comparison because it, like the
## particle swarm, never asks for a derivative, and the objective has none at
## its own minimum. A single call stops short there: from zeros with
## maxit = 20000 and reltol = 1e-12 it lands 7.2e-04 above the value below.
## Restarting it from its own answer until it stops improving closes that gap.
##
## This reference is trustworthy at the p = 3 designs used here. At p >= 8 the
## restarted search itself stalls with one bound still slack, and a line search
## along the remaining free direction finds a strictly better point, so do not
## reuse this helper to police a wider design without checking it again.
ref_lad <- function(X, y) {
    b <- qr.solve(X, y)
    previous <- Inf
    for (i in seq_len(50)) {
        o <- optim(b, obj, X = X, y = y, method = "Nelder-Mead",
                   control = list(maxit = 50000, reltol = 1e-15))
        b <- o$par
        if (o$value >= previous - 1e-14) break
        previous <- o$value
    }
    as.vector(b)
}

## Simulate a regression with heavy tailed errors, which is the setting least
## absolute deviations is for. This is the data the documented example uses.
n <- 400
beta <- c(2, -1.5, 0.8)
p <- length(beta)

set.seed(512)
X <- cbind(1, matrix(rnorm(n * (p - 1)), ncol = p - 1))
y <- as.vector(X %*% beta + rt(n, df = 2))

## The swarm is placed at random through R's stream, so every call that is
## compared against a fixed number is seeded.
set.seed(512)
fit <- lad_reg_pso(X, y)
ref <- ref_lad(X, y)

## --------------------------------------------------------------------------
## lad_reg_pso(): shape and type of the return value
## --------------------------------------------------------------------------

expect_true(is.numeric(fit))
expect_equal(dim(fit), c(p, 1L))
expect_true(all(is.finite(fit)))

## --------------------------------------------------------------------------
## lad_reg_pso(): agreement with the base R reference
## --------------------------------------------------------------------------

## The seeded run reproduces the reference coefficients to 3.5e-10 and the
## unseeded spread over 30 runs on this data is 8.9e-07. The tolerance is set
## three orders above the latter, since a different platform reaches a
## different point on the swarm's trajectory much as a different seed does.
expect_equal(as.vector(fit), ref, tolerance = 1e-3)

## Coefficients are the loose measure here and the objective is the tight one,
## because a piecewise linear surface is flat in the directions that move a
## coefficient without moving a residual across zero. The seeded gap is
## 1.9e-10 on a scale of 658 and the worst unseeded gap is 4.4e-07.
expect_true(obj(fit, X, y) - obj(ref, X, y) < 1e-4)

## The example prints a single-shot Nelder-Mead run beside the swarm. That run
## is the weaker of the two, so the comparison in the documentation is not
## flattering the optimizer it is written to demonstrate.
nm <- optim(rep(0, p), obj, X = X, y = y, method = "Nelder-Mead",
            control = list(maxit = 20000, reltol = 1e-12))
expect_true(obj(fit, X, y) < nm$value)

## The parameters the data was generated from are recovered. The residual gap
## of 0.086 is sampling error at n = 400 with t(2) noise, not optimizer error.
expect_true(max(abs(as.vector(fit) - beta)) < 0.15)

## --------------------------------------------------------------------------
## lad_reg_pso(): the fit sits on the kinks
## --------------------------------------------------------------------------

## A least absolute deviations solution for a design with p columns passes
## exactly through p of the points, which is the reason the objective has no
## derivative at its own minimum. The three interpolated residuals here are
## below 1e-09 and the next smallest is 0.0109, so the threshold separates
## them by two orders in either direction.
resid <- as.vector(y - X %*% fit)
expect_equal(sum(abs(resid) < 1e-4), p)

## --------------------------------------------------------------------------
## lad_reg_pso(): behavior under an outlying response
## --------------------------------------------------------------------------

## Absolute residuals give a distant point a constant rather than a growing
## influence. Moving one response by 200 leaves the fit where it was, while
## the least squares fit follows it. Both fits are computed on the same two
## data sets so the comparison isolates the loss function.
set.seed(31)
n_out <- 200
X_out <- cbind(1, matrix(rnorm(n_out * (p - 1)), ncol = p - 1))
y_clean <- as.vector(X_out %*% beta + rnorm(n_out, sd = 0.3))
y_dirty <- y_clean
y_dirty[1] <- y_dirty[1] + 200

set.seed(41)
lad_clean <- as.vector(lad_reg_pso(X_out, y_clean))
set.seed(41)
lad_dirty <- as.vector(lad_reg_pso(X_out, y_dirty))

ols_clean <- unname(lm.fit(X_out, y_clean)$coefficients)
ols_dirty <- unname(lm.fit(X_out, y_dirty)$coefficients)

## Observed shifts are 4.8e-14 for the swarm and 1.00 for least squares.
expect_true(max(abs(lad_dirty - lad_clean)) < 1e-3)
expect_true(max(abs(ols_dirty - ols_clean)) > 0.5)

## --------------------------------------------------------------------------
## lad_reg_pso(): a single column recovers the median
## --------------------------------------------------------------------------

## With one column of ones the objective is sum |y_i - b|, minimized at the
## median of y. An odd sample size makes that median unique, so the answer is
## a single number rather than an interval. This case also exercises the p = 1
## path through the optimizer's initialization, which handles a one-row bound
## matrix differently from a taller one.
set.seed(3)
y_one <- rt(101, df = 2)
X_one <- matrix(1, nrow = 101, ncol = 1)

set.seed(5)
fit_one <- as.vector(lad_reg_pso(X_one, y_one))
expect_equal(dim(lad_reg_pso(X_one, y_one)), c(1L, 1L))
expect_true(abs(fit_one - median(y_one)) < 1e-4)

## --------------------------------------------------------------------------
## lad_reg_pso(): reproducibility
## --------------------------------------------------------------------------

## The swarm is drawn through Armadillo's random number generator, which
## RcppArmadillo routes onto R's stream, so a fixed seed makes the result
## bit-for-bit reproducible.
set.seed(20260818)
pso_a <- lad_reg_pso(X, y)
set.seed(20260818)
pso_b <- lad_reg_pso(X, y)
expect_identical(pso_a, pso_b)

## The same mechanism means a call consumes random numbers from R's stream and
## therefore shifts whatever draws follow it.
set.seed(11); u_plain <- runif(1)
set.seed(11); invisible(lad_reg_pso(X, y)); u_after <- runif(1)
expect_false(isTRUE(all.equal(u_plain, u_after)))

## --------------------------------------------------------------------------
## lad_reg_pso(): input validation
## --------------------------------------------------------------------------

expect_error(lad_reg_pso(X[1:10, ], y))
expect_error(lad_reg_pso(X[0, , drop = FALSE], y[0]))

## A single non-finite entry makes the objective non-finite for every particle,
## so no particle looks better than any other and the swarm has nothing to move
## toward. Without the guard the untouched starting point comes back as if it
## were a fit.
X_na <- X; X_na[1, 2] <- NA
expect_error(lad_reg_pso(X_na, y))
X_inf <- X; X_inf[1, 2] <- Inf
expect_error(lad_reg_pso(X_inf, y))

## The response here is continuous, so the check on y is finiteness rather
## than the membership in {0, 1} that the logistic examples require.
y_na <- y; y_na[3] <- NA
expect_error(lad_reg_pso(X, y_na))
y_inf <- y; y_inf[3] <- -Inf
expect_error(lad_reg_pso(X, y_inf))

## Rejecting the input is half the claim; the other half is that these guards
## report the same reasons the other examples report for the same mistakes.
err_msg <- function(expr) {
    tryCatch({ expr; "no error" }, error = conditionMessage)
}
expect_identical(err_msg(lad_reg_pso(X[1:10, ], y)),
                 err_msg(logistic_reg_lbfgs(X[1:10, ], y)))
expect_identical(err_msg(lad_reg_pso(X_na, y)),
                 err_msg(logistic_reg_lbfgs(X_na, y)))
