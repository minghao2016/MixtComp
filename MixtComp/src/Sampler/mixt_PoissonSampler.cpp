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
 *  Created on: December 12, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_PoissonSampler.h"
#include "../Mixture/mixt_IMixture.h"

namespace mixt
{
PoissonSampler::PoissonSampler(const IMixture& mixture,
                               AugmentedData<Vector<int> >& augData,
                               const Vector<Real>& param,
                               int nbClass) :
    mixture_(mixture),
    augData_(augData),
    param_(param)
{}

void PoissonSampler::samplingStepCheck(int i,
                                       int z_i)
{
#ifdef MC_DEBUG
  std::cout << "PoissonSampler::sampleIndividual" << std::endl;
  std::cout << "\ti: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (augData_.misData_(i).first != present_)
  {
    augData_.data_(i) = 0;
    bool nonZeroSample = !mixture_.checkSampleCondition(); // check if the 0 value is authorized for this individual in the sample

    int x = -1;
    Real lambda = param_(z_i);

#ifdef MC_DEBUG
    std::cout << "\tlambda: " << lambda << std::endl;
#endif

    switch(augData_.misData_(i).first)
    {
      case missing_:
      {
#ifdef MC_DEBUG
        std::cout << "\tmissing_" << std::endl;
#endif
        if (nonZeroSample == false)
        {
          x = poisson_.sample(lambda);
        }
        else
        {
#ifdef MC_DEBUG
          std::cout << "PoissonSampler::samplingStepCheck, nonZeroSample" << std::endl;
#endif
          x = poisson_.nonZeroSample(lambda);
        }
      }
      break;

      default:
      {}
      break;
    }

#ifdef MC_DEBUG
    std::cout << "\tsampled val: " << x << std::endl;
#endif
    augData_.data_(i) = x;
  }
}

void PoissonSampler::samplingStepNoCheck(int i,
                                         int z_i)
{
#ifdef MC_DEBUG
  std::cout << "PoissonSampler::sampleIndividual" << std::endl;
  std::cout << "\ti: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (augData_.misData_(i).first != present_)
  {
    int x;
    Real lambda = param_(z_i);

#ifdef MC_DEBUG
    std::cout << "\tlambda: " << lambda << std::endl;
#endif

    switch(augData_.misData_(i).first)
    {
      case missing_:
      {
#ifdef MC_DEBUG
        std::cout << "\tmissing_" << std::endl;
#endif
        x = poisson_.sample(lambda);
      }
      break;

      default:
      {}
      break;
    }

#ifdef MC_DEBUG
    std::cout << "\tsampled val: " << x << std::endl;
#endif
    augData_.data_(i) = x;
  }
}

} // namespace mixt
