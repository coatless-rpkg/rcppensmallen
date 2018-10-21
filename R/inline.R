## Copyright (C) 2018 - Present Dirk Eddelbuettel and James Balamuta
##
## This file is part of RcppEnsmallen.
##
## RcppEnsmallen is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 2 of the License, or
## (at your option) any later version.
##
## RcppEnsmallen is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with RcppEnsmallen.  If not, see <http://www.gnu.org/licenses/>.

inlineCxxPlugin <- function(...) {
    is_macos <- Sys.info()[["sysname"]] == "Darwin"
    openmp_flag <- if (is_macos) "" else "$(SHLIB_OPENMP_CFLAGS)"
    plugin <-
        Rcpp::Rcpp.plugin.maker(
            include.before = "#include <RcppEnsmallen.h>",
            libs           = paste(openmp_flag, "$(LAPACK_LIBS) $(BLAS_LIBS) $(FLIBS)"),
            LinkingTo      = c("RcppArmadillo", "Rcpp", "RcppEnsmallen"),
            package        = "RcppEnsmallen"
        )
    settings <- plugin()
    settings$env$PKG_CPPFLAGS <- paste("-I../inst/include", openmp_flag)
    # C++11 is required
    settings$env$USE_CXX11 <- "yes"

    settings
}
