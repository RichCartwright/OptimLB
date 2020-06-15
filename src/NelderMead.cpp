#include "NelderMead.h"

bool NelderMead::RunOptimiser(arma::vec& init_out_vals, std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn, void* opt_data)
{
    // Run the optimiser, values will be updated in init_out_vals
    return optim::nm(init_out_vals, opt_objfn, &opt_data, *algoSettings);
}

void NelderMead::ReflectionParam(double param)
{
    if(algoSettings)
    {
        algoSettings->nm_par_alpha = param;
    }
    else
    {
        std::cout << "NelderMead::ReflectionParam(double param) - algoSettings doesn't exist for some reason!" << std::endl;
    }
}

void NelderMead::ExpansionParam(double param)
{
    if(algoSettings)
    {
        algoSettings->nm_par_gamma = param;
    }
    else
    {
        std::cout << "NelderMead::ExpansionParam(double param) - algoSettings doesn't exist for some reason!" << std::endl;
    }
}

void NelderMead::ContractionParam(double param)
{
    if(algoSettings)
    {
        algoSettings->nm_par_beta = param;
    }
    else
    {
        std::cout << "NelderMead::ContractionParam(double param) - algoSettings doesn't exist for some reason!" << std::endl;
    }
}

void NelderMead::ShrinkageParam(double param)
{
    if(algoSettings)
    {
        algoSettings->nm_par_delta = param;
    }
    else
    {
        std::cout << "NelderMead::ShrinkageParam(double param) - algoSettings doesn't exist for some reason!" << std::endl;
    }
}

