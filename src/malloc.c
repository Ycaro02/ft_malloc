#include "../include/malloc.h"

/**
 * Declare globale t_page pointer, the head of pages linked-list
*/
t_page			*g_data = NULL;
pthread_mutex_t	g_libft_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t	g_libft_malloc_mutex;

/* @brief check env variable to know is we need to build debug data */
/*
*/
void start_call_malloc()
{
	char *env = getenv(MALLOC_TRACE_ENV);
	if (env) {
		ft_printf_fd(1, "MALLOC_TRACE_ENV = %s\n", env);
	}
}

/* @brief Init first pre allocate page for tiny and small block */
inline static int8_t init_first_page()
{
	if (!g_data) {
		start_call_malloc();
		t_page *page = init_page(TINY, 0, PRE_ALLOCATE);
		if (!page) {
			return (FALSE);
		}		
		page_add_back(&g_data, page);
		page = init_page(SMALL, 0, PRE_ALLOCATE);
		if (!page) {
			return (FALSE);
		}		
		page_add_back(&g_data, page);
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

	if (size <= 0) {  /* maybe to change malloc 1 for 0 input ? */
		return (NULL);
	}
	// start_call_malloc();
	/* lock mutex */
	pthread_mutex_lock(&g_libft_malloc_mutex);
	if(init_first_page() == FALSE) {
		pthread_mutex_unlock(&g_libft_malloc_mutex);
		return (NULL);
	}
	write_function_name(MALLOC_CALL, 2); /* Only for call history */
	type = detect_type(size);

	block = init_data(type, size);
	write_block_info(block, size, MALLOC_CALL, 2); /* Only for call history */
	pthread_mutex_unlock(&g_libft_malloc_mutex);
	return (((void *) block) + BLOCK_SIZE);
}