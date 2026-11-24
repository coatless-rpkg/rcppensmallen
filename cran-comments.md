## Test environments

* local: macOS 27.2.1 (aarch64), R 4.6.2
* GitHub Actions:
  - macos-latest, R release
  - windows-latest, R release
  - ubuntu-latest, R devel / release / oldrel-1

## R CMD check results

0 errors | 0 warnings | 0 notes

## Reverse dependencies

We checked all 5 reverse dependencies (mlpack, PJFM, Racmacs,
rcppmlpackexamples, sparsevb), comparing R CMD check results across the CRAN
and development versions of this package.

 * We saw 0 new problems
 * We failed to check 0 packages

### revdepcheck results

    ✔ mlpack 4.8.0-1              ── E: 0 | W: 0 | N: 1
    ✔ PJFM 0.1.0                  ── E: 0 | W: 0 | N: 1
    ✔ Racmacs 1.2.10              ── E: 0 | W: 0 | N: 1
    ✔ rcppmlpackexamples 0.0.1    ── E: 0 | W: 0 | N: 1
    ✔ sparsevb 0.1.1              ── E: 0 | W: 0 | N: 1

The note on each reverse dependency is its own CRAN incoming feasibility
note. It appears identically against the CRAN and development versions of
RcppEnsmallen and is therefore unrelated to this update.
