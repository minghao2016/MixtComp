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

#ifndef MIXT_CATEGORICAL_PJK
#define MIXT_CATEGORICAL_PJK

#include <vector>
#include <set>

#include "../../LinAlg/mixt_LinAlg.h"
#include "../../Various/mixt_Enum.h"
#include "../../Mixture/mixt_IMixture.h"

namespace mixt
{

class Categorical_pjk
{
  public:
    Categorical_pjk(const std::string& idName,
                    int nbClass,
                    Vector<Real>& param,
                    const Vector<std::set<int> >& classInd);

    Vector<bool> acceptedType() const;
    bool checkMaxVal() const;
    bool checkMinVal() const;
    int computeNbFreeParameters() const;
    bool hasModalities() const;
    int nbModality() const;
    int maxVal() const;
    int minVal() const;
    std::string model() const;
    void mStep(EstimatorType bias);
    std::vector<std::string> paramNames() const;
    void setData(Vector<int>& data);
    void setModalities(int nbModalities);
    void writeParameters() const;

    int checkSampleCondition(std::string* warnLog = NULL) const;
  private:
    std::string idName_;
    int nbClass_;
    int nbModality_;
    Vector<int>* p_data_;
    Vector<Real>& param_;
    const Vector<std::set<int> >& classInd_;
};

} // namespace mixt

#endif
