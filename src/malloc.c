#include "../include/malloc.h"

/**
 * Declare globale t_page pointer, the head of pages linked-list
*/
t_page			*g_data = NULL;
pthread_mutex_t	g_libft_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t	g_libft_malloc_mutex;

/* @brief Init first pre allocate page for tiny and small block */
inline static int8_t init_first_page()
{
	/* Variable creation in scope to avoid useless stack allocation when g_data != NULL*/
	if (!g_data) {
		t_page *page;
		int 	fd = -1;
		int8_t special_flag = PRE_ALLOCATE;

		fd = handle_env_variable(&special_flag);
		page = init_page(TINY, 0, special_flag);
		if (!page) {
			pthread_mutex_unlock(&g_libft_malloc_mutex);
			return (FALSE);
		}
		page->fd = fd;
		page_add_back(&g_data, page);

		if (check_debug_flag(ALLOCATION_TRACE)) {
			ft_printf_fd(get_debug_fd(), GREEN"Create first tiny  page (mmap called) %p\n"RESET, page); /* Only for call history */
		}
		/* first page->type contain all debug context no more needed */
		page = init_page(SMALL, 0, PRE_ALLOCATE);
		if (!page) {
			pthread_mutex_unlock(&g_libft_malloc_mutex);
			return (FALSE);
		}		
		page_add_back(&g_data, page);
		if (check_debug_flag(ALLOCATION_TRACE)) {
			ft_printf_fd(get_debug_fd(), GREEN"Create first small page (mmap called) %p\n"RESET, page); /* Only for call history */
		}
		if (check_debug_flag(GARBAGE_COLLECTOR_FREE)) {
			atexit(free_meta_data);
		}
		if (check_debug_flag(DETECT_LEAK)) {
			atexit(check_for_leak);
		}
	}
	return (TRUE);
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

	pthread_mutex_lock(&g_libft_malloc_mutex);
	
	if (size <= 0) {  /* maybe to change malloc 1 for 0 input ? */
		pthread_mutex_unlock(&g_libft_malloc_mutex);
		return (NULL);
	}

	if(init_first_page() == FALSE) {
		pthread_mutex_unlock(&g_libft_malloc_mutex);
		return (NULL);
	}

	type = detect_type(size);
	block = init_data(type, size);

	if (check_debug_flag(ALLOCATION_TRACE)) {
		write_function_name(MALLOC_CALL, get_debug_fd());
		write_block_info(block, size, MALLOC_CALL, get_debug_fd());
	}

	pthread_mutex_unlock(&g_libft_malloc_mutex);
	return (((void *) block) + BLOCK_SIZE);
}