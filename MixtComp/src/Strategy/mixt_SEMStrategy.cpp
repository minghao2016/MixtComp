/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Sept 3, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff, serge.iovleff@stkpp.org
 **/

#include "mixt_SEMStrategy.h"
#include "../Various/mixt_Timer.h"
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Various.h"
#include "../Various/mixt_Constants.h"

namespace mixt {

/** default constructor */
SemStrategy::SemStrategy(
    MixtureComposer* p_composer,
    const StrategyParam& param) :
        p_composer_(p_composer),
        param_(param) {}

std::string SemStrategy::run() {
  std::string warnLog;
  for (Index n = 0; n < nSemTry; ++n) {
    warnLog.clear(); // only the last warn log can be sent

    p_composer_->initData(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
    p_composer_->initParam(); // initialize parameters for each model, usually singling out a single observation as the center of each class

    warnLog = runSEM( //
        burnIn_,
        param_.nbBurnInIter_,
        0, // group
        3); // groupMax
    if (0 < warnLog.size()) continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further

    warnLog = runSEM(
        run_,
        param_.nbIter_,
        1, // group
        3); // groupMax
    if (0 < warnLog.size()) continue;

    // parameters will be saved here, when the option to cache restore / parameters will be implemented

    p_composer_->gibbsSampling(
        callInitDataIfMarkovChain_,
        doNotSampleData_,
        param_.nbGibbsBurnInIter_,
        2, // group
        3); // groupMax

    p_composer_->gibbsSampling(
        doNotCallInitData_,
        sampleData_,
        param_.nbGibbsIter_,
        3, // group
        3); // groupMax

    return warnLog; // at the moment, stop the loop at the first completed run, this will evolve later
  }

  return "";
}

std::string SemStrategy::runSEM(
    RunType runType,
    Index nIter,
    int group,
    int groupMax) {
  std::string warnLog;

  Timer myTimer;

  if (runType == burnIn_) {
    myTimer.setName("SEM: burn-in");
  }
  else if (runType == run_) {
    myTimer.setName("SEM: run");
  }

  for (Index iter = 0; iter < nIter; ++iter) {
    myTimer.iteration(iter, nIter - 1);
    writeProgress(
        group,
        groupMax,
        iter,
        nIter - 1);

    p_composer_->eStep();

    p_composer_->sStepNoCheck(); // no checkSampleCondition performed, to increase speed of sampling
    p_composer_->samplingStepNoCheck(doNotCallInitData_);

    int sampleCond = p_composer_->checkSampleCondition(); // since we are not in initialization, no need for log
    if (sampleCond == 0) {
      return "Error. Full description to be implemented later.";
    }

    p_composer_->mStep(biased_); // biased or unbiased does not matter, as there has been a check on sampling conditions previously

    if (runType == run_) {
      p_composer_->storeSEMRun(iter, nIter - 1);
    }
  }

  return "";
}

} // namespace mixt
