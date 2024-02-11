#include "../include/malloc.h"

/** @brief Exec realloc function, alloc new space with malloc call and copy old data **/
static void *exec_realloc(t_block *block, size_t size)
{
	size_t	i = 0, block_size = block->size;;
	char 	*content = (char *)((void *)block + BLOCK_SIZE), *new_ptr = NULL;
	
	pthread_mutex_unlock(&g_libft_malloc_mutex); /* unlock before malloc call */
	new_ptr = malloc(block->size + size);
	pthread_mutex_lock(&g_libft_malloc_mutex); /* lock after malloc call cause unlock at the end of realloc, maybe can optimise that */
	
	if (!new_ptr) {
		return (NULL);
	}
	/* memcpy instead ?? */
	while (i < block_size) {
		new_ptr[i] = content[i];
		i++;
	}
	return ((void *) new_ptr);
}

/** @brief Check data type and empty space in page to know if realloc is needed **/
static int8_t need_realloc(t_page *page, t_block *block, size_t size)
{
	size_t align = size;
	size_t new_size = block->size + size;
	if (!(page->type & LARGE)) {
		align = get_align_by_type(page->type);
		if (new_size <= align)
			return (FALSE);
	}
	return (TRUE);
}

/**	@brief loop on lst_block to find given ptr and check for realloc needed
 *	@param t_page *prev: pointer on previous page	
 *	@param t_page *current: pointer on current page	
 *	@param t_block *block: list of block in current page
 *	@param void *ptr: pointer given by user, wanted to free him
 *	@param size_t size: size of desired new allocation (add to ptr_len_block)
 *	@return: pointer to block data, NULL for invalid pointer
*/
static void *check_for_realloc_block(t_page *prev, t_page *current, t_block *block, void *ptr, size_t size)
{
	int8_t realloc_needed = FALSE;
	while (block) {
		if (ptr == (void *)block + BLOCK_SIZE) {
			realloc_needed = need_realloc(current, block, size);
			if (realloc_needed == TRUE) {
				ptr = exec_realloc(block, size);
				free_meta_block(block, current);
				if (page_empty(current)== TRUE) { /* if page empty */
					prev == NULL ? (g_data = current->next) : (prev->next = current->next);
					free_page(current);
				}
			}
			else {
				block->size += size;
			}
			return (ptr);
		}
		block = block->next;
	}
	return (NULL);
}

/** @brief Try to find given ptr in g_data page list
 * 	@param void *ptr: ptr given by user
 * 	@param size_t size: size to add at ptr block len
 * 	@return: pointer to new allocation or last ptr if no allocation needed
*/
static void *get_block_addr(void *ptr, size_t size)
{
	t_page *data = g_data;
	t_page *test = check_for_realloc_block(NULL, data, data->block, ptr, size);

	if (test != ptr)
		return (test);
	while(data && data->next)
	{
		test = check_for_realloc_block(data, data->next, data->next->block, ptr, size);
		if (test != ptr)
			return (test);
		data = data->next;
	}
	return (ptr);
}

/**
 * 	@brief -If ptr is NULL, then the call is equivalent to malloc(size)
 *	-If size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr) 
*/
void *realloc(void *ptr, size_t size)
{
	// ft_printf_fd(1, "my realloc called\n");
	void *new_ptr = NULL;
	pthread_mutex_lock(&g_libft_malloc_mutex);

	if (!ptr) {
		pthread_mutex_unlock(&g_libft_malloc_mutex);
		return (malloc(size));
	} 
	else if (size == 0) {
		pthread_mutex_unlock(&g_libft_malloc_mutex);
		free(ptr);
		return (NULL);
	}
	else {
		new_ptr = get_block_addr(ptr, size);
		if (!new_ptr) {
			ft_printf_fd(2, RED"Realloc invalid pointer %p\n"RESET,ptr);
		}
	}

	pthread_mutex_unlock(&g_libft_malloc_mutex);
	return (new_ptr);
}

// if (prev)
// 	printf("all info prev: addr: %p, size: %zu, next: %p\n", prev, prev->block->size, prev->next);
// printf("all info curr: addr: %p, size: %zu, next: %p\n", current, current->block->size, current->next);
// printf("all info save: addr: %p, size: %zu, next: %p\n", save, save->block->size, save->next);
// if (prev)
// 	printf("AFT info prev: addr: %p, size: %zu, next: %p\n", prev, prev->size, prev->next);