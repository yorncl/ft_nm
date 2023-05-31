#include "nm.h"
#include "elf.h"
#include <libft.h>
#include <stdint.h>
#include <sys/types.h>

// program header
Elf64_Ehdr* filehdr;
// section header
Elf64_Shdr* sechdr; 
// string table with symbols name
char* strtab;
// string table with section name
char* shstrtab;




t_list* extract_symbols(void* file, Elf64_Shdr* sym_header)
{
	Elf64_Sym* sentry = file + read64(&sym_header->sh_offset);
	uint64_t n = read64(&sym_header->sh_size) / read64(&sym_header->sh_entsize);
	t_list* symbols = NULL;
	t_list* node;
	symbol_info* symbol;

	for (uint64_t i = 1; i < n; i++) // first entry is undefined
	{
		if (read32(&sentry[i].st_name) != 0)
		{
			symbol = ft_calloc(1, sizeof(symbol_info));
			if (symbol == NULL)
			{
				ft_lstclear(&symbols, free);
				return NULL;
			}
			// Fill out the symbol info
			symbol->st_info = sentry[i].st_info;
			symbol->st_other = sentry[i].st_other;
			symbol->st_value = read64(&sentry[i].st_value);
			symbol->st_shndx = read16(&sentry[i].st_shndx);
			if (symbol->st_shndx < 0xff00) // TODO maybe we need more checks
			{
				Elf64_Shdr *section = &sechdr[symbol->st_shndx];
				symbol->sh_name = get_name(shstrtab, section);
				symbol->sh_flags = read64(&section->sh_flags);
				symbol->sh_type = read32(&section->sh_type);
			}
			if (sentry[i].st_name != 0)
				symbol->st_name = get_name(strtab, &sentry[i]); // TODO what happens if strtab is empty ?
			else
				symbol->st_name = NULL;
			symbol->st_type = compute_type(symbol);
			if (symbol->st_type == 0) // no type --> discard
			{
				free(symbol);
				continue;
			}
			// append to the list
			node = ft_lstnew(symbol);
			if (node == NULL)
			{
				free(symbol);
				ft_lstclear(&symbols, free);
				return NULL;
			}
			ft_lstadd_back(&symbols, node);
		}
	}
	return symbols;
}

uint64_t identify_sections(void* file, t_list** symbol_tables)
{
	size_t n = read16(&filehdr->e_shnum);
	for (size_t i = 1; i < n; i++) // first entry is undefined
	{
		uint32_t type = read32(&sechdr[i].sh_type);
		if (type == SHT_STRTAB)
		{
			if (ft_strcmp(".strtab", get_name(shstrtab, &sechdr[i])) == 0)
				strtab = (char*)file + read64(&sechdr[i].sh_offset);
		}
		if (type == SHT_SYMTAB) // TODO multiple symtab
		{
			if (ft_strcmp(".symtab", get_name(shstrtab, &sechdr[i])) == 0)
			{
				t_list* new = ft_lstnew(&sechdr[i]);
				ft_lstadd_back(symbol_tables, new);
			}
		}
	}
	return 0;
}

int parse_elf_64(void* file)
{
	t_list* symbols = NULL;
	t_list* symbol_tables = NULL;

	// setting default values
	filehdr = file;
	sechdr = file + read64(&filehdr->e_shoff); // get the section header table
	shstrtab = (char*)file + read64(&sechdr[read16(&filehdr->e_shstrndx)].sh_offset); // get the section names string table
	strtab = NULL;

	// TODO check for missing sections

	// identify other sections of interest
	identify_sections(file, &symbol_tables);
	if (strtab == NULL)
	{
		ft_printf("no string table found\n"); // TODO stderr change message
		return 0;
	}
	if (symbol_tables == NULL) // TODO handle multiples
	{
		ft_printf("no symbol table found\n"); // TODO stderr change message
		return 0;
	}
	for (t_list* node = symbol_tables; node != NULL; node = node->next)
	{
		t_list* curr = extract_symbols(file, node->content);
		if (curr == NULL)
		{
			ft_lstclear(&symbols, free);
			// TODO perror ?
			return 1;
		}
		ft_lstadd_back(&symbols, curr);
	}

	// print symbols
	for (t_list* node = symbols; node != NULL; node = node->next)
	{
		symbol_info* entry = node->content;
		
		if (entry->st_value != 0)
			ft_printf("%016x %c %s\n", entry->st_value, entry->st_type, entry->st_name);
		else
			ft_printf("%16c %c %s\n", ' ', entry->st_type, entry->st_name);
		/* ft_printf("Symbol name putain : %s\n", get_symbol_name(entry->sym)); */
	}
	ft_lstclear(&symbol_tables, 0);
	ft_lstclear(&symbols, free);
	return 0;
}
