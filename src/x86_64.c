#include "nm.h"
#include "elf.h"
#include <libft.h>
#include <stdint.h>


t_list* extract_symbols(void* file, Elf64_Shdr *sym_header, char* strings)
{
	Elf64_Sym *table = (void*)(file + read64(&sym_header->sh_offset));
	uint64_t n = read64(&sym_header->sh_size) / read64(&sym_header->sh_entsize);
	for (uint64_t i = 1; i < n; i++) // first entry is undefined
	{
		if (read32(&table[i].st_name) != 0)
		{
			ft_printf("%d: %s\n", i, strings + read32(&table[i].st_name));
		}
	}
	return NULL;
}

uint64_t get_str_table_offset(Elf64_Shdr* table, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		if (read32(&table[i].sh_type) == SHT_STRTAB)
		{
			ft_printf("%d : string section at %x\n", i, read64(&table[i].sh_offset));
			return read64(&table[i].sh_offset);
		}
	}
	return 0;
}

int parse_elf_64(void* file)
{
	Elf64_Ehdr* header = file;
	Elf64_Shdr* sec_table = file + read64(&header->e_shoff);
	t_list* symbols = NULL;

	uint64_t stroff = get_str_table_offset(sec_table, read16(&header->e_shnum));
	// print offest
	ft_printf("offset : %x\n", stroff);
	ft_printf("offset : %u\n", stroff);
	return 1;
	if (stroff == 0)
	{
		ft_printf("Error: no string table\n");
		return 1;
	}

	char* strings = file + stroff;
	ft_printf("This is the address : %p and offset %x \n", strings, stroff);
	for (int i = 0; i < 2000; i++)
	{
		ft_printf("%c", strings[i] == 0 ? ' ' : strings[i]);
	}
		ft_printf("\n");
	return 1;

	// print section types
	for (int i = 0; i < read16(&header->e_shnum); i++)
	{
		ft_printf("section %d type : %x\n", i, read32(&sec_table[i].sh_type));
		if (read32(&sec_table[i].sh_type) == SHT_SYMTAB)
		{
			ft_lstadd_back(&symbols, extract_symbols(file, &sec_table[i], file + stroff));
		}
	}

	ft_printf("value : %x\n", read64(&sec_table[1].sh_type));
	return 0;
}
