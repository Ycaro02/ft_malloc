#include <stdio.h>
#include "libft/libft.h"

int xor_abs_value(int nb)
{
    int y = (nb >> 31);
    printf("y = 1 >> 31 = %d\n", y);
    // printf("reverse = %d\n", ~y);
    printf("nb ^ y = %d\n", nb ^ y);
    printf("nb ^ y = %d\n", (nb ^ y) - y);
    return ((nb ^ (nb >> 31)) - (nb >> 31));
}

int abs_value(int nb)
{
    int max = 1 << 31;
    int min = ~max;


    printf("compute  %d\n", (nb &= (min)));
    return (max);
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        return (1);
    }
    int nb = ft_atoi(argv[1]);
    printf("Abs value of %d is %d\n", nb, xor_abs_value(nb));
    abs_value(nb);
    return (0);
}