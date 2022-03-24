#include <stdio.h>
#include <stdlib.h>
#include "ft_nm.h"

void sl_push_back(t_list** sl, u_int64_t value, char type, char *name)
{
	sl_entry *ptr = ft_calloc(1, sizeof(sl_entry));
	if (ptr == NULL)
		exit(1); // TODO perror ? 
	ptr->value = value;
	ptr->type = type;
	ptr->name = name;
	t_list* new = ft_lstnew(ptr);
	if (new == NULL)
		exit(1); // TODO perror ? 
	ft_lstadd_back(sl, new);
}

void sl_sort(t_list* sl)
{
	(void) sl;
	return;
}

void sl_print(t_list* sl)
{
	// TODO printing with my own functions
	sl_entry* data;
	while (sl)
	{
		data = (sl_entry*)sl->content;
		printf("%lX %c %s\n", data->value, data->type, data->name);
		sl = sl->next;
	}
}
