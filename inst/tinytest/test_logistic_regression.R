## Tests for logistic_reg_lbfgs(), logistic_reg_adam(), and
## logistic_reg_lbfgs_path().

library(RcppEnsmallen)

## An independent reference implementation: penalized logistic regression by
## Newton/IRLS, written in plain R with R's own stable primitives. Matching it
## pins the objective *and* the gradient, penalty included, rather than merely
## checking that the answer looks plausible.
ref_fit <- function(X, y, lambda, iter = 100) {
    b <- rep(0, ncol(X))
    for (i in seq_len(iter)) {
        eta <- as.vector(X %*% b)
        p <- plogis(eta)
        g <- crossprod(X, p - y) + lambda * b
        H <- crossprod(X * pmax(p * (1 - p), 1e-10), X) + lambda * diag(ncol(X))
        b_new <- b - as.vector(solve(H, g))
        if (max(abs(b_new - b)) < 1e-12) {
            b <- b_new
            break
        }
        b <- b_new
    }
    b
}

## Simulate a well-behaved logistic regression problem.
set.seed(42)
n <- 5000
beta <- c(-1, 0.7, -0.5, 1.2)
p <- length(beta)
X <- cbind(1, matrix(rnorm(n * (p - 1)), ncol = p - 1))
y <- rbinom(n, size = 1, prob = 1 / (1 + exp(-(X %*% beta))))

## --------------------------------------------------------------------------
## logistic_reg_lbfgs(): agreement with glm()
## --------------------------------------------------------------------------

fit <- logistic_reg_lbfgs(X, y)
target <- unname(glm.fit(X, y, family = binomial())$coefficients)

## The unpenalized objective is exactly the negative log-likelihood glm()
## maximizes, so the two should agree to well beyond graphical precision.
expect_equal(as.vector(fit), target, tolerance = 1e-6)

## Shape and type of the return value.
expect_true(is.numeric(fit))
expect_equal(dim(fit), c(p, 1L))
expect_true(all(is.finite(fit)))

## --------------------------------------------------------------------------
## logistic_reg_lbfgs(): ridge penalty
## --------------------------------------------------------------------------

## lambda = 0 must reproduce the unpenalized fit exactly.
expect_equal(logistic_reg_lbfgs(X, y, lambda = 0), fit)

## Pin the penalized estimate to the independent reference at each lambda.
## Monotone shrinkage alone would be far too weak: it still holds if the
## penalty is dropped from the objective but kept in the gradient, or if its
## scale is wrong, so it cannot distinguish a correct ridge from a broken one.
for (lambda in c(0, 1, 10, 100, 1000)) {
    expect_equal(as.vector(logistic_reg_lbfgs(X, y, lambda = lambda)),
                 ref_fit(X, y, lambda),
                 tolerance = 1e-6)
}

## Increasing lambda must shrink the coefficient vector monotonically.
norms <- sapply(c(0, 1, 10, 100, 1000),
                function(l) sum(logistic_reg_lbfgs(X, y, lambda = l)^2))
expect_true(all(diff(norms) < 0))

## --------------------------------------------------------------------------
## logistic_reg_lbfgs(): extreme predictor scales
## --------------------------------------------------------------------------

## Data whose linear predictor is enormous. Note what this does and does not
## establish: it confirms the fit stays finite and matches the reference, but
## it is NOT a guard on the stable softplus rewrite. With lambda > 0 the
## penalty bounds |eta| well below the ~709 overflow threshold, and with
## lambda = 0 on separated data there is no finite optimum to compare against.
## The naive log(1 + exp(eta)) form returns Inf rather than crashing, and
## L-BFGS treats that as a rejected step, so the rewrite is not observable
## through this function's return value at all. Do not re-word this comment to
## claim otherwise.
X_sep <- cbind(1, c(-1000, -900, 900, 1000))
y_sep <- c(0, 0, 1, 1)
fit_sep <- logistic_reg_lbfgs(X_sep, y_sep, lambda = 1)
expect_true(all(is.finite(fit_sep)))
expect_false(any(is.nan(fit_sep)))
## Looser than the well-scaled cases above on purpose: predictors of magnitude
## 1e3 scale the Hessian by about 1e6, and L-BFGS's default convergence
## criterion stops well short of the precision Newton reaches. The observed gap
## is ~1.5e-6 relative, which is conditioning rather than a wrong answer.
expect_equal(as.vector(fit_sep), ref_fit(X_sep, y_sep, 1), tolerance = 1e-4)

