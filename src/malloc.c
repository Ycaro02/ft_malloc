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
		data->size_free += align + BLOCK_SIZE;
	}
	block->size = 0;
}

static int check_for_free_page(t_data *prev, t_data *current, t_block *block, void *ptr)
{
	while (block)
	{
		if (ptr == (void *)block + BLOCK_SIZE)
		{
			if (block->size == 0)
				return (-1);
			free_meta_block(block, current);
			(void)prev;
			if (page_empty(current)== TRUE && get_lst_block_len(block) != 1)
			{
				prev == NULL ? (g_data = current->next) : (prev->next = current->next);
				free_page(current);
			}
			return (0);
		}
		block = block->next;
	}
	return(1);
}

static e_bool try_free(void *ptr)
{
	t_data *data = g_data;
	int ret = 1;

	ret = check_for_free_page(NULL, data, data->block, ptr);
	if (ret == 0)
		return (TRUE);
	else if (ret == -1)
		return (FALSE);
	while(data && data->next)
	{
		ret = check_for_free_page(NULL, data, data->block, ptr);
		if (ret == 0)
			return (TRUE);
		else if (ret == -1)
			return (FALSE);
		data = data->next;
	}
	return (FALSE);
}

void free(void* ptr)
{
	// ft_printf_fd(1, "my free called\n");
	if (!ptr)
		return ;
	if (try_free(ptr) == FALSE)
		ft_printf_fd(2, "Invalid free\n");
}

void *malloc(size_t size)
{
	// ft_printf_fd(1, "my malloc called\n");
	e_type type;
	if (size <= 0)
		return (NULL);
	type = detect_type(size);
	t_block *block = init_data(type, size);
	return ((void *)block + BLOCK_SIZE); // CARE arithmétique ptr again
}