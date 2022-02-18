#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

struct mfile
{
	char *data;
	size_t size;
};

int map_file(char *file, struct mfile* mfile);
void unmap_file(struct mfile* mfile);
#endif
