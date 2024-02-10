
#include "../../include/malloc.h"
#include <string.h>

#define M (1024 * 1024)

void print(char *s) { write(1, s, strlen(s)); }

int test0(char *version)
{
    int i;
    char *addr;

    ft_printf_fd(1, "%sTest 0 for %s :%s\n", YELLOW, version, RESET);
    i = 0;
    while (i < 1024)
            i++;
    return (0);
}


int test1(char *version)
{
    int i;
    char *addr;

    ft_printf_fd(1, "%sTest 1 for %s :%s\n", YELLOW, version, RESET);
    i = 0;
    while (i < 1024)
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        i++;
    }
// show_alloc_mem();
    return (0);
}


int test2(char *version)
{
    int i;
    char *addr;

    ft_printf_fd(1, "%sTest 2 for %s :%s\n", YELLOW, version, RESET);
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

int test3(char *version)
{
    char *addr1;
    char *addr3;

    ft_printf_fd(1, "%sTest 3 for %s :%s\n", YELLOW, version, RESET);
    addr1 = (char*)malloc(16*M);
    strcpy(addr1, "Bonjours\n");
    print(addr1);
    addr3 = (char*)realloc(addr1, 128*M);
    addr3[127*M] = 42;
    print(addr3);
    return (0);
}

int test3_bis(char *version)
{
    char *addr1;
    char *addr2;
    char *addr3;

    ft_printf_fd(1, "%sTest 3_bis for %s :%s\n", YELLOW, version, RESET);
    addr1 = (char*)malloc(16*M);
    strcpy(addr1, "Bonjours\n");
    print(addr1);
    addr2 = (char*)malloc(16*M);
    addr3 = (char*)realloc(addr1, 128*M);
    addr3[127*M] = 42;
    print(addr3);
    return (0);
}

int test4(char *version)
{
    char *addr;

    ft_printf_fd(1, "%sTest 4 for %s :%s\n", YELLOW, version, RESET);
    addr = malloc(16);
    free(NULL);
    free((void *)addr + 5);
    if (realloc((void *)addr + 5, 10) == NULL)
    print("Bonjours\n");
}


int main(void)
{

}