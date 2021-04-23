#pragma once

#include <ctime>
#include <bitset>

#define BITS_IN_BYTE 8


//	taken from here:
//		https://stackoverflow.com/questions/20024690/is-there-byte-data-type-in-c
typedef std::bitset<BITS_IN_BYTE> byte;

struct RequestInfo
{
	int id;
	std::time_t recivalTime;
	std::vector<byte> buffer;
};


class IRequestHandler
{
public:
	virtual bool isRequestRelevent(RequestInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo) = 0;
};

