#pragma once

#include <bitset>
#include <vector>

#include <includes/IOConfig.hpp>

class AES
{
public:
    void encrypt(IOConfig& ioConfig);
    void decrypt(IOConfig& ioConfig);
private:
    // key
    void keyExpansionCore(std::bitset<8> word[4], int rconIterationCount);
    std::vector<std::bitset<8>> keyExpansion(const std::string& key);

    // utils
    void addPKCS7Padding(std::vector<std::bitset<8>>& block);
    void removePKCS7Padding(std::vector<std::bitset<8>> &data);
    std::vector<std::bitset<8>> convertCharBufferToBitsetVector(const char *buffer, size_t length);

    // encrypt
    void encryptBlock(std::vector<std::bitset<8>> &plaintext, const std::vector<std::bitset<8>> &expandedKeys);
    void subBytes(std::vector<std::bitset<8>> &state);
    void shiftRows(std::vector<std::bitset<8>> &state);
    void mixColumns(std::vector<std::bitset<8>> &state);

    // decrypt
    void decryptBlock(std::vector<std::bitset<8>> &ciphertext, const std::vector<std::bitset<8>> &expandedKeys);
    void invSubBytes(std::vector<std::bitset<8>> &state);
    void invShiftRows(std::vector<std::bitset<8>> &state);
    void invMixColumns(std::vector<std::bitset<8>> &state);

    // common
    void addRoundKey(std::vector<std::bitset<8>> &state, const std::vector<std::bitset<8>> &roundKey);
};
