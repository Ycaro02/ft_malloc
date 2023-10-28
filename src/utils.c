#include "../include/malloc.h"

size_t ft_strlen(char *str)
{
	int i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void display_line(char *str, char to_display)
{
	int nb = 35 - ft_strlen(str);
	for (int i = 0; i < 15; i++)
			write(1, &to_display, 1);
	if (str)
			write(1, str, ft_strlen(str));
	for (int i = 0; i < nb; i++)
			write(1, &to_display, 1);
	write(1, "\n", 1);
}

size_t get_align_by_type(e_type type)
{
    int align = ALIGN_VALUE;
    if (!(type & LARGE)) 
        type & TINY ? (align = TINY_SIZE) : (align = SMALL_SIZE);
    return (align);
}

int get_lst_block_len(t_block *lst)
{
	int len = 0;
	while (lst)
	{
		lst = lst->next;
		len++;
	}
	return (len);
}

void free_block()
{
	t_data  *data = g_data;
	t_data  *data_tmp;
	while (data)
	{
		data_tmp = data->next;
		free(data);
		data = data_tmp;
	}
}

void    data_add_back(t_data **lst, t_data *new)
{
	t_data  *current;

	if (new == NULL)
			return ;
	if (*lst == NULL)
	{
			*lst = new;
			return ;
	}
	current = *lst;
	while (current->next != NULL)
			current = current->next;
	current->next = new;
}

void    block_add_back(t_block **lst, t_block *new)
{
	t_block  *current;

	if (new == NULL)
			return ;
	if (*lst == NULL)
	{
			*lst = new;
			return ;
	}
	current = *lst;
	while (current->next != NULL)
			current = current->next;
	current->next = new;
}

void print_define(void)
{
    ft_printf_fd(1, "TINY = %zu\n", TINY_BLOCK_PER_PAGE);
    ft_printf_fd(1, "SMALL = %zu\n", SMALL_BLOCK_PER_PAGE);
    ft_printf_fd(1, "size data: %zu, size block: %zu\n", DATA_SIZE, BLOCK_SIZE);
    int block_size = (BLOCK_SIZE * 100) + DATA_SIZE;
    ft_printf_fd(1, "size block * 100 + data size: %u = %zu TINY BLOCK\n", block_size, block_size / TINY_SIZE);
    ft_printf_fd(1, "size block * 100 + data size: %u = %zu SMALL BLOCK\n", block_size, block_size / SMALL_SIZE);
}