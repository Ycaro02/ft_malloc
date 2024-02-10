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

/* @brief check for empty page (all block size == 0) */
int8_t page_empty(t_data *data)
{
	t_block *block = data->block;
	while (block) {
		if (block->size != 0)
			return (FALSE);
		block = block->next;
		// if (block->size == 0)
		// 	block = block->next;
		// else 
		// 	return (FALSE);
	}
	return(TRUE);
}

void free_meta_block(t_block* block, t_data *data)
{
	if (!(data->type & LARGE)) {
		size_t align = get_align_by_type(data->type);
		data->size_free += align + BLOCK_SIZE;
	}
	block->size = 0;
}

static int check_for_free_page(t_data *prev, t_data *current, t_block *block, void *ptr)
{
	while (block)
	{
		if (ptr == (void *)block + BLOCK_SIZE)
		{
			if (block->size == 0)
				return (-1);
			free_meta_block(block, current);
			/* if not pre allocate page and page is empty we can munmap */
			if (!(current->type & PRE_ALLOCATE) && page_empty(current) == TRUE) {
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

// ft_printf_fd(2, "Size before , %u\n", block->size);
// ft_printf_fd(2, "Size after , %u\n", block->size);
// ft_printf_fd(2, "Free, %u\n", block->size);

static int try_free(void *ptr)
{
	t_data *data = g_data;
	if (!data)
		return (FALSE);
	int ret = 1;

	ret = check_for_free_page(NULL, data, data->block, ptr);
	if (ret == 0)
		return (TRUE);
	else if (ret == -1)
		return (FALSE);
	while(data && data->next)
	{
		ret = check_for_free_page(data, data->next, data->next->block, ptr);
		if (ret == 0)
			return (TRUE);
		else if (ret == -1)
			return (FALSE);
		data = data->next;
	}
	return (FALSE);
}

void free(void* ptr)
{
	// ft_printf_fd(1, "my free called\n");
	if (!ptr)
		return ;
	// try_free(ptr);
	if (try_free(ptr) == FALSE) {
		ft_printf_fd(2, "Invalid ptr - block_size == %p\n", ptr - BLOCK_SIZE);
		ft_printf_fd(2, "Invalid ptr == %p\n", ptr);
	}
}