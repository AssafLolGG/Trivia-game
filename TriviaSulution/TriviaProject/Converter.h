#pragma once

#include <math.h>
#include <string>
#include <vector>

#define BINARY_BASE 2
#define DECIMAL_BASE 10
#define NUM_OF_BITS_IN_BYTE 8

void decToBinary(int numToConvert, unsigned char* buffer, int lengthOfBuffer); // copied from serializer. to avoid conflict remove from serializer after merge and delete this comment.

int binToDecimal(long long n);

int stringNumbers(int num1, int num2);

void CharPointerToVector(char* charArray, int sizeOfCharArray, std::vector<uint8_t>& charVector);

void vectorToCharArray(char* charArray, std::vector<uint8_t>& charVector);

int bitsToBytes(int startingIndex, uint8_t* bitsArray);
