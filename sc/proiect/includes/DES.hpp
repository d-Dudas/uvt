#pragma once

#include <string>
#include <bitset>
#include <vector>

#include <includes/IOConfig.hpp>

class DES
{
public:
    void encrypt(IOConfig &ioConfig);
    void decrypt(IOConfig &ioConfig);

private:
    std::bitset<56> initialKeyPermutation(const std::bitset<64> &key);
    std::bitset<28> leftCircularShift(std::bitset<28> &halfKey, int shifts);
    std::vector<std::bitset<48>> generateKeys(const std::bitset<64> &key);
    std::bitset<64> keyStringToBitset(std::string keyStr);
    std::bitset<32> feistelFunction(const std::bitset<32> &halfBlock, const std::bitset<48> &key);
    std::bitset<64> applyPermutations(std::bitset<64> block, std::vector<std::bitset<48>> subKeys);
    void applyFeistelFunction(std::bitset<64> &permutedBlock, std::vector<std::bitset<48>> &subKeys);
    std::bitset<48> createExpandedBlock(const std::bitset<32> &halfBlock);
    std::bitset<32> applySubstitutionBoxesPermutations(const std::bitset<48> &xored);
    std::bitset<32> applyFinalPermutation(const std::bitset<32> &block);
    std::bitset<64> applyInitialPermutation(const std::bitset<64> &block);
    std::bitset<64> applyInversePermutation(const std::bitset<64> &permutedBlock);
    void applyPermutationsOnChunks(IOConfig &ioConfig, std::vector<std::bitset<48>> &subKeys);
};