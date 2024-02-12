#include "../include/malloc.h"


static void put_size_t_fd(size_t n, int fd)
{
	if (n / 10 != 0)
		ft_putnbr_fd(n / 10, fd);
	ft_putchar_fd(n % 10 + '0', fd);
}


void handle_add_color(int fd, char *color)
{
	if (check_debug_flag(ENABLE_COLOR)) {
		ft_printf_fd(fd, color);
	}
}

void handle_reset_color(int fd)
{
	if (check_debug_flag(ENABLE_COLOR)) {
		ft_printf_fd(fd, RESET);
	}
}

void write_function_name(int8_t call, int fd)
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
		ft_printf_fd(fd ,"Malloc  call: ");
	} else if (call == REALLOC_CALL ) {
		ft_printf_fd(fd , "Realloc call: ");
	} else {
		ft_printf_fd(fd , "Free    call: ");
	}
	handle_reset_color(fd);
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
        ft_printf_fd(fd, " Extend block success no need to move data\n");
    } else {
        ft_printf_fd(fd, " No enought space to extend block need to call malloc");
    }
}

void write_block_info(t_block *block, size_t size, int8_t call, int fd)
{
	int8_t type = detect_type(block->size);

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
	put_size_t_fd(size, fd);
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

/*
	- We need to check env variable and set type in consequences
		
		* 	just do a get flag function who check g_data->type, 
			for this we need to create pre allocated page with debug flag in first ;
	
	- Page size are easy to compute, but we need to save all page allocation ( mmap call )in init_page,
	for example large page is very versatile, store munmap call too.
	
		- create file with expected name, just need to store the fd
		- each important call format and write data in fd
	void write_page_info(t_page *page, size_t size)
	{
		char buff[2000];
			if (page->type & TINY)
				ft_strcpy(buff, "Page: tiny")
			else if (page->type & SMALL)		
				ft_strcpy(buff, "Page: small")
			else 
				ft_strcpy(buff, "Page: large")
	}

	Malloc:
		- block type
		- nb bytes expected
		- nb bytes reserved (aligned or tiny/small size)
		- page pointer
			- page status (already allocated or new allocation)
		- block pointer
		- data pointer
		- allocation status
	Realloc:
		- valid pointer
			- old block/data pointer
			- old size
		- new bytes expected
		- new total bytes expected
		- new total bytes reserved 
		- page pointer
			- page status (already allocated or new allocation)
		- block pointer
		- data pointer
		- allocation status
	Free:
		- valid pointer
			- old block/data pointer
			- old size
*/