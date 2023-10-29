#include "../include/malloc.h"

size_t get_page_size(e_type type, size_t size)
{
	size_t m_size = 0;

	if (type & TINY)
		m_size = TINY_PAGE_SIZE;
	else if (type & SMALL)
		m_size = SMALL_PAGE_SIZE;
	else
		m_size = align_mem_block(size + sizeof(t_data), PAGE_SIZE);
	return (m_size);
}

t_data *init_data_by_type(e_type type, size_t size)
{
	t_data *data = NULL;
	size_t page_size = 0;
	page_size = get_page_size(type, size);
	// data = t_malloc(page_size); // to_change for my_t_malloc mmapcall
	data = mmap(0, page_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (!data)
		return (NULL);
	data->type = type;
	data->size = page_size - DATA_SIZE;
	data->block = init_block(data->block, size, type, 0, data);
	data->size_t_free = page_size - data->block->size + DATA_SIZE;
	data->next = NULL;
	return (data);
}

t_block *init_data(e_type type, size_t size)
{
	t_data *data;
	if (!(type & LARGE))
	{
		t_block *block = try_add_block(type, size);
		if (block)
			return (block);
	}
	data = init_data_by_type(type, size);
	data_add_back(&g_data, data);
	return (data->block);
}

e_type detect_type(size_t size)
{
	e_type type;
	if (size <= TINY_SIZE)
		type = TINY;
	else if (size <= SMALL_SIZE)
		type = SMALL;
	else
		type = LARGE;
	return (type);
}