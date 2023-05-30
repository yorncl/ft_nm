#ifndef NM_H
#define NM_H

#include <stdint.h>

#include "elf.h"

typedef struct sym64
{
  Elf64_Sym* sym;
  Elf64_Shdr* shdr;
  char type;
} sym64;

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

char compute_type(unsigned char st_info, unsigned char st_other, uint16_t st_shndx, char* sh_name, uint32_t sh_info, uint64_t sh_flags);
#endif
