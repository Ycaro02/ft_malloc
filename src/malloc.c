#include "../include/malloc.h"

/**
 * Declare globale t_page pointer, the head of pages linked-list
*/
t_page			*g_data = NULL;
pthread_mutex_t	g_libft_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t	g_libft_malloc_mutex;

/* @brief check env variable to know is we need to build debug data */
static char *check_env_variable(char *to_check)
{
	char *env = getenv(to_check);
	if (env) {
		ft_printf_fd(1, "%s =  %s\n", to_check, env);
		return (env);
	}
	return (NULL);
}

/* @brief basic check flag function return bool value */
inline int8_t check_debug_flag(int8_t flag) { return ((g_data->type & flag) == flag); }

/* @brief get debug file fd */
inline int get_debug_fd() { return (g_data->fd); }

// static int8_t get_color_variable()
// {
// 	char *color = getenv(MALLOC_COLOR_ENV);
// 	/*if color == NULL --> 0 * FLAG, otherwise --> 1 *FLAG*/
// 	return ((color != NULL) * ENABLE_COLOR)
// }


static int8_t bool_check_env(char *to_check, int8_t flag)
{
	char *env = getenv(to_check);
	// ft_printf_fd(2, "for to check %s: return %d\n", to_check, (env != NULL) * ENABLE_COLOR);
	/*if env == NULL --> 0 * FLAG, otherwise --> 1 *FLAG*/
	return ((env != NULL) * flag);
}

static int handle_env_variable(int8_t *special_flag)
{
	int 	fd = -1;
	char	*trace_file = check_env_variable(MALLOC_TRACE_ENV);

	if (trace_file) {
		fd = open(trace_file, O_CREAT | O_APPEND | O_RDWR, 00777);
		if (fd > 0) {
			*special_flag += ALLOCATION_TRACE;
		} else {
			ft_printf_fd(2, "Error can't open or create file %s\n", trace_file);
			fd = -1;
		}
	}
	*special_flag += bool_check_env(MALLOC_COLOR_ENV, ENABLE_COLOR);
	*special_flag += bool_check_env(MALLOC_LEAKS_ENV, DETECT_LEAK);
	return (fd);
}

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
			return (FALSE);
		}
		page->fd = fd;
		page_add_back(&g_data, page);
		/* first page->type contain all debug context no more needed */
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

	pthread_mutex_lock(&g_libft_malloc_mutex);

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