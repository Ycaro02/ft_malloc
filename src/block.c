#include "../malloc.h"

void free_meta_block(t_block* block, t_data *data)
{
    size_t align = get_align_by_type(data->type);
    if (!(data->type & LARGE)) 
	{
		// type & TINY ? (align = TINY_SIZE) : (align = SMALL_SIZE);
        data->size_free -= align + BLOCK_SIZE;
        block->size = -1;
    }
}

t_block* refill_block(t_block* block, int pos, int size)
{
    int i = 0;
    while(block)
    {
        if (i == pos)
            block->size = size;
        i++;
        block = block->next;
    }
    return (block);
}

int check_for_free_node(t_block* block)
{
    int pos = 0;
    while(block)
    {
        if (block->size == -1)
            return (pos);
        block = block->next;
    }
    return (-1);
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
			if (g_data->size_free >= BLOCK_SIZE + size)
			{
                pos = check_for_free_node(g_data->block);
				if (pos != -1)
                {
                    new = refill_block(g_data->block, pos, size);
                    break ;
                }
                pos = get_lst_block_len(g_data->block);
				new = init_block(new, size, type, pos, g_data);
				block_add_back(&g_data->block, new);
                size_t align = get_align_by_type(g_data->type);
				g_data->size_free -= (align + BLOCK_SIZE);
				break;
			}
		g_data = g_data->next;
	}
	if (!g_data)
	{
		g_data = head;
		return (NULL);
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
	block = (t_block *)(data) + DATA_SIZE + skip;
	size_t block_size = size;
	if (type & LARGE)
		block_size = align_mem_block(sizeof(t_block) + size, ALIGN_VALUE);
	
	block->size = block_size;
	block->next = NULL;
	return (block);
}