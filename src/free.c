#include "../include/malloc.h"

/* @brief call free_page(munmap call) on all g_data list, if
	you are clever you need to call it at the end of all your allocation
	program speciali if you are using DEBUG flag */
void free_meta_data()
{
	t_page *ptr = g_data;
	int save_fd = -1;


	if (!g_data)
		return ;
	ft_printf_fd(1, GREEN"Free meta data called\n"RESET);
	pthread_mutex_lock(&g_malloc_mutex);
	if (check_debug_flag(ALLOCATION_TRACE)) {
		save_fd = get_debug_fd();
	}
	while (g_data) {
		ptr = ptr->next;
		free_page(g_data);
		g_data = ptr;
	}
	if (save_fd != -1) {
		close(save_fd);
	}
	pthread_mutex_unlock(&g_malloc_mutex);
	/* after investigate we don't need to destroy mutex, structure don't do some allocation
		, destroy function just set invalid value to prevent bad call*/
}

/* @brief munmap call */
void free_page(t_page *data) { munmap(data, data->size); }

/* @brief check for empty page (all block size == 0) and not pre allocated page */
int16_t page_empty(t_page *data)
{
	t_block *block;
	if (data->type & PRE_ALLOCATE)
		return (FALSE);

	block = data->block;
	while (block) {
		if (block->size != 0)
			return (FALSE);
		block = block->next;
	}
	return(TRUE);
}

/* @brief free block, don't call munmap just set block to freed and substract his size to page */
void free_meta_block(t_block* block, t_page *page)
{
	if (!(page->type & LARGE)) {
		size_t align = get_align_by_type(page->type);
		page->size_free += align + BLOCK_SIZE;
	}
	block->size = 0;
}

/**	@brief loop on lst_block to find given ptr and call free_meta_block 
 *	@param t_page *prev: pointer on previous page	
 *	@param t_page *current: pointer on current page	
 *	@param t_block *block: list of block in current page
 *	@param void *ptr: pointer given by user, wanted to free him
 *	@return: -1 for already freed block, 0 for OK block free, 1 for block not found
*/
static int try_free(t_page *prev, t_page *current, t_block *block, void *ptr)
{
	while (block)
	{
		/* if (ptr - BLOCK_SIZE == (void *)block) is the same, but prefer don't do operation on user input  */
		if (ptr == (void *)block + BLOCK_SIZE)
		{
			if (block->size == 0)
				return (BLOCK_ALREADY_FREE);
			if (check_debug_flag(ALLOCATION_TRACE))
				write_block_info(block, block->size, FREE_CALL, get_debug_fd()); /* Only for call history */
			free_meta_block(block, current);
			/* if not pre allocate page and page is empty we can munmap */
			if (page_empty(current) == TRUE) {
				prev == NULL ? (g_data = current->next) : (prev->next = current->next);
				if (check_debug_flag(ALLOCATION_TRACE))
					ft_printf_fd(get_debug_fd(), RED"Free empty page (munmap called) %p last block: %p data %p\n"RESET, current, block, ptr); /* Only for call history */
				free_page(current); /* munmap call */
				// ft_printf_fd(2, "MUNMAP CALLED\n");
			}
			return (BLOCK_FREE_SUCCESS);
		}
		block = block->next;
	}
	return(BLOCK_NOT_FOUND);
}

/** @brief Try to free given ptr, call try free on all page
 * 	@return FALSE if ptr is invalid TRUE for sucess free*/
static int call_free(void *ptr)
{
	int 	ret = 1;
	t_page	*data = g_data;

	if (!data)
		return (FALSE);
	ret = try_free(NULL, data, data->block, ptr); 
	if (ret == BLOCK_FREE_SUCCESS)
		return (TRUE);
	else if (ret == BLOCK_ALREADY_FREE)
		return (FALSE);
	while(data && data->next) {
		ret = try_free(data, data->next, data->next->block, ptr);
		if (ret == BLOCK_FREE_SUCCESS)
			return (TRUE);
		else if (ret == BLOCK_ALREADY_FREE)
			return (FALSE);
		data = data->next;
	}
	return (FALSE);
}

/* @brief The free() function deallocates the memory allocation pointed to by “ptr”. If “ptr”is
 *	a NULL pointer, no operation is performed.
 */
void free(void *ptr)
{
	// ft_printf_fd(1, "%sFree called %p %s\n", YELLOW, ptr, RESET);
	pthread_mutex_lock(&g_malloc_mutex);
	if (check_debug_flag(ALLOCATION_TRACE))
		write_function_name(FREE_CALL, get_debug_fd()); /* Only for call history */
	if (!ptr) {
		ft_printf_fd(get_debug_fd(), "%sCall free NULL need to disable this%s\n", RED, RESET);
		pthread_mutex_unlock(&g_malloc_mutex);
		return ;
	}
	else if (call_free(ptr) == FALSE) {
		ft_printf_fd(2, RED"Free: Invalid pointer %p \n"RESET, ptr);
	}
	pthread_mutex_unlock(&g_malloc_mutex);
		// ft_printf_fd(2, "Invalid ptr - block_size == %p\n", ptr - BLOCK_SIZE);
		// ft_printf_fd(2, "Invalid ptr == %p\n", ptr);
}