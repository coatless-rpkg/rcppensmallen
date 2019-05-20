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

