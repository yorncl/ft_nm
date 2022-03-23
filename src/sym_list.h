#ifndef SYM_LIST_H
#define SYM_LIST_H

#include <libft.h>
#include <stddef.h>
#include <sys/types.h>

typedef struct sl_entry
{
	u_int64_t value;
	char type;
	char* name;
} sl_entry;

void sl_push_back(t_list** l, u_int64_t value, char type, char* name);
void sl_sort(t_list* sl);
void sl_print(t_list* sl);

#endif
