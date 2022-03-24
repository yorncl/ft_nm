#include <stdio.h>
#include <stdlib.h>
#include <libft.h>
#include "ft_nm.h"

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

t_list* load_symbols(Elf64_Shdr* symtab, Elf64_Shdr* strtab, struct mfile* mf)
{
	char *str;
	t_list* symlist = NULL;
	// TODO check for alignement ?
	int64_t nsym = symtab->sh_size / symtab->sh_entsize;
	Elf64_Sym* sentry = (Elf64_Sym*)(mf->data + symtab->sh_offset);

	while(nsym--)
	{
		str = &mf->data[strtab->sh_offset + sentry->st_name];
		sl_push_back(&symlist, sentry->st_value, get_sym_type(sentry->st_info, sentry->st_other, sentry->st_shndx), str); 
		sentry += 1;
	}
	return symlist;
}

t_list* read_ia64(struct mfile* mf)
{
	t_list *symlist = NULL;
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
		if (s[i].sh_type == SHT_SYMTAB)
		{
			symlist = load_symbols(&s[i], strtab, mf); // TODO manage multiple symtab
		}
	}
	return symlist;
}

