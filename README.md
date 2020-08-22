# RcppEnsmallen

<!-- badges: start -->
[![R build status](https://github.com/coatless/rcppensmallen/workflows/R-CMD-check/badge.svg)](https://github.com/coatless/rcppensmallen/actions)
[![License](https://eddelbuettel.github.io/badges/GPL2+.svg)](https://www.gnu.org/licenses/gpl-2.0.html) 
[![CRAN](https://www.r-pkg.org/badges/version/RcppEnsmallen)](https://cran.r-project.org/package=RcppEnsmallen) 
[![Downloads](https://cranlogs.r-pkg.org/badges/RcppEnsmallen?color=brightgreen)](https://www.r-pkg.org/pkg/RcppEnsmallen)
<!-- badges: end -->

## Overview

[Ensmallen](https://ensmallen.org/) is a C++ header-only mathematical optimization
library written by developers behind [MLPACK](https://www.mlpack.org/) that 
provides a simple set of abstractions for writing an objective function to 
optimize. Provided within are various standard and cutting-edge optimizers that
include full-batch gradient descent techniques, small-batch techniques, 
gradient-free optimizers, and constrained optimization.

The RcppEnsmallen package includes the header files from the Ensmallen library
and pairs the appropriate header files from 
[Armadillo](http://arma.sourceforge.net/) through the 
[RcppArmadillo](https://cran.r-project.org/package=RcppArmadillo) package. 
Therefore, users do not need to install Ensmallen nor Armadillo to use RcppEnsmallen. 

## Status

The package is under active development with releases to [CRAN](https://cran.r-project.org/)
about once a month.

### Authors

James Balamuta and Dirk Eddelbuettel

### License

GPL (>= 2)
