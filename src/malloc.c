#include "../include/malloc.h"

t_data *g_data = NULL;

void free_page(t_data *data) { munmap(data, data->size); }

void free_meta_data()
{
	t_data *ptr = g_data;
	while (g_data)
	{
		ptr = ptr->next;
		free_page(g_data);
		g_data = ptr;
	}
}

void free_meta_block(t_block* block, t_data *data)
{
    size_t align = get_align_by_type(data->type);
    if (!(data->type & LARGE)) 
	{
        data->size_free -= align + BLOCK_SIZE;
        block->size = 0;
    }
	else
		free_page(data);
}

e_bool find_valid_ptr(void *ptr, e_event event)
{
	t_data *head = g_data;
	t_block *test = NULL; 

	if (!ptr)
		return (FALSE);
	test = (t_block *)(ptr - BLOCK_SIZE);
	while(g_data)
	{
		t_block *head_block = g_data->block;
		while (g_data->block)
		{
			if (g_data->block == (t_block *)test)
			{
				if (event == FREE)
					free_meta_block(g_data->block, g_data);
				g_data->block = head_block;
				g_data = head;
				return (TRUE);
			}
			g_data->block = g_data->block->next;
		}
		g_data->block = head_block;
		g_data = g_data->next;
	}
	g_data = head;
	return (FALSE);
}

void *malloc(size_t size)
{
	e_type type;
	if (size <= 0)
		return (NULL);
	type = detect_type(size);
	t_block *block = init_data(type, size);
	return ((void *)block + BLOCK_SIZE); // CARE arithmétique ptr again
}

void free(void* ptr)
{
	if (ptr && find_valid_ptr(ptr, FREE) == FALSE)
		ft_printf_fd(2, "Invalid free\n");
}

void *exec_realloc(t_block *block, size_t size)
{
	unsigned char *new_ptr = malloc(block->size + size);
	
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

e_bool check_reallocs(t_data *data, t_block *block, size_t size)
{
	size_t align = size;
	size_t new_size = block->size + size;
	if (!(data->type & LARGE))
	{
		align = get_align_by_type(data->type);
		if (new_size < align)
		{
			block->size += size;
			return (TRUE);
		}
	}
	return (FALSE);
}

void *get_block_addr(void *ptr, size_t size)
{
	t_data *head = g_data;
	t_block *test = NULL; 

	if (!ptr)
		return (NULL);
	test = (t_block *)(ptr - BLOCK_SIZE);
	while(g_data)
	{
		t_block *head_block = g_data->block;
		while (g_data->block)
		{
			if (g_data->block == (t_block *)test)
			{
				if (check_reallocs(g_data, g_data->block, size) == FALSE)
					ptr = exec_realloc(g_data->block, size);
				g_data->block = head_block;
				g_data = head;
				return (ptr);
			}
			g_data->block = g_data->block->next;
		}
		g_data->block = head_block;
		g_data = g_data->next;
	}
	g_data = head;
	return (NULL);
}
// If ptr is NULL, then the call is equivalent to malloc(size)
// If size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)
void *realloc(void *ptr, size_t size)
{
	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	return (get_block_addr(ptr, size));
	// return (NULL);
}

