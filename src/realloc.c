#include "../include/malloc.h"

void *exec_realloc(t_block *block, size_t size)
{
	size_t block_size = block->size;
	char *new_ptr = malloc(block->size + size);
	if (!new_ptr)
		return (NULL);
	size_t i = 0;
	char *content = (char *)((void *)block + BLOCK_SIZE);
	ft_printf_fd(1, "block sizze = %U\n", block->size);
	// ft_printf_fd(1, "block data = %s\n", (char *)block + BLOCK_SIZE);
	while (i < block_size)
	{
		new_ptr[i] = content[i];
		i++;
	}
	return ((void *)new_ptr);
}

e_bool need_realloc(t_data *data, t_block *block, size_t size)
{
	size_t align = size;
	size_t new_size = block->size + size;
	if (!(data->type & LARGE))
	{
		align = get_align_by_type(data->type);
		if (new_size <= align)
			return (FALSE);
	}
	return (TRUE);
}

// ft_printf_fd(1, "MATCH for size = %U block = %p\n", size, block);

void *check_for_realloc_block(t_data *prev, t_data *current, t_block *block, void *ptr, size_t size)
{
	while (block)
	{
		if (ptr == (void *)block + BLOCK_SIZE)
		{
			if (need_realloc(current, block, size) == TRUE)
			{
				ptr = exec_realloc(block, size);
				free_meta_block(block, current);
				if (page_empty(current)== TRUE)
				{
					prev == NULL ? (g_data = current->next) : (prev->next = current->next);
					free_page(current);
				}
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

// If ptr is NULL, then the call is equivalent to malloc(size)
// If size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)
void *realloc(void *ptr, size_t size)
{
	// ft_printf_fd(1, "my realloc called\n");
	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	void *new_ptr = get_block_addr(ptr, size);
	if (!new_ptr)
		ft_printf_fd(2, "get block addr return NULL\n");
	return (new_ptr);
}

// if (prev)
// 	printf("all info prev: addr: %p, size: %zu, next: %p\n", prev, prev->block->size, prev->next);
// printf("all info curr: addr: %p, size: %zu, next: %p\n", current, current->block->size, current->next);
// printf("all info save: addr: %p, size: %zu, next: %p\n", save, save->block->size, save->next);
// if (prev)
// 	printf("AFT info prev: addr: %p, size: %zu, next: %p\n", prev, prev->size, prev->next);