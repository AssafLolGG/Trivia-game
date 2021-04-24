#pragma once

#include <math.h>
#include <string>

#define BINARY_BASE 2
#define DECIMAL_BASE 10

void decToBinary(int numToConvert, unsigned char* buffer, int lengthOfBuffer); // copied from serializer. to avoid conflict remove from serializer after merge and delete this comment.

int binToDecimal(long long n);

int stringNumbers(int num1, int num2);
