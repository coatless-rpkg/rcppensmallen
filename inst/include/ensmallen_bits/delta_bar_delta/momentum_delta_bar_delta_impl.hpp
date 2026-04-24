/**
 * @file momentum_delta_bar_delta_impl.hpp
 * @author Ranjodh Singh
 *
 * Implementation of MomentumDeltaBarDelta class wrapper.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_MOMENTUM_DELTA_BAR_DELTA_IMPL_HPP
#define ENSMALLEN_MOMENTUM_DELTA_BAR_DELTA_IMPL_HPP

// In case it hasn't been included yet.
#include "./momentum_delta_bar_delta.hpp"

namespace ens {

inline MomentumDeltaBarDelta::MomentumDeltaBarDelta(
    const double stepSize,
    const size_t maxIterations,
    const double tolerance,
    const double kappa,
    const double phi,
    const double momentum,
    const double minGain,
    const bool resetPolicy) : 
    optimizer(stepSize,
              maxIterations,
              tolerance,
              MomentumDeltaBarDeltaUpdate(kappa, phi, momentum, minGain),
              NoDecay(),
              resetPolicy)
{
  /* Nothing to do. */
}

} // namespace ens

#endif // ENSMALLEN_MOMENTUM_DELTA_BAR_DELTA_IMPL_HPP
