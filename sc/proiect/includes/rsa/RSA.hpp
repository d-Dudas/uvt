#pragma once

#include <gmpxx.h>
#include <includes/interfaces/ICryptographicAlgorithm.hpp>

namespace rsa
{
class RSA : public ICryptographicAlgorithm
{
public:
  void generateKeys(IOConfig &ioConfig);
  void encrypt(IOConfig &ioConfig) override;
  void decrypt(IOConfig &ioConfig) override;

private:
  void savePublicKey(const std::string &filename, const mpz_class &n);
  void savePrivateKey(const std::string &filename, const mpz_class &n,
                      const mpz_class &d);
  void loadPrivateKey(const std ::string &filename, mpz_class &n, mpz_class &d);
  void loadPublicKey(const std ::string &filename, mpz_class &n);
  mpz_class generateLargePrime();

  const mpz_class e = 65537;
  const int keyBitSize = 1024;
};
} // namespace rsa
