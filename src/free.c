#include "../include/malloc.h"

/* @brief call free_page(munmap call) on all g_data list */
void free_meta_data()
{
	t_data *ptr = g_data;
	while (g_data) {
		ptr = ptr->next;
		free_page(g_data);
		g_data = ptr;
	}
}

/* @brief munmap call */
void free_page(t_data *data) { munmap(data, data->size); }

/* @brief check for empty page (all block size == 0) and not pre allocated page */
int8_t page_empty(t_data *data)
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

/* @brief free block, don' t call munmap just set block to freed and substrack his size to page */
void free_meta_block(t_block* block, t_data *data)
{
	if (!(data->type & LARGE)) {
		size_t align = get_align_by_type(data->type);
		data->size_free += align + BLOCK_SIZE;
	}
	block->size = 0;
}

/* @brief loop on lst_block to find given ptr and call free_meta_block */
static int check_for_free_page(t_data *prev, t_data *current, t_block *block, void *ptr)
{
	while (block)
	{
		/* if (ptr - BLOCK_SIZE == (void *)block) is the same */
		if (ptr == (void *)block + BLOCK_SIZE)
		{
			if (block->size == 0)
				return (-1);
			free_meta_block(block, current);
			/* if not pre allocate page and page is empty we can munmap */
			if (page_empty(current) == TRUE) {
				prev == NULL ? (g_data = current->next) : (prev->next = current->next);
				free_page(current); /* munmap call */
				// ft_printf_fd(2, "MUNMAP CALLED\n");
			}
			return (0);
		}
		block = block->next;
	}
	return(1);
}

/** @brief try to free given ptr
 * 	@return FALSE if ptr is invalid TRUE for sucess free*/
static int try_free(void *ptr)
{
	int 	ret = 1;
	t_data	*data = g_data;

	if (!data)
		return (FALSE);
	ret = check_for_free_page(NULL, data, data->block, ptr);
	if (ret == 0)
		return (TRUE);
	else if (ret == -1)
		return (FALSE);
	while(data && data->next) {
		ret = check_for_free_page(data, data->next, data->next->block, ptr);
		if (ret == 0)
			return (TRUE);
		else if (ret == -1)
			return (FALSE);
		data = data->next;
	}
	return (FALSE);
}

/* @brief mandatory free implementation need to paste def here */
void free(void *ptr)
{
	// ft_printf_fd(1, "%sFree called %p %s\n", YELLOW, ptr, RESET);
	if (!ptr) {
		ft_printf_fd(1, "%sTry to free NULL%s\n", RED, RESET);
		return ;
	}
	else if (try_free(ptr) == FALSE) {
		ft_printf_fd(1, "%sFree: Invalid pointer %p %s\n", RED, ptr, RESET);
		// ft_printf_fd(2, "Invalid ptr - block_size == %p\n", ptr - BLOCK_SIZE);
		// ft_printf_fd(2, "Invalid ptr == %p\n", ptr);
	}
}