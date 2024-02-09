#include "../include/malloc.h"

/** @brief refill block with the accordate size
 * 	@param t_block *lst, pointer on blocks lst
 * 	@param int pos, position of target block in lst
 *  @param size_t size, size of block in bytes
 * 	@return pointer on refilled blocks
*/
static t_block* refill_block(t_block* lst, int pos, size_t size)
{
    int i = 0;
	t_block *tmp = lst;

    while(tmp) {
        if (i == pos) {
            tmp->size = size;
			break ;
		}
        i++;
        tmp = tmp->next;
    }
    return (tmp);
}

static int check_for_free_node(t_block* block)
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

static t_block *add_block(t_data *data, int pos, size_t size, t_block *new, t_block **block)
{
	size_t align = get_align_by_type(data->type);
	if (data->size_free >= BLOCK_SIZE + align)
	{
		pos = check_for_free_node(data->block);
		if (pos != -1)
		{
			new = refill_block(data->block, pos, size);
			data->size_free -= (align + BLOCK_SIZE);
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

t_block *try_add_block(char type, size_t size)
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
	if (m_size < size)
		m_size = size;
	size_t mod = m_size % size;
	if (mod != 0)
		m_size += size - mod;
	return (m_size);
}

t_block	*init_block(t_block *block, size_t size, e_type type, int pos, t_data *data)
{
	size_t skip = 0;
	size_t align = get_align_by_type(data->type);
    
	skip = align * pos;
	block = (t_block *)(((void *) data) + DATA_SIZE + skip);
	
	// size_t block_size = size;
	(void)type;
	// if (type & LARGE)
		// block_size = data->size - DATA_SIZE - BLOCK_SIZE;
		// block_size = align_mem_block(sizeof(t_block) + size, ALIGN_VALUE);
	block->size = size;
	block->next = NULL;
	return (block);
}

/* Basic function block lst add back see page add back */
void    block_add_back(t_block **lst, t_block *new)
{
	t_block  *current;

	if (new == NULL)
			return ;
	if (*lst == NULL) {
			*lst = new;
			return ;
	}
	current = *lst;
	while (current->next != NULL)
			current = current->next;
	current->next = new;
}

/* ptr arithmétique
	block = (t_block *)(data) + DATA_SIZE + skip; // bad ptr arithmétique data not casted and add sizeof(x)
	solution :
	void *ptr = (void *)data + DATA_SIZE + skip; 
	block = (t_block *)ptr;
	or:
	block = (t_block *)((void *)data + DATA_SIZE + skip);
*/
