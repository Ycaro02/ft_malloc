#include "../malloc.h"

t_data *g_data;

e_size find_size(size_t size)
{
	if (size <= TINY_SIZE )
		return (TINY);
	if (size <= SMAL_SIZE)
		return (SMALL);
	return (LARGE);
}

void *ft_malloc(size_t size)
{
	void *ptr = NULL;
	ptr = mmap(0, size + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED)
		return (NULL);
	(void)size;
	return (ptr);
}
