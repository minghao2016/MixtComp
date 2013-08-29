#include <iostream>
#include <math.h>
#include "framework/MixtureInterface/IMixture.h"
#include "Model.h"

Model::Model(IMixture* mixture,int nbsample,int nbcluster) : nbSample_(nbsample),
                                                              nbCluster_(nbcluster)
{
  //Allocate memory for conditional probabilities
  m_Tik_.resize(nbSample_,nbCluster_);

  //Allocate memory for class labels
  v_Zi_  = new int[nbsample];

  //Allocate memory for row proportions
  v_Pie_ = new double[nbcluster];

  //Allocate memory for mixture
  p_Mixture_ = mixture->clone();
  //set this pointer
  p_Mixture_->setModel(this);

}

Model::Model(int nbsample,int nbcluster) : nbSample_(nbsample),
                                           nbCluster_(nbcluster)
{
  //Allocate memory for conditional probabilities
  m_Tik_.resize(nbSample_,nbCluster_);

  //Allocate memory for class labels
  v_Zi_  = new int[nbsample];

  //Allocate memory for row proportions
  v_Pie_ = new double[nbcluster];
}

Model::Model(const Model& other)
{
  nbSample_ = other.nbSample_;
  nbCluster_ = other.nbCluster_;

  //Allocate memory for conditional probabilities and copy values
  m_Tik_.resize(nbSample_,nbCluster_);
  m_Tik_ = other.m_Tik_;

  //Allocate memory for class labels and copy values
  v_Zi_  = new int[nbSample_];

  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = other.v_Zi_[i];
  }
  //Allocate memory for row proportions and copy values
  v_Pie_ = new double[nbCluster_];

  for (int j = 0; j < nbCluster_; ++j) {
    v_Pie_[j] = other.v_Pie_[j];
  }

  //Allocate memory for developer
  p_Mixture_ = other.p_Mixture_->clone();
  //set this pointer
  p_Mixture_->setModel(this);

}

Model* Model::clone(){
  return new Model(*this);
}

Model::~Model()
{
  //release various memories
  if(p_Mixture_!=NULL){
    delete p_Mixture_;
    p_Mixture_ = NULL;
  }

  delete[] v_Pie_;
  delete[] v_Zi_;
}

void Model::mStep()
{
  p_Mixture_->paramUpdateStep();
}

void Model::storeIntermediateResults(int iteration){
  p_Mixture_->storeIntermediateResults(iteration);
}
void Model::seStep()
{
  p_Mixture_->imputationStep();
  p_Mixture_->samplingStep();
  updateModelParameters();
}
void Model::initializeModel()
{
  //randomInitialization();
  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k] = 1.0/nbCluster_;
  }
  p_Mixture_->initializeStep();
}

void Model::finalizeModel()
{
  p_Mixture_->finalizeStep();
}

void Model::setMixture(IMixture* Mixture)
{
  p_Mixture_ = Mixture->clone();
}

void Model::updateModelParameters()
{
  // update t_ik
  for (int i = 0; i < nbSample_; ++i) {
    double sum = 0;
    for (int k = 0; k < nbCluster_; ++k) {
      sum+=v_Pie_[k]*p_Mixture_->posteriorProbability(i,k);
    }

    for (int k = 0; k < nbCluster_; ++k) {
      m_Tik_(i,k) = v_Pie_[k]*p_Mixture_->posteriorProbability(i,k)/sum;
    }
  }

  // update z_i

  srand(time(0));
  for (int i = 0; i < nbSample_; ++i) {
    float randval = float(std::rand())/float(RAND_MAX);
    float cumsum = m_Tik_(i,0);
    for (int k = 0; k < nbCluster_; ++k) {
      if(randval<=cumsum)
      {
        v_Zi_[i] = k;
        break;
      }
      //we can safely use k+1 here as the loop bound to break at last index.
      cumsum+=m_Tik_(i,k+1);
    }
  }

  // update pie

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k] = 0.0;
  }
  for (int i = 0; i < nbSample_; ++i) {
    v_Pie_[v_Zi_[i]]+=1.0;
  }

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k]/=nbSample_;
  }

}

void Model::randomInitialization(){
  //generate random numbers
  srand(time(0));
  std::vector<float> randnumbers(nbSample_);
  for ( int i = 0; i < nbSample_; ++i) {
    randnumbers[i] = float(std::rand())/float(RAND_MAX);
  }

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k] = 0.0;
  }

  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = floor(nbCluster_*randnumbers[i]);
    v_Pie_[v_Zi_[i]]+=1.0;
  }

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k]/=nbSample_;
  }
}
double Model::logLikelihood() const {
  return p_Mixture_->logLikelihood();
}

Model& Model::operator=(const Model& other){
  //copy developer
  p_Mixture_->copy(*other.p_Mixture_);

  //copy values for conditional probabilities
  for (int i = 0; i < nbSample_; ++i) {
    for (int j = 0; j < nbCluster_; ++j) {
      m_Tik_(i,j) = other.m_Tik_(i,j);
    }
  }

  //copy values for class labels
  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = other.v_Zi_[i];
  }
  //copy values for row proportions
  for (int j = 0; j < nbCluster_; ++j) {
    v_Pie_[j] = other.v_Pie_[j];
  }

  return *this;
}

void Model::writeParameters(std::ostream&out) const{
  p_Mixture_->writeParameters(out);
}


void Model::setData()
{
  p_Mixture_->setData();
}
