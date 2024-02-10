#include "../../include/malloc.h"

int main()
{
int i;
char *addr;

i = 0;
while (i < 1024)
{
addr = (char*)malloc(1024);
addr[0] = 42;
// ft_printf_fd(1, "%sJust affter Malloc call nb %d , Before free%s\n", GREEN, i,  RESET);
// show_alloc_mem();
free(addr);
// ft_printf_fd(1, "%sAfter free%s\n", RED, RESET);
// show_alloc_mem();
i++;
}
return (0);
}