#include "../include/malloc.h"

t_data *g_data = NULL;

void *malloc(size_t size)
{
	e_type type;
	if (size <= 0)
		return (NULL);
	type = detect_type(size);
	t_block *block = init_data(type, size);
	return ((void *)block + BLOCK_SIZE); // CARE arithmétique ptr again
}


void free_page(t_data *data)
{
	munmap(data, data->size);
}

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

void free(void* ptr)
{
	t_data *head = g_data;
	t_block *test = NULL; 
	
	if (!ptr)
		return ;
	test = (t_block *)(ptr - BLOCK_SIZE);
	while(g_data)
	{
		t_block *head_block = g_data->block;
		while (g_data->block)
		{
			if (g_data->block == (t_block *)test)
			{
				free_meta_block(g_data->block, g_data);
				g_data->block = head_block;
				g_data = head;
				return ;
			}
			g_data->block = g_data->block->next;
		}
		g_data->block = head_block;
		g_data = g_data->next;
	}
	g_data = head;
}
