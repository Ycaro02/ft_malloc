#include <stdlib.h>

int main()
{
int i;
char **addr;

i = 0;
addr = malloc(sizeof(char *) * 1025);
while (i < 1024)
{
    addr[i] = (char*)malloc(1024);
    addr[i][0] = 42;
    i++;
}
for (int j = 0; j < 1024; j++)
    free(addr[j]);
free(addr);
return (0);
}
