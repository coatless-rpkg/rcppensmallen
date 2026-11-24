# RcppEnsmallen: Header-Only C++ Mathematical Optimization Library for 'Armadillo'

'Ensmallen' is a templated C++ mathematical optimization library (by the
'MLPACK' team) that provides a simple set of abstractions for writing an
objective function to optimize. Provided within are various standard and
cutting-edge optimizers that include full-batch gradient descent
techniques, small-batch techniques, gradient-free optimizers, and
constrained optimization. The 'RcppEnsmallen' package includes the
header files from the 'Ensmallen' library and pairs the appropriate
header files from 'armadillo' through the 'RcppArmadillo' package.
Therefore, users do not need to install 'Ensmallen' nor 'Armadillo' to
use 'RcppEnsmallen'. Note that 'Ensmallen' is licensed under 3-Clause
BSD, 'Armadillo' starting from 7.800.0 is licensed under Apache License
2, 'RcppArmadillo' (the 'Rcpp' bindings/bridge to 'Armadillo') is
licensed under the GNU GPL version 2 or later. Thus, 'RcppEnsmallen' is
also licensed under similar terms. Note that 'Ensmallen' requires a
compiler that supports 'C++14' and 'Armadillo' 10.8.2 or later.

## See also

Useful links:

- <https://github.com/coatless-rpkg/rcppensmallen>

- <https://r-pkg.thecoatlessprofessor.com/rcppensmallen/>

- <https://github.com/mlpack/ensmallen>

- <https://ensmallen.org/>

- Report bugs at <https://github.com/coatless-rpkg/rcppensmallen/issues>

## Author

**Maintainer**: James Joseph Balamuta <balamut2@illinois.edu>
([ORCID](https://orcid.org/0000-0003-2826-8458)) \[copyright holder\]

Authors:

- Dirk Eddelbuettel <edd@debian.org>
  ([ORCID](https://orcid.org/0000-0001-6419-907X)) \[copyright holder\]
