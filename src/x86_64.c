#include "nm.h"
#include "elf.h"
#include <libft.h>
#include <stdint.h>

// program header
Elf64_Ehdr* filehdr;
// section header
Elf64_Shdr* sechdr; 
// string table with symbols name
char* strtab;
// string table with section name
char* shstrtab;
// symbol table
Elf64_Shdr* symtab;

t_list* extract_symbols(void* file, Elf64_Shdr* sym_header)
{
	Elf64_Sym *table = (void*)(file + read64(&sym_header->sh_offset));
	uint64_t n = read64(&sym_header->sh_size) / read64(&sym_header->sh_entsize);
	for (uint64_t i = 1; i < n; i++) // first entry is undefined
	{
		if (read32(&table[i].st_name) != 0)
		{
			ft_printf("%d: %s\n", i, strtab + read32(&table[i].st_name));
		}
	}
	return NULL;
}

char* get_section_name(Elf64_Shdr* sec)
{
	return shstrtab + read32(&sec->sh_name);
}

uint64_t identify_sections(void* file)
{
	size_t n = read16(&filehdr->e_shnum);
	for (size_t i = 1; i < n; i++) // first entry is undefined
	{
		uint32_t type = read32(&sechdr[i].sh_type);
		if (type == SHT_STRTAB)
		{
			if (ft_strcmp(".strtab", get_section_name(&sechdr[i])) == 0)
				strtab = (char*)file + read64(&sechdr[i].sh_offset);
		}
		if (type == SHT_SYMTAB) // TODO multiple symtab
		{
			if (ft_strcmp(".symtab", get_section_name(&sechdr[i])) == 0)
				symtab = &sechdr[i];
		}
	}
	return 0;
}

int parse_elf_64(void* file)
{
	t_list* symbols = NULL;

	// setting default values
	filehdr = file;
	sechdr = file + read64(&filehdr->e_shoff); // get the section header table
	shstrtab = (char*)file + read64(&sechdr[read16(&filehdr->e_shstrndx)].sh_offset); // get the section names string table
	strtab = 0;

	// TODO check for missing sections

	// identify other sections of interest
	identify_sections(file);
	if (strtab == NULL)
	{
		ft_printf("no string table found\n"); // TODO stderr change message
		return 0;
	}

	if (symtab == NULL) // TODO handle multiples
	{
		ft_printf("no symbol table found\n"); // TODO stderr change message
		return 0;
	}

	ft_lstadd_back(&symbols, extract_symbols(file, symtab));
	return 0;
}
