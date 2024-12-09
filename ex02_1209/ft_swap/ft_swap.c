void	ft_swap(int *a, int *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}


#ifdef TEST
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    int a = atoi(argv[1]);
    int b = atoi(argv[2]); 
    ft_swap(&a,&b);
    printf("%d %d\n",a,b);
    return 0;
}
#endif
