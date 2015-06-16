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

#ifndef MIXT_MIXTUREALGO_H
#define MIXT_MIXTUREALGO_H

#include "../Composer/mixt_MixtureComposer.h"

namespace mixt
{

class SEMAlgo
{
  public:
    /** default constructor */
    SEMAlgo(MixtureComposer* p_model,
            int nbIterMax,
            int nbSamplingAttempts);
    /** Copy constructor.
     *  @param algo the algorithm to copy */
    SEMAlgo(SEMAlgo const& algo);
    /** destructor */
    virtual ~SEMAlgo(){};
    /** run the algorithm */
    std::string run(RunType runType,
                    int group,
                    int groupMax);

  private:
    /** pointer on the mixture model */
    MixtureComposer* p_model_;
    /** number of iterations of the algorithm */
    int nbIterMax_;
    /** number of attempts before giving up on sampling */
    int nbSamplingAttempts_;
};
} // namespace mixt

#endif /* MIXT_MIXTUREALGO_H */
