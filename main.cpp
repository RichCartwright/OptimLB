#include <stdio.h>
#include <iostream>
#include <filesystem>

#include "optim.hpp"
#include "Python.h"

struct FuncOptData
{   
    // I figured I would just make this a struct so we know 100% that we're casting to the right thing
    arma::mat X; // This will be the whole data matrix for lookup
};

double GetMI(const arma::vec& vals_inp, arma::vec* grad_out, void* opt_data)
{
    FuncOptData* optData = reinterpret_cast<FuncOptData*>(opt_data);
    
    if(optData !=  nullptr)
    {
        arma::mat X = optData->X;
    }
    else
    {
        std::cout << "Function needs optional data" << std::endl;
        return 0.0;
    }
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
    arma::colvec x(3);
    x.fill(1);

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
    
    std::cout << x << std::endl;
    // clean up and return main
    //delete optData;
    return 0;
}
