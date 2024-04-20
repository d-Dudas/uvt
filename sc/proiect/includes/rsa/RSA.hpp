#pragma once

#include <gmpxx.h>

#include <includes/interfaces/ICryptographicAlgorithm.hpp>

namespace rsa
{
class RSA : public ICryptographicAlgorithm
{
  public:
  void generateKeys(IOConfig& ioConfig);
  void encrypt(IOConfig& ioConfig) override;
  void decrypt(IOConfig& ioConfig) override;

  private:
  void saveKey(
      const std::string& filename,
      const mpz_class& a,
      const mpz_class& b);
  void loadKey(const std ::string& filename, mpz_class& a, mpz_class& b);
  mpz_class generateLargePrime();
  mpz_class rsaEncrypt(
      const mpz_class& m,
      const mpz_class& e,
      const mpz_class& n);
  mpz_class rsaDecrypt(
      const mpz_class& c,
      const mpz_class& d,
      const mpz_class& n);

  const int keyBitSize = 1024;
};
} // namespace rsa
