#ifndef READ_ELF_H
#define READ_ELF_H

#include "elf.h"
#include "map_file.h"

int read_x86(struct mfile* mf);
int read_ia64(struct mfile* mf);


#endif 
