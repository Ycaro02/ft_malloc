#include "../malloc.h"

void print_bloc(t_data *data, e_type type)
{

	while (data && !(data->type & type))
		data = data->next;
	if (data)
	{
		if (type & TINY)
			printf("TINY");
		else if (type & SMALL)
			printf("SMALL");
		else
			printf("LARGE");
		printf(" : %p\n", data);
		t_block *block = data->block;
		while (block)
		{
			printf("%p - %p", (block + BLOCK_SIZE), (block + BLOCK_SIZE) + block->size);
			printf(": %ld bytes\n", (block + BLOCK_SIZE) + block->size - (block + BLOCK_SIZE));
			block = block->next;
		}
	}
}

void show_alloc_mem()
{
	print_bloc(g_data, TINY);
	print_bloc(g_data, SMALL);
	print_bloc(g_data, LARGE);
}
