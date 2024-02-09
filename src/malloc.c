#include "../include/malloc.h"

/**
 * Declare globale t_data pointer, the head of pages linked-list
*/
t_data *g_data = NULL;

/** @brief Malloc main function first call init pages for TINY and SMALL blocks
 * 	@param size_t size: size of desired allocation in bytes
 *	@return Pointer to allocate block, NULL for invalid size ( <= 0) 
*/
void *malloc(size_t size)
{
	// ft_printf_fd(1, "my malloc called\n");
	e_type type;
	t_block *block;

	if (size <= 0) /* maybe to change malloc 1 for 0 input ? */
		return (NULL);
	type = detect_type(size);
	block = init_data(type, size);
	return (((void *) block) + BLOCK_SIZE);
}