## Test environments

* local macOS install, R 4.4.2
* ubuntu 20.04 (with GitHub Actions), R 4.4.2
* win-builder (devel and release)

## R CMD check results

0 errors | 0 warnings | 0 note

## Reverse dependencies

With this update, only `sparsevb` failed to install due to the C++14 requirement. 
This is due to the `RcppEnsmallen` package now requiring C++14. I have 
contacted the maintainer of `sparsevb` to update their package to work with
C++14 and provided a patch to do so. However, I have not received a response
yet. 

> https://gitlab.com/gclara/varpack/-/issues/2
> https://gitlab.com/gclara/varpack/-/merge_requests/1

Per discussions with Kurt, it is fine to temporarily break `sparsevb` in order
to allow `mlpack` to be updated to the latest version.

### revdepcheck results

✔ HDJM 0.1.0                             ── E: 0     | W: 0     | N: 0    
✔ mlpack 4.5.0                           ── E: 0     | W: 1     | N: 1    
✔ PJFM 0.1.0                             ── E: 0     | W: 0     | N: 0    
✔ Racmacs 1.2.9                          ── E: 0     | W: 0     | N: 1    
I sparsevb 0.1.0                         ── E: 0  +1 | W: 0     | N: 0-1  
✔ VBJM 0.1.0                             ── E: 0     | W: 0     | N: 0   
