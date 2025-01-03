/**
 * @file constr_lpball.hpp
 * @author Chenzhe Diao
 *
 * Lp ball constrained for FrankWolfe algorithm. Used as LinearConstrSolverType.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_FW_CONSTR_LPBALL_HPP
#define ENSMALLEN_FW_CONSTR_LPBALL_HPP

namespace ens {

/**
 * LinearConstrSolver for FrankWolfe algorithm. Constraint domain given in the
 * form of lp ball. That is, given \f$ v \f$, solve
 * \f$
 * s:=arg\min_{s\in D} <s, v>
 * \f$
 * when \f$ D \f$ is a regularized lp ball. That is,
 * \f[
 * D = \{ x: (\sum_j|\lambda_j x_j|^p)^{1/p}\leq 1 \}.
 * \f]
 * If \f$ \lambda \f$ is not given in the constructor, default is using all
 * \f$ \lambda_j = 1 \f$ for all \f$ j \f$.
 *
 * In applications such as Orthogonal Matching Pursuit (OMP), \f$ \lambda \f$
 * could be ideally set to the norm of the elements in the dictionary.
 *
 * For \f$ p=1 \f$: take (one) \f$ k = arg\max_j |v_j/\lambda_j|\f$, then the
 * solution is:
 * \f[
 * s_k = -sign(v_k)/\lambda_k, \qquad s_j = 0, \quad j\neq k.
 * \f]
 *
 * For \f$ 1<p<\infty \f$: the solution is
 * \f[
 * t_j = -sign(v_j) |v_j/\lambda_j|^{q-1}, \qquad
 * s_j = \frac{t_j}{||t||_p\cdot\lambda_j}, \quad
 * 1/p + 1/q = 1.
 * \f]
 *
 * For \f$ p=\infty \f$: the solution is
 * \f[
 * s_j = -sign(v_j)/\lambda_j
 * \f]
 *
 */
class ConstrLpBallSolver
{
 public:
  /**
   * Construct the solver of constrained problem. The constrained domain should
   * be unit lp ball for this class.
   *
   * @param p The constraint is unit lp ball.
   */
  ConstrLpBallSolver(const double p) : p(p)
  { /* Do nothing. */ }

  /**
   * Construct the solver of constrained problem, with regularization parameter
   * lambda here.
   *
   * @param p The constraint is unit lp ball.
   * @param lambda Regularization parameter.
   */
  ConstrLpBallSolver(const double p, const arma::vec lambda) :
      p(p), regFlag(true), lambda(lambda)
  { /* Do nothing. */ }


  /**
   * Optimizer of Linear Constrained Problem for FrankWolfe.
   *
   * @param v Input local gradient.
   * @param s Output optimal solution in the constrained domain (lp ball).
   */
  template<typename MatType>
  void Optimize(const MatType& v,
                MatType& s)
  {
    typedef typename MatType::elem_type ElemType;

    if (p == std::numeric_limits<double>::infinity())
    {
      // l-inf ball.
      s = -arma::sign(v);
      if (regFlag)
      {
        // Do element-wise division.
        s /= arma::conv_to<arma::Col<ElemType>>::from(lambda);
      }
    }
    else if (p > 1.0)
    {
      // lp ball with 1<p<inf.
      if (regFlag)
        s = v / arma::conv_to<arma::Col<ElemType>>::from(lambda);
      else
        s = v;

      double q = 1 / (1.0 - 1.0 / p);
      s = -arma::sign(v) % arma::pow(arma::abs(s), q - 1);
      s = arma::normalise(s, p);

      if (regFlag)
        s = s / arma::conv_to<arma::Col<ElemType>>::from(lambda);
    }
    else if (p == 1.0)
    {
      // l1 ball, also used in OMP.
      if (regFlag)
        s = arma::abs(v / arma::conv_to<arma::Col<ElemType>>::from(lambda));
      else
        s = arma::abs(v);

      // k is the linear index of the largest element.
      arma::uword k = s.index_max();
      s.zeros();
      // Take the sign of v(k).
      s(k) = -((0.0 < v(k)) - (v(k) < 0.0));

      if (regFlag)
        s = s / arma::conv_to<arma::Col<ElemType>>::from(lambda);
    }
    else
    {
      Warn << "Wrong norm p!" << std::endl;
    }

    return;
  }

  //! Get the p-norm.
  double P() const { return p; }
  //! Modify the p-norm.
  double& P() { return p;}

  //! Get regularization flag.
  bool RegFlag() const { return regFlag; }
  //! Modify regularization flag.
  bool& RegFlag() { return regFlag; }

  //! Get the regularization parameter.
  arma::vec Lambda() const { return lambda; }
  //! Modify the regularization parameter.
  arma::vec& Lambda() { return lambda; }

 private:
  //! lp norm, 1<=p<=inf;
  //! use std::numeric_limits<double>::infinity() for inf norm.
  double p;

  //! Regularization flag.
  bool regFlag = false;

  //! Regularization parameter.
  arma::vec lambda;
};

} // namespace ens

#endif
