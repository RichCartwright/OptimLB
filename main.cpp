#include <stdio.h>
#include <iostream>
#include <filesystem>

#include "optim.hpp"

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
        
        // This is C++17 and up only - A bit naughty really but the only way I know how to check without using posix stat()
        if(!std::filesystem::exists(fileLoc))
        {
            std::cout << "Argument given is not a file" << std::endl;
            return 1;
        }
    }

    arma::mat hemelbData = ReadCSV(fileLoc);
    std::cout << hemelbData << std::endl;    

    return 0;
}
