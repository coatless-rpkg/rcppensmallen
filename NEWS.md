# RcppEnsmallen 0.2.17.0.1

- Upgraded to ensmallen 2.17.0: "Pachis Din Me Pesa Double" (2021-07-06)
  - CheckArbitraryFunctionTypeAPI extended for MOO support
    ([#283](https://github.com/mlpack/ensmallen/pull/283)).
  - Refactor NSGA2
    ([#263](https://github.com/mlpack/ensmallen/pull/263),
    [#304](https://github.com/mlpack/ensmallen/pull/304)).
  - Add Indicators for Multiobjective optimizers
    ([#285](https://github.com/mlpack/ensmallen/pull/285)).
  - Make Callback flexible for MultiObjective Optimizers
    ([#289](https://github.com/mlpack/ensmallen/pull/289)).
  - Add ZDT Test Suite
    ([#273](https://github.com/mlpack/ensmallen/pull/273)).
  - Add MOEA-D/DE Optimizer
    ([#269](https://github.com/mlpack/ensmallen/pull/269)).
  - Introduce Policy Methods for MOEA/D-DE
    ([#293](https://github.com/mlpack/ensmallen/pull/293)).
  - Add Das-Dennis weight initialization method
    ([#295](https://github.com/mlpack/ensmallen/pull/295)).
  - Add Dirichlet Weight Initialization
    ([#296](https://github.com/mlpack/ensmallen/pull/296)).
  - Improved installation and compilation instructions
    ([#300](https://github.com/mlpack/ensmallen/pull/300)).
  - Disable building the tests by default for faster installation
    ([#303](https://github.com/mlpack/ensmallen/pull/303)).
  - Modify matrix initialisation to take into account
    default element zeroing in Armadillo 10.5
    ([#305](https://github.com/mlpack/ensmallen/pull/305)).

# RcppEnsmallen 0.2.16.2.1 (GitHub-only Release)

- Upgraded to ensmallen 2.16.2: "Severely Dented Can Of Polyurethane" (2021-03-25)
  - Fix CNE test trials
    ([#267](https://github.com/mlpack/ensmallen/pull/267)).
  - Update Catch2 to 2.13.4
    ([#268](https://github.com/mlpack/ensmallen/pull/268)).
  - Fix typos in documentation
    ([#270](https://github.com/mlpack/ensmallen/pull/270),
     [#271](https://github.com/mlpack/ensmallen/pull/271)).
  - Add clarifying comments in problems/ implementations
    ([#276](https://github.com/mlpack/ensmallen/pull/276)).
  - Remove `AdamSchafferFunctionN2Test` test from Adam test suite to prevent
    spurious issue on some aarch64 ([#265](https://github.com/mlpack/ensmallen/pull/259)).

# RcppEnsmallen 0.2.16.1.1

- Upgraded to ensmallen 2.16.1: "Severely Dented Can Of Polyurethane" (2021-03-04)
  - Fix test compilation issue when `ENS_USE_OPENMP` is set
    ([#255](https://github.com/mlpack/ensmallen/pull/255)).
  - Fix CNE initial population generation to use normal distribution
    ([#258](https://github.com/mlpack/ensmallen/pull/258)).
  - Fix compilation warnings
    ([#259](https://github.com/mlpack/ensmallen/pull/259)).

# RcppEnsmallen 0.2.16.0.1 (GitHub-Only Release)

- Upgraded to ensmallen 2.16.0: "Severely Dented Can Of Polyurethane" (2021-02-19)
  - Expand README with example installation and add simple example program
    showing usage of the L-BFGS optimizer
    ([#248](https://github.com/mlpack/ensmallen/pull/248)).
  - Refactor tests to increase stability and reduce random errors
    ([#249](https://github.com/mlpack/ensmallen/pull/249)).
  

# RcppEnsmallen 0.2.15.1.1

- Upgraded to ensmallen 2.15.1: "Why Can't I Manage To Grow Any Plants?" (2020-11-05)
  - Fix include order to ensure traits is loaded before reports
    ([#239](https://github.com/mlpack/ensmallen/pull/239)).

# RcppEnsmallen 0.2.15.0.1

- Upgraded to ensmallen 2.15.0: "No Direction Home" (2020-11-03)
  - Make a few tests more robust
    ([#228](https://github.com/mlpack/ensmallen/pull/228)).
  - Add release date to version information. ([#226](https://github.com/mlpack/ensmallen/pull/226))
  - Fix typo in release script
    ([#236](https://github.com/mlpack/ensmallen/pull/236)).
  - Add optimizer summary report callback 
    ([#213](https://github.com/mlpack/ensmallen/pull/213)).

# RcppEnsmallen 0.2.14.2.1

- Upgraded to ensmallen 2.14.2: "No Direction Home" (2020-09-05)
  - Fix implementation of fonesca fleming problem function f1 and f2 
    type usage and negative signs. ([#223](https://github.com/mlpack/ensmallen/pull/223))

# RcppEnsmallen 0.2.14.1.1

- Upgraded to ensmallen 2.14.1: "No Direction Home" (2020-08-21)
  - Fix release script (remove hardcoded information, trim leading
    whitespaces introduced by `wc -l` in MacOS)
    ([#216](https://github.com/mlpack/ensmallen/pull/216), [#220](https://github.com/mlpack/ensmallen/pull/220)).
  - Adjust tolerance for AugLagrangian convergence based on element type ([#217](https://github.com/mlpack/ensmallen/pull/217)). 
  - Add NSGA2 optimizer for multi-objective functions ([#149](https://github.com/mlpack/ensmallen/pull/149)). 
  - Update automatic website update release script ([#207](https://github.com/mlpack/ensmallen/pull/207)). 
  - Clarify and fix documentation for constrained optimizers ([#201](https://github.com/mlpack/ensmallen/pull/201)). 
  - Fix L-BFGS convergence when starting from a minimum ([#201](https://github.com/mlpack/ensmallen/pull/201)).
- Switch GitHub Actions to use reference tags to always be up-to-date with 
  CRAN's check grid (`oldrel`, `release`, `devel`). ([#29](https://github.com/coatless/rcppensmallen/pull/29), 
  [#32](https://github.com/coatless/rcppensmallen/pull/32))
- Added a GitHub Action to automatically create a PR with the new version of
  Ensmallen when a new release is detected. ([#30](https://github.com/coatless/rcppensmallen/pull/30), [#33](https://github.com/coatless/rcppensmallen/pull/33))

# RcppEnsmallen 0.2.13.0.1

- Upgraded to ensmallen 2.13.0: "Automatically Automated Automation" (2020-07-15)
  - Fix CMake package export
    ([#198](https://github.com/mlpack/ensmallen/pull/198)).
  - Allow early stop callback to accept a lambda function
    ([#165](https://github.com/mlpack/ensmallen/pull/165)).

# RcppEnsmallen 0.2.12.1.1

- Upgraded to ensmallen 2.12.1: "Stir Crazy" (2020-04-20)
 - Fix total number of epochs and time estimation for ProgressBar callback
 (mlpack/ensmallen#181).
 - Handle SpSubview_col and SpSubview_row in Armadillo 9.870
 (mlpack/ensmallen#194).
 - Minor documentation fixes
 (mlpack/ensmallen#197).
 - Correction in the formulation of sigma in CMA-ES
 (mlpack/ensmallen#183).
 - Remove deprecated methods from PrimalDualSolver implementation
 (mlpack/ensmallen#185).
 (mlpack/ensmallen#186).

# RcppEnsmallen 0.2.11.3.1

- Upgraded to ensmallen 2.11.3: "The Poster Session Is Full" (2020-02-19)
  - Prevent spurious compiler warnings
 (mlpack/ensmallen#161).
  - Fix minor memory leaks
 (mlpack/ensmallen#167).
  - Revamp CMake configuration
 (mlpack/ensmallen#152).
  - Allow callback instantiation for SGD based optimizer 
 (mlpack/ensmallen#155).
  - Minor test stability fixes on i386
 (mlpack/ensmallen#156).
  - Fix Lookahead MaxIterations() check.
 (mlpack/ensmallen#159).

# RcppEnsmallen 0.2.11.1.1

- Upgraded to ensmallen 2.11.1: "The Poster Session Is Full" (2019-12-28)
  - Fix Lookahead Synchronization period type
 (mlpack/ensmallen#153).
  - Add Lookahead
 (mlpack/ensmallen#138).
  - Add AdaBound and AMSBound
 (mlpack/ensmallen#137).
  - SGD callback test 32-bit safety (big number)
 (mlpack/ensmallen#143).
  - Use "arbitrary" and "separable" terms in static function type checks
 (mlpack/ensmallen#145).
  - Remove 'using namespace std' from `problems/` files
 (mlpack/ensmallen#147).
  - Add optional tests building.
 (mlpack/ensmallen#141).
  - Make code samples collapsible in the documentation.
 (mlpack/ensmallen#140).
- Switched deployment from TravisCI to GitHub Actions. ([#17](https://github.com/coatless/rcppensmallen/pull/17), [#22](https://github.com/coatless/rcppensmallen/pull/22))
- Removed check on header file inclusion ([#21](https://github.com/coatless/rcppensmallen/pull/21))

# RcppEnsmallen 0.2.10.3.1

- Upgraded to ensmallen 2.10.3: "Fried Chicken" (2019-09-26)
  - Add release script to rel/ for maintainers
 (mlpack/ensmallen#128).
  - Fix Armadillo version check
 (mlpack/ensmallen#133).
  - Documentation fix for callbacks
 (mlpack/ensmallen#129).
  - Compatibility fixes for ensmallen 1.x
 (mlpack/ensmallen#131).
  - Fix ParallelSGD runtime bug.
 (mlpack/ensmallen#135).
  - Add additional L-BFGS convergence check
 (mlpack/ensmallen#136).
- Added vignette with worked example for linear regression and for package 
  inclusion.

# RcppEnsmallen 0.2.10.0.1

- Upgraded to ensmallen 2.10.0: "Fried Chicken" (2019-09-07)
  - All `Optimize()` functions now take any matrix type; so, e.g., `arma::fmat`
    or `arma::sp_mat` can be used for optimization.  See the documentation for
 (mlpack/ensmallen#113).
 (mlpack/ensmallen#119).
  - Introduce callback support.  Callbacks can be appended as the last arguments
    of an `Optimize()` call, and can perform custom behavior at different points
    during the optimization.  See the documentation for more details
 (mlpack/ensmallen#119).
  - Slight speedups for `FrankWolfe` optimizer
 (mlpack/ensmallen#127).

# RcppEnsmallen 0.1.16.0.1

- Upgraded to ensmallen release 1.16.0 "Loud Alarm Clock" (2019-08-09)
  - Add option to avoid computing exact objective at the end of the optimization
 (mlpack/ensmallen#109).
 (mlpack/ensmallen#118).
 (mlpack/ensmallen#118).
  - Introduce local-best particle swarm optimization, `LBestPSO`, for
 (mlpack/ensmallen#86).
- (mlpack/ensmallen#123).

# RcppEnsmallen 0.1.15.0.1

- Upgraded to ensmallen release 1.15.0 "Wrong Side Of The Road" (2019-05-14)
 (mlpack/ensmallen#81).
- (mlpack/ensmallen#115).
- (mlpack/ensmallen#115).

# RcppEnsmallen 0.1.14.4.1

- Upgraded to ensmallen release 1.14.4 "Difficult Crimp" (2019-05-12)
 (mlpack/ensmallen#91).
 (mlpack/ensmallen#100).
 (mlpack/ensmallen#97).
 (mlpack/ensmallen#98).
 (mlpack/ensmallen#92).

# RcppEnsmallen 0.1.14.1.1

- Upgraded to ensmallen release 1.14.1 "Difficult Crimp" (2019-03-09)
 (mlpack/ensmallen#87).
 (mlpack/ensmallen#90).
     in CNE to be a normal distribution about the given starting point, 
     which should accelerate convergence.
 (mlpack/ensmallen#77).
 (mlpack/ensmallen#83).

# RcppEnsmallen 0.1.13.0.1

- Upgraded to ensmallen release 1.13.0 "Coronavirus Invasion" (2019-01-14)
 (mlpack/ensmallen#66).
 (mlpack/ensmallen#69).
   - Fix list of contributors.
   - Make sure all files end with newlines.
- Reordered SPSA parameters to quiet initialization error surfaced with `-Wreorder`.

# RcppEnsmallen 0.1.12.0.1

- Upgraded to ensmallen release 1.12.0 "New Year's Party" (2018-12-30)
   - Add link to ensmallen PDF to README.md.
   - Minor documentation fixes.  Remove too-verbose documentation from source for
 (mlpack/ensmallen#61).
 (mlpack/ensmallen#48).
 (mlpack/ensmallen#42).
 (mlpack/ensmallen#46).
 (mlpack/ensmallen#45).
 (mlpack/ensmallen#60).
- Updated citation information

# RcppEnsmallen 0.1.11.1.1

- Upgraded to ensmallen release 1.11.1 "Jet Lag" (2018-11-28)
    - Minor documentation fixes.
    - Add WNGrad optimizer.
    - Fix header name in documentation samples.
- Added citation information

# RcppEnsmallen 0.1.10.0.1

## New

- Support for compiling via _R_ the mathematical optimizers in the _C++_
  [ensmallen](http://ensmallen.org/docs.html) library.
- Includes ensmallen release 1.10.0 "Corporate Catabolism" (2018-10-19)

## Special Thanks

- The crux of this package has largely been developed by 
  Ensmallen development team that consists of Conrad Sanderson, Ryan Curtin, 
  and the MLPACK developers
