#ifndef NM_H
#define NM_H

#include <stdint.h>


int parse_elf_64(void*);

// little endian read
uint8_t read8_little(void* addr);
uint16_t read16_little(void* addr);
uint32_t read32_little(void* addr);
uint64_t read64_little(void* addr);

// big endian read
uint8_t read8_big(void* addr);
uint16_t read16_big(void* addr);
uint32_t read32_big(void* addr);
uint64_t read64_big(void* addr);

// vars that point to the correct read functions
// first defined in utils.c
// then set in main.c as of now
extern uint8_t (*read8)(void* file);
extern uint16_t (*read16)(void* file);
extern uint32_t (*read32)(void* file);
extern uint64_t (*read64)(void* file);
#endif
