# sparsevb

<details>

* Version: 0.1.0
* GitHub: NA
* Source code: https://github.com/cran/sparsevb
* Date/Publication: 2021-01-15 09:20:02 UTC
* Number of recursive dependencies: 18

Run `revdepcheck::revdep_details(, "sparsevb")` for more info

</details>

## Newly broken

*   checking whether package ‘sparsevb’ can be installed ... ERROR
    ```
    Installation failed.
    See ‘/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/checks.noindex/sparsevb/new/sparsevb.Rcheck/00install.out’ for details.
    ```

## Newly fixed

*   checking C++ specification ... NOTE
    ```
      Specified C++11: please drop specification unless essential
    ```

## Installation

### Devel

```
* installing *source* package ‘sparsevb’ ...
** package ‘sparsevb’ successfully unpacked and MD5 sums checked
** using staged installation
** libs
using C++ compiler: ‘Apple clang version 16.0.0 (clang-1600.0.26.4)’
using C++11
using SDK: ‘MacOSX15.1.sdk’
clang++ -arch arm64 -std=gnu++11 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG  -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/Rcpp/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/RcppArmadillo/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/RcppEnsmallen/new/RcppEnsmallen/include' -I/opt/R/arm64/include    -fPIC  -falign-functions=64 -Wall -g -O2   -c RcppExports.cpp -o RcppExports.o
In file included from RcppExports.cpp:5:
In file included from /Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/RcppEnsmallen/new/RcppEnsmallen/include/RcppEnsmallen.h:25:
/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/RcppEnsmallen/new/RcppEnsmallen/include/ensmallen.hpp:32:4: error: "*** C++14 compiler required; enable C++14 mode in your compiler, or use an earlier version of ensmallen"
   32 |   #error "*** C++14 compiler required; enable C++14 mode in your compiler, or use an earlier version of ensmallen"
      |    ^
1 error generated.
make: *** [RcppExports.o] Error 1
ERROR: compilation failed for package ‘sparsevb’
* removing ‘/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/checks.noindex/sparsevb/new/sparsevb.Rcheck/sparsevb’


```
### CRAN

```
* installing *source* package ‘sparsevb’ ...
** package ‘sparsevb’ successfully unpacked and MD5 sums checked
** using staged installation
** libs
using C++ compiler: ‘Apple clang version 16.0.0 (clang-1600.0.26.4)’
using C++11
using SDK: ‘MacOSX15.1.sdk’
clang++ -arch arm64 -std=gnu++11 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG  -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/Rcpp/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/RcppArmadillo/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/RcppEnsmallen/old/RcppEnsmallen/include' -I/opt/R/arm64/include    -fPIC  -falign-functions=64 -Wall -g -O2   -c RcppExports.cpp -o RcppExports.o
clang++ -arch arm64 -std=gnu++11 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG  -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/Rcpp/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/RcppArmadillo/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/RcppEnsmallen/old/RcppEnsmallen/include' -I/opt/R/arm64/include    -fPIC  -falign-functions=64 -Wall -g -O2   -c fit_linear.cpp -o fit_linear.o
clang++ -arch arm64 -std=gnu++11 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG  -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/Rcpp/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/sparsevb/RcppArmadillo/include' -I'/Users/ronin/Documents/GitHub/r-pkg/rcppensmallen/revdep/library.noindex/RcppEnsmallen/old/RcppEnsmallen/include' -I/opt/R/arm64/include    -fPIC  -falign-functions=64 -Wall -g -O2   -c fit_logit.cpp -o fit_logit.o
...
** R
** byte-compile and prepare package for lazy loading
** help
*** installing help indices
** building package indices
** testing if installed package can be loaded from temporary location
** checking absolute paths in shared objects and dynamic libraries
** testing if installed package can be loaded from final location
** testing if installed package keeps a record of temporary installation path
* DONE (sparsevb)


```
