#ifndef MODEL_H_
#define MODEL_H_
class IMixture;
#include <iostream>
#include "stkpp/include/STKpp.h"
#undef Real
class Model
{
  public:
    Model();
    Model(int nbsample,int nbcluster);
    Model(IMixture* developer,int nbsample,int nbcluster);
    Model(const Model&);
    Model* clone();
    void initializeModel();
    void seStep();
    void mStep();
    void setData();
    void storeIntermediateResults(int iteration);
    void finalizeModel();
    void setMixture(IMixture* Mixture);
    double logLikelihood() const;
    int nbCluster() const {return nbCluster_;}
    int nbSample() const {return nbSample_;}
    double** conditionalProbabilities() const;
    int* classLabels() const {return v_Zi_;}
    double* proportions() const {return v_Pie_;}
    const IMixture& getMixture() {return *p_Mixture_;}
    /** overloaded assignment operator*/
    Model& operator=(const Model&);
    void writeParameters(std::ostream&) const;
    ~Model();
  protected:
    IMixture *p_Mixture_;
    STK::Array2D<double> m_Tik_;
    int* v_Zi_;
    double* v_Pie_;
    int nbSample_;
    int nbCluster_;

    //protected member functions
    void updateModelParameters();
    void randomInitialization();
};

inline double** Model::conditionalProbabilities() const{
  return m_Tik_.allocator().p_data();
}

#endif /* MODEL_H_ */