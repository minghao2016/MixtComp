/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: July 28, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_CategoricalLikelihood.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{

CategoricalLikelihood::CategoricalLikelihood(const Vector<Real>* p_param,
                                             const AugmentedData<Matrix<int> >* p_augData,
                                             const Eigen::Matrix<std::vector<std::pair<int, Real> >,
                                                                 Eigen::Dynamic,
                                                                 Eigen::Dynamic>* p_dataStatStorage,
                                             int nbClass) :
    nbClass_(nbClass),
    p_param_(p_param),
    p_augData_(p_augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

CategoricalLikelihood::~CategoricalLikelihood()
{}

void CategoricalLikelihood::lnCompletedLikelihood(Vector<Real>* lnComp, int k)
{
  int nbModalities = p_param_->rows() / nbClass_;
  for (int j = 0; j < p_augData_->data_.cols(); ++j)
  {
    for (int i = 0; i < p_augData_->data_.rows(); ++i)
    {
      if (p_augData_->misData_(i, j).first == present_) // likelihood for present data
      {
        int ind = k * nbModalities + p_augData_->data_(i, j) - minModality;
        Real proba = (*p_param_)(ind,
                                 j);
#ifdef MC_DEBUG
        if (proba < epsilon)
        {
          Real sum = 0.;
          std::cout << "Null proba detected, k: " << k << std::endl;
          std::cout << "p_augData_->data_(i, j) - minModality: " << p_augData_->data_(i, j) - minModality << std::endl;
          std::cout << "param: " << std::endl;
          for (int p = 0; p < nbModalities; ++p)
          {
            std::cout << p_param_->elt(k * nbModalities + p, j) << std::endl;
            sum += p_param_->elt(k * nbModalities + p, j);
          }
          std::cout << "sum: " << sum << std::endl;
        }
#endif
        (*lnComp)(i) += std::log(proba);
      }
      else // likelihood for estimated missing values, imputation by the mode
      {
        Real proba = (*p_param_)(k * nbModalities + (*p_dataStatStorage_)(i, j)[0].first - minModality,
                                        j);
        (*lnComp)(i) += std::log(proba); // added lnLikelihood using the mode
      }
    }
  }
}

void CategoricalLikelihood::lnObservedLikelihood(Vector<Real>* lnComp, int k)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalLikelihood::lnObservedLikelihood" << std::endl;
#endif
  int nbModalities = p_param_->rows() / nbClass_;
  for (int j = 0; j < p_augData_->data_.cols(); ++j)
  {
    for (int i = 0; i < p_augData_->data_.rows(); ++i)
    {
#ifdef MC_DEBUG
          std::cout << "\ti: " << i << ", j: " << j << std::endl;
#endif
      switch (p_augData_->misData_(i, j).first)
      {
        case present_: // likelihood for present data
        {
          Real proba = (*p_param_)(k * nbModalities + p_augData_->data_(i, j) - minModality,
                                   j);
          (*lnComp)(i) += std::log(proba);
        }
        break;

        case missing_: // no contribution to the observed likelihood
        {}
        break;

        case missingFiniteValues_: // adding the contributions of the various modalities
        {
#ifdef MC_DEBUG
          std::cout << "missingFiniteValues" << std::endl;
          std::cout << "p_param_->rows(): " << p_param_->rows() << ", p_param_->cols(): " << p_param_->cols() << std::endl;
#endif
          Real proba = 0.;

          for (std::vector<int>::const_iterator itMiss = p_augData_->misData_(i, j).second.begin();
               itMiss != p_augData_->misData_(i, j).second.end();
               ++itMiss)
          {
#ifdef MC_DEBUG
            std::cout << "k: " << k << ", j: " << j << ", nbModalities: " << nbModalities << ", *itMiss: " << *itMiss << std::endl;
#endif
            proba += (*p_param_)(k * nbModalities + *itMiss - minModality,
                                 j);
          }
          (*lnComp)(i) += std::log(proba);
        }
        break;

        default:
        {}
        break;
      }
    }
  }
}

} /* namespace mixt */
