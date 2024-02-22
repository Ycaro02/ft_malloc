#include "../include/malloc.h"

void	put_hex(size_t nbr, int fd)
{
	char	*base_16 = "0123456789abcdef";
	
	if (nbr > 15)
		put_hex(nbr / 16, fd);
	ft_putchar_fd(base_16[nbr % 16], fd);
}


void display_hex_data(t_block *block, void *ptr)
{
	for (size_t idx = 0; idx < block->size; ++idx) {
		ft_printf_fd(1, RED"Idx%s %s|%d|"RESET, RESET, YELLOW, idx);
		ft_printf_fd(1, PURPLE"|0x");
		put_hex((size_t) (((char *) ptr)[idx]), 1);
		ft_printf_fd(1, "|"RESET);
		/* if idx % 7 == 0 print '\n' else space*/
		char sep = (((idx != 0) && (idx % 7 == 0)) * '\n') + (((idx == 0) || (idx % 7 != 0)) * ' ');

		ft_printf_fd(1, "%c", sep);
	} 
}

static void display_type(int16_t type)
{
	if (type & TINY) {
		ft_printf_fd(1, TINY_NAME);
	} else if (type & SMALL) {
		ft_printf_fd(1, SMALL_NAME);
	} else {
		ft_printf_fd(1, LARGE_NAME);
	}
}

/** @brief	Display page function, iter on each block and display his information
 * 	@param	t_page *data, pointer on page to display
*/
static size_t print_bloc(t_page *data, int16_t hex_flag)
{
	int 	total = 0;
	t_block	*block;

	display_type(data->type);
	ft_printf_fd(1, " : %p\n", data);
	block = data->block;
	while (block) {
		void *ptr = (void *)block + BLOCK_SIZE;
		if (hex_flag) {
			display_hex_data(block, ptr);
		} else {
			ft_printf_fd(1, "%p - %p", ptr, ptr + block->size);
			ft_printf_fd(1, ": %U bytes\n", ptr + block->size - ptr);
		}
		total += ptr + block->size - ptr;
		block = block->next;
	}
	/*char c = (hex_flag == 0) * '\n' */
	if (hex_flag)
		ft_printf_fd(1, "\n");
	return (total);
}

void show_alloc_mem_hex()
{
	t_page *tmp = g_data;
	
	pthread_mutex_lock(&g_malloc_mutex); /* lock before read g_data */
	display_line(NULL, '-');
	while (tmp) {
		print_bloc(tmp, 1);
		tmp = tmp->next;
	}
	pthread_mutex_unlock(&g_malloc_mutex);
}


/** @brief Mandatory function show allocated memory blocks/pages */
void show_alloc_mem()
{
	t_page *tmp;
	size_t total = 0;
	
	pthread_mutex_lock(&g_malloc_mutex); /* lock before read g_data */
	tmp = g_data;
	display_line(NULL, '-');
	while (tmp) {
		total += print_bloc(tmp, 0);
		tmp = tmp->next;
	}
	ft_printf_fd(1, "Total: %U bytes\n", total);
	display_line(NULL, '-');
	pthread_mutex_unlock(&g_malloc_mutex);
}
