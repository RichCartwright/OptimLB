#ifndef __OPTIMISER_H__
#define __OPTIMISER_H__

#include <optim.hpp>

// Abstract super class for the optimiser 

class Optimiser 
{
    public:
        
        Optimiser(optim::algo_settings_t* settings = nullptr)
        {
            if(settings)
            {
                algoSettings = settings;
            }
            else
            {
                algoSettings = new optim::algo_settings_t();
            }
        }
        
        virtual ~Optimiser()
        {
            delete algoSettings;
        }
        virtual bool RunOptimiser(  arma::vec& init_out_vals,
                                    std::function<double (const arma::vec& vals_inp,
                                    arma::vec* grad_out, void* opt_data)> opt_objfn,
                                    void* opt_data) = 0;
        
        // These settings seem to be shared by ALL the optimisation methods
        // further specific functions are defined in their respective subclasses
        void SetErrorTolerance(double err_tol);
        void SetIterationMax(int iter_max);
        void SetValueBoundary(arma::vec& lower, arma::vec& upper);

    protected: 
        optim::algo_settings_t* algoSettings = nullptr; 
};

#endif
