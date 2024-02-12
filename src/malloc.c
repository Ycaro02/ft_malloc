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
char *check_env_variable(char *to_check)
{
	char *env = getenv(to_check);
	if (env) {
		ft_printf_fd(1, "%s =  %s\n", to_check, env);
		return (env);
	}
	return (NULL);
}


inline int8_t check_debug_flag(int8_t flag)
{
	return ((g_data->type & flag) == flag);
}

inline int get_debug_fd()
{
	return (g_data->fd);
}

/* @brief Init first pre allocate page for tiny and small block */
inline static int8_t init_first_page()
{
	char 	*env;
	int 	fd = -1;

	if (!g_data) {
		int8_t special_flag = PRE_ALLOCATE;
		env = check_env_variable(MALLOC_TRACE_ENV);
		if (env) {
			fd = open(env, O_CREAT | O_APPEND | O_RDWR, 00777);
			if (fd > 0) {
				special_flag += ALLOCATION_TRACE;
			} else {
				fd = -1;
			}
		}
		t_page *page = init_page(TINY, 0, special_flag);
		if (!page) {
			return (FALSE);
		}
		page->fd = fd;
		page_add_back(&g_data, page);
		/* first page->type contain all debug context no more needed */
		page = init_page(SMALL, 0, ALLOCATION_TRACE);
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
	type = detect_type(size);
	block = init_data(type, size);
	if (check_debug_flag(ALLOCATION_TRACE)) {
		write_function_name(MALLOC_CALL, get_debug_fd()); /* Only for call history/trace */
		write_block_info(block, size, MALLOC_CALL, get_debug_fd()); /* Only for trace */
	}
	pthread_mutex_unlock(&g_libft_malloc_mutex);
	return (((void *) block) + BLOCK_SIZE);
}