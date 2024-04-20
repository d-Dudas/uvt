#include <fstream>
#include <includes/rsa/RSA.hpp>
#include <iostream> // remove this later
#include <random>
#include <sstream>

#include <gmp.h>

namespace rsa
{
void RSA::savePublicKey(const std::string &filename, const mpz_class &n)
{
  std::ofstream outFile(filename, std::ios::trunc);
  if (outFile.is_open())
  {
    outFile << n.get_str(16) << std::endl;
    outFile.close();
  }
  else
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
  }
}

void RSA::savePrivateKey(const std::string &filename, const mpz_class &n,
                         const mpz_class &d)
{
  std::ofstream outFile(filename, std::ios::trunc);
  if (outFile.is_open())
  {
    outFile << n.get_str(16) << std::endl;
    outFile << d.get_str(16) << std::endl;
    outFile.close();
  }
  else
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
  }
}

void RSA::loadPrivateKey(const std ::string &filename, mpz_class &n,
                         mpz_class &d)
{
  std::ifstream inFile(filename);
  if (inFile.is_open())
  {
    std::string line;
    std::getline(inFile, line);
    n = mpz_class(line, 16);
    std::getline(inFile, line);
    d = mpz_class(line, 16);
    inFile.close();
  }
  else
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
  }
}

void RSA::loadPublicKey(const std ::string &filename, mpz_class &n)
{
  std::ifstream inFile(filename);
  if (inFile.is_open())
  {
    std::string line;
    std::getline(inFile, line);
    n = mpz_class(line, 16);
    inFile.close();
  }
  else
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
  }
}

mpz_class RSA::generateLargePrime()
{
  gmp_randclass rng(gmp_randinit_default);
  rng.seed(std::random_device{}());
  mpz_class prime = rng.get_z_bits(keyBitSize);

  mpz_setbit(prime.get_mpz_t(), 0);
  mpz_nextprime(prime.get_mpz_t(), prime.get_mpz_t());

  return prime;
}

void RSA::generateKeys(IOConfig &ioConfig)
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
  if (!mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t()))
  {
    throw std::runtime_error("Modular inverse of e cannot be computed.");
  }

  const std::string privateKeyFile = ioConfig.keyFile;
  const std::string publicKeyFile = ioConfig.keyFile + ".pub";

  savePrivateKey(privateKeyFile, n, d);
  savePublicKey(publicKeyFile, n);
}

void RSA::encrypt(IOConfig &ioConfig)
{
  mpz_class n, d;
  loadPrivateKey(ioConfig.keyFile, n, d);

  std::cout << "n bits:" << mpz_sizeinbase(n.get_mpz_t(), 2) << std::endl;
  std::cout << "d bits:" << mpz_sizeinbase(d.get_mpz_t(), 2) << std::endl;

  std::cout << "Encrypting" << std::endl;
}

void RSA::decrypt(IOConfig &ioConfig)
{
  std::cout << "Decrypting" << std::endl;
}
} // namespace rsa
