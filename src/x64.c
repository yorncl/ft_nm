#include "elf.h"
#include "read_elf.h"
#include "utils.h"
#include "map_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <libft.h>

Elf64_Shdr* find_strtab(Elf64_Shdr* s, int16_t n, char* names)// TODO check if 2 strtab ?
{
	for (int16_t i = 0; i < n; i++)
	{
		if (s[i].sh_type == SHT_STRTAB && 
			ft_strcmp(&names[s[i].sh_name], ".strtab") == 0) //TODO manage multiple string tab
		{
			printf("STRTAB found at index : %u\n", i);
			return &s[i];
		}
	}
	return NULL;
}

int print_symbols(Elf64_Shdr* symtab, Elf64_Shdr* strtab, struct mfile* mf)
{
	// TODO check for alignement ?
	int64_t nsym = symtab->sh_size / symtab->sh_entsize;
	printf("entity size : %lu\n", symtab->sh_entsize);
	Elf64_Sym* sentry = (Elf64_Sym*)(mf->data + symtab->sh_offset);
	char *str;
	while(nsym--)
	{
		str = (char*)(mf->data + strtab->sh_offset + sentry->st_name);
		printf("%s\n", str);
		sentry += 1;
	}
	return 0;
}

int read_ia64(struct mfile* mf)
{
	Elf64_Ehdr* h = (Elf64_Ehdr*)mf->data;

	if (h->e_shstrndx == 0 ) // SHN_UNDEF
	{
		printf("There isn't any section name table :(\n");
		exit(1);
		// TODO what to do in this case ?
	}


	int64_t shstart = h->e_shoff; // section header offset
	// TODO check if offset bigger than file ?
	int16_t n = h->e_shnum;
	printf("Section header is at offset %lu\n", shstart);
	printf("There is %d sections\n", n);

	Elf64_Shdr* s = (Elf64_Shdr*)&mf->data[shstart];
	Elf64_Shdr* ns = &s[h->e_shstrndx]; // name table for sections headers
	Elf64_Shdr* strtab = find_strtab(s, n, &mf->data[ns->sh_offset]);

	for(int16_t i = 0; i < n; i++)
	{
		printf("Type of section %x\n", s[i].sh_type);
		if (s[i].sh_type == SHT_SYMTAB)
		{

			printf("Value of %x\n", s[i].sh_type);
			print_symbols(&s[i], strtab, mf);
		}
	}
	return 0;
}

