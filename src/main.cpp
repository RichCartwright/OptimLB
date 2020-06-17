#if !defined(ARMA_64BIT_WORD)
    #define ARMA_64BIT_WORD
#endif
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <limits>
#include <Python.h>
#include <optim.hpp>

#include "GradientDescent.h"
#include "NelderMead.h"


struct FuncOptData
{   
    // I've made the "optdata" a struct so we know what we are unpacking
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
    std::cout << vals_inp << std::endl;

    arma::mat x;
    FuncOptData* optData = reinterpret_cast<FuncOptData*>(opt_data);
    
    if(optData !=  nullptr)
    {
        // let us save the matrix, this is a bit wasteful with a copy but we can sort it out later

        x = optData->X.t();
    }
    if(optData == nullptr)
    {
        std::cout << "Function needs optional data" << std::endl;
        return 0.0;
    }
   
    x = GetClosestMatrixCol(x, 1, vals_inp[0]); // Theta
    x = GetClosestMatrixCol(x, 2, vals_inp[1]); // Phi
    x = GetClosestMatrixCol(x, 3, vals_inp[2]); // Alpha
    
    // Negate and return the MI of the result
    // NOTE: we don't need to worry about columns and rows here, 
    //       a 1D accessor to a matrix will assume its flat like a memory ptr
    std::cout << x(4)*-1 << std::endl;
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
    arma::vec x = {120,120,-5};

    GradientDescent* gd = new GradientDescent();
    gd->SetGDMethod(GDMethod::Basic);
    gd->SetStepSize(1e-10);
    gd->SetErrorTolerance(1e-10);
    gd->RunOptimiser(x, GetMI, &optData);
    
    std::cout << x << std::endl;
    // Run the optimiser
    //bool success = optim::nm(x, GetMI, &optData, optiSettings);
    // clean up and return main
    delete gd;
    return 0;
}
