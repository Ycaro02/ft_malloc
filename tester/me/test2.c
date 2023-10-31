#include "../../include/malloc.h"

int main()
{
int i;
char *addr;

i = 0;
char *coucou = malloc(1024);
while (i < 64)
{
    addr = (char*)malloc(1024);
    addr[0] = 42;
    free(coucou);
    display_line("before", '-');
    show_alloc_mem();
    free(addr);
    display_line("after", '-');
    show_alloc_mem();
    i++;
}
show_alloc_mem();
return (0);
}