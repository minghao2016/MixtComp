/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: August 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_RankClass.h"

namespace mixt
{
//RankClass::RankClass(int currClass) :
//    currClass_(currClass),
//    nbInd_(0),
//    nbPos_(0),
//    pi_(0.)
//{}

RankClass::RankClass(int nbClass,
                     Vector<RankIndividual>& data,
                     const RankVal& mu,
                     Real pi) :
    currClass_(nbClass),
    nbInd_(data.size()),
    nbPos_(mu.getNbPos()),
    data_(data),
    mu_(mu),
    pi_(pi)
{}

void RankClass::removeMissing()
{
  for (int i = 0; i < nbInd_; ++i)
  {
    data_(i).removeMissing();
  }
}

Real RankClass::lnCompletedProbability() const
{
  Real logProba = 0.;
  int a, g; // a and g are only used in the mStep, here they are dummy variables

  for (int i = 0; i < nbInd_; ++i)
  {
    logProba += data_(i).lnCompletedProbability(mu_, pi_, a, g);
  }

  return logProba;
}

Real RankClass::lnCompletedProbability(int& a, int& g) const
{
  Real logProba = 0.;
  a = 0;
  g = 0;

  for (int i = 0; i < nbInd_; ++i)
  {
    int currA, currG;
    logProba += data_(i).lnCompletedProbability(mu_, pi_, currA, currG);
    a += currA;
    g += currG;
  }

  return logProba;
}

/**
 * Perform one round of Gibbs sampling for the central rank */
void RankClass::sampleMu()
{
  Vector<Real, 2> logProba; // first element: current log proba, second element: logProba of permuted state
  Vector<Real, 2> proba   ; // multinomial distribution obtained from the logProba

  logProba(0) = lnCompletedProbability(); // proba of current mu

  for (int p = 0; p < nbPos_ - 1; ++p)
  {
    mu_.permutation(p);
    logProba(1) = lnCompletedProbability();
    proba.logToMulti(logProba);

#ifdef MC_DEBUG
    std::cout << "p: " << p << ", logProba: " << logProba.transpose() << ", proba: " << proba.transpose() << std::endl;
#endif

    if (multi_.sample(proba) == 1) // switch to permuted state ?
    {
      logProba(0) = logProba(1); // accept permutation
    }
    else
    {
      mu_.permutation(p); // revert to previous state
    }
  }
}

void RankClass::mStep()
{
  Vector<RankVal> mu(nbGibbsIterRank);
  Vector<Real> pi(nbGibbsIterRank);
  Vector<Real> logProba(nbGibbsIterRank);

  int a, g;
  for (int i = 0; i < nbGibbsIterRank; ++i)
  {
    sampleMu();
    mu(i) = mu_;
    logProba(i) = lnCompletedProbability(a, g);
    pi(i) = Real(g) / Real(a);

#ifdef MC_DEBUG
    std::cout << "Rank::mStep, i: " << i << ", mu: " << mu(i).o().transpose() << ", pi: " << pi(i) << ", logProba: " << logProba(i) << std::endl;
#endif
  }

  int bestTheta;
  logProba.maxCoeff(&bestTheta);

  mu_ = mu(bestTheta);
  pi_ = pi(bestTheta);
}

void RankClass::samplingStep(int ind)
{
  data_(ind).sampleY(mu_, pi_);
}
} // namespace mixt
