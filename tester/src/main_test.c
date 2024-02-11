#include "../../include/malloc.h"
#define MAX 10000

// t_page *g_data = NULL;

// void basic_test(int max, int len)
// {
// 	for (int j = 0; j < max; j++)
// 	{
// 		char *test = (char *)malloc(len + 1);
// 		for (int i = 0; i < len; i++)
// 			test[i] = '3';
// 		test[len] = '\0';
// 	}
// }

// void free_test()
// {
// 	char *test[10];
// 	char *large = malloc(4000);
// 	for (int j = 0; j < 4000; j++)
// 		large[j] = 'b';
// 	for (int i = 0; i < 6; i++)
// 	{
// 		test[i] = malloc(50 * i + 50);
// 		for (int j = 0; j < 100; j++)
// 			test[i][j] = 'o';
// 	}
// 	display_line(NULL, '-');
// 	display_line("before free", '-');
// 	show_alloc_mem();
// 	free(test[0]);
// 	free(test[1]);
// 	display_line("after free test[0/1] ", '-');
// 	show_alloc_mem();
// 	for (int i = 2; i < 6; i++)
// 		free(test[i]);
// 	free(large);
// 	display_line("after free all", '-');
// 	show_alloc_mem();

// }

void realloc_test()
{
	// char *b = malloc(3000);
	// b[0] = 2;
	char *a = malloc(2);
	ft_printf_fd(1, "After 2 first malloc\n");
	a[0] = 'a';
	show_alloc_mem();
	int len = 100;
	char *test = NULL;
	test = realloc(test, len);
	if (!test) {
		ft_printf_fd(1, "Realloc1 return NULL\n");
		show_alloc_mem();
		free_meta_data();
		return ;
	}
	for (int i = 0; i < len; i++)
		test[i] = 'k';
	ft_printf_fd(1, "After first realloc\n");
	show_alloc_mem();
	len = 900;
	test = realloc(test, len);
	if (!test) {
		ft_printf_fd(1, "Realloc2 return NULL\n");
		// show_alloc_mem();
		free_meta_data();
		show_alloc_mem();
		return ;
	}
	ft_printf_fd(1, "Realloc ok\n");
	// for (int i = 0; i < len; i++)
	// 	test[i] = 'l';
	// len = 3000;
	// show_alloc_mem();
	// test = realloc(test, len);
	// for (int i = 0; i < len; i++)
	// 	test[i] = 'z';
	// show_alloc_mem();
}

int main(void)
{
	realloc_test();
	return (0);
}
