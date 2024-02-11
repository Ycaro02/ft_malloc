#include "../include/malloc.h"

/**
 * Declare globale t_page pointer, the head of pages linked-list
*/
t_page *g_data = NULL;

/**	@brief Check for pre allocated page for TINY and SMALL block
 *  @param e_type type: type of searched page
 *	@return TRUE if any pre allocate page of this type exist otherwise FALSE
*/
static int8_t check_preallocate_page(e_type type)
{
	t_page *data = g_data;
	while (data)
	{
		if (data->type & type && data->type & PRE_ALLOCATE)
			return (TRUE);
		data = data->next;
	}
	return (FALSE);
}

/** @brief Init pre allocate space for TINY or SMALL block
 *  @param e_type type: type of searched page
 * 	@return FALSE for mmap error otherwise TRUE
*/
static int8_t first_page_allocation(e_type type)
{
	t_page *page;

	if (type == TINY && check_preallocate_page(type) == FALSE) {
		page = alloc_first_page(TINY, TINY_SIZE, TINY_PAGE_SIZE);
		if (!page) {
			return (FALSE);
		}
		// ft_printf_fd(1, "\n%sINIT First Tiny page%s\n", RED, RESET);
		page_add_back(&g_data, page);
	} else if (type == SMALL && check_preallocate_page(type) == FALSE) {
		page = alloc_first_page(SMALL, SMALL_SIZE, SMALL_PAGE_SIZE);
		if (!page) {
			return (FALSE);
		}		
		// ft_printf_fd(1, "\n%sINIT First Small page%s\n", RED, RESET);
		page_add_back(&g_data, page);
	}
	return (TRUE);
}

/** @brief Malloc main function first call init pages for TINY and SMALL blocks
 * 	@param size_t size: size of desired allocation in bytes
 *	@return Pointer to allocate block, NULL for invalid size ( <= 0) 
*/
void *malloc(size_t size)
{
	// ft_printf_fd(1, "my malloc called\n");
	e_type type;
	t_block *block;

	if (size <= 0) {  /* maybe to change malloc 1 for 0 input ? */
		return (NULL);
	}
	type = detect_type(size);
	if (first_page_allocation(type) == FALSE)
		return (NULL);
	block = init_data(type, size);
	return (((void *) block) + BLOCK_SIZE);
}