#include "Converter.h"

/*
function that converts the base of a num from decimal to binary.
input: the num to be converted to binary, the buffer which the num in binary base will be stored in,
and the length of the buffer.
output: None.
*/
void decToBinary(int numToConvert, unsigned char* buffer, int lengthOfBuffer)
{
	int i = 0;
	while (numToConvert > 0 && i < lengthOfBuffer)
	{
		buffer[lengthOfBuffer - i - 1] = numToConvert % BINARY_BASE;
		numToConvert /= BINARY_BASE;
		i++;
	}
}

/*
    convert binary into decimal
*/
int binToDecimal(long long n)
{
    int decimalNumber = 0, i = 0, remainder = 0;
    while (n != 0)
    {
        remainder = n % DECIMAL_BASE;
        n /= DECIMAL_BASE;
        decimalNumber += remainder * pow(BINARY_BASE, i);
        ++i;
    }
    return decimalNumber;
}

/*
    string two numbers
*/
int stringNumbers(int num1, int num2)
{
    std::string combined_numbers =
        std::to_string(num1) + std::to_string(num2);


    return std::stoi(combined_numbers);
}

/*
function that converts char pointer to vector of unsinged char.
input: char array, its size and the vector to store the char array.
output: None.
*/
void CharPointerToVector(char* charArray, int sizeOfCharArray, std::vector<uint8_t>& charVector)
{
	for (int i = 0; i < sizeOfCharArray; i++)
	{
		charVector.push_back(charArray[i]);
	}
}

/*
function that converts vector to a char array.
input: char array to store the vector content, its size and a vector of unsinged char.
output: None.
*/
void vectorToCharArray(char* charArray, std::vector<uint8_t>& charVector)
{
	int lengthOfVector = charVector.size();
	for (int i = 0; i < lengthOfVector; i++)
	{
		charArray[i] = charVector[i];
	}
}