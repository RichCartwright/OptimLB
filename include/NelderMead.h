#ifndef __NELDERMEAD_H__
#define __NELDERMEAD_H__

#include "Optimiser.h"

class NelderMead : public Optimiser
{
    public:

        NelderMead(optim::algo_settings_t* settings = nullptr) : Optimiser(settings)
        {
            // Calls the superclass first then comes here
        }

        bool RunOptimiser(  arma::vec& init_out_vals, 
                            std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn,
                            void* opt_data);
        
        void ReflectionParam(double param);
        void ExpansionParam(double param);
        void ContractionParam(double param);
        void ShrinkageParam(double param); 
};
#endif
