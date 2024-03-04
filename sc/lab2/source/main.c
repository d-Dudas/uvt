#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_MODE "r"
#define WRITE_MODE "w"

#define HELP_PRINTED 1
#define SUCCESS 0
#define EXPECTED_ARGUMENT_NOT_FOUND -1
#define UNKNOWN_ARGUMENT -2
#define ERROR_OPENING_FILE -3
#define ERROR_READING_FILE -4
#define UNKNOWN_MODE -5

#define VERIFY_RETURN_VALUE(value) \
    if (value != SUCCESS)          \
    {                              \
        return value;              \
    }

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

int isEncryptFileFlag(const char *flag)
{
    return strcmp(flag, "-e") == 0;
}

int isDecryptyFileFlag(const char *flag)
{
    return strcmp(flag, "-d") == 0;
}

int isOutputFileFlag(const char *flag)
{
    return strcmp(flag, "-o") == 0;
}

int isHelpFlag(const char *flag)
{
    return strcmp(flag, "-h") == 0 || strcmp(flag, "--help") == 0;
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

int configureInputFile(const char *filename, const mode mode)
{
    FILE *file = fopen(filename, READ_MODE);
    if (file == NULL)
    {
        return ERROR_OPENING_FILE;
    }

    clearConfigInputFile();
    globalConfig.inputFile = file;
    globalConfig.mode = mode;

    return SUCCESS;
}

int configureOutputFile(const char *filename)
{
    FILE *file = fopen(filename, WRITE_MODE);
    if (file == NULL)
    {
        return ERROR_OPENING_FILE;
    }

    clearConfigOutputFile();
    globalConfig.outputFile = file;

    return SUCCESS;
}

void printHelpMessage(const char *appName)
{
    printf("%s usage:\n", appName);
    printf("%s <flag> <filename> <flag> <filename>\n", appName);
    printf("\n");
    printf("Flags:\n");
    printf("-e <filename>\t\t Encrypt file\n");
    printf("-d <filename>\t\t Decrypt file\n");
    printf("-o <filename>\t\t Output file\n");
    printf("\n");
    printf("\nAn output file should be specified.\n");
}

int readFlags(int argc, char **args)
{
    int ret;
    for (int i = 1; i < argc; i++)
    {
        if (isEncryptFileFlag(args[i]))
        {
            if (++i < argc)
            {
                ret = configureInputFile(args[i], encrypt);
                VERIFY_RETURN_VALUE(ret);
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
                ret = configureInputFile(args[i], decrypt);
                VERIFY_RETURN_VALUE(ret);
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
                ret = configureOutputFile(args[i]);
                VERIFY_RETURN_VALUE(ret);
            }
            else
            {
                return EXPECTED_ARGUMENT_NOT_FOUND;
            }
        }
        else if (isHelpFlag(args[i]))
        {
            printHelpMessage(args[0]);
            return HELP_PRINTED;
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

int writeToOutputFile(const char *output)
{
    fwrite(output, sizeof(char), globalConfig.inputFileSize, globalConfig.outputFile);

    return SUCCESS;
}

int encryptData(const char *inputData)
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

int decryptData(const char *inputData)
{
    char *auxData = (char *)malloc(globalConfig.inputFileSize);
    strcpy(auxData, inputData);

    for (int i = 0; i < globalConfig.inputFileSize; i++)
    {
        auxData[i] = (auxData[i] - globalConfig.cipher) % 255;
    }

    writeToOutputFile(auxData);

    return SUCCESS;
}

int main(int argc, char **args)
{
    int ret;
    globalConfig.cipher = 5;

    ret = readFlags(argc, args);
    if (ret != SUCCESS)
    {
        if (ret == HELP_PRINTED)
        {
            return SUCCESS;
        }
        goto error;
    }

    char *inputData = readInputFile();
    if (inputData == NULL)
    {
        ret = ERROR_READING_FILE;
        goto error;
    }

    switch (globalConfig.mode)
    {
    case encrypt:
        ret = encryptData(inputData);
        if (ret != SUCCESS)
            goto error;
        break;
    case decrypt:
        ret = decryptData(inputData);
        if (ret != SUCCESS)
            goto error;

    default:
        ret = UNKNOWN_MODE;
        goto error;
        break;
    }

    return SUCCESS;
error:
    printf("Something went wrong.\n");
    printf("Error code: %d\n", ret);

    clearConfigInputFile();
    clearConfigOutputFile();

    return ret;
}
