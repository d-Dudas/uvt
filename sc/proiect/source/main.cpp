#include <getopt.h>
#include <iostream>
#include <memory>
#include <string.h>

#include <includes/Constants.hpp>
#include <includes/IOConfig.hpp>
#include <includes/des/DES.hpp>
#include <includes/aes/AES.hpp>

void printUsage(char programName[])
{
  std::cout << "Usage: " << programName
            << " -t (type) -(d/e) inputfile -o outputfile -k key\n";
  std::cout << "Type: [\"des\", \"aes_128\", \"aes_192\", \"aes_256\"]\n";
}

int parseOptions(int argc, char *argv[], IOConfig &ioConfig,
                 std::unique_ptr<ICryptographicAlgorithm> &algorithm)
{
  int opt;
  while ((opt = getopt(argc, argv, "d:e:o:k:t:")) != -1)
  {
    switch (opt)
    {
    case 'd':
      ioConfig.inputFile = optarg;
      break;
    case 'e':
      ioConfig.inputFile = optarg;
      ioConfig.encrypt = true;
      break;
    case 'o':
      ioConfig.outputFile = optarg;
      break;
    case 'k':
      ioConfig.key = optarg;
      if (ioConfig.key.size() != 8)
      {
        std::cerr << "Key must be 8 bytes (characters) long\n";
        return FAILURE;
      }
      break;
    case 't':
      if (strcmp(optarg, "des") == 0)
      {
        algorithm = std::make_unique<des::DES>();
      }
      else if (strcmp(optarg, "aes_128") == 0)
      {
        algorithm = std::make_unique<AES>(aes::Versions::AES_128);
      }
      else if (strcmp(optarg, "aes_192") == 0)
      {
        algorithm = std::make_unique<AES>(aes::Versions::AES_192);
      }
      else if (strcmp(optarg, "aes_256") == 0)
      {
        algorithm = std::make_unique<AES>(aes::Versions::AES_256);
      }
      else if (strcmp(optarg, "rsa") == 0)
      {
        // TBD
      }
      else
      {
        std::cerr << "Invalid algorithm\n";
        printUsage(argv[0]);
        return FAILURE;
      }
      break;
    default:
      printUsage(argv[0]);
      return FAILURE;
    }
  }

  if (ioConfig.inputFile.empty() || ioConfig.key.empty() ||
      algorithm.get() == nullptr)
  {
    printUsage(argv[0]);
    return FAILURE;
  }

  return SUCCESS;
}

int main(int argc, char *argv[])
{
  IOConfig config;
  std::unique_ptr<ICryptographicAlgorithm> algorithm;

  if (parseOptions(argc, argv, config, algorithm) != SUCCESS)
  {
    return FAILURE;
  }

  if (config.encrypt)
  {
    algorithm->encrypt(config);
  }
  else
  {
    algorithm->decrypt(config);
  }

  return 0;
}
