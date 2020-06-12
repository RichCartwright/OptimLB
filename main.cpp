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

arma::mat GetClosestMatrixCol(arma::mat& inMat, int col, double inputVal)
{
    // NOTE:    The way im doing this has its flaws, if I use this data directly the way it is ill need to address it.
    //          It uses the Theta value as the most important when filtering, with the rotation (or last) value of the least importance.
    //
    arma::mat colExtract = inMat(col, arma::span::all); 
    double closestIdx = arma::abs(colExtract - inputVal).index_min();
    
    // Due to the nature of arma::find, we need to pass in an unsorted matrix subview
    //  then we can get the indices to apply to the full matrix
    arma::uvec finalIdx = arma::find(colExtract == colExtract(closestIdx));
    
    // Return the final "frame"
    return inMat.cols(finalIdx);
}

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
   
    x = GetClosestMatrixCol(x, 1, vals_inp[0]); // Theta
    x = GetClosestMatrixCol(x, 2, vals_inp[1]); // Phi
    x = GetClosestMatrixCol(x, 3, vals_inp[2]); // Alpha
    // First lets get a sub view of all the matrice's(?) theta values
    // then get an index to the closest value!
    //arma::mat thetaVals = x(1, arma::span(0,x.n_cols-1));
    //Lets get the closest theta value!
    //double closestThetaIdx = arma::abs(thetaVals - vals_inp[0]).index_min();
    //std::cout << thetaVals(closestThetaIdx) << std::endl;
    
   // arma::uvec finalThetaIdx = arma::find(thetaVals == thetaVals(closestThetaIdx));
    // This is why we took a copy, we can fuck directly with it at the end of each step
    //x = x.cols(finalThetaIdx);

    // Now we need to do the same for Phi...
    //  TODO: We're copying a tonne of code here, put it in a loop...
    //arma::mat phiVals = x(2, arma::span(0,x.n_cols-1));
    //double closestPhiIdx = arma::abs(phiVals - vals_inp[1]).index_min();
    //arma::uvec finalPhiIdx = arma::find(phiVals == phiVals(closestPhiIdx));
    //x = x.cols(finalPhiIdx);

    // And now for the alpha (zoom)...
    //arma::mat alphaVals = x(3, arma::span(0,x.n_cols-1));
    //double closestAlphaIdx = arma::abs(alphaVals - vals_inp[2]).index_min();
    //arma::uvec finalAlphaIdx = arma::find(alphaVals == alphaVals(closestAlphaIdx));
    //x = x.cols(finalAlphaIdx);

    std::cout << x << std::endl;
    // Negate and return the MI of the result
    // NOTE: we don't need to worry about columns and rows here, 
    //       a 1D accessor to a matrix will assume its flat like a memory ptr
    return x(4)*-1;
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
    arma::colvec x = {9,9,-6};


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
    bool success = optim::nm(x, GetMI, &optData, optiSettings);
    // clean up and return main
    return 0;
}
