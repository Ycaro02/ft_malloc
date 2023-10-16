#include "../malloc.h"
#include <stdio.h>
#define MAX 10000

t_data *g_data = NULL;

// e_bool check_type(e_type type, e_type data_type)
// { type == data_type ? return (TRUE) : return (FALSE); }

int main(void)
{
	t_block *ptr = init_data(TINY, sizeof(char) * 4);

	init_data(SMALL, 2000);
	char *test = (char *)init_data(TINY, 32) + BLOCK_SIZE;
	for (int i = 0; i < 31; i++)
		test[i] = '2';
	test[31] = '\0';
	printf("test = %s\n", test);

	char *test1 = (char *)t_malloc(32);
	for (int i = 0; i < 31; i++)
		test[i] = '3';
	test[31] = '\0';
	printf("test1 = %s\n", test);

	char *lol = (char *)(ptr + sizeof(t_block));
	lol[0] = 'a';
	lol[1] = 'l';
	lol[2] = 'o';
	lol[3] = '\0';
	printf("block size = %zu, ptr = %p\n", ptr->size, ptr);
	printf("lol ptr = %p, *lol = %s\n", lol, lol);
	// init_data(LARGE, 4000);
	show_alloc_mem();
	free_block();

	// print_define();

	// printf("test = %d\n",  0xA004A - 0xA0020);
	// char *str = malloc(sizeof(char) * 4);
	// str[0] = 'a';
	// str[1] = 'b';
	// str[2] = 'c';
	// str[3] = '\0';
	// printf("addr ptr = %p end at %p\n", str, str + 3);
	// printf("addr ptr = %c\n", *(str +2));
	// printf("page size %d", PAGE_SIZE);
	return (0);
}
