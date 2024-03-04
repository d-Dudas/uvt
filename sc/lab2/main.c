#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_MODE "r"
#define WRITE_MODE "w"

#define SUCCESS 0
#define EXPECTED_ARGUMENT_NOT_FOUND -1
#define UNKNOWN_ARGUMENT -2
#define ERROR_OPENING_FILE -3

typedef enum mode
{
    unknown,
    encrypt,
    decrypt
} mode;

typedef struct config
{
    FILE *inputFile;
    long inputFileSize;
    FILE *outputFile;
    mode mode;
    __uint8_t cipher;
} config;

config globalConfig;

int isEncryptFileFlag(char *flag)
{
    return strcmp(flag, "-e") == 0;
}

int isDecryptyFileFlag(char *flag)
{
    return strcmp(flag, "-d") == 0;
}

int isOutputFileFlag(char *flag)
{
    return strcmp(flag, "-o") == 0;
}

void clearConfigInputFile()
{
    if (globalConfig.inputFile != NULL)
    {
        fclose(globalConfig.inputFile);
        globalConfig.inputFile = NULL;
    }
}

void clearConfigOutputFile()
{
    if (globalConfig.outputFile != NULL)
    {
        fclose(globalConfig.outputFile);
        globalConfig.outputFile = NULL;
    }
}

int readFlags(int argc, char **args)
{
    for (int i = 1; i < argc; i++)
    {
        if (isEncryptFileFlag(args[i]))
        {
            if (++i < argc)
            {
                FILE *file = fopen(args[i], "r");

                if (file != NULL)
                {
                    clearConfigInputFile();
                    globalConfig.inputFile = file;
                    globalConfig.mode = encrypt;
                }
                else
                {
                    return ERROR_OPENING_FILE;
                }
            }
            else
            {
                return EXPECTED_ARGUMENT_NOT_FOUND;
            }
        }
        else if (isDecryptyFileFlag(args[i]))
        {
            if (++i < argc)
            {
                FILE *file = fopen(args[i], "r");

                if (file != NULL)
                {
                    clearConfigInputFile();
                    globalConfig.inputFile = file;
                    globalConfig.mode = decrypt;
                }
                else
                {
                    return ERROR_OPENING_FILE;
                }
            }
            else
            {
                return EXPECTED_ARGUMENT_NOT_FOUND;
            }
        }
        else if (isOutputFileFlag(args[i]))
        {
            if (++i < argc)
            {
                FILE *file = fopen(args[i], "w");

                if (file != NULL)
                {
                    clearConfigOutputFile();
                    globalConfig.outputFile = file;
                }
                else
                {
                    return ERROR_OPENING_FILE;
                }
            }
            else
            {
                return EXPECTED_ARGUMENT_NOT_FOUND;
            }
        }
        else
        {
            return UNKNOWN_ARGUMENT;
        }
    }

    return SUCCESS;
}

char *readInputFile()
{
    fseek(globalConfig.inputFile, 0, SEEK_END);
    globalConfig.inputFileSize = ftell(globalConfig.inputFile);
    rewind(globalConfig.inputFile);

    char *buffer = (char *)malloc(globalConfig.inputFileSize + 1);
    if (buffer == NULL)
    {
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, globalConfig.inputFileSize, globalConfig.inputFile);
    if (bytesRead < globalConfig.inputFileSize)
    {
        free(buffer);
        return NULL;
    }

    buffer[globalConfig.inputFileSize] = '\0';

    clearConfigInputFile();

    return buffer;
}

int writeToOutputFile(char *output)
{
    fputs(output, globalConfig.outputFile);

    return SUCCESS;
}

int encryptData(char *inputData)
{
    char *auxData = (char *)malloc(globalConfig.inputFileSize);
    strcpy(auxData, inputData);

    for (int i = 0; i < globalConfig.inputFileSize; i++)
    {
        auxData[i] = (auxData[i] + globalConfig.cipher) % 255;
    }

    writeToOutputFile(auxData);

    return SUCCESS;
}

int decryptData(char* inputData)
{
    char *auxData = (char *)malloc(globalConfig.inputFileSize);
    strcpy(auxData, inputData);

    for (int i = 0; i < globalConfig.inputFileSize; i++)
    {
        auxData[i] = (auxData[i] - globalConfig.cipher) % 255;
    }

    printf("%s\n", auxData);

    writeToOutputFile(auxData);

    return SUCCESS;
}

int main(int argc, char **args)
{
    int ret;
    globalConfig.cipher = 5;

    ret = readFlags(argc, args);
    if (ret != 0)
        goto error;

    char *inputData = readInputFile();
    if (inputData == NULL)
        goto error;

    encryptData(inputData);

    switch (globalConfig.mode)
    {
    case encrypt:
        encryptData(inputData);
        break;
    case decrypt:
        decryptData(inputData);
    
    default:
        break;
    }

    return SUCCESS;
error:
    printf("Something went wrong\nError code: %d\n", ret);

    clearConfigInputFile();
    clearConfigOutputFile();

    return ret;
}
