#include "../malloc.h"

void t_free(void* ptr)
{
	t_data *head = g_data;
	t_block *test = (t_block *)(ptr - BLOCK_SIZE);
	if (!ptr)
		return ;
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
	return ;
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
		printf("free large block maybe munmap instant\n");
}

t_block* refill_block(t_block* block, int pos, int size)
{
    int i = 0;
	t_block *tmp = block;
    while(tmp)
    {
        if (i == pos)
		{
            tmp->size = size;
			break ;
		}
        i++;
        tmp = tmp->next;
    }
    return (tmp);
}

int check_for_free_node(t_block* block)
{
    int pos = 0;
    while(block)
    {
        if (block->size == 0)
            return (pos);
		pos++;
        block = block->next;
    }
    return (-1);
}

t_block *add_block(t_data *data, int pos, size_t size, t_block *new, t_block **block)
{
	size_t align = get_align_by_type(data->type);
	if (data->size_free >= BLOCK_SIZE + align)
	{
		pos = check_for_free_node(data->block);
		if (pos != -1)
		{
			new = refill_block(data->block, pos, size);
			return (new);
		}
		pos = get_lst_block_len(data->block);
		new = init_block(new, size, data->type, pos, data);
		block_add_back(block, new);
		data->size_free -= (align + BLOCK_SIZE);
		return (new);
	}
	return (NULL);
}

t_block *try_add_block(e_type type, size_t size)
{
	t_data *head = g_data;
	t_block *new = NULL;
    int pos = 0;
	if (!g_data)
		return (NULL);
	while (g_data)
	{
		if (g_data->type == type)
		{
			new = add_block(g_data, pos, size, new, &g_data->block);
			if (new)
				break ;
		}
		g_data = g_data->next;
	}
	g_data = head;
	return (new);
}

size_t align_mem_block(size_t m_size, size_t size)
{
	size_t mod = m_size % size;
	if (m_size < size)
		m_size = size;
	else if (mod != 0)
		m_size += size - mod;
	return (m_size);
}

t_block	*init_block(t_block *block, size_t size, e_type type, int pos, t_data *data)
{
	size_t skip = 0;
	size_t align = get_align_by_type(data->type);
    skip = align * pos;
	block = (t_block *)((void *)data + DATA_SIZE + skip);
	
	size_t block_size = size;
	if (type & LARGE)
		block_size = align_mem_block(sizeof(t_block) + size, ALIGN_VALUE);
	block->size = block_size;
	block->next = NULL;
	return (block);
}

/* ptr arithmétique
	block = (t_block *)(data) + DATA_SIZE + skip; // bad ptr arithmétique data not casted and add sizeof(x)
	solution :
	void *ptr = (void *)data + DATA_SIZE + skip; 
	block = (t_block *)ptr;
	or:
	block = (t_block *)((void *)data + DATA_SIZE + skip);
*/