#include "display_file.h"
#include "elf.h"


int display_file(struct mfile* mf)
{
	(void)mf;
	if (validate_elf(mf) == -1)
	{
		return -1;
	}
	return 0;
}
