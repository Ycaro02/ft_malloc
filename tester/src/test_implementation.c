
# ifdef USE_LIBFT_MALLOC /* define at compilation time */
# include "../../include/malloc.h" /* libft malloc version */
# define TEST_VERSION_NAME "Libft Version"
# else
# include <stdlib.h> /* real malloc version */
# define TEST_VERSION_NAME "Malloc Version"
# endif

# include "../../include/basic_define.h"
# include <string.h>
# include <unistd.h>

enum test_flag {
    TEST0=(1 << 0),
    TEST1=(1 << 1),
    TEST2=(1 << 2),
    TEST3=(1 << 3),
    TEST3_BIS=(1 << 4),
    TEST4=(1 << 5),
};

#define M (1024 * 1024)

void print(char *s) { write(1, s, strlen(s)); }

void prints(char *s1, char *s2, char *s3, char*s4) { 
    write(1, s1, strlen(s1));
    write(1, s2, strlen(s2));
    write(1, s3, strlen(s3));
    write(1, s4, strlen(s4));
    write(1, "\n", 1);
}

int test0(char *version)
{
    int i;
    char *addr;

    prints(YELLOW, "Test 0 for : ", version, RESET);
    i = 0;
    while (i < 1024)
            i++;
    return (0);
}


int test1(char *version)
{
    int i;
    char *addr;

    prints(YELLOW, "Test 1 for : ", version, RESET);
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

    prints(YELLOW, "Test 2 for : ", version, RESET);
    i = 0;
    while (i < 1024)
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        // prints("%sJust affter Malloc call nb %d , Before free%s\n", GREEN, i,  RESET);
        // show_alloc_mem();
        free(addr);
        // prints("%sAfter free%s\n", RED, RESET);
        // show_alloc_mem();
        i++;
    }
    return (0);
}

int test3(char *version)
{
    char *addr1;
    char *addr3;

    prints(YELLOW, "Test 3 for : ", version, RESET);
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

    prints(YELLOW, "Test 3_bis for : ", version, RESET);
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

    prints(YELLOW, "Test 4 for : ", version, RESET);
    addr = malloc(16);
    // free(NULL);
    prints(RED"Try to give NULL ptr to free for", version, "\n", RESET);

    char *str = NULL;
    free(str);
    // free((void *)addr + 5);
    if (realloc((void *)addr + 5, 10) == NULL) {
        print("Bonjours\n");
    }
    # ifdef USE_LIBFT_MALLOC /* define at compilation time */
        print("Show alloc mem after\n");
        show_alloc_mem();    
    #endif
}

int main(int argc, char **argv)
{
    int test_flag = 0;
    if (argc == 2) {
        test_flag = atoi(argv[1]);
    }
    if (test_flag & TEST0)
        test0(TEST_VERSION_NAME);
    if (test_flag & TEST1)
        test1(TEST_VERSION_NAME);
    if (test_flag & TEST2)
        test2(TEST_VERSION_NAME);
    if (test_flag & TEST3)
        test3(TEST_VERSION_NAME);
    if (test_flag & TEST3_BIS)
        test3_bis(TEST_VERSION_NAME);



    if (test_flag & TEST4) {
        # ifdef USE_LIBFT_MALLOC /* define at compilation time */
            free_meta_data();   /* just free all page before call test 4*/
        #endif
        test4(TEST_VERSION_NAME);
    }
    // prints(Define USE_LIBFT_MALLOC :|%d| %d%s\n", USE_LIBFT_MALLOC);
}