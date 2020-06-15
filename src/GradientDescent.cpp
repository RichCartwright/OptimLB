#include "GradientDescent.h"

bool GradientDescent::RunOptimiser(arma::vec& init_out_vals, std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn, void* opt_data)
{
    // Run the optimiser, values will be updated in init_out_vals
    return optim::gd(init_out_vals, opt_objfn, &opt_data, *algoSettings);
}

void GradientDescent::SetGDMethod(GDMethod gdMethod)
{
   if(algoSettings)
   {
       algoSettings->gd_method = static_cast<int>(gdMethod);
   }
}
