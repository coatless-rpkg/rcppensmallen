# RcppEnsmallen 0.2.20.0.1

- Upgraded to ensmallen 2.20.0: "Eight Ball Deluxe" (2023-10-05)
  - Implementation of Active CMAES
    ([#367](https://github.com/mlpack/ensmallen/pull/367)).
  - LBFGS: avoid generation of NaNs, and add checks for finite values
    ([#368](https://github.com/mlpack/ensmallen/pull/368)).
  - Fix CNE test tolerances
    ([#360](https://github.com/mlpack/ensmallen/pull/360)).
  - Rename `SCD` optimizer, to `CD`
    ([#379](https://github.com/mlpack/ensmallen/pull/379)).
- Updated GitHub Action runners `update-ensmallen-refresh` and `pkgdown` to address deprecation notices.
    ([#53](https://github.com/coatless-rpkg/rcppensmallen/53), [#54](https://github.com/coatless-rpkg/rcppensmallen/54), [#55](https://github.com/coatless-rpkg/rcppensmallen/55)).
- Removed explicit C++11 requirement from `DESCRIPTION` and `Makevars{.win}`.

# RcppEnsmallen 0.2.19.1.1

- Upgraded to ensmallen 2.19.1: "Eight Ball Deluxe" (2023-02-08)
 - Avoid deprecation warnings in Armadillo 11.2+
    ([#347](https://github.com/mlpack/ensmallen/pull/347)).
- Updated GitHub Action's runner to the latest versions 
    ([#51](https://github.com/coatless-rpkg/rcppensmallen/51)).

# RcppEnsmallen 0.2.19.0.1

- Upgraded to ensmallen 2.19.0: "Eight Ball Deluxe" (2022-04-11)
 - Added DemonSGD and DemonAdam optimizers
    ([#211](https://github.com/mlpack/ensmallen/pull/211)).
  - Fix bug with Adam-like optimizers not resetting when `resetPolicy` is `true`.
    ([#340](https://github.com/mlpack/ensmallen/pull/340)).
  - Add Yogi optimizer
    ([#232](https://github.com/mlpack/ensmallen/pull/232)).
  - Add AdaBelief optimizer
    ([#233](https://github.com/mlpack/ensmallen/pull/233)).
  - Add AdaSqrt optimizer
    ([#234](https://github.com/mlpack/ensmallen/pull/234)).
    
  - Bump check for minimum supported version of Armadillo
    ([#342](https://github.com/mlpack/ensmallen/pull/342)).

# RcppEnsmallen 0.2.18.2.1

- Upgraded to ensmallen 2.18.2: "Fairmount Bagel" (2022-02-14)
  - Update Catch2 to 2.13.8
   ([#336](https://github.com/mlpack/ensmallen/pull/336)).
  - Fix epoch timing output
   ([#337](https://github.com/mlpack/ensmallen/pull/337)).
- Update _R_ package URLs.

# RcppEnsmallen 0.2.18.1.1

- Upgraded to ensmallen 2.18.1: "Fairmount Bagel" (2021-11-20)
  - Accelerate SGD test time
    ([#330](https://github.com/mlpack/ensmallen/pull/300)).
  - Fix potential infinite loop in CMAES
    ([#331](https://github.com/mlpack/ensmallen/pull/331)).
  - Fix SCD partial gradient test
    ([#332](https://github.com/mlpack/ensmallen/pull/332)).

# RcppEnsmallen 0.2.18.0.1

- Upgraded to ensmallen 2.18.0: "Fairmount Bagel" (2021-10-21)
  - Add gradient value clipping and gradient norm scaling callback
    ([#315](https://github.com/mlpack/ensmallen/pull/315)).
  - Remove superfluous CMake option to build the tests
    ([#313](https://github.com/mlpack/ensmallen/pull/313)).
  - Bump minimum Armadillo version to 9.800
    ([#318](https://github.com/mlpack/ensmallen/pull/318)).
  - Update Catch2 to 2.13.7
    ([#322](https://github.com/mlpack/ensmallen/pull/322)).
  - Remove redundant template argument for C++20 compatibility
    ([#324](https://github.com/mlpack/ensmallen/pull/324)).
  - Fix MOEAD test stability
    ([#327](https://github.com/mlpack/ensmallen/pull/327)).
- Update GitHub Actions to the standard `r-lib/actions` configuration.
  ([#45](https://github.com/coatless-rpkg/rcppensmallen/pull/45))

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
    spurious issue on some aarch64 ([#259](https://github.com/mlpack/ensmallen/pull/259)).

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
  CRAN's check grid (`oldrel`, `release`, `devel`). ([#29](https://github.com/coatless-rpkg/rcppensmallen/pull/29), 
  [#32](https://github.com/coatless-rpkg/rcppensmallen/pull/32))
- Added a GitHub Action to automatically create a PR with the new version of
  Ensmallen when a new release is detected. ([#30](https://github.com/coatless-rpkg/rcppensmallen/pull/30), [#33](https://github.com/coatless-rpkg/rcppensmallen/pull/33))

# RcppEnsmallen 0.2.13.0.1

- Upgraded to ensmallen 2.13.0: "Automatically Automated Automation" (2020-07-15)
  - Fix CMake package export
    ([#198](https://github.com/mlpack/ensmallen/pull/198)).
  - Allow early stop callback to accept a lambda function
    ([#165](https://github.com/mlpack/ensmallen/pull/165)).

# RcppEnsmallen 0.2.12.1.1

- Upgraded to ensmallen 2.12.1: "Stir Crazy" (2020-04-20)
 - Fix total number of epochs and time estimation for ProgressBar callback
 ([#181](https://github.com/mlpack/ensmallen/pull/181)).
 - Handle SpSubview_col and SpSubview_row in Armadillo 9.870
 ([#194](https://github.com/mlpack/ensmallen/pull/194)).
 - Minor documentation fixes
 ([#197](https://github.com/mlpack/ensmallen/pull/197)).
 - Correction in the formulation of sigma in CMA-ES
 ([#183](https://github.com/mlpack/ensmallen/pull/183)).
 - Remove deprecated methods from PrimalDualSolver implementation
 ([#185](https://github.com/mlpack/ensmallen/pull/185)).
 ([#186](https://github.com/mlpack/ensmallen/pull/186)).

# RcppEnsmallen 0.2.11.3.1

- Upgraded to ensmallen 2.11.3: "The Poster Session Is Full" (2020-02-19)
  - Prevent spurious compiler warnings
 ([#161](https://github.com/mlpack/ensmallen/pull/161)).
  - Fix minor memory leaks
 ([#167](https://github.com/mlpack/ensmallen/pull/167)).
  - Revamp CMake configuration
 ([#152](https://github.com/mlpack/ensmallen/pull/152)).
  - Allow callback instantiation for SGD based optimizer 
 ([#155](https://github.com/mlpack/ensmallen/pull/155)).
  - Minor test stability fixes on i386
 ([#156](https://github.com/mlpack/ensmallen/pull/156)).
  - Fix Lookahead MaxIterations() check.
 ([#159](https://github.com/mlpack/ensmallen/pull/159)).

# RcppEnsmallen 0.2.11.1.1

- Upgraded to ensmallen 2.11.1: "The Poster Session Is Full" (2019-12-28)
  - Fix Lookahead Synchronization period type
 ([#153](https://github.com/mlpack/ensmallen/pull/153)).
  - Add Lookahead
 ([#138](https://github.com/mlpack/ensmallen/pull/138)).
  - Add AdaBound and AMSBound
 ([#137](https://github.com/mlpack/ensmallen/pull/137)).
  - SGD callback test 32-bit safety (big number)
 ([#143](https://github.com/mlpack/ensmallen/pull/143)).
  - Use "arbitrary" and "separable" terms in static function type checks
 ([#145](https://github.com/mlpack/ensmallen/pull/145)).
  - Remove 'using namespace std' from `problems/` files
 ([#147](https://github.com/mlpack/ensmallen/pull/147)).
  - Add optional tests building.
 ([#141](https://github.com/mlpack/ensmallen/pull/141)).
  - Make code samples collapsible in the documentation.
 ([#140](https://github.com/mlpack/ensmallen/pull/140)).
- Switched deployment from TravisCI to GitHub Actions. ([#17](https://github.com/coatless-rpkg/rcppensmallen/pull/17), [#22](https://github.com/coatless-rpkg/rcppensmallen/pull/22))
- Removed check on header file inclusion ([#21](https://github.com/coatless-rpkg/rcppensmallen/pull/21))

# RcppEnsmallen 0.2.10.3.1

- Upgraded to ensmallen 2.10.3: "Fried Chicken" (2019-09-26)
  - Add release script to rel/ for maintainers
 ([#128](https://github.com/mlpack/ensmallen/pull/128)).
  - Fix Armadillo version check
 ([#133](https://github.com/mlpack/ensmallen/pull/133)).
  - Documentation fix for callbacks
 ([#129](https://github.com/mlpack/ensmallen/pull/129)).
  - Compatibility fixes for ensmallen 1.x
 ([#131](https://github.com/mlpack/ensmallen/pull/131)).
  - Fix ParallelSGD runtime bug.
 ([#135](https://github.com/mlpack/ensmallen/pull/135)).
  - Add additional L-BFGS convergence check
 ([#136](https://github.com/mlpack/ensmallen/pull/136)).
- Added vignette with worked example for linear regression and for package 
  inclusion.

# RcppEnsmallen 0.2.10.0.1

- Upgraded to ensmallen 2.10.0: "Fried Chicken" (2019-09-07)
  - All `Optimize()` functions now take any matrix type; so, e.g., `arma::fmat`
    or `arma::sp_mat` can be used for optimization.  See the documentation for
 ([#113](https://github.com/mlpack/ensmallen/pull/113)).
 ([#119](https://github.com/mlpack/ensmallen/pull/119)).
  - Introduce callback support.  Callbacks can be appended as the last arguments
    of an `Optimize()` call, and can perform custom behavior at different points
    during the optimization.  See the documentation for more details
 ([#119](https://github.com/mlpack/ensmallen/pull/119)).
  - Slight speedups for `FrankWolfe` optimizer
 ([#127](https://github.com/mlpack/ensmallen/pull/127)).

# RcppEnsmallen 0.1.16.0.1

- Upgraded to ensmallen release 1.16.0 "Loud Alarm Clock" (2019-08-09)
  - Add option to avoid computing exact objective at the end of the optimization
 ([#109](https://github.com/mlpack/ensmallen/pull/109)).
 ([#118](https://github.com/mlpack/ensmallen/pull/118)).
 ([#118](https://github.com/mlpack/ensmallen/pull/118)).
  - Introduce local-best particle swarm optimization, `LBestPSO`, for
 ([#86](https://github.com/mlpack/ensmallen/pull/86)).
- ([#123](https://github.com/mlpack/ensmallen/pull/123)).

# RcppEnsmallen 0.1.15.0.1

- Upgraded to ensmallen release 1.15.0 "Wrong Side Of The Road" (2019-05-14)
 ([#81](https://github.com/mlpack/ensmallen/pull/81)).
- ([#115](https://github.com/mlpack/ensmallen/pull/115)).
- ([#115](https://github.com/mlpack/ensmallen/pull/115)).

# RcppEnsmallen 0.1.14.4.1

- Upgraded to ensmallen release 1.14.4 "Difficult Crimp" (2019-05-12)
 ([#91](https://github.com/mlpack/ensmallen/pull/91)).
 ([#100](https://github.com/mlpack/ensmallen/pull/100)).
 ([#97](https://github.com/mlpack/ensmallen/pull/97)).
 ([#98](https://github.com/mlpack/ensmallen/pull/98)).
 ([#92](https://github.com/mlpack/ensmallen/pull/92)).

# RcppEnsmallen 0.1.14.1.1

- Upgraded to ensmallen release 1.14.1 "Difficult Crimp" (2019-03-09)
 ([#87](https://github.com/mlpack/ensmallen/pull/87)).
 ([#90](https://github.com/mlpack/ensmallen/pull/90)).
     in CNE to be a normal distribution about the given starting point, 
     which should accelerate convergence.
 ([#77](https://github.com/mlpack/ensmallen/pull/77)).
 ([#83](https://github.com/mlpack/ensmallen/pull/83)).

# RcppEnsmallen 0.1.13.0.1

- Upgraded to ensmallen release 1.13.0 "Coronavirus Invasion" (2019-01-14)
 ([#66](https://github.com/mlpack/ensmallen/pull/66)).
 ([#69](https://github.com/mlpack/ensmallen/pull/69)).
   - Fix list of contributors.
   - Make sure all files end with newlines.
- Reordered SPSA parameters to quiet initialization error surfaced with `-Wreorder`.

# RcppEnsmallen 0.1.12.0.1

- Upgraded to ensmallen release 1.12.0 "New Year's Party" (2018-12-30)
   - Add link to ensmallen PDF to README.md.
   - Minor documentation fixes.  Remove too-verbose documentation from source for
 ([#61](https://github.com/mlpack/ensmallen/pull/61)).
 ([#48](https://github.com/mlpack/ensmallen/pull/48)).
 ([#42](https://github.com/mlpack/ensmallen/pull/42)).
 ([#46](https://github.com/mlpack/ensmallen/pull/46)).
 ([#45](https://github.com/mlpack/ensmallen/pull/45)).
 ([#60](https://github.com/mlpack/ensmallen/pull/60)).
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
