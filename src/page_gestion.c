#include "../include/malloc.h"

/** @brief Detect block type to alloc
 * 	@param size_t size: size of desired allocation in bytes
 *	@return e_type enum represent the type of desired block 
*/
e_type detect_type(size_t size)
{
	/* (size > SMALL_SIZE) * 2) 
		Large bool need no be count * 2 idx = 2
		1 for small perfect for bool otherwise tiny block*/
	e_type const type_array[3] = {TINY, SMALL, LARGE};

	return ((type_array[(size > TINY_SIZE && size <= SMALL_SIZE) + ((size > SMALL_SIZE) << 1)]));
}

/** @brief Get page size for desired type, or size for LARGE block
 *	@param	e_type enum represent the type of desired block
 *	@param	size_t size: size of desired allocation in bytes
 *	@return size_t page size, default value for TINY and SMALL, aligned memory block for LARGE 
*/
inline size_t get_page_size(e_type type, size_t size)
{
	/* (TINY >> 1)=0, (SMALL >> 1)=1, (LARGE >> 1)=2 */
	size_t const page_size[3] = {TINY_PAGE_SIZE, SMALL_PAGE_SIZE, align_mem_block(size + DATA_SIZE + BLOCK_SIZE, PAGE_SIZE)};

	return (page_size[type >> 1]);
}

/*	
	* Don't worry about 'type >> 1', pre allocate/debug value in type, is given in different args to prevent this 
	* in init_first page when we call init_page function 
*/

/** @brief	Init page with mmmap call 
 *	@param	e_type enum represent the type of desired block
 *	@param	size_t size: size of desired block allocation in bytes
 *	@param	e_type pre_aloc: PRE_ACLLOCATE flag for pre allocated page else 0
 *	@return pointer on allocated t_page struct, the first address return by mmap 
*/
t_page *init_page(e_type type, size_t size, e_type pre_aloc)
{
	t_page *data;
	size_t page_size;

	page_size = get_page_size(type, size);

	data = mmap(0, page_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (data == MAP_FAILED) {
		ft_printf_fd(2, "mmap call failed check erno for details\n");
		return (NULL);
	}
	data->type = (type | pre_aloc);
	data->size = page_size;
	data->block = init_block(data->block, size, 0, data);
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
	t_page	*data;
	t_block *block;

	if (!(type & LARGE)) {
		block = try_add_block(type, size);
		if (block)
			return (block);
	}
	data = init_page(type, size, 0);
	page_add_back(&g_data, data);
	if (check_debug_flag(ALLOCATION_TRACE)) {
		ft_printf_fd(get_debug_fd(), GREEN"Create new page (mmap called) %p first block: %p\n"RESET, data, data->block); /* Only for call history */
	}
	return (data->block);
}


/** Simple data add, similar to lst_addback*/
void    page_add_back(t_page **lst, t_page *data)
{
	t_page  *current;

	if (!data)
		return ;
	if (!(*lst)) {
		*lst = data;
		return ;
	}
	current = *lst;
	while (current->next != NULL)
			current = current->next;
	current->next = data;
}

/* Mmap: Map addresses starting near ADDR and extending for LEN bytes.  from
   OFFSET into the file FD describes according to PROT and FLAGS.  If ADDR
   is nonzero, it is the desired mapping address.  If the MAP_FIXED bit is
   set in FLAGS, the mapping will be at ADDR exactly (which must be
   page-aligned); otherwise the system chooses a convenient nearby address.
   The return value is the actual mapping address chosen or MAP_FAILED
   for errors (in which case `errno' is set).  A successful `mmap' call
   deallocates any previous mapping for the affected region.  */