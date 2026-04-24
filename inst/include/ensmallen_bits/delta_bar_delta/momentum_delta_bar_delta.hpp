/**
 * @file momentum_delta_bar_delta.hpp
 * @author Ranjodh Singh
 *
 * Class wrapper for the MomentumDeltaBarDelta update policy.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_MOMENTUM_DELTA_BAR_DELTA_HPP
#define ENSMALLEN_MOMENTUM_DELTA_BAR_DELTA_HPP

#include <ensmallen_bits/gradient_descent/gradient_descent.hpp>
#include "update_policies/momentum_delta_bar_delta_update.hpp"

namespace ens {

/**
 * MomentumDeltaBarDelta Optimizer.
 *
 * A DeltaBarDelta variant that incorporates the following modifications:
 *  - In the original DeltaBarDelta, the momentum term (delta_bar) is used
 *    solely for sign comparison with the current gradient and does not
 *    participate in the parameter update. In this modified variant, the
 *    momentum term (velocity) is directly used to update the parameters.
 *  - Instead of adjusting the step size directly, each parameter maintains
 *    a gain value initialized to 1.0. Updates apply additive increases or
 *    multiplicative decreases to this gain. The effective step size for a
 *    parameter is the product of its initial step size and its current gain.
 *
 * Note: This variant originates from optimization of the t-SNE cost function.
 *
 * @code
 * @article{maaten2008visualizing,
 *   title={Visualizing data using t-SNE},
 *   author={van der Maaten, Laurens and Hinton, Geoffrey},
 *   journal={Journal of machine learning research},
 *   volume={9},
 *   pages={2579--2605},
 *   month={11},
 *   year={2008}
 * }
 * @endcode
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
class MomentumDeltaBarDelta
{
 public:
  /**
   * Construct the MomentumDeltaBarDelta optimizer with the given function and
   * parameters.
   *
   * @param stepSize Step size (initial).
   * @param maxIterations Maximum number of iterations allowed (0 means no
   *     limit).
   * @param tolerance Maximum absolute tolerance to terminate algorithm.
   * @param kappa Additive increase constant for step size.
   * @param phi Multiplicative decrease factor for step size.
   * @param momentum The momentum decay hyperparameter.
   * @param minGain Minimum allowed gain (scaling factor) for any parameter
   *     (default: 1e-8).
   * @param resetPolicy If true, parameters are reset before every Optimize
   *     call; otherwise, their values are retained.
   */
  MomentumDeltaBarDelta(const double stepSize = 1.0,
                        const size_t maxIterations = 100000,
                        const double tolerance = 1e-5,
                        const double kappa = 0.2,
                        const double phi = 0.8,
                        const double momentum = 0.5,
                        const double minGain = 1e-8,
                        const bool resetPolicy = true);

  /**
   * Optimize the given function using MomentumDeltaBarDelta.
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

  //! Get the momentum decay hyperparameter.
  double Momentum() const { return optimizer.UpdatePolicy().Momentum(); }
  //! Modify the momentum decay hyperparameter.
  double& Momentum() { return optimizer.UpdatePolicy().Momentum(); }

  //! Get the minimum allowed gain (scaling factor) for any parameter.
  double MinGain() const { return optimizer.UpdatePolicy().MinGain(); }
  //! Modify the minimum allowed gain (scaling factor) for any parameter.
  double& MinGain() { return optimizer.UpdatePolicy().MinGain(); }

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
  //! The GradientDescentType object with MomentumDeltaBarDelta policy.
  GradientDescentType<MomentumDeltaBarDeltaUpdate, NoDecay> optimizer;
};

} // namespace ens

// Include implementation.
#include "momentum_delta_bar_delta_impl.hpp"

#endif // ENSMALLEN_MOMENTUM_DELTA_BAR_DELTA_HPP
