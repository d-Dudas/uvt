#pragma once

#include <includes/IOConfig.hpp>

class ICryptographicAlgorithm
{
  public:
  virtual void encrypt(IOConfig& ioConfig) = 0;
  virtual void decrypt(IOConfig& ioConfig) = 0;

  virtual ~ICryptographicAlgorithm() = default;
};
