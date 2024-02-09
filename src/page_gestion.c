#include "../include/malloc.h"


/** @brief Detect block type to alloc
 * 	@param size_t size: size of desired allocation in bytes
 *	@return e_type enum represent the type of desired block 
*/
e_type detect_type(size_t size)
{
	e_type type;
	if (size <= TINY_SIZE)
		type = TINY;
	else if (size <= SMALL_SIZE)
		type = SMALL;
	else
		type = LARGE;
	return (type);
}


/** @brief Get page size for desired type, or size for LARGE block
 *	@param	e_type enum represent the type of desired block
 *	@param	size_t size: size of desired allocation in bytes
 *	@return size_t page size, default value for TINY and SMALL, aligned memory block for LARGE 
*/
size_t get_page_size(e_type type, size_t size)
{
	size_t m_size = 0;

	if (type & TINY)
		m_size = TINY_PAGE_SIZE;
	else if (type & SMALL)
		m_size = SMALL_PAGE_SIZE;
	else
		m_size = align_mem_block(size + DATA_SIZE + BLOCK_SIZE, PAGE_SIZE);
	return (m_size);
}

/** @brief	Init page with mmmap call 
 *	@param	e_type enum represent the type of desired block
 *	@param	size_t size: size of desired page allocation in bytes
 *	@return pointer on allocated t_data struct, the first address return by mmap 
*/
static t_data *init_data_by_type(e_type type, size_t size)
{
	t_data *data = NULL;
	size_t page_size = 0;

	page_size = get_page_size(type, size);
	data = mmap(0, page_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (data == MAP_FAILED) {
		ft_printf_fd(2, "mmap call failed check erno for details\n");
		return (NULL);
	}
	data->type = type;
	data->size = page_size;
	data->block = init_block(data->block, size, type, 0, data);
	data->size_free = page_size - (data->block->size + DATA_SIZE);
	data->next = NULL;
	return (data);
}

/** @brief	Init data function called in malloc after detect_type
 * 	if block is't LARGE block, try to add him to pre allocated page
 *	else create new page and add it to g_data linked list 
 *	@param	e_type enum represent the type of desired block
 *	@param	size_t size: size of block in bytes
 *	@return pointer on selected block for the user 
*/
t_block *init_data(e_type type, size_t size)
{
	t_data *data;
	if (!(type & LARGE)) {
		t_block *block = try_add_block(type, size);
		if (block)
			return (block);
	}
	data = init_data_by_type(type, size);
	data_add_back(&g_data, data);
	return (data->block);
}


/** Simple data add, similar to lst_addback
 * 
*/
void    data_add_back(t_data **lst, t_data *new)
{
	t_data  *current;

	if (!new)
		return ;
	if (!(*lst)) {
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next != NULL)
			current = current->next;
	current->next = new;
}

/* Map addresses starting near ADDR and extending for LEN bytes.  from
   OFFSET into the file FD describes according to PROT and FLAGS.  If ADDR
   is nonzero, it is the desired mapping address.  If the MAP_FIXED bit is
   set in FLAGS, the mapping will be at ADDR exactly (which must be
   page-aligned); otherwise the system chooses a convenient nearby address.
   The return value is the actual mapping address chosen or MAP_FAILED
   for errors (in which case `errno' is set).  A successful `mmap' call
   deallocates any previous mapping for the affected region.  */
