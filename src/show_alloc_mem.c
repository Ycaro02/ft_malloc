#include "../include/malloc.h"

/** @brief	Display page function, iter on each block and display his information
 * 	@param	t_page *data, pointer on page to display
*/
static size_t print_bloc(t_page *data)
{
	int total = 0;
	if (data->type & TINY)
		ft_printf_fd(1, "TINY");
	else if (data->type & SMALL)
		ft_printf_fd(1, "SMALL");
	else
		ft_printf_fd(1, "LARGE");
	ft_printf_fd(1, " : %p\n", data);
	t_block *block = data->block;
	while (block)
	{
		void *ptr = (void *)data->block + BLOCK_SIZE;
		ft_printf_fd(1, "%p - %p", ptr, ptr + block->size);
		ft_printf_fd(1, ": %U bytes\n", ptr + block->size - ptr);
		total += ptr + block->size - ptr;
		block = block->next;
	}
	return (total);
}

/** @brief Mandatory function show allocated memory blocks/pages */
void show_alloc_mem()
{
	t_page *tmp;
	size_t total = 0;
	
	pthread_mutex_lock(&g_libft_malloc_mutex); /* lock before read g_data */
	tmp = g_data;
	display_line(NULL, '-');
	while (tmp) {
		total += print_bloc(tmp);
		tmp = tmp->next;
	}
	ft_printf_fd(1, "Total: %U bytes\n", total);
	display_line(NULL, '-');
	pthread_mutex_unlock(&g_libft_malloc_mutex);
}