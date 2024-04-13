#include <includes/DES.hpp>

#include <iostream>
#include <fstream>

int PC1[56] =
{
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

int shifts[16] =
{
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1
};

int PC2[48] =
{
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int IP[64] =
{
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

int FP[64] =
{
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9,  49, 17, 57, 25
};

int E[48] =
{
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1
};

int S[8][4][16]=
{
    {
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    },
    {
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    },
    {
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    },
    {
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    },
    {
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
    },
    {
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    },
    {
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    },
    {
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    }
};

int P[32] =
{
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

std::bitset<32> DES::feistelFunction(const std::bitset<32>& halfBlock, const std::bitset<48>& key)
{
    // Expand the half-block from 32 bits to 48 bits
    std::bitset<48> expandedBlock;
    for (int i = 0; i < 48; i++)
    {
        expandedBlock[47 - i] = halfBlock[32 - E[i]];
    }

    // XOR with the subkey
    std::bitset<48> xored = expandedBlock ^ key;

    // Apply the S-boxes
    std::bitset<32> output;
    for (int i = 0; i < 8; i++)
    {
        int row = (xored[6 * i] << 1) + xored[6 * i + 5];
        int col = (xored[6 * i + 1] << 3) + (xored[6 * i + 2] << 2) + (xored[6 * i + 3] << 1) + xored[6 * i + 4];
        int num = S[i][row][col];
        for (int j = 0; j < 4; j++)
        {
            output[4 * i + j] = (num >> (3 - j)) & 1;
        }
    }

    // Permutation after the S-boxes
    std::bitset<32> permuted;
    for (int i = 0; i < 32; i++)
    {
        permuted[31 - i] = output[32 - P[i]];
    }

    return permuted;
}

std::bitset<64> DES::encryptBlock(std::bitset<64> block, std::vector<std::bitset<48>> subKeys)
{
    std::cout << "Encrypting block " << block << std::endl;
    std::bitset<64> permutedBlock;
    for (int i = 0; i < 64; i++) {
        permutedBlock[63 - i] = block[64 - IP[i]];
    }

    std::bitset<32> left = std::bitset<32>((permutedBlock >> 32).to_ulong());
    std::bitset<32> right = std::bitset<32>((permutedBlock & std::bitset<64>(0xFFFFFFFF)).to_ulong());
    std::bitset<32> originalLeft;

    for (int i = 0; i < 16; i++)
    {
        originalLeft = left;
        left = right;
        right = originalLeft ^ feistelFunction(right, subKeys[i]);  // Placeholder for the F function
    }

    permutedBlock = (std::bitset<64>(right.to_ulong()) << 32) | std::bitset<64>(left.to_ulong());

    std::bitset<64> finalBlock;
    for (int i = 0; i < 64; i++) {
        finalBlock[63 - i] = permutedBlock[64 - FP[i]];
    }

    return finalBlock;
}

std::bitset<64> DES::decryptBlock(std::bitset<64> block, std::vector<std::bitset<48>> subKeys)
{
    std::bitset<64> permutedBlock;
    for (int i = 0; i < 64; i++) {
        permutedBlock[63 - i] = block[64 - IP[i]];
    }

    std::bitset<32> right = std::bitset<32>((permutedBlock >> 32).to_ulong());
    std::bitset<32> left = std::bitset<32>((permutedBlock & std::bitset<64>(0xFFFFFFFF)).to_ulong());
    std::bitset<32> originalRight;

    // Perform rounds in reverse order
    for (int i = 15; i >= 0; i--) {
        originalRight = right;
        right = left;
        // Apply the F function and round key in reverse
        left = originalRight ^ feistelFunction(right, subKeys[i]);  // Feistel function is the same as encryption
    }

    // Swap left and right before final permutation
    permutedBlock = (std::bitset<64>(left.to_ulong()) << 32) | std::bitset<64>(right.to_ulong());

    std::bitset<64> plaintext;
    // Final permutation
    for (int i = 0; i < 64; i++) {
        plaintext[63 - i] = permutedBlock[64 - FP[i]];
    }

    std::cout << "Decrypted block " << plaintext << std::endl;

    return plaintext;
}

void DES::encrypt(IOConfig& ioConfig)
{
    std::bitset<64> key = keyStringToBitset(ioConfig.key);
    std::vector<std::bitset<48>> subKeys = generateKeys(key);

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
        std::cout << "Buffer: " << buffer << "\n";
        std::bitset<64> block;
        for (int i = 0; i < inputFile.gcount(); ++i)
        {
            block = (block << 8) | std::bitset<64>(static_cast<unsigned char>(buffer[i]));
        }

        if (inputFile.gcount() < 8)
        {
            block <<= 8 * (8 - inputFile.gcount());
        }

        std::bitset<64> encryptedBlock = encryptBlock(block, subKeys);
        for (int i = 7; i >= 0; --i)
        {
            outputFile.put(static_cast<char>((encryptedBlock >> (i * 8)).to_ulong()));
        }
    }

    inputFile.close();
    outputFile.close();
}

void DES::decrypt(IOConfig& ioConfig)
{
    std::bitset<64> key = keyStringToBitset(ioConfig.key);
    std::vector<std::bitset<48>> subKeys = generateKeys(key);

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
        std::bitset<64> block;
        for (int i = 0; i < inputFile.gcount(); ++i)
        {
            block = (block << 8) | std::bitset<64>(static_cast<unsigned char>(buffer[i]));
        }

        if (inputFile.gcount() < 8)
        {
            block <<= 8 * (8 - inputFile.gcount());
        }

        std::bitset<64> encryptedBlock = decryptBlock(block, subKeys);
        for (int i = 7; i >= 0; --i)
        {
            outputFile.put(static_cast<char>((encryptedBlock >> (i * 8)).to_ulong()));
        }
    }

    inputFile.close();
    outputFile.close();
}

std::bitset<56> DES::initialPermutation(const std::bitset<64>& key)
{
    std::bitset<56> permutedKey;
    for (int i = 0; i < 56; i++) {
        permutedKey[55 - i] = key[64 - PC1[i]];
    }
    return permutedKey;
}

std::bitset<28> DES::leftCircularShift(std::bitset<28>& halfKey, int shifts)
{
    return (halfKey << shifts) | (halfKey >> (28 - shifts));
}

std::vector<std::bitset<48>> DES::generateKeys(const std::bitset<64>& key) {
    std::bitset<56> permutedKey = initialPermutation(key);
    std::bitset<28> left = std::bitset<28>((permutedKey >> 28).to_ulong());
    std::bitset<28> right = std::bitset<28>((permutedKey & std::bitset<56>(0x00FFFFFFF)).to_ulong());
    std::vector<std::bitset<48>> subKeys;

    for (int i = 0; i < 16; i++) {
        left = leftCircularShift(left, shifts[i]);
        right = leftCircularShift(right, shifts[i]);

        std::bitset<56> combinedKey = (std::bitset<56>(left.to_ulong()) << 28) | std::bitset<56>(right.to_ulong());
        std::bitset<48> compressedKey;
        for (int j = 0; j < 48; j++) {
            compressedKey[47 - j] = combinedKey[56 - PC2[j]];
        }
        subKeys.push_back(compressedKey);
    }
    return subKeys;
}

std::bitset<64> DES::keyStringToBitset(std::string keyStr)
{
    std::bitset<64> bitset;
    for (size_t i = 0; i < 8; i++) {
        std::bitset<8> charBits = std::bitset<8>(keyStr[i]);
        for (int j = 0; j < 8; j++) {
            bitset[64 - 8 * i + j - 1] = charBits[7 - j];
        }
    }

    return bitset;
}
