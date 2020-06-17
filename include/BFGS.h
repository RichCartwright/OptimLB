#ifndef __BFGS_H__
#define __BFGS_H__

#include "Optimiser.h"

class BFGS : public Optimiser
{
    public:

        BFGS(bool L_BFGS, optim::algo_settings_t* settings = nullptr) : Optimiser(settings)
        {
            // Calls the superclass first then comes here

            limitedMemory = L_BFGS ? true : false;
        }
        
        ~BFGS(){};

        bool RunOptimiser(  arma::vec& init_out_vals, 
                            std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn,
                            void* opt_data) override;
    private:
        bool limitedMemory = false; // This determines if it will run L-BFGS or normal BFGS
};

#endif
