#pragma once

#include <includes/IOConfig.hpp>
#include <includes/interfaces/ICryptographicAlgorithm.hpp>
#include <memory>

class OptionParser
{
  public:
  OptionParser(int argc, char** argv, IOConfig& ioConfig);
  int parseOptions(std::unique_ptr<ICryptographicAlgorithm>& algorithm);

  private:
  const void printUsage();
  const bool isDES();
  const bool isAES();
  const bool isRSA();
  const bool hasKeyFile();
  const bool hasPassphrase();
  const bool hasInputFile();
  const bool shouldGenerateKey();
  const bool isIoConfigGoodForRSAKeyGeneration();
  const bool isIoConfigGoodForRSAOperations();
  const bool isIoConfigGoodForAESOrDESOperations();

  IOConfig& ioConfig;
  int argc;
  char** argv;
};
