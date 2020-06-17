#include "ConjugateGradient.h"

bool ConjugateGradient::RunOptimiser(arma::vec& init_out_vals, std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn, void* opt_data)
{
    // Run the optimiser, values will be updated in init_out_vals
    return optim::cg(init_out_vals, opt_objfn, opt_data, *algoSettings);
}

void ConjugateGradient::SetCGMethod(CGMethod cgMethod)
{
   if(algoSettings)
   {
       algoSettings->cg_method = static_cast<int>(cgMethod);
   }
}

void ConjugateGradient::RestartThreshold(double param)
{ 
    if(algoSettings)
    {
        algoSettings->cg_restart_threshold = param;
    }
}
