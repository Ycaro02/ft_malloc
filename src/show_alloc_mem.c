#include "../malloc.h"

size_t print_bloc(t_data *data)
{
	int total = 0;
	if (data->type & TINY)
		printf("TINY");
	else if (data->type & SMALL)
		printf("SMALL");
	else
		printf("LARGE");
	printf(" : %p\n", data);
	t_block *block = data->block;
	while (block)
	{
		void *ptr = (void *)block + BLOCK_SIZE;
		printf("%p - %p", ptr, ptr + block->size);
		printf(": %ld bytes\n", ptr + block->size - ptr);
		total += ptr + block->size - ptr;
		block = block->next;
	}
	return (total);
}

void show_alloc_mem()
{
	t_data *tmp = g_data;
	size_t total = 0;
	printf("--------------------------------------------------\n");
	while (tmp)
	{
		total += print_bloc(tmp);
		tmp = tmp->next;
	}
	printf("Total: %zu bytes\n", total);
	printf("--------------------------------------------------\n");
}
