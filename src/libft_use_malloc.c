#include "include/malloc.h"

int main(void)
{
    char *coucou = ft_strdup("KOALAlolmysda\n");
    ft_printf_fd(1, "%s\n", coucou);
    show_alloc_mem();
    free(coucou);
    
    show_alloc_mem();
    free_meta_data();
    return(0);
}