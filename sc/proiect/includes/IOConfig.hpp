#pragma once

#include <string>

struct IOConfig
{
  std::string inputFile;
  std::string outputFile;
  std::string key;
  bool encrypt = false;
};
