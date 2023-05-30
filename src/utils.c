#include "nm.h"

uint64_t read64_little(void* addr)
{
	return *(uint32_t*)addr;
}

uint32_t read32_little(void* addr)
{
	return *(uint32_t*)addr;
}

uint16_t read16_little(void* addr)
{
	return *(uint16_t*)addr;
}

uint8_t read8_little(void* addr)
{
	return *(uint8_t*)addr;
}

uint64_t read64_big(void* addr)
{
	uint64_t ret = 0;
	uint8_t* ptr = addr;
	for (int i = 0; i < 8; i++)
	{
		ret <<= 8;
		ret |= ptr[i];
	}
	return ret;
}

uint32_t read32_big(void* addr)
{
	uint32_t ret = 0;
	uint8_t* ptr = addr;
	for (int i = 0; i < 4; i++)
	{
		ret <<= 8;
		ret |= ptr[i];
	}
	return ret;
}

uint16_t read16_big(void* addr)
{
	uint16_t ret = 0;
	uint8_t* ptr = addr;
	for (int i = 0; i < 8; i++)
	{
		ret <<= 8;
		ret |= ptr[i];
	}
	return ret;
}

uint8_t read8_big(void* addr)
{
	uint8_t ret = 0;
	uint8_t* ptr = addr;
	for (int i = 0; i < 8; i++)
	{
		ret <<= 8;
		ret |= ptr[i];
	}
	return ret;
}

