/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JMixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"

TEST(DataHandlerJson, listData_getData) {
    std::string in = "toto";
    str::string out = "toto";

    ASSERT_EQ(in, out);
}
