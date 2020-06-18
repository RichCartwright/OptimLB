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
        virtual void SetErrorTolerance(double param)
        {
            algoSettings->err_tol = param;
        }
        virtual void SetIterationMax(int param)
        {
            algoSettings->iter_max = param;
        }

        virtual void SetValsBound(bool param)
        {
            // This will segfault if the boundaries aren't set FIRST
            // silly design, ill find a work around later
            algoSettings->vals_bound = param;
        }

        virtual void SetValueBoundary(arma::vec& lower, arma::vec& upper)
        {
            algoSettings->lower_bounds = lower;
            algoSettings->upper_bounds = upper;
        }

        virtual void PrintSettings()
        {
            if(algoSettings)
            {
                std::cout << "--- Optimisation Settings ----" << std::endl;
                std::cout << "Error Tol: " << algoSettings->err_tol << "    " << "Max Iterations: " << algoSettings->iter_max << std::endl;
                std::cout << "Value Boundaries: "; 
                if(!algoSettings->vals_bound)
                {   
                    std::cout << "Value boundaries = false" << std::endl;
                }
                else
                {
                    std::cout << "Lower: " << algoSettings->lower_bounds << " - Upper:" << algoSettings->upper_bounds << std::endl;
                }
            }
        }

    protected: 
        optim::algo_settings_t* algoSettings = nullptr; 
};

#endif
