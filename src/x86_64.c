#include "nm.h"
#include "elf.h"
#include <libft.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

// program header
Elf64_Ehdr* filehdr;
// section header
Elf64_Shdr* sechdr; 
// string table with symbols name
char* strtab;
// string table with section name
char* shstrtab;

size_t symcount;




int extract_symbols(void* file, Elf64_Shdr* sym_header, symbol_info** symbols_array)
{
	Elf64_Sym* sentry = file + read64(&sym_header->sh_offset);
	symbol_info* symbol;

	size_t j = 0;
	for (uint64_t i = 1; i < symcount; i++) // skipping first entry
	{
		if (read32(&sentry[i].st_name) != 0)
		{
			symbol = ft_calloc(1, sizeof(symbol_info));
			
			if (symbol == NULL)
				return 1; // TODO error handling
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
				symbol->st_name = "";
			symbol->st_type = compute_type(symbol);
			symbols_array[j] = symbol;
			j++;
		}
	}
	symcount = j;
	return 0;
}

int identify_sections(void* file, Elf64_Shdr** symtab)
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
				*symtab = &sechdr[i];
				// TODO feels ugly, refactor ?
				// TODO what about multiple .symtab sections
				symcount += read64(&sechdr[i].sh_size) / read64(&sechdr[i].sh_entsize); 
			}
		}
	}
	return 0;
}

int parse_elf_64(void* file)
{
	symbol_info** symbols_array = NULL;
	Elf64_Shdr* symtab = NULL;
	symcount = 0;

	// setting default values

	filehdr = file;
	// get the section header table
	sechdr = file + read64(&filehdr->e_shoff);
	// get the section names string table
	shstrtab = (char*)file + read64(&sechdr[read16(&filehdr->e_shstrndx)].sh_offset);
	strtab = NULL;

	// TODO check for missing sections

	// identify other sections of interest
	identify_sections(file, &symtab);
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


	symbols_array = ft_calloc(symcount, sizeof(symbol_info*));
	if (symbols_array == NULL)
		return 1; // TODO error handling
	if (extract_symbols(file, symtab, symbols_array))
	{
		for (size_t i = 0; symbols_array[i] != 0; i++)
			free(symbols_array[i]);	
		free(symbols_array);
		ft_printf("Errororrorororor\n"); // TODO stderr change message
		return 1;
	}

	// sorting symbols by name TODO maybe do the options for symbol size and whatnot
	quick_sort(symbols_array, 0, symcount - 1, &compare_symbol_name);

	// print symbols
	for (size_t i = 0; i < symcount; i++)
	{
		/* ft_printf("pointer value : %p\n", symbols_array[i]); */
		symbol_info* entry = symbols_array[i];
		if (entry->st_type == '?')
			continue;
		
		if (entry->st_value != 0)
			ft_printf("%016x %c %s\n", entry->st_value, entry->st_type, entry->st_name);
		else
			ft_printf("%16c %c %s\n", ' ', entry->st_type, entry->st_name);
	}
	// freeing everything
	for (size_t i = 0; i < symcount; i++)
		free(symbols_array[i]);	
	free(symbols_array);
	return 0;
}

