#ifndef NM_H
#define NM_H

#include <stdint.h>

int parse_elf_64(void*);

// little endian read
uint64_t read64_little(void* addr);
uint32_t read32_little(void* addr);
uint16_t read16_little(void* addr);
uint8_t read8_little(void* addr);

// big endian read
uint64_t read64_big(void* addr);
uint32_t read32_big(void* addr);
uint16_t read16_big(void* addr);
uint8_t read8_big(void* addr);


#endif
