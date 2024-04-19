#include <getopt.h>
#include <iostream>
#include <memory>
#include <string.h>

#include <includes/Constants.hpp>
#include <includes/IOConfig.hpp>
#include <includes/aes/AES.hpp>
#include <includes/des/DES.hpp>
#include <includes/rsa/RSA.hpp>

void printUsage(char programName[])
{
  std::cout
      << "Usage: " << programName
      << " -t (type) -(d/e) inputfile -o outputfile -p passphrase -k keyfile\n";
  std::cout << "Type: [\"des\", \"aes_128\", \"aes_192\", \"aes_256\"]\n";
}

int parseOptions(int argc, char *argv[], IOConfig &ioConfig,
                 std::unique_ptr<ICryptographicAlgorithm> &algorithm)
{
  int opt;
  while ((opt = getopt(argc, argv, "d:e:o:p:t:k:")) != -1)
  {
    switch (opt)
    {
    case 'd':
      ioConfig.inputFile = optarg;
      ioConfig.operation = Operations::decrypt;
      break;
    case 'e':
      ioConfig.inputFile = optarg;
      ioConfig.operation = Operations::encrypt;
      break;
    case 'o':
      ioConfig.outputFile = optarg;
      break;
    case 'p':
      ioConfig.passphrase = optarg;
      if (ioConfig.passphrase.size() != 8)
      {
        std::cerr << "Passphrase must be 8 bytes (characters) long\n";
        return FAILURE;
      }
      break;
    case 'k':
      ioConfig.keyFile = optarg;
      break;
    case 't':
      if (strcmp(optarg, "des") == 0)
      {
        algorithm = std::make_unique<des::DES>();
      }
      else if (strcmp(optarg, "aes_128") == 0)
      {
        algorithm = std::make_unique<aes::AES>(aes::Versions::AES_128);
      }
      else if (strcmp(optarg, "aes_192") == 0)
      {
        algorithm = std::make_unique<aes::AES>(aes::Versions::AES_192);
      }
      else if (strcmp(optarg, "aes_256") == 0)
      {
        algorithm = std::make_unique<aes::AES>(aes::Versions::AES_256);
      }
      else if (strcmp(optarg, "rsa") == 0)
      {
        algorithm = std::make_unique<rsa::RSA>();
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

  if (ioConfig.outputFile.empty())
  {
    ioConfig.outputFile = ioConfig.inputFile + ".out";
  }

  if (dynamic_cast<rsa::RSA *>(algorithm.get()) != nullptr and
      ioConfig.operation == Operations::generateKey and
      not ioConfig.keyFile.empty())
  {
    // key generation only reguires a filename to save the keys
    return SUCCESS;
  }

  if (ioConfig.inputFile.empty() or ioConfig.passphrase.empty() or
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

  if (config.operation == Operations::generateKey)
  {
    dynamic_cast<rsa::RSA *>(algorithm.get())->generateKeys(config);
  }
  else if (config.operation == Operations::encrypt)
  {
    algorithm->encrypt(config);
  }
  else
  {
    algorithm->decrypt(config);
  }

  return SUCCESS;
}
