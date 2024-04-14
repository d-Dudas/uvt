#include <algorithm>
#include <fstream>
#include <iostream>

#include <includes/des/DES.hpp>
#include <includes/des/Structures.hpp>

namespace des
{
const std::bitset<56> DES::initialKeyPermutation(const std::bitset<64> &key)
{
  std::bitset<56> permutedKey;
  for (int i = 0; i < 56; i++)
  {
    permutedKey[55 - i] = key[64 - PC1[i]];
  }
  return permutedKey;
}

const std::bitset<28> DES::leftCircularShift(std::bitset<28> &halfKey,
                                             int shifts)
{
  return (halfKey << shifts) | (halfKey >> (28 - shifts));
}

const std::vector<std::bitset<48>> DES::generateKeys(const std::bitset<64> &key)
{
  std::bitset<56> permutedKey = initialKeyPermutation(key);
  std::bitset<28> left = std::bitset<28>((permutedKey >> 28).to_ulong());
  std::bitset<28> right =
      std::bitset<28>((permutedKey & std::bitset<56>(0x00FFFFFFF)).to_ulong());
  std::vector<std::bitset<48>> subKeys;

  for (int i = 0; i < 16; i++)
  {
    left = leftCircularShift(left, shifts[i]);
    right = leftCircularShift(right, shifts[i]);

    std::bitset<56> combinedKey = (std::bitset<56>(left.to_ulong()) << 28) |
                                  std::bitset<56>(right.to_ulong());
    std::bitset<48> compressedKey;
    for (int j = 0; j < 48; j++)
    {
      compressedKey[47 - j] = combinedKey[56 - PC2[j]];
    }
    subKeys.push_back(compressedKey);
  }
  return subKeys;
}

const std::bitset<64> DES::keyStringToBitset(std::string &keyStr)
{
  std::bitset<64> bitset;
  for (size_t i = 0; i < 8; i++)
  {
    std::bitset<8> charBits = std::bitset<8>(keyStr[i]);
    for (int j = 0; j < 8; j++)
    {
      bitset[64 - 8 * i + j - 1] = charBits[7 - j];
    }
  }

  return bitset;
}

const std::bitset<48> DES::createExpandedBlock(const std::bitset<32> &halfBlock)
{
  std::bitset<48> expandedBlock;
  for (int i = 0; i < 48; i++)
  {
    expandedBlock[47 - i] = halfBlock[32 - expansionTable[i]];
  }
  return expandedBlock;
}

const std::bitset<32>
DES::applySubstitutionBoxesPermutations(const std::bitset<48> &xored)
{
  std::bitset<32> output;
  for (int i = 0; i < 8; i++)
  {
    int row = (xored[6 * i] << 1) + xored[6 * i + 5];
    int col = (xored[6 * i + 1] << 3) + (xored[6 * i + 2] << 2) +
              (xored[6 * i + 3] << 1) + xored[6 * i + 4];
    int num = substitutionBoxes[i][row][col];
    for (int j = 0; j < 4; j++)
    {
      output[4 * i + j] = (num >> (3 - j)) & 1;
    }
  }

  return output;
}

const std::bitset<32> DES::applyFinalPermutation(const std::bitset<32> &block)
{
  std::bitset<32> permuted;
  for (int i = 0; i < 32; i++)
  {
    permuted[31 - i] = block[32 - permutationTable[i]];
  }
  return permuted;
}

const std::bitset<32> DES::feistelFunction(const std::bitset<32> &halfBlock,
                                           const std::bitset<48> &key)
{
  std::bitset<48> expandedBlock = createExpandedBlock(halfBlock);
  std::bitset<48> xored = expandedBlock ^ key;
  std::bitset<32> output = applySubstitutionBoxesPermutations(xored);
  std::bitset<32> permuted = applyFinalPermutation(output);

  return permuted;
}

const std::bitset<64> DES::applyInitialPermutation(const std::bitset<64> &block)
{
  std::bitset<64> permutedBlock;
  for (int i = 0; i < 64; i++)
  {
    permutedBlock[63 - i] = block[64 - initialPermutationTable[i]];
  }

  return permutedBlock;
}

const void DES::applyFeistelFunction(std::bitset<64> &permutedBlock,
                                     std::vector<std::bitset<48>> &subKeys)
{
  std::bitset<32> left = std::bitset<32>((permutedBlock >> 32).to_ulong());
  std::bitset<32> right =
      std::bitset<32>((permutedBlock & std::bitset<64>(0xFFFFFFFF)).to_ulong());
  std::bitset<32> originalLeft;

  for (int i = 0; i < 16; i++)
  {
    originalLeft = left;
    left = right;
    right = originalLeft ^ feistelFunction(right, subKeys[i]);
  }

  permutedBlock = (std::bitset<64>(right.to_ulong()) << 32) |
                  std::bitset<64>(left.to_ulong());
}

const std::bitset<64>
DES::applyInversePermutation(const std::bitset<64> &permutedBlock)
{
  std::bitset<64> finalBlock;
  for (int i = 0; i < 64; i++)
  {
    finalBlock[63 - i] = permutedBlock[64 - inversePermutationTable[i]];
  }

  return finalBlock;
}

const std::bitset<64>
DES::applyPermutations(std::bitset<64> &block,
                       std::vector<std::bitset<48>> &subKeys)
{
  std::bitset<64> permutedBlock = applyInitialPermutation(block);
  applyFeistelFunction(permutedBlock, subKeys);
  std::bitset<64> finalBlock = applyInversePermutation(permutedBlock);

  return finalBlock;
}

const std::bitset<64> DES::convertCharBufferToBitset(const char *buffer,
                                                     int bufferSize)
{
  std::bitset<64> block;
  for (int i = 0; i < bufferSize; ++i)
  {
    block =
        (block << 8) | std::bitset<64>(static_cast<unsigned char>(buffer[i]));
  }

  return block;
}

const void DES::addPaddingToBitset(std::bitset<64> &block, int originalSize)
{
  block <<= 8 * (8 - originalSize);
}

const void DES::applyPermutationsOnChunks(IOConfig &ioConfig,
                                          std::vector<std::bitset<48>> &subKeys)
{
  std::ifstream inputFile(ioConfig.inputFile, std::ios::binary);
  std::ofstream outputFile(ioConfig.outputFile, std::ios::binary);

  if (!inputFile.is_open() || !outputFile.is_open())
  {
    std::cerr << "Error opening files\n";
    return;
  }

  char buffer[8];
  while (inputFile.read(buffer, 8) || inputFile.gcount() > 0)
  {
    std::bitset<64> block =
        convertCharBufferToBitset(buffer, inputFile.gcount());

    if (inputFile.gcount() < 8)
    {
      addPaddingToBitset(block, inputFile.gcount());
    }

    std::bitset<64> encryptedBlock = applyPermutations(block, subKeys);
    for (int i = 7; i >= 0; --i)
    {
      outputFile.put(static_cast<char>((encryptedBlock >> (i * 8)).to_ulong()));
    }
  }

  inputFile.close();
  outputFile.close();
}

void DES::encrypt(IOConfig &ioConfig)
{
  std::bitset<64> key = keyStringToBitset(ioConfig.key);
  std::vector<std::bitset<48>> subKeys = generateKeys(key);

  applyPermutationsOnChunks(ioConfig, subKeys);
}

void DES::decrypt(IOConfig &ioConfig)
{
  std::bitset<64> key = keyStringToBitset(ioConfig.key);
  std::vector<std::bitset<48>> subKeys = generateKeys(key);
  std::reverse(subKeys.begin(), subKeys.end());

  applyPermutationsOnChunks(ioConfig, subKeys);
}
} // namespace des
