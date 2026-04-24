/**
 * @file delta_bar_delta.hpp
 * @author Ranjodh Singh
 *
 * Class wrapper for the DeltaBarDelta update policy.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_DELTA_BAR_DELTA_HPP
#define ENSMALLEN_DELTA_BAR_DELTA_HPP

#include <ensmallen_bits/gradient_descent/gradient_descent.hpp>
#include "update_policies/delta_bar_delta_update.hpp"

namespace ens {

/**
 * DeltaBarDelta optimizer.
 *
 * A heuristic designed to accelerate convergence by
 * adapting the learning rate of each parameter individually.
 *
 * According to the Delta-Bar-Delta update:
 *
 * - If the current gradient and the exponential average of
 *   past gradients corresponding to a parameter have the same
 *   sign, then the step size for that parameter is incremented by
 *   \f$\kappa\f$. Otherwise, it is decreased by a proportion \f$\phi\f$
 *   of its current value (additive increase, multiplicative decrease).
 *
 * @note This implementation uses a minStepSize parameter to set a lower
 *     bound for the learning rate. This prevents the learning rate from
 *     dropping to zero, which can occur due to floating-point underflow.
 *     For tasks which require extreme fine-tuning, you may need to lower
 *     this parameter below its default value (1e-8) in order to allow for
 *     smaller learning rates.
 *
 * @code
 * @article{jacobs1988increased,
 *   title     = {Increased Rates of Convergence Through Learning Rate
 *                Adaptation},
 *   author    = {Jacobs, Robert A.},
 *   journal   = {Neural Networks},
 *   volume    = {1},
 *   number    = {4},
 *   pages     = {295--307},
 *   year      = {1988},
 *   publisher = {Pergamon}
 * }
 * @endcode
 */
class DeltaBarDelta
{
 public:
  /**
   * Construct the DeltaBarDelta optimizer with the given function and
   * parameters. DeltaBarDelta is very sensitive to its parameters (kappa
   * and phi) hence a good hyperparameter selection is necessary as its
   * default may not fit every case.
   *
   * @param stepSize Step size (initial).
   * @param maxIterations Maximum number of iterations allowed (0 means no
   *     limit).
   * @param tolerance Maximum absolute tolerance to terminate algorithm.
   * @param kappa Additive increase constant for step size.
   * @param phi Multiplicative decrease factor for step size.
   * @param theta Decay rate for the exponential moving average.
   * @param minStepSize Minimum allowed step size for any parameter
   *     (default: 1e-8).
   * @param resetPolicy If true, parameters are reset before every Optimize
   *     call; otherwise, their values are retained.
   */
  DeltaBarDelta(const double stepSize = 1.0,
                const size_t maxIterations = 100000,
                const double tolerance = 1e-5,
                const double kappa = 0.2,
                const double phi = 0.2,
                const double theta = 0.5,
                const double minStepSize = 1e-8,
                const bool resetPolicy = true);

  /**
   * Optimize the given function using DeltaBarDelta.
   * The given starting point will be modified to store the finishing
   * point of the algorithm, and the final objective value is returned.
   *
   * @tparam SeparableFunctionType Type of the function to optimize.
   * @tparam MatType Type of matrix to optimize with.
   * @tparam GradType Type of matrix to use to represent function gradients.
   * @tparam CallbackTypes Types of callback functions.
   * @param function Function to optimize.
   * @param iterate Starting point (will be modified).
   * @param callbacks Callback functions.
   * @return Objective value of the final point.
   */
  template<typename SeparableFunctionType,
           typename MatType,
           typename GradType,
           typename... CallbackTypes>
  typename std::enable_if<IsMatrixType<GradType>::value,
      typename MatType::elem_type>::type
  Optimize(SeparableFunctionType& function,
           MatType& iterate,
           CallbackTypes&&... callbacks)
  {
    return optimizer.Optimize<SeparableFunctionType, MatType, GradType,
        CallbackTypes...>(function, iterate,
        std::forward<CallbackTypes>(callbacks)...);
  }

  //! Forward the MatType as GradType.
  template<typename SeparableFunctionType,
           typename MatType,
           typename... CallbackTypes>
  typename MatType::elem_type Optimize(SeparableFunctionType& function,
                                       MatType& iterate,
                                       CallbackTypes&&... callbacks)
  {
    return Optimize<SeparableFunctionType, MatType, MatType,
        CallbackTypes...>(function, iterate,
        std::forward<CallbackTypes>(callbacks)...);
  }

  //! Get the initial step size.
  double StepSize() const { return optimizer.StepSize(); }
  //! Modify the initial step size.
  double& StepSize() { return optimizer.StepSize(); }

  //! Get the maximum number of iterations (0 indicates no limit).
  size_t MaxIterations() const { return optimizer.MaxIterations(); }
  //! Modify the maximum number of iterations (0 indicates no limit).
  size_t& MaxIterations() { return optimizer.MaxIterations(); }

  //! Get the additive increase constant for step size.
  double Kappa() const { return optimizer.UpdatePolicy().Kappa(); }
  //! Modify the additive increase constant for step size.
  double& Kappa() { return optimizer.UpdatePolicy().Kappa(); }

  //! Get the multiplicative decrease factor for step size.
  double Phi() const { return optimizer.UpdatePolicy().Phi(); }
  //! Modify the multiplicative decrease factor for step size.
  double& Phi() { return optimizer.UpdatePolicy().Phi(); }

  //! Get the decay rate for the exponential moving average.
  double Theta() const { return optimizer.UpdatePolicy().Theta(); }
  //! Modify the decay rate for the exponential moving average.
  double& Theta() { return optimizer.UpdatePolicy().Theta(); }

  //! Get the minimum allowed step size for any parameter.
  double MinStepSize() const { return optimizer.UpdatePolicy().MinStepSize(); }
  //! Modify the minimum allowed step size for any parameter.
  double& MinStepSize() { return optimizer.UpdatePolicy().MinStepSize(); }

  //! Get the tolerance for termination.
  double Tolerance() const { return optimizer.Tolerance(); }
  //! Modify the tolerance for termination.
  double& Tolerance() { return optimizer.Tolerance(); }

  //! Get whether or not the update policy parameters are reset before
  //! Optimize call.
  bool ResetPolicy() const { return optimizer.ResetPolicy(); }
  //! Modify whether or not the update policy parameters are reset before
  //! Optimize call.
  bool& ResetPolicy() { return optimizer.ResetPolicy(); }

 private:
  //! The GradientDescentType object with DeltaBarDelta policy.
  GradientDescentType<DeltaBarDeltaUpdate, NoDecay> optimizer;
};

} // namespace ens

// Include implementation.
#include "delta_bar_delta_impl.hpp"

#endif // ENSMALLEN_DELTA_BAR_DELTA_HPP
