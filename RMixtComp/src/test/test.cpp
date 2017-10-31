/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Feb 25, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <RInside.h> // for the embedded R via RInside
#include <fstream>

int main(int argc, char *argv[]) {
	RInside R(argc, argv); // create an embedded R instance

	//  R.parseEvalQ("setwd(\"/media/sf_Documents/160803 - debug MixtComp/171004 - Etienne Goffinet\")");
	//  R.parseEvalQ("source(\"run.r\")");

	R.parseEvalQ("library(RMixtComp)");
	R.parseEvalQ("testCategorical()");

	exit(0);
}
