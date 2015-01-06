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
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_GAUSSIAN_SJK_H
#define MIXT_GAUSSIAN_SJK_H

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DVector.h"
#include "../../Statistic/mixt_NormalStatistic.h"

namespace mixt
{

class Gaussian_sjk
{
  public:
    typedef STK::Real Type;

    Gaussian_sjk(int nbCluster);
    ~Gaussian_sjk();

    int computeNbFreeParameters() const;

    void getParameters(STK::Array2DVector<STK::Real>& param) const;

    double lnComponentProbability(int i, int k) const;

    /** Set the parameters after the SEM, to the mean estimates for example */
    void setParameters(const STK::Array2DVector<STK::Real>& param);

    void setData(STK::Array2D<Type>& data);

    void initializeModel();

    void initializeStep();

    /** Algorithm based on http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Incremental_algorithm
     * using the biased estimator which corresponds to the maximum likelihood estimator */
    std::string mStep();

    int nbVariable() const;

    void setMixtureParameters(STK::Array2DVector<int> const* p_zi);

    void setModalities(int nbModalities);

    void writeParameters(std::ostream& out) const;
  private:
    int nbCluster_;
    STK::Array2DVector<STK::Real> param_;
    STK::Array2D<Type>* p_data_;
    STK::Array2DVector<int> const* p_zi_;

    /** Statistic object to describe Poisson law */
    NormalStatistic normal_;
};

} // namespace mixt

#endif