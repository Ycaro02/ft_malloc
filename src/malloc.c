#include "../include/malloc.h"

t_data *g_data = NULL;

void t_free_page(t_data *data) { munmap(data, data->size); }

void t_free_meta_data()
{
	t_data *ptr = g_data;
	while (g_data)
	{
		ptr = ptr->next;
		t_free_page(g_data);
		g_data = ptr;
	}
}

void t_free_meta_block(t_block* block, t_data *data)
{
    size_t align = get_align_by_type(data->type);
	data->size_t_free -= align + BLOCK_SIZE;
	block->size = 0;
}

e_bool find_valid_ptr(void *ptr, e_event event)
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
				if (event == FREE)
					t_free_meta_block(tmp, data);
				return (TRUE);
			}
			tmp = tmp->next;
		}
		data = data->next;
	}
	return (FALSE);
}

void *t_malloc(size_t size)
{
	// printf("my t_malloc called\n");
	e_type type;
	if (size <= 0)
		return (NULL);
	type = detect_type(size);
	t_block *block = init_data(type, size);
	return ((void *)block + BLOCK_SIZE); // CARE arithmétique ptr again
}

void t_free(void* ptr)
{
	if (ptr && find_valid_ptr(ptr, FREE) == FALSE)
		ft_printf_fd(2, "Invalid t_free\n");
}



// char *check = (char *)ptr;
// for (size_t i = 0; i< size; i++)
// 	check[i] = 'k';
// check[size] = '\0';

// printf("prev next = %p currnet = %p, save %p, save->next = %p\n", prev->next, current, save, save->next);
