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
 *  Created on: Sept 3, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff, serge.iovleff@stkpp.org,
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file mixt_MixtureStrategy.h
 *  @brief In this file we define the strategy to use in order to estimate a
 *  mixture model.
 **/

#ifndef MIXT_MIXTURESTRATEGY_H
#define MIXT_MIXTURESTRATEGY_H

#include "stkpp/projects/Clustering/include/STK_Clust_Util.h"

namespace mixt
{
/**
 *  A SemStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 **/
class SemStrategy
{
  public:
    /** default constructor.
     *  @param p_composer the model to estimate
     **/
    SemStrategy(STK::IMixtureComposerBase*& p_composer,
                STK::Clust::initType init,
                int nbTrialInInit,
                int nbBurnInIter,
                int nbIter);

    /** copy constructor
     *  @param strategy the strategy to copy
     **/
    SemStrategy(SemStrategy const& strategy);
    
    /** destructor */
    virtual ~SemStrategy();
    
    /** run the strategy */
    bool run();
    
  protected:
     /** reference on the main model */
    STK::IMixtureComposerBase*& p_composer_;
    /** initialization method */
    STK::IMixtureInit* p_init_;
    
    /** number of estimation to try */
    int nbTry_;
    /** algorithm for burn-in */
    STK::IMixtureAlgo* p_burnInAlgo_;
    /** algorithm for subsequent long run */
    STK::IMixtureAlgo* p_longAlgo_;
};

}  // namespace mixt

#endif /* MIXT_MIXTURESTRATEGY_H_ */
