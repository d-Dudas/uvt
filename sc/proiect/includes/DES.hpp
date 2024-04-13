#pragma once

#include <string>
#include <bitset>
#include <vector>

#include <includes/IOConfig.hpp>

class DES
{
public:
    const void encrypt(IOConfig &ioConfig);
    const void decrypt(IOConfig &ioConfig);

private:
    const std::bitset<56> initialKeyPermutation(const std::bitset<64> &key);
    const std::bitset<28> leftCircularShift(std::bitset<28> &halfKey, int shifts);
    const std::vector<std::bitset<48>> generateKeys(const std::bitset<64> &key);
    const std::bitset<64> keyStringToBitset(std::string& keyStr);
    const std::bitset<32> feistelFunction(const std::bitset<32> &halfBlock, const std::bitset<48> &key);
    const std::bitset<64> applyPermutations(std::bitset<64> &block, std::vector<std::bitset<48>> &subKeys);
    const void applyFeistelFunction(std::bitset<64> &permutedBlock, std::vector<std::bitset<48>> &subKeys);
    const std::bitset<48> createExpandedBlock(const std::bitset<32> &halfBlock);
    const std::bitset<32> applySubstitutionBoxesPermutations(const std::bitset<48> &xored);
    const std::bitset<32> applyFinalPermutation(const std::bitset<32> &block);
    const std::bitset<64> applyInitialPermutation(const std::bitset<64> &block);
    const std::bitset<64> applyInversePermutation(const std::bitset<64> &permutedBlock);
    const void applyPermutationsOnChunks(IOConfig &ioConfig, std::vector<std::bitset<48>> &subKeys);
    const std::bitset<64> convertCharBufferToBitset(const char *buffer, int bufferSize);
    const void addPaddingToBitset(std::bitset<64> &block, int originalSize);
};