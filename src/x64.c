#include "elf.h"
#include "read_elf.h"
#include "utils.h"
#include "map_file.h"
#include <stdio.h>

Elf64_Shdr* find_strtab(Elf64_Shdr* s, uint16_t n)// TODO check if 2 strtab ?
{
	
	while (n--)
	{
		if (s->sh_type == SHT_STRTAB)
			return s;
		s += 1;
	}
	return NULL;
}


int print_symbols(Elf64_Shdr* symtab, Elf64_Shdr* strtab, struct mfile* mf)
{
	// TODO check for alignement ?
	uint64_t nsym = symtab->sh_size / symtab->sh_entsize;
	Elf64_Sym* sentry = (Elf64_Sym*)(mf->data + symtab->sh_offset);
	char *str;
	while(nsym--)
	{
		printf("%lu\n", strtab->sh_offset);
		str = (char*)(mf->data + strtab->sh_offset + sentry->st_name);
		printf("%s\n", str);
		sentry += 1;
	}
	return 0;
}

#include <unistd.h>

int read_ia64(struct mfile* mf)
{
	(void) mf;
	Elf64_Ehdr* h = (Elf64_Ehdr*)mf->data;

	uint64_t shstart = h->e_shoff; // section header offset
	// TODO check if offset bigger than file ?
	uint16_t n = h->e_shnum;
	printf("Section header is at offset %lu\n", shstart);
	printf("There is %u sections\n", n);

	Elf64_Shdr* s = (Elf64_Shdr*)(mf->data + shstart);
	Elf64_Shdr* strtab = find_strtab(s, n);

	// Reading sections 
	while(n--)
	{
		if (s->sh_type == SHT_SYMTAB)
			print_symbols(s, strtab, mf);
		s += 1;
	}
	return 0;
}
