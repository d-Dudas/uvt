#include <iostream>
#include <getopt.h>

#include <includes/IOConfig.hpp>
#include <includes/Constants.hpp>
#include <includes/DES.hpp>

int parseOptions(int argc, char* argv[], IOConfig& ioConfig)
{
	int opt;
	while ((opt = getopt(argc, argv, "d:e:o:k:")) != -1)
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
			default:
				std::cerr << "Usage: " << argv[0] << " -(d/e) inputfile [-o outputfile]\n";
				return FAILURE;
		}
	}

	if (ioConfig.inputFile.empty() || ioConfig.key.empty())
	{
        std::cerr << "Usage: " << argv[0] << " -(d/e) inputfile -k key [-o outputfile]\n";
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

	DES des;

	if(config.encrypt)
		des.encrypt(config);
	else
		des.decrypt(config);

	return 0;
}

