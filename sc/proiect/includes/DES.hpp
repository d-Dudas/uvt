#pragma once

#include <string>
#include <bitset>
#include <vector>

#include <includes/IOConfig.hpp>

class DES {
public:
    void encrypt(IOConfig& ioConfig);
    void decrypt(IOConfig& ioConfig);

private:
    std::bitset<56> initialPermutation(const std::bitset<64> &key);
    std::bitset<28> leftCircularShift(std::bitset<28>& halfKey, int shifts);
    std::vector<std::bitset<48>> generateKeys(const std::bitset<64> &key);
    std::bitset<64> keyStringToBitset(std::string keyStr);
    std::bitset<32> feistelFunction(const std::bitset<32> &halfBlock, const std::bitset<48> &key);
    std::bitset<64> encryptBlock(std::bitset<64> block, std::vector<std::bitset<48>> subKeys);
    std::bitset<64> decryptBlock(std::bitset<64> block, std::vector<std::bitset<48>> subKeys);
};