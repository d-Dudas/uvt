//	*********************************************************************
//	*  sha1.cpp : cpp source for the hash function sha-1 implemntation  *
//	*  the implementation is based on the specification doc fips 180-1  *
//	*********************************************************************

#include "sha1.h"

#include <string.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <iomanip>

void initHs()
{
	// load the initial values for H's
	H[0] = 0x67452301;
	H[1] = 0xefcdab89;
	H[2] = 0x98badcfe;
	H[3] = 0x10325476;
	H[4] = 0xc3d2e1f0;
}

//	------------------------------------------------------------------

void initKs()
{
	// load the initial values for K's
	int i;
	for (i=00; i<20 ; i++)   K[i] = 0x5a827999;
	for (i=20; i<40 ; i++)   K[i] = 0x6ed9eba1;
	for (i=40; i<60 ; i++)   K[i] = 0x8f1bbcdc;
	for (i=60; i<80 ; i++)   K[i] = 0xca62c1d6;
}

//	------------------------------------------------------------------

int paddMessage(int messLen)
{
	int origLenBits = messLen * 8;
	int fullLen = ((messLen + 8) / 64) * 64 + 64;

	binmsg[messLen] = 0x80; // 0x80 in binary si 10000000

	for (int i = messLen + 1; i < fullLen - 8; i++) {
		binmsg[i] = 0;
	}

	for (int i = 0; i < 8; i++) {
        binmsg[fullLen - 1 - i] = (origLenBits >> (i * 8)) & 0xFF;
    }

	return fullLen / 64;
}

//	------------------------------------------------------------------

void getWsfromM(int currentBlock)
{
    uint32 word;

    for (int i = 0; i < 16; i++) {
        word = 0;
        for (int j = 0; j < 4; j++) {
            word = (word << 8) | binmsg[currentBlock * 64 + i * 4 + j];
        }
        W[i] = word;
    }
}

//	------------------------------------------------------------------

void getAsfromHs()
{
    A = H[0];
    B = H[1];
    C = H[2];
    D = H[3];
    E = H[4];
}

//	------------------------------------------------------------------

std::string digestToString(uint32 H[])
{
	char buffer[256];
	sprintf(buffer, "%9x %9x %9x %9x %9x",
			H[0], H[1], H[2], H[3], H[4]);
	return std::string(buffer);
}

//	------------------------------------------------------------------

void hashBlocks(uint numBlocks)
{
	for (int i=0; i<numBlocks; i++)
	{
		getWsfromM(i);

		for (int j=16; j<80; j++)
		{
			W[j] = W[j-3]^W[j-8]^W[j-14]^W[j-16];
			W[j] = S(W[j], 1);
		}

		getAsfromHs();

		for (int j=0; j<80; j++)
		{
			TEMP = S(A,5);
			int cit = j / 20;

			switch (cit)
			{
			case 0:
				TEMP += F0(B,C,D);
				break;
			case 1:
				TEMP += F1(B,C,D);
				break;
			case 2:
				TEMP += F2(B,C,D);
				break;
			case 3:
				TEMP += F3(B,C,D);
				break;
			}

			TEMP = TEMP + E + W[j] + K[j];

			E=D;
			D=C;
			C=S(B,30);
			B=A;
			A=TEMP;
		}

		H[0] = H[0] + A;
		H[1] = H[1] + B;
		H[2] = H[2] + C;
		H[3] = H[3] + D;
		H[4] = H[4] + E;
	}
}

//	------------------------------------------------------------------

struct IOConfiguratuin
{
	std::string inputFile;
	std::string outputFile;
	bool inputSpecified = false;
};

//	------------------------------------------------------------------

int parseOptions(int argc, char* argv[], IOConfiguratuin& ioConfig)
{
	int opt;
	while ((opt = getopt(argc, argv, "i:o:")) != -1)
	{
		switch (opt)
		{
			case 'i':
				ioConfig.inputFile = optarg;
				ioConfig.inputSpecified = true;
				break;
			case 'o':
				ioConfig.outputFile = optarg;
				break;
			default:
				std::cerr << "Usage: " << argv[0] << " -i inputfile [-o outputfile]\n";
				return FAILURE;
		}
	}

	if (!ioConfig.inputSpecified)
	{
        std::cerr << "Usage: " << argv[0] << " -i inputfile [-o outputfile]\n";
		return FAILURE;
	}

	return SUCCESS;
}

//	------------------------------------------------------------------

void writeResult(IOConfiguratuin &ioConfig)
{
    if (!ioConfig.outputFile.empty())
    {
        std::ofstream out(ioConfig.outputFile);
        if (out.is_open())
        {
            out << digestToString(H);
            out.close();
            std::cout << "Result written to " << ioConfig.outputFile << "\n";
        }
        else
        {
            std::cerr << "Could not open output file\n";
            std::cout << digestToString(H) << "\n";
        }
    }
    else
    {
        std::cout << digestToString(H) << "\n";
    }
}

//	==================================================================

int main(int argc, char *argv[])
{
    IOConfiguratuin ioConfig;

	if(parseOptions(argc, argv, ioConfig) != SUCCESS)
	{
		return FAILURE;
	}

	std::ifstream inFile(ioConfig.inputFile, std::ios::binary);
	if (!inFile)
	{
		std::cerr << "Could not open input file\n";
		return FAILURE;
	}

	initKs();
	initHs();

	int numBlocks;
	char buffer[512];
	while (inFile.read(buffer, sizeof(buffer)))
	{
		std::streamsize bytesRead = inFile.gcount();
		memcpy(binmsg, buffer, bytesRead);

		numBlocks = paddMessage(bytesRead);
		hashBlocks(numBlocks);
	}

	// Handle last block if it's smaller than 512 bytes
	if (inFile.gcount() > 0)
	{
		std::streamsize bytesRead = inFile.gcount();
		memcpy(binmsg, buffer, bytesRead);

		numBlocks = paddMessage(bytesRead);
		hashBlocks(numBlocks);
	}

	inFile.close();

    writeResult(ioConfig);

    return SUCCESS;
}

//	******************************************************************
