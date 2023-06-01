#include "nm.h"
#include "elf.h"
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

// This following functions send shivers down my spine
// I don't know if I'm horrified or proud
// Turns out that for Elf64_Sym and Elf32_Sym, the first 4 bytes are the same field
// Same for Elf64_Shdr and Elf32_Shdr
char* get_name(char* base, void* ptr)
{
	return base + read32(ptr);
}

char compute_type(symbol_info* info)
{
	uint8_t type = ELF_ST_TYPE(info->st_info);
	uint8_t bind = ELF_ST_BIND(info->st_info);

	// filter for sectiopn index
	if (info->st_shndx >= 0xff00) // TODO maybe we need more checks
		return '?';
	// now that the section index is valid, we can get the section name
	if (info->sh_type == SHT_PROGBITS && info->sh_flags & SHF_ALLOC && info->sh_flags ^ SHF_WRITE)
		return bind == STB_GLOBAL ? 'R' : 'r';	
	if (ft_strcmp(".text", info->sh_name) == 0)
		return bind == STB_GLOBAL ? 'T' : 't';
	if (ft_strcmp(".bss", info->sh_name) == 0)
		return bind == STB_GLOBAL ? 'B' : 'b';
	if (info->sh_flags & SHF_ALLOC && info->sh_flags & SHF_WRITE)
		return bind == STB_GLOBAL ? 'D' : 'd';
	if (type == STT_COMMON)
		return 'C';
	if (info->st_shndx == SHN_UNDEF)
		return 'U';
	return '?';
}

#define ISALNUM(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9'))
#define TOUPPER(c) ('a' <= c && c <= 'z' ? c - 32 : c)

int compare_symbol_name(symbol_info* a, symbol_info* b)
{
	char *s1 = a->st_name;
	char *s2 = b->st_name;
	char c1, c2;
	while (!ISALNUM(*s1))
		s1++;
	while (!ISALNUM(*s2)) // TODO testcase if it ignores all the non alphanum in comparison, even after first alphanum
		s2++;
	while (*s1 && *s2)
	{
		while (*s1 && !ISALNUM(*s1))
			s1++;
		while (*s2 && !ISALNUM(*s2))
			s2++;
		c1 = TOUPPER(*s1);
		c2 = TOUPPER(*s2);
		if (c1 != c2)
			break;
		s1++;
		s2++;
	}
	if (*s1 == 0 && *s2 == 0)
		return ft_strlen(a->st_name) > ft_strlen(b->st_name);
	return TOUPPER(*s1) - TOUPPER(*s2) < 0;
}

void quick_sort(symbol_info** arr, int low, int high, int (*compare)(symbol_info*, symbol_info*))
{
	int top = low;
	symbol_info* tmp;

	for (int i = low; i < high; i++)
	{
		if (compare(arr[i], arr[high]))
		{
			tmp = arr[i];
			arr[i] = arr[top];
			arr[top] = tmp;
			top++;
		}
	}
	tmp = arr[top];
	arr[top] = arr[high];
	arr[high] = tmp;

	if (top - 1 > low)
		quick_sort(arr, low, top - 1, compare);
	if (top + 1 < high)
		quick_sort(arr, top + 1, high, compare);
}
