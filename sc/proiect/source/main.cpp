#include <iostream>
#include <memory>

#include <includes/Constants.hpp>
#include <includes/IOConfig.hpp>
#include <includes/OptionParser.hpp>

#include <includes/rsa/RSA.hpp>

int main(int argc, char *argv[])
{
  IOConfig config;
  OptionParser parser(argc, argv, config);
  std::unique_ptr<ICryptographicAlgorithm> algorithm;

  if (parser.parseOptions(algorithm) != SUCCESS)
  {
    return FAILURE;
  }

  if (algorithm == nullptr)
  {
    return SUCCESS;
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
