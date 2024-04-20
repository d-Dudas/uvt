#pragma once

#include <bitset>
#include <vector>

#include <includes/IOConfig.hpp>
#include <includes/interfaces/ICryptographicAlgorithm.hpp>

namespace aes
{
class AES : public ICryptographicAlgorithm
{
public:
  AES(Algorithms algorithm);
  void encrypt(IOConfig &ioConfig) override;
  void decrypt(IOConfig &ioConfig) override;

private:
  u_int8_t numberOfRounds;
  u_int8_t keyCoefficient;
  static const u_int8_t blockSize = 16;

  // key
  void keyExpansionCore(std::bitset<8> word[4], int rconIterationCount);
  std::vector<std::bitset<8>> keyExpansion(const std::string &key);

  // utils
  void addPKCS7Padding(std::vector<std::bitset<8>> &block);
  void removePKCS7Padding(std::vector<std::bitset<8>> &data);
  std::vector<std::bitset<8>>
  convertCharBufferToBitsetVector(const char *buffer, size_t length);

  // encrypt
  void encryptBlock(std::vector<std::bitset<8>> &plaintext,
                    const std::vector<std::bitset<8>> &expandedKeys);
  void subBytes(std::vector<std::bitset<8>> &state);
  void shiftRows(std::vector<std::bitset<8>> &state);
  void mixColumns(std::vector<std::bitset<8>> &state);

  // decrypt
  void decryptBlock(std::vector<std::bitset<8>> &ciphertext,
                    const std::vector<std::bitset<8>> &expandedKeys);
  void invSubBytes(std::vector<std::bitset<8>> &state);
  void invShiftRows(std::vector<std::bitset<8>> &state);
  void invMixColumns(std::vector<std::bitset<8>> &state);

  // common
  void addRoundKey(std::vector<std::bitset<8>> &state,
                   const std::vector<std::bitset<8>> &roundKey);
};
} // namespace aes
