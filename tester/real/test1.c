#include <stdlib.h>

int main()
{
int i;
char **addr;

i = 0;
addr = t_malloc(sizeof(char *) * 1025);
while (i < 1024)
{
    addr[i] = (char*)t_malloc(1024);
    addr[i][0] = 42;
    i++;
}
for (int j = 0; j < 1024; j++)
    t_free(addr[j]);
// t_free(addr);
return (0);
}
