#include <includes/aes/AES.hpp>
#include <includes/aes/Structures.hpp>

#include <iostream> // remove this later
#include <fstream>

void AES::keyExpansionCore(std::bitset<8> word[4], int rconIterationCount)
{
    // Rotate left
    std::bitset<8> temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;

    // S-Box
    for (int i = 0; i < 4; i++)
    {
        word[i] = s[word[i].to_ulong()];
    }

    // RCon
    word[0] ^= rcon[rconIterationCount];
}

std::vector<std::bitset<8>> AES::keyExpansion(const std::string& key)
{
    std::vector<std::bitset<8>> expandedKey;
    for (int i = 0; i < 16; i++)
    {
        expandedKey.push_back(std::bitset<8>(key[i]));
    }

    int bytesGenerated = 16;
    int rconIterationCount = 1;
    std::bitset<8> temp[4];

    while (bytesGenerated < 176)
    {
        for (int i = 0; i < 4; i++)
        {
            temp[i] = expandedKey[i + bytesGenerated - 4];
        }

        if (bytesGenerated % 16 == 0)
        {
            keyExpansionCore(temp, rconIterationCount);
            rconIterationCount++;
        }

        for (int i = 0; i < 4; i++)
        {
            expandedKey.push_back(expandedKey[bytesGenerated - 16] ^ temp[i]);
            bytesGenerated++;
        }
    }

    return expandedKey;
}

void AES::subBytes(std::vector<std::bitset<8>>& state)
{
    for (auto& byte : state)
    {
        byte = s[byte.to_ulong()];
    }
}

void AES::invSubBytes(std::vector<std::bitset<8>> &state)
{
    for (auto& byte : state)
    {
        byte = inv_s[byte.to_ulong()];
    }
}

void AES::shiftRows(std::vector<std::bitset<8>>& state)
{
    std::vector<std::bitset<8>> temp(16);

    temp[0] = state[0];
    temp[1] = state[5];
    temp[2] = state[10];
    temp[3] = state[15];

    temp[4] = state[4];
    temp[5] = state[9];
    temp[6] = state[14];
    temp[7] = state[3];

    temp[8] = state[8];
    temp[9] = state[13];
    temp[10] = state[2];
    temp[11] = state[7];

    temp[12] = state[12];
    temp[13] = state[1];
    temp[14] = state[6];
    temp[15] = state[11];

    state = temp;
}

void AES::invShiftRows(std::vector<std::bitset<8>> &state)
{
    std::vector<std::bitset<8>> temp(16);

    temp[0] = state[0];
    temp[1] = state[13];
    temp[2] = state[10];
    temp[3] = state[7];

    temp[4] = state[4];
    temp[5] = state[1];
    temp[6] = state[14];
    temp[7] = state[11];

    temp[8] = state[8];
    temp[9] = state[5];
    temp[10] = state[2];
    temp[11] = state[15];

    temp[12] = state[12];
    temp[13] = state[9];
    temp[14] = state[6];
    temp[15] = state[3];

    state = temp;
}

void AES::mixColumns(std::vector<std::bitset<8>>& state)
{
    std::vector<std::bitset<8>> temp(16);

    for (int i = 0; i < 4; i++)
    {
        int col = i * 4;
        temp[col] = std::bitset<8>(mul2[state[col].to_ulong()] ^ mul3[state[col + 1].to_ulong()] ^ state[col + 2].to_ulong() ^ state[col + 3].to_ulong());
        temp[col + 1] = std::bitset<8>(state[col].to_ulong() ^ mul2[state[col + 1].to_ulong()] ^ mul3[state[col + 2].to_ulong()] ^ state[col + 3].to_ulong());
        temp[col + 2] = std::bitset<8>(state[col].to_ulong() ^ state[col + 1].to_ulong() ^ mul2[state[col + 2].to_ulong()] ^ mul3[state[col + 3].to_ulong()]);
        temp[col + 3] = std::bitset<8>(mul3[state[col].to_ulong()] ^ state[col + 1].to_ulong() ^ state[col + 2].to_ulong() ^ mul2[state[col + 3].to_ulong()]);
    }

    state = temp; // Copy the transformed columns back into the original state
}

void AES::invMixColumns(std::vector<std::bitset<8>> &state)
{
    std::vector<std::bitset<8>> temp(16);

    for (int i = 0; i < 4; i++)
    {
        int col = i * 4;
        temp[col] = std::bitset<8>(mul14[state[col].to_ulong()] ^ mul11[state[col + 1].to_ulong()] ^ mul13[state[col + 2].to_ulong()] ^ mul9[state[col + 3].to_ulong()]);
        temp[col + 1] = std::bitset<8>(mul9[state[col].to_ulong()] ^ mul14[state[col + 1].to_ulong()] ^ mul11[state[col + 2].to_ulong()] ^ mul13[state[col + 3].to_ulong()]);
        temp[col + 2] = std::bitset<8>(mul13[state[col].to_ulong()] ^ mul9[state[col + 1].to_ulong()] ^ mul14[state[col + 2].to_ulong()] ^ mul11[state[col + 3].to_ulong()]);
        temp[col + 3] = std::bitset<8>(mul11[state[col].to_ulong()] ^ mul13[state[col + 1].to_ulong()] ^ mul9[state[col + 2].to_ulong()] ^ mul14[state[col + 3].to_ulong()]);
    }

    state = temp; // Copy the transformed columns back into the original state
}

