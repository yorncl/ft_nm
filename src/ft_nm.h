#ifndef FT_NM_H
#define FT_NM_H


#include <stddef.h>
#include <sys/types.h>


#include <libft.h>
#include "elf.h"

typedef struct sl_entry
{
	u_int64_t value;
	char type;
	char* name;
} sl_entry;

void sl_push_back(t_list** l, u_int64_t value, char type, char* name);
void sl_sort(t_list* sl);
void sl_print(t_list* sl);

char get_sym_type(u_int64_t info, u_int64_t other, u_int64_t st_shndx);

struct mfile
{
	char *data;
	size_t size;
};

t_list* read_x86(struct mfile* mf);
t_list* read_ia64(struct mfile* mf);


int map_file(char *file, struct mfile* mfile);
void unmap_file(struct mfile* mfile);

#endif
