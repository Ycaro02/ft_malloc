# include "../../include/malloc.h" /* libft malloc version */

int main()
{
    ft_printf_fd(1, YELLOW"Test 5 \n"RESET);
    malloc(128);
    malloc(1024);
    malloc(1024 * 32);
    malloc(1024 * 1024);
    malloc(1024 * 1024 * 16);
    malloc(1024 * 1024 * 128);
    show_alloc_mem();
    free_meta_data();
    show_alloc_mem();
    return (0);
}