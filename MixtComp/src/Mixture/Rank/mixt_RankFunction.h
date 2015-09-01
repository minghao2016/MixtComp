/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKFUNCTION_H
#define MIXT_RANKFUNCTION_H

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

namespace RankFunction
{

/** Recursive computation of */
void index2Vector(Vector<std::pair<Vector<int>, Real> >& res,
                  Vector<int>& vec,
                  const std::set<int>& remainingMod,
                  int firstElem,
                  int nbElem,
                  int currPos,
                  int nbPos);

} // namespace RankFunction

} // namespace mixt

#endif // MIXT_RANKFUNCTION_H
