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
 *  Created on: Jan 17, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_Imputer.h
 *  @brief In this file 
 **/

#ifndef MIXT_IMPUTER_H_
#define MIXT_IMPUTER_H_

#include "stkpp/projects/STatistiK/include/MersenneTwister.h"

namespace mixt
{

template <typename DataType>
class Imputer
{
  public:
    /** sample uniformly from a closed range [infVal, supVal] */
    DataType sampleRange(DataType infVal, DataType supVal);
    /** sample uniformly from a list of values */
    DataType sampleVals(std::vector<DataType> listVals);
  private:
    MTRand mtrand_;
};

template<>
STK::Real Imputer<STK::Real>::sampleRange(STK::Real infVal, STK::Real supVal)
{
  return mtrand_.randExc(supVal - infVal) + infVal;
}

template<>
int Imputer<int>::sampleRange(int infVal, int supVal)
{
  return mtrand_.randInt(supVal - infVal) + infVal;
}

} /* namespace mixt */

#endif /* MIXT_IMPUTER_H_ */
