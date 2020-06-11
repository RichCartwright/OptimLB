#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <limits>
#include <Python.h>

#include "optim.hpp"

struct FuncOptData
{   
    // I figured I would just make this a struct so we know 100% that we're casting to the right thing
    arma::mat X; // This will be the whole data matrix for lookup
};

double GetMI(const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)
{
    arma::mat x; 
    FuncOptData* optData = reinterpret_cast<FuncOptData*>(opt_data);
    
    if(optData !=  nullptr)
    {
        // let us save the matrix, this is a bit wasteful with a copy but we can sort it out later
        x = optData->X.t();
    }
    else
    {
        std::cout << "Function needs optional data" << std::endl;
        return 0.0;
    }
    
    // First lets get a sub view of all the matrice's(?) theta values
    arma::mat thetaVals = x(1, arma::span(0,x.n_cols-1));
    arma::uvec idx = arma::sort_index(thetaVals);
    //std::cout << idx << std::endl;
    
    arma::mat sorted = x.cols(idx);
    arma::uvec idxA = {1,3,4};
    std::cout << sorted.cols(idxA) << std::endl;
    // this filters out column indexes!
    //arma::uvec idx = {1, 3, 5}; 
    //std::cout << x.cols(idx) << std::endl;
    //std::cout << vals_inp << std::endl; 
    
    // Now for the messy shit because armadillo does NOTHING

    //std::cout << x.n_cols << " " << x.n_rows << std::endl;
    return 0.0;
}

arma::mat ReadCSV(const std::string &filename, const std::string &delimiter = ",")
{
    std::ifstream csv(filename);
    std::vector<std::vector<double>> wholeData;

    for(std::string line; std::getline(csv, line); /**/)
    {
        std::vector<double> data;

        auto start = 0U;
        auto end = line.find(delimiter);
        
        while(end != std::string::npos)
        {
            data.push_back(std::stod(line.substr(start, end-start)));
            start = end + delimiter.length();
            end = line.find(delimiter, start);
        }
        data.push_back(std::stod(line.substr(start, end)));
        wholeData.push_back(data);
    }

    arma::mat dataMatrix = arma::zeros<arma::mat>(wholeData.size(), wholeData[0].size());

    for(uint i = 0; i < wholeData.size(); i++)
    {
        arma::mat r(wholeData[i]);
        dataMatrix.row(i) = r.t();
    }

    return dataMatrix;
}

int main(int argc, char** argv)
{
    std::string fileLoc;

    if(argc != 2)
    {
        std::cout << "No arguments given - need a path to the data" << std::endl;
        return 1; 
    }
    else
    {
        fileLoc = argv[1];
        
        // This is C++17 and up only - A bit naughty, really but the only way I know how to check without using posix stat()
        if(!std::filesystem::exists(fileLoc))
        {
            std::cout << "Argument given is not a file" << std::endl;
            return 1;
        }
    }
    
    // First, let read the CSV data and get the matrix
    FuncOptData optData;
    optData.X = ReadCSV(fileLoc);
    // This is the intial vector - it will also be the final result vector!
    arma::vec x = {1,2,3};

    // Configure the settings for the optimiser
    optim::algo_settings_t optiSettings;
    
    //############################################################################################//
    // These settings are dependent on the optimiser used. I'll think of a smart solution for this /
    // but for now, it's best to just comment out what we dont need                                /
    /*                                                                                             /
     * Gradient decent methods (gd_method)                                                         /
     * 0 = basic GD                                                                                /
     * 1 = GD with momentum (governed by "momentum_par")                                           /
     * 2 = Nesterov accelerated (NAG)                                                              /
     * 3 = AdaGrad (used a normalisation term "norm_term")                                         /
     * 4 = RMSProp ("ada_rho")                                                                     /
     * 5 = AdaDelta                                                                                /
     * 6 = Adam (adaptive Moment Estimation) and AdaMax                                            /
     */

    optiSettings.gd_method = 6; 
    optiSettings.gd_settings.step_size = 0.1;
    // END settings
    
    // Run the optimiser
    bool success = optim::gd(x, GetMI, &optData, optiSettings);
    
    // clean up and return main
    return 0;
}
