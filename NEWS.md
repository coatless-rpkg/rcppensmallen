# RcppEnsmallen 0.1.14.1.1

- Upgraded to ensmallen release 1.14.1 "Difficult Crimp" (2019-03-09)
   - Fixes for SPSA (#87).
   - Optimized CNE and DE (#90). Changed initial population generation 
     in CNE to be a normal distribution about the given starting point, 
     which should accelerate convergence.
   - Add DE optimizer (#77).
   - Fix for Cholesky decomposition in CMAES (#83).

# RcppEnsmallen 0.1.13.0.1

- Upgraded to ensmallen release 1.13.0 "Coronavirus Invasion" (2019-01-14)
   - Enhance options for AugLagrangian optimizer (#66).
   - Add SPSA optimizer (#69).
   - Fix list of contributors.
   - Make sure all files end with newlines.
- Reordered SPSA parameters to quiet initialization error surfaced with `-Wreorder`.

# RcppEnsmallen 0.1.12.0.1

- Upgraded to ensmallen release 1.12.0 "New Year's Party" (2018-12-30)
   - Add link to ensmallen PDF to README.md.
   - Minor documentation fixes.  Remove too-verbose documentation from source for
     each optimizer (#61).
   - Add FTML optimizer (#48).
   - Add SWATS optimizer (#42).
   - Add Padam optimizer (#46).
   - Add Eve optimizer (#45).
   - Add ResetPolicy() to SGD-like optimizers (#60).
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

