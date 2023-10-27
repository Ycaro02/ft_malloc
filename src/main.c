#include "../malloc.h"
#include <stdio.h>
#define MAX 10000

// t_data *g_data = NULL;

void basic_test(int max, int len)
{
	for (int j = 0; j < max; j++)
	{
		char *test = (char *)t_malloc(len + 1);
		for (int i = 0; i < len; i++)
			test[i] = '3';
		test[len] = '\0';
	}
}

void replace_test(void)
{
	char *ptr1 = t_malloc(sizeof(char) * (12 + 1));
	char *ptr2 = t_malloc(sizeof(char) * (13 + 1));
	char *ptr3 = t_malloc(sizeof(char) * (14 + 1));
	display_line(NULL, '-');
	printf("ptr1 = %p\n", ptr1);
	printf("ptr2 = %p\n", ptr2);
	printf("ptr3 = %p\n", ptr3);
	for (int i = 0; i < 12; i++)
		ptr1[i] = '1';
	ptr1[12] = '\0';
	for (int i = 0; i < 13; i++)
		ptr2[i] = '2';
	ptr2[13] = '\0';
	for (int i = 0; i < 14; i++)
		ptr3[i] = '3';
	ptr3[14] = '\0';
	show_alloc_mem();
	printf("1: %s\n2: %s\n3: %s\n", ptr1, ptr2, ptr3);
	t_free(ptr2);
	display_line(NULL, '-');
	display_line("FREE", '-');
	show_alloc_mem();
	char* ptr4 = t_malloc(sizeof(char) * (15 + 1));
	for (int i = 0; i < 15 ;i++)
		ptr4[i] = 'K';
	ptr4[15] = '\0';
	display_line("REFILL", '-');
	display_line(NULL, '-');
	printf("ptr1 = %p\n", ptr1);
	printf("ptr4 = %p\n", ptr4);
	printf("ptr3 = %p\n", ptr3);
	display_line(NULL, '-');
	printf("1: %s\n2: %s\n3: %s\n", ptr1, ptr4, ptr3);
	show_alloc_mem();
}

int main(int argc, char **argv)
{
	// if (argc < 2)
	// 	return (1);
	// int max = atoi(argv[1]);
	// int len = atoi(argv[2]);
	// 	basic_test(max, len);
	replace_test();

	free_block();
	return (0);
}
