#include <getopt.h>
#include <iostream>
#include <string.h>

#include <includes/Constants.hpp>
#include <includes/OptionParser.hpp>

#include <includes/aes/AES.hpp>
#include <includes/des/DES.hpp>
#include <includes/rsa/RSA.hpp>

OptionParser::OptionParser(int argc, char **argv, IOConfig &ioConfig)
    : argc(argc), argv(argv), ioConfig(ioConfig)
{
}

const void OptionParser::printUsage()
{
  const std::string programName = argv[0];
  std::cout << "Usage: " << programName
            << " -t (type) [-(d/e) <inputfile>] [-o <outputfile>] [-p "
               "<passphrase>] [-k <keyfile>]\n";
  std::cout
      << "Types: [\"des\", \"aes_128\", \"aes_192\", \"aes_256\", \"rsa\"]\n";

  std::cout << "\nExample for RSA key generation:\n";
  std::cout << programName << " -t rsa -k keyfile\n";

  std::cout << "\nExample for RSA encryption:\n";
  std::cout << programName
            << " -t rsa -e inputfile [-o outputfile] -k keyfile\n";

  std::cout << "\nExample for AES encryption:\n";
  std::cout << programName
            << " -t aes_128 -e inputfile [-o outputfile] -p passphrase\n";
}

const bool OptionParser::isDES()
{
  return ioConfig.algorithm == Algorithms::DES;
}

const bool OptionParser::isAES()
{
  return ioConfig.algorithm == Algorithms::AES_128 or
         ioConfig.algorithm == Algorithms::AES_192 or
         ioConfig.algorithm == Algorithms::AES_256;
}

const bool OptionParser::isRSA()
{
  return ioConfig.algorithm == Algorithms::RSA;
}

const bool OptionParser::hasKeyFile()
{
  return not ioConfig.keyFile.empty();
}

const bool OptionParser::hasPassphrase()
{
  return not ioConfig.passphrase.empty();
}

const bool OptionParser::hasInputFile()
{
  return not ioConfig.inputFile.empty();
}

const bool OptionParser::shouldGenerateKey()
{
  return ioConfig.operation == Operations::generateKey;
}

const bool OptionParser::isIoConfigGoodForRSAKeyGeneration()
{
  return isRSA() and hasKeyFile() and shouldGenerateKey();
}

const bool OptionParser::isIoConfigGoodForRSAOperations()
{
  return isRSA() and hasKeyFile() and not shouldGenerateKey() and
         hasInputFile();
}

const bool OptionParser::isIoConfigGoodForAESOrDESOperations()
{
  return (isDES() or isAES()) and hasPassphrase() and hasInputFile();
}

int OptionParser::parseOptions(
    std::unique_ptr<ICryptographicAlgorithm> &algorithm)
{
  int opt;
  while ((opt = getopt(argc, argv, "t:d:e:o:p:k:h")) != -1)
  {
    switch (opt)
    {
    case 't':
    {
      const std::string type = optarg;
      if (type == "des")
      {
        ioConfig.algorithm = Algorithms::DES;
      }
      else if (type == "aes_128")
      {
        ioConfig.algorithm = Algorithms::AES_128;
      }
      else if (type == "aes_192")
      {
        ioConfig.algorithm = Algorithms::AES_192;
      }
      else if (type == "aes_256")
      {
        ioConfig.algorithm = Algorithms::AES_256;
      }
      else if (type == "rsa")
      {
        ioConfig.algorithm = Algorithms::RSA;
      }
      else
      {
        printUsage();
        return FAILURE;
      }
      break;
    }
    case 'd':
    {
      ioConfig.inputFile = optarg;
      ioConfig.operation = Operations::decrypt;
      break;
    }
    case 'e':
    {
      ioConfig.inputFile = optarg;
      ioConfig.operation = Operations::encrypt;
      break;
    }
    case 'o':
    {
      ioConfig.outputFile = optarg;
      break;
    }
    case 'p':
    {
      ioConfig.passphrase = optarg;
      break;
    }
    case 'k':
    {
      ioConfig.keyFile = optarg;
      break;
    }
    case 'h':
    {
      printUsage();
      return SUCCESS;
    }
    default:
    {
      printUsage();
      return FAILURE;
    }
    }
  }

  if (ioConfig.outputFile.empty())
  {
    ioConfig.outputFile = ioConfig.inputFile + ".out";
  }

  if (isIoConfigGoodForRSAKeyGeneration())
  {
    algorithm = std::make_unique<rsa::RSA>();
  }
  else if (isIoConfigGoodForRSAOperations())
  {
    algorithm = std::make_unique<rsa::RSA>();
  }
  else if (isIoConfigGoodForAESOrDESOperations())
  {
    if (isDES())
    {
      algorithm = std::make_unique<des::DES>();
    }
    else if (isAES())
    {
      algorithm = std::make_unique<aes::AES>(ioConfig.algorithm);
    }
  }
  else
  {
    printUsage();
    return FAILURE;
  }

  return SUCCESS;
}
