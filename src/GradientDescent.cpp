#include "GradientDescent.h"

bool GradientDescent::RunOptimiser(arma::vec& init_out_vals, std::function<double (const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)> opt_objfn, void* opt_data)
{
    // Run the optimiser, values will be updated in init_out_vals
    return optim::gd(init_out_vals, opt_objfn, opt_data, *algoSettings);
}

void GradientDescent::SetGDMethod(GDMethod gdMethod)
{
   if(algoSettings)
   {
       algoSettings->gd_method = static_cast<int>(gdMethod);
   }
}

void GradientDescent::SetStepSize(double param)
{
    algoSettings->gd_settings.step_size = param;
}

void GradientDescent::SetMomentum(double param)
{
    algoSettings->gd_settings.momentum_par = param;
}

void GradientDescent::SetNormTerm(double param)
{
    algoSettings->gd_settings.norm_term = param;
}


void GradientDescent::SetLearningScale(double param)
{
    algoSettings->gd_settings.ada_rho = param;
}

void GradientDescent::SetAdamBeta(double betaX, double betaY)
{
    algoSettings->gd_settings.adam_beta_1 = betaX;
    algoSettings->gd_settings.adam_beta_2 = betaY;
}

void GradientDescent::SetAdaMax(bool param)
{
    algoSettings->gd_settings.ada_max = param;
}
