/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: June 1, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Function.h"

namespace mixt {

void Function::setSize(Index nT,
                       Index nSub) {
  nTime_ = nT;
  nSub_ = nSub;
  t_.resize(nT);
  x_.resize(nT);
  w_.resize(nSub);
}

void Function::setVal(const Vector<Real>& t,
                      const Vector<Real>& x,
                      const Vector<std::list<Index> >& w) {
  nTime_ = t.size();
  nSub_ = w.size();
  t_ = t;
  x_ = x;
  w_ = w;
}

void Function::setValTime(Index i,
                          Real t,
                          Real x) {
  t_(i) = t;
  x_(i) = x;
}

void Function::computeVandermonde(Index nCoeff) {
  vandermondeMatrix(t_, nCoeff, vandermonde_);
}

void Function::computeJointLogProba(const Matrix<Real>& alpha,
                                    const Matrix<Real>& beta,
                                    const Vector<Real>& sd,
                                    Matrix<Real>& jointLogProba) const {
  jointLogProba.resize(nTime_, nSub_);
  NormalStatistic normal;
  Vector<Real> currKappa(nSub_);
  for (Index i = 0; i < nTime_; ++i) {
    kappaMatrix(t_(i), alpha, currKappa);

    for (Index s = 0; s < nSub_; ++s) {
      Real currExpectation = vandermonde_.row(i).dot(beta.row(s)); // since the completed probability is computed, only the current subregression is taken into account in the computation
      Real logAPosteriori = normal.lpdf(x_(i), currExpectation, sd(s));

      Real logAPriori = std::log(currKappa(s));

      jointLogProba(i, s) = logAPriori + logAPosteriori;
    }
  }
}

Real Function::lnCompletedProbability(const Matrix<Real>& alpha,
                                      const Matrix<Real>& beta,
                                      const Vector<Real>& sd) const {
  Real logProba = 0.;

  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);
  for (Index s = 0; s < nSub_; ++s) {
    for (std::list<Index>::const_iterator it  = w_(s).begin(),
                                          itE = w_(s).end();
         it != itE;
         ++it) {
      logProba += jointLogProba(*it, s); // only the completed value of w is taken into account
    }
  }

  return logProba;
}

Real Function::lnObservedProbability(const Matrix<Real>& alpha,
                                     const Matrix<Real>& beta,
                                     const Vector<Real>& sd) const {
  Real logProba = 0.;

  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);
  for (Index i = 0; i < nTime_; ++i) {
    for (Index s = 0; s < nSub_; ++s) {
      logProba += jointLogProba(i, s); // every possible value of w is taken into account, to marginalize over w
    }
  }

  return logProba;
}

void Function::sampleW(const Matrix<Real>& alpha,
                       const Matrix<Real>& beta,
                       const Vector<Real>& sd) {
  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);

  for (Index s = 0; s < nSub_; ++s) {
    w_(s).clear();
  }

  Vector<Real> currProba;
  for (Index i = 0; i < nTime_; ++i) {
    currProba.logToMulti(jointLogProba.row(i));
    w_(multi_.sample(currProba)).push_back(i);
  }
}

void Function::removeMissing() {
  for (Index s = 0; s < nSub_; ++s) {
    w_(s).clear();
  }

  for (Index i = 0; i < nTime_; ++i) {
    w_(multi_.sampleInt(0, nSub_ - 1)).push_back(i);
  }
}

double Function::costAndGrad(unsigned nParam,
                             const double* alpha,
                             double* grad) const {
  double cost;
  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;

  timeValue(t_,
            nParam,
            alpha,
            logValue,
            logSumExpValue);

  costFunction(t_,
               logValue,
               logSumExpValue,
               w_,
               cost);

  if (grad != NULL) {
    gradCostFunction(t_,
                     logValue,
                     logSumExpValue,
                     w_,
                     grad);
  }

  return cost;
}

} // namespace mixt