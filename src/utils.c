#include "nm.h"
#include <stdint.h>
#include <libft.h>

uint16_t (*read16)(uint16_t* file) = 0;
uint32_t (*read32)(uint32_t* file) = 0;
uint64_t (*read64)(uint64_t* file) = 0;

// TODO check for bounds ?

uint64_t read64_little(uint64_t* ptr)
{
	return *ptr;
}

uint32_t read32_little(uint32_t* ptr)
{
	return *ptr;
}

uint16_t read16_little(uint16_t* ptr)
{
	return *ptr;
}


uint64_t read64_big(uint64_t* ptr)
{
	return ptr[0] << 56 | ptr[1] << 48 | ptr[2] << 40 | ptr[3] << 32 | ptr[4] << 24 | ptr[5] << 16 | ptr[6] << 8 | ptr[7];
}

uint32_t read32_big(uint32_t* ptr)
{
	return ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 | ptr[3];
}

uint16_t read16_big(uint16_t* ptr)
{
	return  ptr[0] << 8 | ptr[1];
}


char compute_type(unsigned char st_info, unsigned char st_other, uint16_t st_shndx, char* sh_name, uint32_t sh_info, uint64_t sh_flags)
{
	ft_printf("st_info: %d, st_other: %d, sh_name: %s, sh_info: %d, sh_flags: %d\n", st_info, st_other, sh_name, sh_info, sh_flags);		

	if (st_shndx == SHN_ABS)
		return 0;
	return '?';
}

