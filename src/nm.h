#ifndef NM_H
#define NM_H

#include <stdint.h>

#include "elf.h"

typedef struct symbol_info
{
  // ElfX_Sym fields
  uint8_t st_info;
  uint8_t st_other;
  uint64_t st_value;
  uint16_t st_shndx;
  // infos of the corresponding section
  uint32_t sh_flags;
  uint64_t sh_type;
  char* sh_name;
  // Processed symbol fields
  char* st_name;
  char st_type;
} symbol_info;

int parse_elf_64(void*);

// little endian read
uint16_t read16_little(uint16_t*);
uint32_t read32_little(uint32_t*);
uint64_t read64_little(uint64_t*);

// big endian read
uint16_t read16_big(uint16_t*);
uint32_t read32_big(uint32_t*);
uint64_t read64_big(uint64_t*);

// vars that point to the correct read functions
// first defined in utils.c
// then set in main.c as of now
extern uint16_t (*read16)(uint16_t* file);
extern uint32_t (*read32)(uint32_t* file);
extern uint64_t (*read64)(uint64_t* file);

// Other section/symbol flags that aren't in the ELF header from linux
#define BSF_INDIRECT            (1 << 13)
	
#define ISALNUM(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9'))
#define TOUPPER(c) ('a' <= c && c <= 'z' ? c - 32 : c)

char* get_name(char* base, void* ptr);
int compare_symbol_name(symbol_info* a, symbol_info* b);
void quick_sort(symbol_info** arr, int low, int high, int (*compare)(symbol_info*, symbol_info*));
char compute_type(symbol_info* info); // sec_size is the size of a section, use to offset access
#endif
