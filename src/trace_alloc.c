#include "../include/malloc.h"


static void put_size_t_fd(size_t n, int fd)
{
	if (n / 10 != 0)
		ft_putnbr_fd(n / 10, fd);
	ft_putchar_fd(n % 10 + '0', fd);
}

static void handle_add_color(int fd, char *color)
{
	if (check_debug_flag(ENABLE_COLOR)) {
		ft_printf_fd(fd, color);
	}
}

static void handle_reset_color(int fd)
{
	if (check_debug_flag(ENABLE_COLOR)) {
		ft_printf_fd(fd, RESET);
	}
}

static void display_realloc_block(t_block *block, e_type type, size_t size, int fd)
{
    ft_printf_fd(fd, " allocated, try to add: ");

	handle_add_color(fd, BLUE);
	put_size_t_fd(size, fd);
	handle_reset_color(fd);

	ft_printf_fd(fd, " additional bytes.\nNew expected size: ");

	handle_add_color(fd, GREEN);
    put_size_t_fd(size + block->size, fd);
	handle_reset_color(fd);

    if (!(type & LARGE) && size + block->size <= get_page_size(type, size)) {
        ft_printf_fd(fd, " Extend block success no need to move data");
    } else {
        ft_printf_fd(fd, " No enought space to extend block need to call malloc");
    }
}

void write_function_name(int16_t call, int fd)
{
	if (check_debug_flag(ENABLE_COLOR)) {
		char *color = GREEN;
		if (call == REALLOC_CALL)
			color = PURPLE;
		else if (call == FREE_CALL)
			color = RED;
		ft_printf_fd(fd, color);
	}

	if (call == MALLOC_CALL) {
		ft_printf_fd(fd ,"Malloc  call:\n");
	} else if (call == REALLOC_CALL ) {
		ft_printf_fd(fd , "Realloc call:\n");
	} else {
		ft_printf_fd(fd , "Free    call:\n");
	}
	handle_reset_color(fd);
}

void write_block_info(t_block *block, size_t size, int16_t call, int fd)
{
	int16_t type = detect_type(block->size);

	handle_add_color(fd, YELLOW);
	if (type & TINY) {
		ft_printf_fd(fd, "tiny  block ");
	} else if (type & SMALL) {
		ft_printf_fd(fd, "small block ");
	} else {
		ft_printf_fd(fd, "large block ");
	}
	handle_reset_color(fd);

	ft_printf_fd(fd, "of size: ");
	handle_add_color(fd, GREEN);
	put_size_t_fd(block->size, fd);
	handle_reset_color(fd);

	if (call == REALLOC_CALL) {
        display_realloc_block(block, type, size, fd);
    }

	ft_printf_fd(fd, "\nBlock : ");
	handle_add_color(fd, CYAN);
	ft_printf_fd(fd, "%p ", block);
	handle_reset_color(fd);

	ft_printf_fd(fd, " Data : ");
	handle_add_color(fd, CYAN);
	ft_printf_fd(fd, "%p\n", (void *)block + BLOCK_SIZE);
	handle_reset_color(fd);

	handle_add_color(fd, YELLOW);
	ft_printf_fd(fd, "-------------------------------------------\n");
	handle_reset_color(fd);
}

void check_for_leak()
{
	t_page	*current = g_data;
	t_block *block;

	if (!g_data)
		return ;

	pthread_mutex_lock(&g_malloc_mutex); /* lock before read g_data */
	while (current) {
		block = current->block;
		while (block) {
			if (block->size != 0 ) {
				char *color = RED;
				int16_t type = detect_type(block->size);
				if (type & TINY) {
					color = YELLOW;
				} else if (type & SMALL) {
					color = PURPLE;
				}
				ft_printf_fd(2, "%sLeak in page: %p, block %p, potential data %p of size %u not free\n"RESET\
				, color, current, block, (void *)block + BLOCK_SIZE, block->size);
			}
			block = block->next;
		}
		current = current->next;;
	}
	pthread_mutex_unlock(&g_malloc_mutex);
}