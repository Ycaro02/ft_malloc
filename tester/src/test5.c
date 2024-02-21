# include "../../include/malloc.h" /* libft malloc version */
#include <string.h>
#include <unistd.h>


void print(char *s)
{
    write(1, s, strlen(s));
}

int     align_test5()
{
    int     i  = 1, ret = 0, alignment = 2 * sizeof(size_t);;
    char    *addr;

    while (i <= 100)
    {
        addr = (char*)malloc(i);
        if (addr == NULL) {
            ft_printf_fd(1, RED"Failed to allocate memory\n"RESET);
            exit(1);
        }
        if ((((unsigned long) (addr)) % alignment) != 0) {
            ft_printf_fd(1, RED"malloc returned a non aligned boundary %p\n"RESET, addr);
            ret = 1;
        } 
        i++;
        free(addr);
    }
    return (ret);
}


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
    ft_printf_fd("%sAligne size: %s%sOK\n"RESET, YELLOW, RESET, GREEN);
    int ret = align_test5();
    if (ret == 0) {
        ft_printf_fd(1, "%sAligne size: %s%sOK\n"RESET, YELLOW, RESET, GREEN);
    }
    return (0);
}