void AES::addRoundKey(std::vector<std::bitset<8>>& state, const std::vector<std::bitset<8>>& roundKey)
{
    for (size_t i = 0; i < state.size(); i++)
    {
        state[i] ^= roundKey[i];
    }
}

void AES::encryptBlock(std::vector<std::bitset<8>>& plaintext, const std::vector<std::bitset<8>>& expandedKeys)
{
    std::vector<std::bitset<8>> state = plaintext;

    // Initial round key addition
    addRoundKey(state, std::vector<std::bitset<8>>(expandedKeys.begin(), expandedKeys.begin() + 16));

    // Main rounds
    for (int round = 1; round < 10; ++round) { // 9 rounds if AES-128
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, std::vector<std::bitset<8>>(expandedKeys.begin() + 16 * round, expandedKeys.begin() + 16 * (round + 1)));
    }

    // Final round (no MixColumns)
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, std::vector<std::bitset<8>>(expandedKeys.begin() + 160, expandedKeys.begin() + 176));

    plaintext = state; // Output the encrypted state
}

std::vector<std::bitset<8>> AES::convertCharBufferToBitsetVector(const char* buffer, size_t length)
{
    std::vector<std::bitset<8>> bitsetVector;
    bitsetVector.reserve(length);

    for (size_t i = 0; i < length; ++i)
    {
        bitsetVector.emplace_back(static_cast<unsigned char>(buffer[i]));
    }

    return bitsetVector;
}

void AES::addPKCS7Padding(std::vector<std::bitset<8>>& block)
{
    size_t blockSize = 16;
    size_t paddingSize = blockSize - (block.size() % blockSize);
    paddingSize = (paddingSize == 0) ? blockSize : paddingSize;

    block.resize(block.size() + paddingSize);

    for (size_t i = 0; i < paddingSize; ++i)
    {
        block[block.size() - paddingSize + i] = static_cast<unsigned char>(paddingSize);
    }
}

void AES::encrypt(IOConfig& ioConfig)
{
    for (int i = 0; i < 16 && ioConfig.key.size() < 16; i++)
    {
        ioConfig.key += ioConfig.key[i];
    }

    std::vector<std::bitset<8>> expandedKey = keyExpansion(ioConfig.key);

    std::ifstream inputFile(ioConfig.inputFile, std::ios::binary);
    std::ofstream outputFile(ioConfig.outputFile, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cerr << "Error opening files\n";
        return;
    }

    char buffer[16];
    while (inputFile.read(buffer, 16))
    {
        std::vector<std::bitset<8>> block = convertCharBufferToBitsetVector(buffer, inputFile.gcount());

        encryptBlock(block, expandedKey);
        for(auto byte : block)
        {
            outputFile.put(static_cast<char>(byte.to_ulong()));
        }
    }

    // handle last chunk
    std::vector<std::bitset<8>> block = convertCharBufferToBitsetVector(buffer, inputFile.gcount());
    addPKCS7Padding(block);

    encryptBlock(block, expandedKey);
    for(auto byte : block)
    {
        outputFile.put(static_cast<char>(byte.to_ulong()));
    }

    inputFile.close();
    outputFile.close();
}

void AES::decryptBlock(std::vector<std::bitset<8>> &ciphertext, const std::vector<std::bitset<8>> &expandedKeys)
{
    std::vector<std::bitset<8>> state = ciphertext;

    // Initial round key addition
    addRoundKey(state, std::vector<std::bitset<8>>(expandedKeys.begin() + 160, expandedKeys.begin() + 176));

    // Main rounds
    for (int round = 9; round > 0; --round) { // 9 rounds if AES-128
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, std::vector<std::bitset<8>>(expandedKeys.begin() + 16 * round, expandedKeys.begin() + 16 * (round + 1)));
        invMixColumns(state);
    }

    // Final round (no MixColumns)
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, std::vector<std::bitset<8>>(expandedKeys.begin(), expandedKeys.begin() + 16));

    ciphertext = state; // Output the decrypted state
}

void AES::removePKCS7Padding(std::vector<std::bitset<8>>& data)
{
    if (data.empty())
    {
        return;
    }

    size_t paddingSize = data.back().to_ulong();

    if (paddingSize == 0 || paddingSize > data.size())
    {
        return;
    }

    for (size_t i = 0; i < paddingSize; i++)
    {
        if (data[data.size() - 1 - i].to_ulong() != paddingSize) {
            return;
        }
    }

    data.resize(data.size() - paddingSize);
}

void AES::decrypt(IOConfig& ioConfig)
{
    for (int i = 0; i < 16 && ioConfig.key.size() < 16; i++)
    {
        ioConfig.key += ioConfig.key[i];
    }

    std::vector<std::bitset<8>> expandedKey = keyExpansion(ioConfig.key);

    std::ifstream inputFile(ioConfig.inputFile, std::ios::binary);
    std::ofstream outputFile(ioConfig.outputFile, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cerr << "Error opening files\n";
        return;
    }

    char buffer[16];
    while (inputFile.read(buffer, 16))
    {
        std::vector<std::bitset<8>> block = convertCharBufferToBitsetVector(buffer, inputFile.gcount());

        decryptBlock(block, expandedKey);
        removePKCS7Padding(block);
        for(auto byte : block)
        {
            outputFile.put(static_cast<char>(byte.to_ulong()));
        }
    }

    inputFile.close();
    outputFile.close();
}