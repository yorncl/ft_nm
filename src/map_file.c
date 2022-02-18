#include "map_file.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/mman.h>

int map_file(char* file, struct mfile* mf)
{
	int fd;
	if ((fd = open(file, O_RDONLY)) == -1)
		return -1;
	struct stat statbuf;
	if (fstat(fd, &statbuf) == -1)
	{
		close(fd);
		return -1;
	}
	mf->size = statbuf.st_size;
	mf->data = mmap(NULL, mf->size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (mf->data == MAP_FAILED)
		return -1;
	return 0;
}

void unmap_file(struct mfile* mf)
{
	munmap(mf->data, mf->size);
}
