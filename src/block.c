#include "../include/malloc.h"

/** @brief check for find empty block in lst_block
 * 	@param t_block *lst_block, pointer on blocks lst
 * 	@return int position of empty block, -1 for nothing found
*/
static t_block *mark_empty_block(t_block* lst_block, size_t size)
{
    int pos = 0;
    while(lst_block) {
        if (lst_block->size == 0) {
			lst_block->size = size;
            return (lst_block);
		}
		pos++;
        lst_block = lst_block->next;
    }
    return (NULL);
}

/** @brief add_block, try to add block to existent lst_block in data page, first check for space in data page,
 * 	second check for freed block otherwise add a new block to list
 * 	@param t_page *data:
 * 	@param int pos, position of target block in lst
 *  @param size_t size, size of block in bytes
 *  @param t_block *new: pointer on new block to give adress and return
 *  @param t_block **lst_block: head of lst_block in page
 * 	@return pointer on new find block, NULL if not enough space in page
*/
static t_block *add_block(t_page *data, int pos, size_t size, t_block *new, t_block **lst_block)
{
	size_t align = get_align_by_type(data->type); 	/* get aligned size for allocation */
	/* if space free in page >= block size metadata + aligned_size || if this size <= space free in page */
	if (data->size_free >= (align + BLOCK_SIZE)) {
		new = mark_empty_block(data->block, size);
		if (new) {
			data->size_free -= (align + BLOCK_SIZE);
			return (new);
		}
		/* else if no freed block found */
		pos = get_lst_block_len(data->block);
		new = init_block(new, size, pos, data); /* need to create new block,  compute his address */
		block_add_back(lst_block, new);							/* add him to block lst */
		data->size_free -= (align + BLOCK_SIZE);
		return (new);
	}
	return (NULL);
}

/** @brief loop on g_data page to find existent page of given type
 *  @param size_t size, size of block in bytes
 *  @param e_type type: type of searched page
 * 	@return pointer on new find block, NULL if g_data not set or can't add block
*/
t_block *try_add_block(char type, size_t size)
{
	t_page	*head = g_data;
	t_block *block = NULL;
    int 	pos = 0;
	
	if (!g_data)
		return (NULL);
	while (g_data) {
		if (g_data->type & type) {
			block = add_block(g_data, pos, size, block, &g_data->block);
			if (block)
				break ;
		}
		g_data = g_data->next;
	}
	g_data = head;
	return (block);
}

/** @brief get aligned size for LARGE block */
inline size_t align_mem_block(size_t m_size, size_t size)
{
	if (m_size < size)
		m_size = size;
	size_t mod = m_size % size;
	if (mod != 0)
		m_size += size - mod;
	return (m_size);
}
/* @brief init block structure, compute his addr and set size and next value */
t_block	*init_block(t_block *block, size_t size, int pos, t_page *data)
{
	size_t skip = 0;
	size_t align = get_align_by_type(data->type);
    
	skip = align * pos; /* (block size + size of block struct) * block position ) */
	/* compute block address, data: adress return by mmap for the page, + size of data struct + skip*/
	block = (t_block *)(((void *) data) + DATA_SIZE + skip);

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

	old compute block addr for large 
	// if (type & LARGE) LARGE gestion in page
	// block_size = data->size - DATA_SIZE - BLOCK_SIZE;
	// block_size = align_mem_block(sizeof(t_block) + size, ALIGN_VALUE);
*/
