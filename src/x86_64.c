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

char* get_section_name(Elf64_Shdr* sec)
{
	return shstrtab + read32(&sec->sh_name);
}

char* get_symbol_name(Elf64_Sym* sym)
{
	return strtab + read32(&sym->st_name);
}

t_list* extract_symbols(void* file, Elf64_Shdr* sym_header)
{
	Elf64_Sym* sentry = file + read64(&sym_header->sh_offset);
	uint64_t n = read64(&sym_header->sh_size) / read64(&sym_header->sh_entsize);
	t_list* symbols = NULL;
	Elf64_Shdr* s;
	char type;
	t_list* node;
	sym64* symbol;

	for (uint64_t i = 1; i < n; i++) // first entry is undefined
	{
		if (read32(&sentry[i].st_name) != 0)
		{
			ft_printf("%d: %s\n", i, get_symbol_name(&sentry[i]));
			s = sechdr + read16(&sentry[i].st_shndx); // TODO problem if shndx has a speical value

			// TODO change, gross hack to compile
			uint16_t index = read16(&sentry[i].st_shndx);
			type = 0;
			if (index != SHN_ABS)
				type = compute_type(sentry[i].st_info, // TODO move to printing stage ?
							sentry[i].st_other,
							read16(&sentry[i].st_shndx),
							get_section_name(s),
							read32(&s->sh_info),
							read64(&s->sh_flags)
							);
			// if the type is 0, the symbol is discarded
			if (type == 0)
				continue;
			symbol = ft_calloc(1, sizeof(sym64));
			if (symbol == NULL)
			{
				ft_lstclear(&symbols, free);
				return NULL;
			}
			symbol->sym = &sentry[i];
			symbol->shdr = s;
			symbol->type = type;
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
			if (ft_strcmp(".strtab", get_section_name(&sechdr[i])) == 0)
				strtab = (char*)file + read64(&sechdr[i].sh_offset);
		}
		if (type == SHT_SYMTAB) // TODO multiple symtab
		{
			if (ft_strcmp(".symtab", get_section_name(&sechdr[i])) == 0)
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
	ft_printf("Extracting symbols\n");

	// print symbols
	for (t_list* node = symbols; node != NULL; node = node->next)
	{
		sym64* entry = node->content;
		
		uint64_t value = read64(&entry->sym->st_value);
		if (value != 0)
			ft_printf("%016x %c %s\n", value, entry->type, get_symbol_name(entry->sym));
		else
			ft_printf("%16c %c %s\n", ' ', entry->type, get_symbol_name(entry->sym));
		/* ft_printf("Symbol name putain : %s\n", get_symbol_name(entry->sym)); */
	}
	ft_lstclear(&symbol_tables, 0);
	ft_lstclear(&symbols, free);
	return 0;
}
