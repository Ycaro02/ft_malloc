#include "../include/malloc.h"


void	put_hex(size_t nbr, int fd)
{
	char	*base_16;

	base_16 = "0123456789abcdef";
	if (nbr > 15)
		put_hex(nbr / 16, fd);
	ft_putchar_fd(base_16[nbr % 16], fd);
}


/** @brief	Display page function, iter on each block and display his information
 * 	@param	t_page *data, pointer on page to display
*/
static size_t print_bloc(t_page *data, int8_t hex_flag)
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
	while (block) {
		size_t i = 0;
		void *ptr = (void *)block + BLOCK_SIZE;
		ft_printf_fd(1, "%p - %p", ptr, ptr + block->size);
		ft_printf_fd(1, ": %U bytes", ptr + block->size - ptr);
		
		if (hex_flag) {
			ft_printf_fd(1, " -> ");
			for (i = 0; i < block->size; ++i) {
				ft_printf_fd(1, RED"Idx%s %s|%d|"RESET, RESET, YELLOW, i);
				ft_printf_fd(1, PURPLE"|0x");
				put_hex((size_t) (((char *) ptr)[i]), 1);
				ft_printf_fd(1, "|"RESET);
				/* 10 ascii val for \n, 32 for space */
				char sep = (((i % 7) == 0) * '\n') + (((i % 7) != 0) * ' ');
				ft_printf_fd(1, "%c", sep);
			} 
		}
		else
			ft_printf_fd(1, "\n");
		ft_printf_fd(1, "\n");
		total += ptr + block->size - ptr;
		block = block->next;
	}
	return (total);
}

void show_alloc_mem_hex()
{
	t_page *tmp;
	tmp = g_data;
	pthread_mutex_lock(&g_libft_malloc_mutex); /* lock before read g_data */
	display_line(NULL, '-');
	while (tmp) {
		print_bloc(tmp, 1);
		tmp = tmp->next;
	}
	pthread_mutex_unlock(&g_libft_malloc_mutex);
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
		total += print_bloc(tmp, 0);
		tmp = tmp->next;
	}
	ft_printf_fd(1, "Total: %U bytes\n", total);
	display_line(NULL, '-');
	pthread_mutex_unlock(&g_libft_malloc_mutex);

	ft_printf_fd(1, "Dump hex:\n");
	show_alloc_mem_hex();
}
