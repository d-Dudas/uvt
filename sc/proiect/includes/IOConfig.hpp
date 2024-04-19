#pragma once

#include <includes/Operations.hpp>
#include <string>

struct IOConfig
{
  std::string inputFile;
  std::string outputFile;
  std::string keyFile;
  std::string passphrase;
  Operations operation = Operations::generateKey;
};
