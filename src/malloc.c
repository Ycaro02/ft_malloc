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

e_bool page_empty(t_data *data)
{
	t_block *block = data->block;
	while (block)
	{
		if (block->size == 0)
			block = block->next;
		else 
			return (FALSE);
	}
	return(TRUE);
}

void free_meta_block(t_block* block, t_data *data)
{
	if (!(data->type & LARGE))
	{
		size_t align = get_align_by_type(data->type);
		data->size_free -= align + BLOCK_SIZE;
	}
	block->size = 0;
}

e_bool try_free(void *ptr)
{
	t_data *data = g_data;

	if (!ptr)
		return (FALSE);
	while(data)
	{
		t_block *tmp = data->block;
		while (tmp)
		{
			if (ptr == (void *)((void *)tmp + BLOCK_SIZE))
			{
				free_meta_block(tmp, data);
				return (TRUE);
			}
			tmp = tmp->next;
		}
		data = data->next;
	}
	return (FALSE);
}

void *malloc(size_t size)
{
	// printf("my malloc called\n");
	e_type type;
	if (size <= 0)
		return (NULL);
	type = detect_type(size);
	t_block *block = init_data(type, size);
	return ((void *)block + BLOCK_SIZE); // CARE arithmétique ptr again
}

void free(void* ptr)
{
	if (ptr && try_free(ptr) == FALSE)
		ft_printf_fd(2, "Invalid free\n");
}



// char *check = (char *)ptr;
// for (size_t i = 0; i< size; i++)
// 	check[i] = 'k';
// check[size] = '\0';

// printf("prev next = %p currnet = %p, save %p, save->next = %p\n", prev->next, current, save, save->next);
