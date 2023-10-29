#include <stdio.h>
#include <unistd.h>

int main(void)
{
	char *test = t_malloc(sizeof(char) * 3);
	test[0] = 'a';
	test[1] = 'b';
	test[2] = '\0';
	write(1, test, 2);
	

	char* test2 = t_malloc(sizeof(char) * 3);
	test2[0] = 'K';
	test2[1] = 'K';
	test2[2] = '\0';
	write(1, test2, 2);
	printf("first addr %p second %p, diff = %zu\n", test, test2, test - test2);
}
