/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 11, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_GIBBSSTRATEGY_H
#define MIXT_GIBBSSTRATEGY_H

#include "../Composer/mixt_MixtureComposer.h"
#include "../Strategy/mixt_StrategyParam.h"

namespace mixt {
/**
 *  A SemStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 **/
class GibbsStrategy {
  public:
    /** default constructor.
     *  @param p_composer the model to estimate
     **/
    GibbsStrategy(MixtureComposer* p_composer,
                  const StrategyParam& param);
    
    /** run the strategy */
    std::string run();
    
  protected:
     /** reference on the main model */
    mixt::MixtureComposer* p_composer_;

    const StrategyParam& param_;
};

}  // namespace mixt

#endif /* MIXT_GIBBSSTRATEGY_H */
