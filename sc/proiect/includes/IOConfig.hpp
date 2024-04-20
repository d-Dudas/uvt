#pragma once

#include <includes/Operations.hpp>
#include <includes/Algorithms.hpp>
#include <string>

struct IOConfig
{
  std::string inputFile;
  std::string outputFile;
  std::string keyFile;
  std::string passphrase;
  Algorithms algorithm = Algorithms::invalid;
  Operations operation = Operations::generateKey;
};
