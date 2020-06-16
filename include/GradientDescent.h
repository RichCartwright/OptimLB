#ifndef __GRADIENTDESCENT_H__
#define __GRADIENTDESCENT_H__

#include "Optimiser.h"

enum class GDMethod: int 
{
    Basic = 0,
    Momentum = 1,
    NAG = 2, 
    AdaGrad = 3,
    RMSProp = 4, 
    AdaDelta = 5,
    Adam = 6, 
    Nadam_NadaMax = 7
};

class GradientDescent : public Optimiser
{
    public:

        GradientDescent(optim::algo_settings_t* settings = nullptr) : Optimiser(settings)
        {
            // Calls the superclass first then comes here
        }
        
        ~GradientDescent(){};

        bool RunOptimiser(  arma::vec& init_out_vals, 
                            std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn,
                            void* opt_data) override;
        
        void SetGDMethod(GDMethod gdMethod);
        void SetStepSize(double param);
        void SetMomentum(double param);
        void SetNormTerm(double param);
        void SetLearningScale(double param);
        void SetAdamBeta(double betaX, double betaY);
        void SetAdaMax(bool param);


};

#endif
