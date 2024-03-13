# include "include/malloc.h"

int main(void) {
	char *addr = malloc(12);

	addr[0] = 42;
	show_alloc_mem();
	return (0);
}