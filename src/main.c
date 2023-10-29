#include "../include/malloc.h"
#include <stdio.h>
#define MAX 10000

// t_data *g_data = NULL;

void basic_test(int max, int len)
{
	for (int j = 0; j < max; j++)
	{
		char *test = (char *)malloc(len + 1);
		for (int i = 0; i < len; i++)
			test[i] = '3';
		test[len] = '\0';
	}
}

void tesrealloc_large_only()
{
	char *test = NULL;
	for (int i = 1; i < 10; i ++)
	{
		int j = 0;
		int len = 4096 * i;
		test = realloc(test, len);
		while (j < len)
		{
			test[j] = 'b';
			j++;
		}
		i++;
		display_line(NULL, '-');
		display_line(NULL, '-');
		ft_printf_fd(1, "second test = %d\n", len);
    	show_alloc_mem();
	}
}

void realloc_test(int max)
{
	char *coucou = NULL;
	char *test = NULL;
	for ( int i = 1; i < max; i++)
	{
		if (i == 1)
		{
			coucou = malloc(10 + i);
			for (int k =0; k < 10 + i; k++)
				coucou[k] = '3';
		}
		if (i == max - 1)
			free(coucou);
		int power = i * 3000;
		test = realloc(test, power);
		int j = 0;
		while (j < power)
		{
    		test[j] ='a';
			j++;
		}
		test[j] = '\0';
		display_line(NULL, '-');
		display_line(NULL, '-');
		ft_printf_fd(1, "test R = %d\n", power);
		if (i == 2)
			basic_test(3, 200);
    	show_alloc_mem();
	}
 }

void replace_test(void)
{
	char *ptr1 = malloc(sizeof(char) * (12 + 1));
	char *ptr2 = malloc(sizeof(char) * (13 + 1));
	char *ptr3 = malloc(sizeof(char) * (14 + 1));
	display_line(NULL, '-');
	ft_printf_fd(1, "ptr1 = %p\n", ptr1);
	ft_printf_fd(1, "ptr2 = %p\n", ptr2);
	ft_printf_fd(1, "ptr3 = %p\n", ptr3);
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
	ft_printf_fd(1, "1: %s\n2: %s\n3: %s\n", ptr1, ptr2, ptr3);
	free(ptr2);
	display_line(NULL, '-');
	display_line("free", '-');
	show_alloc_mem();
	char* ptr4 = malloc(sizeof(char) * (15 + 1));
	for (int i = 0; i < 15 ;i++)
		ptr4[i] = 'K';
	ptr4[15] = '\0';
	display_line("REFILL", '-');
	display_line(NULL, '-');
	ft_printf_fd(1, "ptr1 = %p\n", ptr1);
	ft_printf_fd(1, "ptr4 = %p\n", ptr4);
	ft_printf_fd(1, "ptr3 = %p\n", ptr3);
	display_line(NULL, '-');
	ft_printf_fd(1, "1: %s\n2: %s\n3: %s\n", ptr1, ptr4, ptr3);
	show_alloc_mem();
}

void basic_realloc_test(int max)
{
	char *test = NULL;
	for ( int i = 1; i < max; i++)
	{
		int power = i;
		test = realloc(test, power);
		int j = 0;
		while (j < power)
		{
    		test[j] ='a';
			j++;
		}
		test[j] = '\0';
		// display_line(NULL, '-');
		// display_line(NULL, '-');
		// ft_printf_fd(1, "test R = %d\n", power);
    	// show_alloc_mem();
	}
 }


int main(void)
{
	basic_test(2, 2);
	show_alloc_mem();
	basic_realloc_test(50);
	show_alloc_mem();
	basic_test(2, 200);
	show_alloc_mem();
	basic_realloc_test(70);
	show_alloc_mem();
	// basic_realloc_test(100);
	// show_alloc_mem();

	// realloc_test(8);
	// display_line("After free meta data", '-');
	free_meta_data();
	return (0);
}
