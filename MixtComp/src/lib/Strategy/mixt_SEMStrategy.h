/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Sept 3, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file mixt_MixtureStrategy.h
 *  @brief In this file we define the strategy to use in order to estimate a
 *  mixture model.
 **/

#ifndef LIB_STRATEGY_MIXT_SEMSTRATEGY_H
#define LIB_STRATEGY_MIXT_SEMSTRATEGY_H

#include <Composer/mixt_MixtureComposer.h>
#include <Various/mixt_Timer.h>
#include <Various/mixt_Various.h>

namespace mixt {

/**
 *  A SemStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 **/
template<typename Graph>
class SemStrategy {

public:
	/** default constructor.
	 *  @param p_composer the model to estimate
	 **/
	SemStrategy(MixtureComposer& composer, const Graph& algo) :
			composer_(composer), algo_(algo) {
	}

	/** run the strategy */
	std::string run() {
		std::string warnLog;

		Index nSemTry = algo_.template get_payload<Index>( { }, "nSemTry");
		Index nInitPerClass = algo_.template get_payload<Index>( { }, "nInitPerClass");
		Index nbBurnInIter = algo_.template get_payload<Index>( { }, "nbBurnInIter");
		Index nbIter = algo_.template get_payload<Index>( { }, "nbIter");

		try {
			for (Index n = 0; n < nSemTry; ++n) {
				std::cout << "SemStrategy::run, attempt n: " << n << std::endl;
				warnLog.clear(); // only the last warn log can be sent

				composer_.initData(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
				warnLog = composer_.checkNbIndPerClass(); // useless because a new check is performed in initParam TODO: remove if really useless
				if (0 < warnLog.size()) {
					std::cout << "Not enough individuals per class." << std::endl;
					continue;
				}
				//		p_composer_->printClassInd();

				composer_.initParam(); // initialize iterative estimators
				warnLog = composer_.initParamSubPartition(nInitPerClass); // initialize parameters for each model, usually singling out an observation as the center of each class
				if (0 < warnLog.size()) {
					std::cout << "initParam failed." << std::endl;
					continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further
				}

				std::cout << "SemStrategy::run, initParam succeeded." << std::endl;

				composer_.writeParameters(); // for debugging purposes

				warnLog = composer_.initializeLatent(); // use observed probability to initialize classes
				if (0 < warnLog.size()) {
					continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further
				}

				std::cout << "SemStrategy::run initializeLatent succeeded." << std::endl;
				std::cout << "SEM initialization complete. SEM run can start." << std::endl;

				warnLog = runSEM(burnIn_, nbBurnInIter, 0, // group
						3); // groupMax
				if (0 < warnLog.size())
					continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further

				warnLog = runSEM(run_, nbIter, 1, // group
						3); // groupMax
				if (0 < warnLog.size())
					continue;

				return ""; // at the moment, stop the loop at the first completed run, this will evolve later
			}
		} catch (const std::string& str) {
			warnLog = str;
		}

		return warnLog;
	}

	/**
	 * run the algorithm, only kept during the transition, as an archive
	 * @return string describing the problem in case of soft degeneracy */
	std::string runSEM(RunType runType, Index nIter, int group, int groupMax) {
		std::string warnLog;

		Timer myTimer;

		if (runType == burnIn_) {
			std::cout << "Burnin Iterations begin" << std::endl;
			myTimer.setName("SEM: burn-in");
		} else if (runType == run_) {
			std::cout << "Iterations begins" << std::endl;
			myTimer.setName("SEM: run");
		}

		for (Index iter = 0; iter < nIter; ++iter) {
			std::cout << "Iteration " << iter + 1 << std::endl;
			myTimer.iteration(iter, nIter - 1);
#ifdef MC_PROGRESS
			writeProgress(group, groupMax, iter, nIter - 1);
#endif

			composer_.eStepCompleted();
			composer_.sampleZ(); // no checkSampleCondition performed, to increase speed of sampling
			composer_.sampleUnobservedAndLatent();

			std::string warnLog = composer_.checkSampleCondition(); // since we are not in initialization, no need for log
			if (0 < warnLog.size()) {
				std::cout << "runSEM, checkSampleCondition failed." << std::endl;
				return warnLog;
			}

			warnLog += composer_.mStep(); // biased or unbiased does not matter, as there has been a check on sampling conditions previously
			if (0 < warnLog.size()) {
				std::cout << "runSEM, mStep failed." << std::endl;
				return warnLog;
			}
			//		p_composer_->writeParameters();

			composer_.storeSEMRun(iter, nIter - 1, runType);
		}

		return "";
	}

private:
	/** reference on the main model */
	MixtureComposer& composer_;

	const Graph& algo_;
};

}  // namespace mixt

#endif /* MIXT_MIXTURESTRATEGY_H_ */
