/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 6, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SGRAPHVISPRINT_H
#define SGRAPHVISPRINT_H

#include "mixt_MixtComp.h"

namespace mixt {

class SGraphVisPrint: public boost::static_visitor<std::string> {
public:
	std::string operator()(Index i) const;

	std::string operator()(const Real r) const;

	std::string operator()(const std::string& str) const;

	std::string operator()(const NamedVector<Real>& vec) const;

	std::string operator()(const NamedMatrix<Real>& mat) const;
};

}

#endif
