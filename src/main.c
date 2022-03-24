#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include "ft_nm.h"

static int display_file(struct mfile* mf)
{
	if (mf->size < sizeof(Elf32_Ehdr))
		return -1;
	Elf32_Ehdr* tmp = (Elf32_Ehdr*)mf->data;

	// Check MAGIC NUMBER
	uint8_t magic[4] = { 0x7f, 'E', 'L', 'F' };
	if (ft_memcmp(tmp, magic, 4))
	{
		printf("This is not an elf file !!! \n");
		return -1;
	}

	unsigned int class = tmp->e_ident[4];
	int r;
	t_list* symlist = NULL;
	switch (class)
	{
		case 1:
			//symlist = read_x86(mf);
			break;
		case 2:
			symlist = read_ia64(mf);
			break;
		default:
			printf("Class not supported\n");
			r = -1;

	}
	sl_print(symlist);
	return r;
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
