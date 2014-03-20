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
 *  Created on: March 18, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_MIXTUREMANAGER_H
#define MIXT_MIXTUREMANAGER_H

#include "../Mixture/StkppMixture/mixt_GaussianMixture.h"

namespace mixt
{

template<class DataHandler>
class MixtureManager
{
  public:
    typedef STK::IDataHandler::InfoMap InfoMap;
    typedef std::vector<STK::IMixture*>::const_iterator ConstMixtIterator;
    typedef std::vector<STK::IMixture*>::iterator MixtIterator;

    MixtureManager(DataHandler const& handler) : handler_(handler) {}

    void createMixtures(STK::MixtureComposer& composer, int nbCluster)
    {
      for (InfoMap::const_iterator it=handler_.info().begin(); it!=handler_.info().end(); ++it)
      {
        std::string idName = it->first;
        std::string model= it->second;
        STK::Clust::Mixture idModel = STK::Clust::stringToMixture(model);
        // get a mixture fully
        STK::IMixture* p_mixture = createMixture(idModel, idName, nbCluster);
        if (p_mixture) composer.registerMixture(p_mixture);
      }
    }

    /** create a mixture and initialize it*
     *  @param idModel id of the model
     *  @param idName name of the model
     *  @param nbCluster number of cluster of the model
     **/
    STK::IMixture* createMixture(STK::Clust::Mixture idModel, std::string const& idName, int nbCluster)
    {
      switch (idModel)
      {
        case STK::Clust::Gaussian_sjk_:
        {
          MixtureGaussian_sjk_m* p_mixture = new MixtureGaussian_sjk_m(idName, nbCluster);
          p_mixture->setData(this);
          return p_mixture;
        }
        break;
        default:
          return 0;
        break;
      }
      return 0; // 0 if idModel is not a stk++ model
    }

    /** Get data from DataHandler
     *  @param idName name of the model
     *  @param data set to fill
     *  @param nbVariable number of variable of the model
     **/
    template<typename Data>
    inline void getData(std::string const& idName, Data& data, int& nbVariable) const
    {handler_.getData(idName, data, nbVariable);}

  private:
    /** pointer to the dataHandler */
    DataHandler const& handler_;
};

} // namespace mixt

#endif /* MIXT_MIXTUREMANAGER_H */