## --------------------------------------------------------------------------
## logistic_reg_lbfgs(): input validation
## --------------------------------------------------------------------------

expect_error(logistic_reg_lbfgs(X[1:10, ], y))
expect_error(logistic_reg_lbfgs(X, y + 0.5))
expect_error(logistic_reg_lbfgs(X, y, lambda = -1))

## A non-finite X must be rejected rather than silently returning the all-zero
## starting point, which is what happens without an explicit check.
X_na <- X; X_na[1, 2] <- NA
expect_error(logistic_reg_lbfgs(X_na, y))
X_inf <- X; X_inf[1, 2] <- Inf
expect_error(logistic_reg_lbfgs(X_inf, y))

## --------------------------------------------------------------------------
## logistic_reg_adam(): reproducibility
## --------------------------------------------------------------------------

## Shuffle() draws on Armadillo's RNG, which RcppArmadillo routes onto R's
## stream, so a fixed seed makes Adam bit-for-bit reproducible.
set.seed(20260731)
adam_a <- logistic_reg_adam(X, y)
set.seed(20260731)
adam_b <- logistic_reg_adam(X, y)
expect_identical(adam_a, adam_b)

## The same mechanism means a call consumes R random numbers, advancing the
## stream for whatever follows.
set.seed(11); u_plain <- runif(1)
set.seed(11); invisible(logistic_reg_adam(X, y, max_epochs = 1)); u_after <- runif(1)
expect_false(isTRUE(all.equal(u_plain, u_after)))

## --------------------------------------------------------------------------
## logistic_reg_adam(): agreement with the full-batch solution
## --------------------------------------------------------------------------

## Seeded, so this is deterministic. The tolerance absorbs genuine mini-batch
## optimizer slack against the L-BFGS target, not RNG variation; the observed
## gap is around 0.005.
set.seed(987)
fit_adam <- logistic_reg_adam(X, y)
expect_equal(dim(fit_adam), c(p, 1L))
expect_true(all(is.finite(fit_adam)))
expect_true(max(abs(as.vector(fit_adam) - target)) < 0.05)

## --------------------------------------------------------------------------
## logistic_reg_adam(): input validation
## --------------------------------------------------------------------------

expect_error(logistic_reg_adam(X[1:10, ], y))
expect_error(logistic_reg_adam(X, y + 0.5))
expect_error(logistic_reg_adam(X, y, batch_size = 0))
expect_error(logistic_reg_adam(X, y, batch_size = n + 1))
expect_error(logistic_reg_adam(X, y, max_epochs = 0))
expect_error(logistic_reg_adam(X_na, y))

## A batch size equal to the full sample is legal, i.e. full-batch Adam.
expect_silent(logistic_reg_adam(X, y, batch_size = n, max_epochs = 2))

## --------------------------------------------------------------------------
## logistic_reg_lbfgs_path(): shape and endpoints
## --------------------------------------------------------------------------

## The objective the C++ side minimizes, written with R's own stable
## primitives: log(1 + exp(eta)) is -plogis(-eta, log.p = TRUE), which stays
## finite for any eta. Evaluating this at every column tests the recorded
## iterates against the penalized negative log-likelihood itself rather than
## against the optimizer's own bookkeeping.
pen_nll <- function(b, X, y, lambda) {
    eta <- as.vector(X %*% b)
    sum(-plogis(-eta, log.p = TRUE) - y * eta) + 0.5 * lambda * sum(b^2)
}

path <- logistic_reg_lbfgs_path(X, y)

expect_true(is.matrix(path))
expect_true(is.numeric(path))
expect_true(all(is.finite(path)))

## Rows are coefficients and columns are iterations, so the row count is p for
## any design matrix and the column count is one starting point plus the
## accepted steps. The two-column design pins the orientation: a transposed
## result would report 2 columns and many rows instead.
expect_equal(nrow(path), p)
expect_equal(nrow(logistic_reg_lbfgs_path(X[, 1:2], y)), 2L)
expect_true(ncol(path) >= 2L)

## Column one is the vector of zeros logistic_reg_lbfgs() also starts from.
expect_true(all(path[, 1] == 0))

