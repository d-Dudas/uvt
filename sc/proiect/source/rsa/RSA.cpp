#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include <gmp.h>

#include <includes/rsa/RSA.hpp>

namespace rsa
{
void RSA::saveKey(
    const std::string& filename,
    const mpz_class& a,
    const mpz_class& b)
{
  std::ofstream outFile(filename, std::ios::trunc);
  if (outFile.is_open())
  {
    outFile << a.get_str(16) << std::endl;
    outFile << b.get_str(16) << std::endl;
    outFile.close();
  }
  else
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
  }
}

void RSA::loadKey(const std ::string& filename, mpz_class& a, mpz_class& b)
{
  std::ifstream inFile(filename);
  if (inFile.is_open())
  {
    std::string line;
    std::getline(inFile, line);
    a = mpz_class(line, 16);
    std::getline(inFile, line);
    b = mpz_class(line, 16);
    inFile.close();
  }
  else
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
  }
}

mpz_class RSA::generateLargePrime()
{
  gmp_randclass rng(gmp_randinit_mt);
  rng.seed(std::random_device{}());
  mpz_class prime = rng.get_z_bits(keyBitSize);

  mpz_setbit(prime.get_mpz_t(), 0);
  mpz_nextprime(prime.get_mpz_t(), prime.get_mpz_t());

  return prime;
}

void RSA::generateKeys(IOConfig& ioConfig)
{
  mpz_class p, q;

  p = generateLargePrime();
  q = generateLargePrime();

  mpz_class n = p * q;
  mpz_class phi = (p - 1) * (q - 1);

  mpz_class e = 65537;

  while (mpz_gcd_ui(nullptr, phi.get_mpz_t(), e.get_ui()) != 1)
  {
    e += 2;
  }

  mpz_class d;
  if (! mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t()))
  {
    throw std::runtime_error("Modular inverse of e cannot be computed.");
  }

  const std::string privateKeyFile = ioConfig.keyFile;
  const std::string publicKeyFile = ioConfig.keyFile + ".pub";

  saveKey(privateKeyFile, n, d);
  saveKey(publicKeyFile, n, e);
}

mpz_class RSA::rsaEncrypt(
    const mpz_class& m,
    const mpz_class& e,
    const mpz_class& n)
{
  mpz_class c;
  mpz_powm(c.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  return c;
}

void RSA::encrypt(IOConfig& ioConfig)
{
  mpz_class n, e;
  const std::string publicKeyFile = ioConfig.keyFile;
  loadKey(publicKeyFile, n, e);

  std::ifstream inputFile(ioConfig.inputFile, std::ios::binary);
  std::ofstream outputFile(ioConfig.outputFile, std::ios::binary);

  if (inputFile.is_open() && outputFile.is_open())
  {
    char buffer[128];
    while (! inputFile.eof())
    {
      inputFile.read(buffer, sizeof(buffer));
      std::streamsize bytesRead = inputFile.gcount();

      if (bytesRead < sizeof(buffer))
      {
        std::memset(buffer + bytesRead, 0, sizeof(buffer) - bytesRead);
      }

      mpz_class m;
      mpz_import(
          m.get_mpz_t(), sizeof(buffer), 1, sizeof(buffer[0]), 0, 0, buffer);

      mpz_class c = rsaEncrypt(m, e, n);

      size_t countp;
      char* cipherText =
          (char*)mpz_export(nullptr, &countp, 1, 1, 0, 0, c.get_mpz_t());

      outputFile.write(cipherText, countp);

      delete[] cipherText;
    }

    inputFile.close();
    outputFile.close();
  }
  else
  {
    std::cerr << "Failed to open file." << std::endl;
  }
}

mpz_class RSA::rsaDecrypt(
    const mpz_class& c,
    const mpz_class& d,
    const mpz_class& n)
{
  mpz_class m;
  mpz_powm(m.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
  return m;
}

void RSA::decrypt(IOConfig& ioConfig)
{
  mpz_class n, d;
  const std::string privateKeyFile = ioConfig.keyFile;
  loadKey(privateKeyFile, n, d);

  std::ifstream inputFile(ioConfig.inputFile, std::ios::binary);
  std::ofstream outputFile(ioConfig.outputFile, std::ios::binary);

  if (inputFile.is_open() && outputFile.is_open())
  {
    char buffer[256];
    while (inputFile.read(buffer, sizeof(buffer)))
    {
      std::streamsize bytesRead = inputFile.gcount();

      mpz_class c;
      mpz_import(
          c.get_mpz_t(), sizeof(buffer), 1, sizeof(buffer[0]), 0, 0, buffer);

      mpz_class m = rsaDecrypt(c, d, n);

      size_t countp;
      char* plainText =
          (char*)mpz_export(nullptr, &countp, 1, 1, 0, 0, m.get_mpz_t());

      while (countp > 0 && plainText[countp - 1] == 0)
      {
        --countp;
      }

      outputFile.write(plainText, countp);

      delete[] plainText;
    }

    inputFile.close();
    outputFile.close();
  }
  else
  {
    std::cerr << "Failed to open file." << std::endl;
  }
}
} // namespace rsa
