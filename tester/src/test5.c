# include "../../include/malloc.h" /* libft malloc version */
#include <string.h>
#include <unistd.h>


void print(char *s)
{
    write(1, s, strlen(s));
}

int     align_test5()
{
    int     i;
    int     alignment;
    char    *addr;

    i = 20000;
    alignment = 2 * sizeof(size_t);
    while (i <= 20100)
    {
        addr = (char*)malloc(i);
        if (addr == NULL)
        {
            print("Failed to allocate memory\n");
            exit(1);
        }
        if ((((unsigned long) (addr)) % alignment) != 0) {
            ft_printf_fd(1, RED"malloc returned a non aligned boundary %p\n"RESET, addr);
        } 
        // else {
        //     ft_printf_fd(1, GREEN"malloc returned a aligned boundary %p\n"RESET, addr);
        // }
        i++;
        free(addr);
    }
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
    align_test5();
    return (0);
}