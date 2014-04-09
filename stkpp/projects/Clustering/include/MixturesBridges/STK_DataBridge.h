/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 15 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_DataBridge.h
 *  @brief In this file we define the data bridge class.
 **/

#ifndef STK_DATABRIDGE_H
#define STK_DATABRIDGE_H

namespace STK
{
/** @ingroup Clustering
 *  @brief bridge a data set with a MixtureBridge.
 *
 * @tparam Id is any identifier of a concrete model deriving from the
 * interface class STK::IMixtureModel.
 */
template<int Id>
struct DataBridge
{
  // data type handle
  typedef typename Clust::BridgeTraits<Id>::Data Data;
  // type of the data
  typedef typename Clust::BridgeTraits<Id>::Type Type;
  /** default constructor. */
  DataBridge() : m_dataij_(), nbVariable_(0), v_missing_() {}
  /** copy constructor
   *  @param data the DataBridge to copy
   **/
  DataBridge( DataBridge const& data)
            : m_dataij_(data.m_dataij_)
            , nbVariable_(data.nbVariable_)
            , v_missing_(data.v_missing_)
  {}
  /** copy operator.
   * @param data the DataBridge to copy
   **/
  DataBridge& operator=( DataBridge const& data)

  { m_dataij_ = data.m_dataij_;
    nbVariable_ = data.nbVariable_;
    v_missing_ = data.v_missing_;
    return *this;
  }
  /** This is a standard clone function in usual sense.
   *  @return New instance of class as that of calling object.
   */
  DataBridge* clone() const
  { return new DataBridge(*this);}
  /** getter. @return a constant reference on the data set */
  Data const& m_dataij() const { return m_dataij_;}
  /** getter. @return a the number of variables (the number of columns of m_dataij_)  */
  int const& nbVariable() const { return nbVariable_;}
  /** getter. @return the coordinates of the missing values in the data set */
  std::vector<std::pair<int,int> > const& v_missing() const { return v_missing_;}
  /** utility function for lookup the data set and find missing values
   *  coordinates. */
  void findMissing()
  {
    for (int j=m_dataij_.firstIdxCols(); j<= m_dataij_.lastIdxCols(); ++j)
      for (int i=m_dataij_.firstIdxRows(); i<= m_dataij_.lastIdxRows(); ++i)
      {
        if (Arithmetic<Type>::isNA(m_dataij_(i,j)))
        { v_missing_.push_back(std::pair<int,int>(i,j));}
      }
  }

  /** This function will be defined to set the data into the data containers.
   *  To facilitate data handling, framework provide a MixtureManager and
   *  templated functions, that can be called directly to get the data.
   */
   template<class MixtureManager>
   void setData(MixtureManager const* p_manager,String const& idName)
   {
     p_manager->getData(idName, m_dataij_, nbVariable_ );
     findMissing();
   }
   /** data set */
   Data m_dataij_;

  private:
    /** number of variables in the data set */
    int nbVariable_;
    /** vector with the coordinates of the missing values */
    std::vector<std::pair<int,int> > v_missing_;
};

} // namespace STK

#endif /* DATABRIDGE_H */
