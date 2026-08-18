## Checks that the standalone example file in inst/examples/ still agrees with
## the versions compiled into the package. The two hold the same mathematics on
## purpose, since the vignette develops the standalone file and then shows how
## it moves into a package; this test is what stops them drifting apart.
##
## Compiling the example pulls in the ensmallen and Armadillo templates, which
## is far too slow for CRAN, so this runs only under tinytest::test_all().

library(RcppEnsmallen)

if (at_home()) {

    example_file <- system.file("examples", "logistic-regression.cpp",
                                package = "RcppEnsmallen")
    expect_true(nzchar(example_file))
    expect_true(file.exists(example_file))

    ## Compile into a dedicated environment so the freshly built functions do
    ## not mask the package's identically named exports. This also exercises
    ## the inlineCxxPlugin() in R/inline.R, which nothing else covers.
    standalone <- new.env()
    Rcpp::sourceCpp(example_file, env = standalone)

    expect_true(is.function(standalone$logistic_reg_lbfgs))
    expect_true(is.function(standalone$logistic_reg_adam))
    expect_true(is.function(standalone$logistic_reg_lbfgs_path))

    set.seed(2024)
    n <- 2000
    beta <- c(-1, 0.7, -0.5, 1.2)
    p <- length(beta)
    X <- cbind(1, matrix(rnorm(n * (p - 1)), ncol = p - 1))
    y <- rbinom(n, size = 1, prob = 1 / (1 + exp(-(X %*% beta))))

    ## L-BFGS is deterministic in both builds, so these must match exactly
    ## rather than merely closely.
    for (lambda in c(0, 5, 50)) {
        expect_equal(standalone$logistic_reg_lbfgs(X, y, lambda = lambda),
                     logistic_reg_lbfgs(X, y, lambda = lambda))
        ## The callback records the same run, so the whole matrix has to
        ## agree, not only the column holding the fit.
        expect_equal(standalone$logistic_reg_lbfgs_path(X, y, lambda = lambda),
                     logistic_reg_lbfgs_path(X, y, lambda = lambda))
    }

    ## Argument validation should behave identically too.
    expect_error(standalone$logistic_reg_lbfgs(X, y, lambda = -1))
    expect_error(standalone$logistic_reg_adam(X, y, batch_size = 0))

    ## Adam is stochastic, so compare against the MLE rather than to the
    ## package build coefficient by coefficient.
    target <- unname(glm.fit(X, y, family = binomial())$coefficients)
    expect_true(max(abs(as.vector(standalone$logistic_reg_adam(X, y)) -
                        target)) < 0.05)
}
