#include "../include/malloc.h"

void *exec_t_realloc(t_block *block, size_t size)
{
	unsigned char *new_ptr = t_malloc(block->size + size);
	if (!new_ptr)
		return (NULL);
	size_t i = 0;
	unsigned char *content = (unsigned char *)(void *)block + BLOCK_SIZE;
	while (i < block->size)
	{
		new_ptr[i] = content[i];
		i++;
	}
	return ((void *)new_ptr);
}

e_bool check_t_reallocs(t_data *data, t_block *block, size_t size)
{
	size_t align = size;
	size_t new_size = block->size + size;
	if (!(data->type & LARGE))
	{
		align = get_align_by_type(data->type);
		if (new_size <= align)
			return (TRUE);
	}
	return (FALSE);
}

void *check_for_realloc_block(t_data *prev, t_data *current, t_block *block, void *ptr, size_t size)
{
	while (block)
	{
		if (block && ptr == (void *)block + BLOCK_SIZE)
		{
			if (check_t_reallocs(current, block, size) == FALSE)
			{
				ptr = exec_t_realloc(block, size);
				t_free_meta_block(block, current);
				prev == NULL ? (g_data = current->next) : (prev->next = current->next);
				t_free_page(current);
			}
			else
				block->size += size;
			break;
		}
		block = block->next;
	}
	return (ptr);
}

void *get_block_addr(void *ptr, size_t size)
{
	t_data *data = g_data;

	if (!ptr)
		return (NULL);
	t_data *test = check_for_realloc_block(NULL, data, data->block, ptr, size);
	if (test != ptr)
		return (test);
	while(data && data->next)
	{
		test = check_for_realloc_block(data, data->next, data->next->block, ptr, size);
		if (test != ptr)
			return (test);
		data = data->next;
	}
	return (ptr);
}

// If ptr is NULL, then the call is equivalent to t_malloc(size)
// If size is equal to zero, and ptr is not NULL, then the call is equivalent to t_free(ptr)
void *t_realloc(void *ptr, size_t size)
{
	// printf("my t_realloc called\n");
	if (!ptr)
		return (t_malloc(size));
	if (size == 0)
	{
		t_free(ptr);
		return (NULL);
	}
	void *new_ptr = get_block_addr(ptr, size);
	if (!new_ptr)
		printf("get block addr return NULL\n");
	return (new_ptr);
}

// if (prev)
// 	printf("all info prev: addr: %p, size: %zu, next: %p\n", prev, prev->block->size, prev->next);
// printf("all info curr: addr: %p, size: %zu, next: %p\n", current, current->block->size, current->next);
// printf("all info save: addr: %p, size: %zu, next: %p\n", save, save->block->size, save->next);
// if (prev)
// 	printf("AFT info prev: addr: %p, size: %zu, next: %p\n", prev, prev->size, prev->next);