## cbind(starting point, answer) satisfies every check above. L-BFGS takes on
## the order of ten accepted steps on this data, so two columns would mean the
## intermediate iterates never reached the caller.
expect_true(ncol(path) > 2L)

## The last column is where logistic_reg_lbfgs() stops, and that point is the
## maximum likelihood estimate glm() reports.
path_end <- as.vector(path[, ncol(path)])
expect_equal(path_end, as.vector(fit), tolerance = 1e-6)
expect_equal(path_end, target, tolerance = 1e-6)

## --------------------------------------------------------------------------
## logistic_reg_lbfgs_path(): the objective never rises along the path
## --------------------------------------------------------------------------

## L-BFGS accepts a step only when its line search reports sufficient
## decrease, so the objective evaluated at the recorded columns is
## non-increasing. The slack allowed here is relative to the objective's own
## scale, which starts at n * log(2), about 3466 for this data, and swamps
## the rounding error of a 5000-term sum.
for (lambda_path in c(0, 10)) {
    path_l <- logistic_reg_lbfgs_path(X, y, lambda = lambda_path)
    obj <- vapply(seq_len(ncol(path_l)),
                  function(k) pen_nll(path_l[, k], X, y, lambda_path),
                  numeric(1))
    expect_true(all(diff(obj) <= 1e-8 * max(abs(obj))))
    ## Non-increasing alone is also satisfied by a path that never moves.
    expect_true(obj[length(obj)] < obj[1])
}

## --------------------------------------------------------------------------
## logistic_reg_lbfgs_path(): ridge penalty
## --------------------------------------------------------------------------

## lambda = 0 is the default, so passing it explicitly changes nothing.
expect_equal(logistic_reg_lbfgs_path(X, y, lambda = 0), path)

## The endpoint at a given lambda is the penalized fit. Pinning it to the same
## Newton reference the logistic_reg_lbfgs() tests use means a penalty of the
## wrong scale, or one carried by the callback but not by the objective, shows
## up here rather than only in the shrinkage direction.
path_ridge <- logistic_reg_lbfgs_path(X, y, lambda = 1000)
end_ridge <- as.vector(path_ridge[, ncol(path_ridge)])
expect_equal(end_ridge, as.vector(logistic_reg_lbfgs(X, y, lambda = 1000)),
             tolerance = 1e-6)
expect_equal(end_ridge, ref_fit(X, y, 1000), tolerance = 1e-6)

## Growing lambda pulls the endpoint toward zero, matching the shrinkage the
## returned-coefficient tests establish for logistic_reg_lbfgs() itself. The
## observed squared norms run from about 2.86 down to about 0.44.
end_norms <- sapply(c(0, 1, 10, 100, 1000), function(l) {
    pth <- logistic_reg_lbfgs_path(X, y, lambda = l)
    sum(pth[, ncol(pth)]^2)
})
expect_true(all(diff(end_norms) < 0))
expect_true(end_norms[length(end_norms)] < 0.5 * end_norms[1])

## --------------------------------------------------------------------------
## logistic_reg_lbfgs_path(): input validation
## --------------------------------------------------------------------------

expect_error(logistic_reg_lbfgs_path(X[1:10, ], y))
expect_error(logistic_reg_lbfgs_path(X, y + 0.5))
expect_error(logistic_reg_lbfgs_path(X, y, lambda = -1))
expect_error(logistic_reg_lbfgs_path(X_na, y))
expect_error(logistic_reg_lbfgs_path(X_inf, y))
expect_error(logistic_reg_lbfgs_path(X[0, , drop = FALSE], y[0]))

## Rejecting the input is half the claim; the other half is that the two
## functions reject it for the same stated reason, which fails if the path
## variant grows its own guards or drops one and dies further downstream.
## The "no error" branch keeps a call that wrongly succeeds reportable as a
## failed comparison, rather than as an error raised inside the test file.
err_msg <- function(expr) {
    tryCatch({ expr; "no error" }, error = conditionMessage)
}
expect_identical(err_msg(logistic_reg_lbfgs_path(X, y, lambda = -1)),
                 err_msg(logistic_reg_lbfgs(X, y, lambda = -1)))
expect_identical(err_msg(logistic_reg_lbfgs_path(X_na, y)),
                 err_msg(logistic_reg_lbfgs(X_na, y)))
