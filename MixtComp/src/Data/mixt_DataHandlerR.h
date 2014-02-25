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
 *  Created on: Nov 21, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_DataHandlerR.h
 *  @brief In this file 
 **/

#ifndef MIXT_DATAHANDLERR_H_
#define MIXT_DATAHANDLERR_H_

#include <vector>
#include <Rcpp.h>
#include "stkpp/projects/DManager/include/STK_IDataHandler.h"
#include "mixt_AugmentedData.h"

namespace mixt
{

/** combination of a sample number and column number to identify a value */
typedef std::pair<int, int> pos;

class DataHandlerR: public STK::IDataHandler
{
  public:
    /** map: id -> vector of indices in rList_ */
    typedef std::map<std::string, std::vector<int> > DataMap;

    /** default constructor */
    DataHandlerR();
    /** destructor */
    virtual ~DataHandlerR();

    /** @return the number of samples (the number of rows of the data) */
    inline virtual int nbSample() const { return nbSamples_;}
        /** @return the number of variables (the number of columns of the data) */
    inline virtual int nbVariable() const {return nbVariables_;};

    /** read a data file and its companion description file,
      and fill the infoMap_ (id -> model) and dataMap_ (id -> vector of positions)
      members */
    bool readDataFromRList(Rcpp::List);

    /** return in an Array2D<int> the data with the given idData */
    virtual void getData(std::string const& idData, STK::Array2D<int>& data, int& nbVariable) const;
    /** return in an Array2D<Real> the data with the given idData */
    virtual void getData(std::string const& idData, STK::Array2D<STK::Real>& data, int& nbVariable) const;
    /** return in an Array2D<std::string> the data with the given idData */
    virtual void getData(std::string const& idData, STK::Array2D<std::string>& data, int& nbVariable) const;
    
    /** return in an AugmentedData object the missing values of various types */
    void getData(std::string const& idData,
                 AugmentedData<STK::Array2D<STK::Real> >& data,
                 int& nbVariable) const;
    
    /** write information on the localization of data in the rList */
    void writeDataMap() const;
  private:
    int nbSamples_;
    int nbVariables_;
    /** map: id -> vector of positions in rList_, as typedef-ed above */
    DataMap dataMap_;
    /** A list of the mixtures transmitted by R, as s4 objects */
    Rcpp::List rList_;
    
    /** read data structure independently of the type (integer, numeric, character) */
    template<class RcppClass>
    void readDataFromRListHelper(int i, Rcpp::S4 s4);
};

template<class RcppClass>
void DataHandlerR::readDataFromRListHelper(int i, Rcpp::S4 s4)
{
  std::string modelname = s4.slot("model");
  std::string id = s4.slot("id");
  Rcpp::List ls = s4.slot("augData");
  Rcpp::NumericVector nv = ls["data"];
  
  nbSamples_ = nv.size(); // overwritten, because check has already been performed on the R side
  addInfo(id, modelname);
  std::vector<int>& v_pos = dataMap_[id]; // dataMap_[id] created if not already existing
  v_pos.push_back(i);
  ++nbVariables_;
}

} /* namespace mixt */
#endif /* MIXT_DATAHANDLERR_H_ */