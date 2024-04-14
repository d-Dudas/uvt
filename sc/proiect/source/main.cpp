#include <iostream>
#include <getopt.h>

#include <includes/IOConfig.hpp>
#include <includes/Constants.hpp>
#include <includes/DES.hpp>
#include <includes/aes/AES.hpp>

int parseOptions(int argc, char* argv[], IOConfig& ioConfig)
{
	int opt;
	while ((opt = getopt(argc, argv, "d:e:o:k:t:")) != -1)
	{
		switch (opt)
		{
			case 'd':
				ioConfig.inputFile = optarg;
				break;
			case 'e':
				ioConfig.inputFile = optarg;
				ioConfig.encrypt = true;
				break;
			case 'o':
				ioConfig.outputFile = optarg;
				break;
			case 'k':
				ioConfig.key = optarg;
				if(ioConfig.key.size() != 8)
				{
					std::cerr << "Key must be 8 bytes (characters) long\n";
					return FAILURE;
				}
				break;
			case 't':
				switch(optarg[0]) // TODO: update this to compare strings
				{
					case 'd':
						ioConfig.algorithm = AlgorithmSuite::DES;
						break;
					case 'a':
						ioConfig.algorithm = AlgorithmSuite::AES;
						break;
					case 'r':
						ioConfig.algorithm = AlgorithmSuite::RSA;
						break;
					default:
						std::cerr << "Invalid algorithm\nValid algorithms are: [\"des\", \"aes\", \"rsa\"]\n";
						return FAILURE;
				}
				break;
			default:
				std::cerr << "Usage: " << argv[0] << " -(d/e) inputfile -k key -t (rsa/dsa/aes) [-o outputfile]\n";
				return FAILURE;
		}
	}

	if (ioConfig.inputFile.empty() || ioConfig.key.empty() || ioConfig.algorithm == AlgorithmSuite::INVALID)
	{
        std::cerr << "Usage: " << argv[0] << " -(d/e) inputfile -k key -t (rsa/dsa/aes) [-o outputfile]\n";
		return FAILURE;
	}

	return SUCCESS;
}

int main(int argc, char* argv[])
{
	IOConfig config;

	if(parseOptions(argc, argv, config) != SUCCESS)
	{
		return FAILURE;
	}

	switch (config.algorithm)
	{
		case AlgorithmSuite::DES:
			DES des;
			if(config.encrypt)
				des.encrypt(config);
			else
				des.decrypt(config);
			break;
		case AlgorithmSuite::AES:
			AES aes;
			if(config.encrypt)
				aes.encrypt(config);
			else
			    aes.decrypt(config);
			break;
	}

	return 0;
}
