#include "BFGS.h"

// No controls really for this, just run the optimiser, I guess...
bool BFGS::RunOptimiser(arma::vec& init_out_vals, std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn, void* opt_data)
{
    // Run the optimiser, values will be updated in init_out_vals
    if(!limitedMemory)
        return optim::bfgs(init_out_vals, opt_objfn, opt_data, *algoSettings);
    else
        return optim::lbfgs(init_out_vals, opt_objfn, opt_data, *algoSettings);
        
}
