#ifndef __CONJUGATEGRAD_H__
#define __CONJUGATEGRAD_H__

#include "Optimiser.h"

enum class CGMethod: int 
{
    FletcherReeves = 1,
    PolakRibiere = 2,
    FR_PR = 3, 
    HestenesStiefel = 4,
    DiaYuan = 5, 
    HagerZhang = 6
};

class ConjugateGradient : public Optimiser
{
    public:

        ConjugateGradient(optim::algo_settings_t* settings = nullptr) : Optimiser(settings)
        {
            // Calls the superclass first then comes here
        }
        
        ~ConjugateGradient(){};

        bool RunOptimiser(  arma::vec& init_out_vals, 
                            std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn,
                            void* opt_data) override;
        
        void SetCGMethod(CGMethod cgMethod);
        void RestartThreshold(double param);
        
};

#endif
