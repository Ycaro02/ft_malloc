#include "../../include/malloc.h"
#define MAX 10000

void free_test()
{
	char *test[10];
	char *large = malloc(4000);
	for (int j = 0; j < 4000; j++)
		large[j] = 'b';
	for (int i = 0; i < 6; i++)
	{
		test[i] = malloc(50 * i + 50);
		for (int j = 0; j < 100; j++)
			test[i][j] = 'o';
	}
	ft_printf_fd(1, YELLOW"Start free test\n"RESET);
	display_line(NULL, '-');
	show_alloc_mem();
	free(test[0]);
	free(test[1]);
	display_line("after free test[0]/[1] ", '-');
	show_alloc_mem();
	for (int i = 2; i < 6; i++)
		free(test[i]);
	free(large);
	display_line("after free all", '-');
	show_alloc_mem();
	ft_printf_fd(1, GREEN"Free test OK\n"RESET);
}


void basic_alloc_free_test()
{
	char *large = malloc(6000);
	for (int j = 0; j < 6000; j++)
		large[j] = 'b';
	
	char **tiny = malloc(sizeof(char *) * 5);
	for (int i = 0; i < 5; ++i) {
		tiny [i] = malloc(99);
		for (int j = 0; j < 99 ; ++j) {
			tiny[i][j] = 'P';
		}
	}
	
	char **small = malloc(sizeof(char *) * 5);
	for (int i = 0; i < 5; ++i) {
		small [i] = malloc(400);
		for (int j = 0; j < 400 ; ++j) {
			small[i][j] = 'L';
		}
	}

	ft_printf_fd(1, YELLOW"Start basic alloc free test \n"RESET);
	display_line(NULL, '-');
	display_line("initial data", '-');
	show_alloc_mem(); /* show initial data */
	display_line("Free tiny[0] and small [0]", '-');
	free(tiny[0]);
	free(small[0]);
	show_alloc_mem();
	display_line("Realloc it", '-');
	char *a = malloc(12);
	char *b = malloc(420);
	a[0] = 'c';
	b[0] = 'l';
	show_alloc_mem();
	free_meta_data();
	ft_printf_fd(1, GREEN"Basic alloc free test OK \n"RESET);
}

void alloc_free_test()
{
	basic_alloc_free_test();
	char *large = malloc(4000);
	char **tiny = malloc(10);
	for (int i = 0; i < 10; ++i) {
		tiny[i] = malloc(3);
		tiny[i][0] = 'k';
	}
	ft_printf_fd(1, YELLOW"Start alloc free test \n"RESET);
	display_line("New data", '-');
	show_alloc_mem();
	display_line("Free 3 elem", '-');
	free(tiny[1]);
	free(tiny[5]);
	free(tiny[8]);
	show_alloc_mem();
	display_line("Realloc it with 42 size", '-');
	tiny[1] = malloc(42);
	tiny[5] = malloc(42);
	tiny[8] = malloc(42);
	show_alloc_mem();
	free(large);
	show_alloc_mem();
	ft_printf_fd(1, GREEN"Alloc free test OK \n"RESET);
}

void realloc_test()
{
	// char *b = malloc(3000);
	// b[0] = 2;
	ft_printf_fd(1, YELLOW"Start Realloc test \n"RESET);
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
		test[i] = 'P';
	test[len - 1] = '\0';
	ft_printf_fd(1, "After first realloc\n");
	show_alloc_mem();
	len = 900;
	test = realloc(test, len);
	if (!test) {
		ft_printf_fd(1, "Realloc2 return NULL\n");
		free_meta_data();
		return ;
	}
	ft_printf_fd(1, GREEN"Realloc ok\n"RESET);
	free_meta_data();
}

void test_show_mem_hex()
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
	for (int i = 0; i < len - 1; i++)
		test[i] = 'P';
	ft_printf_fd(1, "After first realloc\n");
	show_alloc_mem();
	show_alloc_mem_hex();
	test = realloc(test, 500);
	for (int i = len-1; i < 599; i++) {
		test[i] = 2;
	}
	ft_printf_fd(1, "After second realloc\n");
	show_alloc_mem_hex();
	ft_printf_fd(1, GREEN"Show mem hex OK\n"RESET);
}



int main(void)
{
	print_define();
	alloc_free_test(); /* first */ 
	free_meta_data();
	realloc_test(); /* second */
	free_test();
	test_show_mem_hex(); /* third */
	free_meta_data();
	return (0);
}
