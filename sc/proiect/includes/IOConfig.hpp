#pragma once

#include <string>

#include <includes/AlgorithmsSuite.hpp>

struct IOConfig
{
    std::string inputFile;
    std::string outputFile;
    std::string key;
    bool encrypt = false;
    AlgorithmSuite algorithm = AlgorithmSuite::INVALID;
};
