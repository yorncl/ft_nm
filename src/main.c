#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include "map_file.h"

static int display_file(struct mfile* mf)
{
	(void) mf;	
	return 0;
}

int main(int ac, char** av)
{

	char *filename = "a.out";
	if (ac > 1) // TODO check for number of arguments by not counting options
		filename = av[1];
	struct mfile mf;
	int r = map_file(filename, &mf);
	if (r == -1)
	{
		printf("MAP ERROR\n"); //TODO make a ft_perror function
		return 1;
	}
	r = display_file(&mf);
	if (r == -1)
		r = 1;
	unmap_file(&mf);
	return r;
}
