#include "../include/malloc.h"

/**
 * Declare globale t_page pointer, the head of pages linked-list
*/
t_page			*g_data = NULL;
pthread_mutex_t	g_libft_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t	g_libft_malloc_mutex;


/**	@brief Check for pre allocated page for TINY and SMALL block
 *  @param e_type type: type of searched page
 *	@return TRUE if any pre allocate page of this type exist otherwise FALSE
*/
static int8_t check_preallocated_page(e_type type)
{
	t_page *data = g_data;
	while (data) {
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

	if (type == TINY && check_preallocated_page(type) == FALSE) {
		page = init_page(TINY, 0, PRE_ALLOCATE);
		if (!page) {
			return (FALSE);
		}
		// ft_printf_fd(1, "\n%sINIT First Tiny page%s\n", RED, RESET);
		page_add_back(&g_data, page);
	} else if (type == SMALL && check_preallocated_page(type) == FALSE) {
		page = init_page(SMALL, 0, PRE_ALLOCATE);
		if (!page) {
			return (FALSE);
		}		
		// ft_printf_fd(1, "\n%sINIT First Small page%s\n", RED, RESET);
		page_add_back(&g_data, page);
	}
	return (TRUE);
}



void	put_size_t_fd(size_t n, int fd)
{
	if (n / 10 != 0)
		ft_putnbr_fd(n / 10, fd);
	ft_putchar_fd(n % 10 + '0', fd);
}


/* @brief check env variable to know is we need to build debug data */
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

void start_call_malloc()
{
	char *test = getenv("");
}
*/

// void	put_hexa_fd(void *nbr, int fd)
// {
// 	char			*base_16;
// 	unsigned int	nb = nbr;

// 	base_16 = "0123456789abcdef";
// 	if (nb > 15)
// 		put_hexa_fd(nb / 16, fd);
// 	ft_putchar_fd(base_16[nb % 16], fd);
// }


void write_function_call(int8_t call, int fd)
{
	char buff[200];

	ft_bzero(buff, 200);
	if (call == MALLOC_CALL) {
		ft_strcpy(buff, "Malloc call\n", ft_strlen("Malloc alloc \n"));
	} else if (call == REALLOC_CALL ) {
		ft_strcpy(buff, "Realloc call\n", ft_strlen("Realloc try to extend \n"));
	} else {
		ft_strcpy(buff, "Free call\n", ft_strlen("Free \n"));
	}
	ft_printf_fd(fd,RED"%s"RESET, buff);
}

// static int str_count_cpy(char *dest, char *src)
// {
// 	int len = ft_strlen(src);
// 	ft_strcpy(dest, src, len);
// 	dest[len + 1] = '\0';
// 	return (len + 1);
// }

static size_t maximum_size_by_type(int8_t type)
{
	return (type & TINY ? TINY_SIZE : SMALL_SIZE);
}

void write_block_info(t_block *block, size_t size, int8_t call, int fd)
{
	// char buff[2000];
	// int last = 0;

	// ft_bzero(buff, 2000);
	write_function_call(call, fd);

	int8_t type = detect_type(block->size);
	if (type & TINY) {
		ft_printf_fd(fd, YELLOW"block: tiny "RESET);
	} else if (type & SMALL) {
		ft_printf_fd(fd, YELLOW"block: small "RESET);
	} else {
		ft_printf_fd(fd, YELLOW"block: large "RESET);
	}	
	ft_printf_fd(fd, "of size: ");
	// write_reset_buff
	if (call == REALLOC_CALL) {
		put_size_t_fd(block->size, fd);
		ft_printf_fd(fd, " allocated, try to add: ");
		// write_reset_buff
		put_size_t_fd(size, fd);
		ft_printf_fd(fd, " additional bytes\nNew expected size: ");
		// write_reset_buff
		put_size_t_fd(size + block->size, fd);
		if (!(type & LARGE) && size + block-> size <= maximum_size_by_type(type)) {
			ft_printf_fd(fd, " Extend block success no need to move data\n");
		} else {
			ft_printf_fd(fd, " No enought space to extend block need to call malloc\n");
		}
		// write_reset_buff
	} else {
		put_size_t_fd(size, fd);
	}
	ft_printf_fd(fd, "\nBlock adress: \n");
	ft_printf_fd(fd, CYAN"%p\n"RESET, block);
	ft_printf_fd(fd, "Data pointer adress: \n");
	ft_printf_fd(fd, CYAN"%p\n"RESET, block + BLOCK_SIZE);

}

/** @brief The malloc() function allocates “size” bytes of memory and returns a pointer to the
 *	allocated memory
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
	/* lock mutex */
	pthread_mutex_lock(&g_libft_malloc_mutex);

	type = detect_type(size);
	if (first_page_allocation(type) == FALSE) {
		pthread_mutex_unlock(&g_libft_malloc_mutex);
		return (NULL);
	}
	
	block = init_data(type, size);
	write_block_info(block, size, MALLOC_CALL, 1);
	pthread_mutex_unlock(&g_libft_malloc_mutex);
	return (((void *) block) + BLOCK_SIZE);
}