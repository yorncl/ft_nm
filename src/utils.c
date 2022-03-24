#include "ft_nm.h"


char get_sym_type(u_int64_t info, u_int64_t other, u_int64_t st_shndx)
{
	char c = '?';
	u_int64_t type = info & 0xf;
	u_int64_t bind = info >> 4;
	u_int64_t vis = other & 0x3;
	(void) type;
	(void) bind;
	(void) vis;

	if (st_shndx)
		c = 'A';

	return c;
}
