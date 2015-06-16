/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

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
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff, serge.iovleff@stkpp.org
 **/

#include "mixt_AugmentedData.h"

namespace mixt
{

template<>
Range<Real>::Range(Real min,
                   Real max,
                   bool hasRange) :
    min_(min),
    max_(max),
    range_(max - min),
    hasRange_(hasRange)
{}

template<>
Range<int>::Range(int min,
                  int max,
                  bool hasRange) :
    min_(min),
    max_(max),
    range_(max - min + 1), // used to store the number of modalities, for example,
    hasRange_(hasRange)
{}

template<>
void AugmentedData<Vector<Real> >::removeMissing()
{
  for (int i = 0; i < misData_.rows(); ++i)
  {
    if (misData_(i).first != present_)
    {
      Real sampleVal;
      switch(misData_(i).first) // (iterator on map)->(mapped element).(MisType)
      {
        case present_:
        {}
        break;

        case missing_:
        {
          Real min = dataRange_.min_;
          Real max = dataRange_.max_;
          sampleVal = uniform_.sample(min,
                                      max);
        }
        break;

        case missingFiniteValues_: // no missing finite values for continuous data
        {}
        break;

        case missingIntervals_:
        {
#ifdef MC_DEBUG
          std::cout << "AugmentedData<Matrix<Real> >::removeMissing" << std::endl;
          std::cout << "case missingIntervals_" << std::endl;
          std::cout << "misData_(i).second.size(): " << misData_(i).second.size() << std::endl;
#endif
          Real infBound = misData_(i).second[0]; // (iterator on map)->(mapped element).(vector of parameters)[element]
          Real supBound = misData_(i).second[1];
          sampleVal = uniform_.sample(infBound,
                                      supBound);
        }
        break;

        case missingLUIntervals_:
        {
          Real min = dataRange_.min_;
          Real supBound = misData_(i).second[0];
          if (min < supBound)
          {
            sampleVal = uniform_.sample(min,
                                        supBound);
          }
          else
          {
            sampleVal = supBound;
          }
        }
        break;

        case missingRUIntervals_:
        {
          Real infBound = misData_(i).second[0];
          Real max = dataRange_.max_;
          if (infBound < max)
          {
            sampleVal = uniform_.sample(infBound,
                                        max);
          }
          else
          {
            sampleVal = infBound;
          }
        }
        break;
      }
      data_(i) = sampleVal;
    }
  }
}

template<>
void AugmentedData<Vector<int> >::removeMissing()
{
#ifdef MC_DEBUG
  std::cout << "AugmentedData<Matrix<int> >::removeMissing" << std::endl;
#endif

  for (int i = 0; i < misData_.rows(); ++i)
  {
    if (misData_(i).first != present_)
    {
      int sampleVal;
      int nbModalities = dataRange_.range_;
#ifdef MC_DEBUG
      std::cout << "i: " << i << ", j: " << j << std::endl;
      std::cout << "firstModality: " << firstModality << ", nbModalities: " << nbModalities << std::endl;
#endif
      switch(misData_(i).first) // (iterator on map)->(mapped element).(MisType)
      {
        case present_:
        {}
        break;

        case missing_:
        {
          Vector<Real> modalities(nbModalities);
          modalities = 1. / nbModalities;
          sampleVal = multi_.sample(modalities);
        }
        break;

        case missingFiniteValues_:
        {
          Real proba = 1. / misData_(i).second.size(); // (iterator on map)->(mapped element).(vector of parameters)
          Vector<Real> modalities(nbModalities);
          modalities = 0.;
          for(std::vector<int>::const_iterator itParam = misData_(i).second.begin();
              itParam != misData_(i).second.end();
              ++itParam)
          {
#ifdef MC_DEBUG
          std::cout << "\tproba: " << proba << std::endl;
#endif
            modalities[*itParam] = proba;
          }
          sampleVal = multi_.sample(modalities);
        }
        break;

        default: // other types of intervals not present in integer data
        {}
        break;
      }
      data_(i) = sampleVal;
    }
  }
}

} // namespace mixt
