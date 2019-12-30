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
- Switched deployment from TravisCI to GitHub Actions.

# RcppEnsmallen 0.2.10.3.1

- Upgraded to ensmallen 2.10.3: "Fried Chicken" (2019-09-26)
  - Add release script to rel/ for maintainers
    ([#128](https://github.com/mlpack/ensmallen/pull/128)).
  - Fix Armadillo version check
    ([#133](https://github.com/mlpack/ensmallen/pull/133)).
  - Documentation fix for callbacks
    ([#129](https://github.com/mlpack/ensmallen/pull/129).
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
    more details ([#113](https://github.com/mlpack/ensmallen/pull/113),
    [#119](https://github.com/mlpack/ensmallen/pull/119)).
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
  - Fix handling of curvature for BigBatchSGD ([#118](https://github.com/mlpack/ensmallen/pull/118)).
  - Reduce runtime of tests ([#118](https://github.com/mlpack/ensmallen/pull/118)).
  - Introduce local-best particle swarm optimization, `LBestPSO`, for
    unconstrained optimization problems ([#86](https://github.com/mlpack/ensmallen/pull/86)).
- Fix return type error in `PSO` ([#123](https://github.com/mlpack/ensmallen/pull/123))

# RcppEnsmallen 0.1.15.0.1

- Upgraded to ensmallen release 1.15.0 "Wrong Side Of The Road" (2019-05-14)
   - Added QHAdam and QHSGD optimizers ([#81](https://github.com/mlpack/ensmallen/pull/81)).
- Fix `-Wreorder` in `qhadam` warning ([#115](https://github.com/mlpack/ensmallen/pull/115)).
- Fix `-Wunused-private-field` warning in `spsa` ([#115](https://github.com/mlpack/ensmallen/pull/115)).

# RcppEnsmallen 0.1.14.4.1

- Upgraded to ensmallen release 1.14.4 "Difficult Crimp" (2019-05-12)
   - Fixes for BigBatchSGD ([#91](https://github.com/mlpack/ensmallen/pull/91)).
   - Handle eig_sym() failures correctly ([#100](https://github.com/mlpack/ensmallen/pull/100)).
   - SPSA test tolerance fix ([#97](https://github.com/mlpack/ensmallen/pull/97)).
   - Minor documentation fixes ([#95](https://github.com/mlpack/ensmallen/pull/95), [#98](https://github.com/mlpack/ensmallen/pull/98)).
   - Fix newlines at end of file ([#92](https://github.com/mlpack/ensmallen/pull/92)).

# RcppEnsmallen 0.1.14.1.1

- Upgraded to ensmallen release 1.14.1 "Difficult Crimp" (2019-03-09)
   - Fixes for SPSA ([#87](https://github.com/mlpack/ensmallen/pull/87)).
   - Optimized CNE and DE ([#90](https://github.com/mlpack/ensmallen/pull/90)). Changed initial population generation 
     in CNE to be a normal distribution about the given starting point, 
     which should accelerate convergence.
   - Add DE optimizer ([#77](https://github.com/mlpack/ensmallen/pull/77)).
   - Fix for Cholesky decomposition in CMAES ([#83](https://github.com/mlpack/ensmallen/pull/83)).

# RcppEnsmallen 0.1.13.0.1

- Upgraded to ensmallen release 1.13.0 "Coronavirus Invasion" (2019-01-14)
   - Enhance options for AugLagrangian optimizer ([#66](https://github.com/mlpack/ensmallen/pull/66)).
   - Add SPSA optimizer ([#69](https://github.com/mlpack/ensmallen/pull/69)).
   - Fix list of contributors.
   - Make sure all files end with newlines.
- Reordered SPSA parameters to quiet initialization error surfaced with `-Wreorder`.

# RcppEnsmallen 0.1.12.0.1

- Upgraded to ensmallen release 1.12.0 "New Year's Party" (2018-12-30)
   - Add link to ensmallen PDF to README.md.
   - Minor documentation fixes.  Remove too-verbose documentation from source for
     each optimizer ([#61](https://github.com/mlpack/ensmallen/pull/61)).
   - Add FTML optimizer ([#48](https://github.com/mlpack/ensmallen/pull/48)).
   - Add SWATS optimizer ([#42](https://github.com/mlpack/ensmallen/pull/42)).
   - Add Padam optimizer ([#46](https://github.com/mlpack/ensmallen/pull/46)).
   - Add Eve optimizer ([#45](https://github.com/mlpack/ensmallen/pull/45)).
   - Add ResetPolicy() to SGD-like optimizers ([#60](https://github.com/mlpack/ensmallen/pull/60)).
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

