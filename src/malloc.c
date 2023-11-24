#include "../include/malloc.h"

t_data *g_data = NULL;

void *malloc(size_t size)
{
	// ft_printf_fd(1, "my malloc called\n");
	e_type type;
	if (size <= 0)
		return (NULL);
	type = detect_type(size);
	t_block *block = init_data(type, size);
	return ((void *)block + BLOCK_SIZE); // CARE arithmétique ptr
}