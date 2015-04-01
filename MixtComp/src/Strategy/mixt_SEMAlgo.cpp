/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: Mar 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

#include "mixt_SEMAlgo.h"
#include "../Various/mixt_Constants.h"
#include "../Various/mixt_Timer.h"
#include "../IO/mixt_IO.h"

namespace mixt
{

SEMAlgo::SEMAlgo(MixtureComposer* p_model,
                 int nbIterMax,
                 int nbSamplingAttempts) :
    p_model_(p_model),
    nbIterMax_(nbIterMax),
    nbSamplingAttempts_(nbSamplingAttempts)
{}

SEMAlgo::SEMAlgo(SEMAlgo const& algo) :
    p_model_(algo.p_model_),
    nbIterMax_(algo.nbIterMax_),
    nbSamplingAttempts_(algo.nbSamplingAttempts_)
{}

std::string SEMAlgo::run(RunType runType)
{
#ifdef MC_DEBUG
  std::cout << "SEMAlgo::run, entering" << std::endl;
#endif

  Timer myTimer;

  if (runType == burnIn_)
  {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, initial partition export" << std::endl;
    std::cout << "SEMAlgo::run, p_model_->storeShortRun" << std::endl;
#endif
    myTimer.setName("SEM burn-in");
    p_model_->storeSEMBurnIn(-1,
                             nbIterMax_ - 1); // export of the initial partition
  }
  else if (runType == longRun_)
  {
    myTimer.setName("SEM run");
  }

  for (int iter = 0; iter < nbIterMax_; ++iter)
  {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, iter: " << iter << std::endl;
#endif

    myTimer.iteration(iter, nbIterMax_);
    // SE steps (e followed by s)
    if (runType == burnIn_
        && (iter / moduloMisClass > 0)
        && (iter % moduloMisClass == 0)) // perform an eStep to remove class locking
    {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, p_model_->misClasStep" << std::endl;
#endif
      p_model_->misClasStep(iter);
    }
    else // perform a standard eStep
    {
      p_model_->eStep();
    }
    for (int iterSample = 0; iterSample < nbSamplingAttempts_; ++iterSample) // sample until there are enough individuals per class
    {
#ifdef MC_DEBUG
      std::cout << "\titerSample: " << iterSample << std::endl;
#endif
      int nbIndPerClass = p_model_->sStep();
      if (nbIndPerClass > minIndPerClass)
      {
        break; // enough individuals in each class to carry on
      }
      else
      {
        if (iterSample == nbSamplingAttempts_ - 1) // on last attempt, exit with error message
        {
          std::stringstream sstm;
          sstm << "Sampling step problem in SEM. The class with the lowest number "
               << "of individuals has " << nbIndPerClass << " individuals. Each class must have at least "
               << minIndPerClass << " individuals. There has been " << nbSamplingAttempts
               << " partition samplings before failure. The number of classes might be too important"
               << " relative to the number of individuals." << std::endl;
          return sstm.str();
        }
      }
    }
    p_model_->samplingStep();

    // M step
    std::string warn = p_model_->mStep();
    if (warn.size() > 0)
    {
      return warn; // error reported in the mStep, terminate the SEM algo, and report it to the strategy.
    }

    // storage steps
    if (runType == burnIn_)
    {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, p_model_->storeShortRun" << std::endl;
#endif
      p_model_->storeSEMBurnIn(iter,
                            nbIterMax_ - 1);
    }

    if (runType == longRun_)
    {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, p_model_->storeLongRun" << std::endl;
#endif
      p_model_->storeSEMRun(iter,
                           nbIterMax_ - 1);
    }
  }

  return std::string(); // success: return empty string
}

} // namespace mixt
