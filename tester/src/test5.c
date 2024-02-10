# include "../../include/malloc.h" /* libft malloc version */

int main()
{
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