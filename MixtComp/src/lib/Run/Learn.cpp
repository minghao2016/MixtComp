/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 7, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/variant.hpp>
#include <iostream>

#include "Learn.h"
#include <Composer/mixt_MixtureComposer.h>
#include <IO/IOFunctions.h>
#include <Manager/MixtureManager.h>
#include <Statistic/mixt_RNG.h>
#include <Strategy/mixt_SEMStrategy.h>
#include <Strategy/mixt_GibbsStrategy.h>
#include <Various/mixt_Timer.h>

namespace mixt {

/**
 * The learn algorithm is encapsulated in a function.
 */
SGraph learn(const SGraph& algo, const SGraph& data, const SGraph& desc) {
	SGraph res;

	SGraph param = dummySGraph(desc); // dummy SGraph to be provided as unused argument in setDataParam in learning

	std::cout << "MixtComp, learn, version: " << version << std::endl;
	std::cout << "Deterministic mode: " << deterministicMode() << std::endl;
//	std::cout<< "Number of threads: " << omp_get_num_threads() << std::endl;

	Timer totalTimer("Total Run");

	std::string warnLog; // string to log warnings

	// Create the composer and read the data
	MixtureComposer composer(algo, data, param);

	Timer readTimer("Read Data");
	std::cout << "top" << std::endl;
	warnLog += createAllMixtures(algo, desc, composer);
	std::cout << "top" << std::endl;
	warnLog += composer.setDataParam(learning_);
	std::cout << "top" << std::endl;
	readTimer.top("data has been read");

	if (0 < warnLog.size()) {
		res.add_payload("warnLog", warnLog);
		return res;
	}

	// Run the SEM strategy

	SemStrategy semStrategy(composer, algo);
	Timer semStratTimer("SEM Strategy Run");
	warnLog += semStrategy.run();
	semStratTimer.top("SEM strategy run complete");

	if (0 < warnLog.size()) {
		res.add_payload("warnLog", warnLog);
		return res;
	}

	// Run the Gibbs strategy

	GibbsStrategy gibbsStrategy(composer, algo, 2);
	Timer gibbsStratTimer("Gibbs Strategy Run");
	warnLog += gibbsStrategy.run();
	gibbsStratTimer.top("Gibbs strategy run complete");

	if (0 < warnLog.size()) {
		res.add_payload("warnLog", warnLog);
		return res;
	}

	// Run has been successful, export everything

	composer.writeParameters();

	Real runTime = totalTimer.top("end of run");
	std::string mode = "learn";

	SGraph mixture = composer.exportMixture(runTime);

	SGraph variable = composer.exportDataParam();

	res.add_child("algo", algo);
	res.add_child("mixture", mixture);
	res.add_child("variable", variable);

	return res;
}

}
