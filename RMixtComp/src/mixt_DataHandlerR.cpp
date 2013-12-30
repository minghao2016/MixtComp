/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public
 License along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place,
 Suite 330,
 Boston, MA 02111-1307
 USA

 */

/*
 * Project:  MixtComp
 * created on: Nov 21, 2013
 * Author:   Vincent KUBICKI
 **/

/** @file mixt_DataHandlerR.cpp
 *  @brief In this file 
 **/

#include "mixt_DataHandlerR.h"

namespace mixt
{


DataHandlerR::DataHandlerR()
{
  // TODO Auto-generated constructor stub
}

DataHandlerR::~DataHandlerR()
{
  // TODO Auto-generated destructor stub
}

bool DataHandlerR::readDataFromRList(Rcpp::List rList)
{
  int k = 0; // 
  for (int i = 0; i < rList.size(); ++i)
  {
    rList_ = rList;
    Rcpp::S4 s4 = rList_[i];
    std::string objType = s4.slot("type");
    if (objType == "double")
    {
      readDataFromRListHelper<Rcpp::NumericMatrix>(i, s4);
    }
    else if (objType == "integer")
    {
      readDataFromRListHelper<Rcpp::IntegerMatrix>(i, s4);
    }
    else if (objType == "character")
    {
      readDataFromRListHelper<Rcpp::CharacterMatrix>(i, s4);
    }
  }
  return true;
}

void DataHandlerR::getData(std::string const& idData, STK::Array2D<int>& data, int& nbVariable) const
{

}

void DataHandlerR::getData(std::string const& idData, STK::Array2D<STK::Real>& data, int& nbVariable) const
{
  std::vector<int> const& v_pos = dataMap_.at(idData);
  nbVariable = v_pos.size();
  data.resize(nbSamples_, nbVariable); // R has already enforced that all data has the same number of rows
  int j = data.firstIdxCols();
  for (std::vector<int>::const_iterator it = v_pos.begin(); it != v_pos.end(); ++it, ++j)
  {
    Rcpp::S4 s4 = rList_[(*it)];
    Rcpp::List ls = s4.slot("augData");
    Rcpp::NumericVector nv = ls["data"];
    for (int iS = data.firstIdxRows(), iR = 0; iR < nbSamples_; ++iS, ++iR)
    {
      data(iS, j) = nv(iR);
    }
  }
}

void DataHandlerR::getData(std::string const& idData, STK::Array2D<std::string>& data, int& nbVariable) const
{

}

void DataHandlerR::getData(std::string const& idData,
                           AugmentedData<STK::Array2D<STK::Real> >& data,
                           int& nbVariable) const
{
  getData(idData, data.data, nbVariable);
}

void DataHandlerR::writeDataMap() const
{
  stk_cout << "Position of data in input: \n";
  for (DataMap::const_iterator it_id = dataMap_.begin(); it_id != dataMap_.end(); ++it_id)
  {
    stk_cout << "\tname: " << (*it_id).first << "\n";
    for (std::vector<int>::const_iterator it_dp = (*it_id).second.begin(); it_dp != (*it_id).second.end(); ++it_dp)
      stk_cout << "\t\trList_ position: " << (*it_dp) << "\n";
  }
}

} /* namespace mixt */
