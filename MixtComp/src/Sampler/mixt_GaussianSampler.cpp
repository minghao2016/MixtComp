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
 *  Created on: Feb 18, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianSampler.h"

namespace mixt
{
GaussianSampler::GaussianSampler(AugmentedData<Matrix<Real> >* p_augData,
                                 const Vector<Real>* p_param,
                                 int nbClass) :
    p_augData_(p_augData),
    p_param_(p_param)
{}

GaussianSampler::~GaussianSampler()
{}

void GaussianSampler::sampleIndividual(int i, int z_i)
{
#ifdef MC_DEBUG
  std::cout << "GaussianSampler::sampleIndividual" << std::endl;
  std::cout << "\ti: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (p_augData_->misData_(i, 0).first != present_)
  {
    Real z;
    Real mean  = p_param_->elt(2 * z_i    );
    Real sd    = p_param_->elt(2 * z_i + 1);

#ifdef MC_DEBUG
    std::cout << "\tmean: " << mean << ", sd: " << sd << std::endl;
#endif

    switch(p_augData_->misData_(i, 0).first)
    {
      case missing_:
      {
#ifdef MC_DEBUG
        std::cout << "\tmissing_" << std::endl;
#endif
        z = normal_.sample(0.,
                           1.);
      }
      break;

      case missingIntervals_:
      {
#ifdef MC_DEBUG
        std::cout << "\tmissingIntervals_" << std::endl;
#endif
        Real infBound = p_augData_->misData_(i, 0).second[0];
        Real supBound = p_augData_->misData_(i, 0).second[1];

        z = normal_.sampleI(mean,
                            sd,
                            infBound,
                            supBound);
      }
      break;

      case missingLUIntervals_: // missingLUIntervals
      {
#ifdef MC_DEBUG
        std::cout << "\tmissingLUIntervals_" << std::endl;
#endif
        Real supBound = p_augData_->misData_(i, 0).second[0];
        z = normal_.sampleSB(mean,
                             sd,
                             supBound);
      }
      break;

      case missingRUIntervals_: // missingRUIntervals
      {
#ifdef MC_DEBUG
        std::cout << "\tmissingRUIntervals_" << std::endl;
#endif
        Real infBound = p_augData_->misData_(i, 0).second[0];
        z = normal_.sampleIB(mean,
                             sd,
                             infBound);
      }
      break;

      default:
      {
#ifdef MC_DEBUG
        std::cout << "\tunsupported missing value type" << std::endl;
#endif
      }
      break;
    }

#ifdef MC_DEBUG
    std::cout << "\tsampled val: " << z * sd + mean << std::endl;
#endif
    p_augData_->data_(i, 0) = z * sd + mean;
  }
}
} // namespace mixt
