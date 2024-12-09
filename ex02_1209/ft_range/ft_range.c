#include <stdlib.h>
#define abs(x) ((x>0)? (x):(-x))

// void	ft_swap(int *a, int *b)
// {
//     *a ^= *b;
//     *b ^= *a;
//     *a ^= *b;
// }

int     *ft_range(int start, int end)
{
    int len = end -start;
    int diff = 1;
    if (start > end)
    {
        diff = -1;
        len *= -1;
    }
    int *ret = malloc(sizeof(int) * len+1);
    int i = 0;
    ret[i] = start;
    while(i<len)
    {
        start += diff;
        ret[++i] = start;
    }
    return ret;
}
#ifdef TEST
#include <stdio.h>
int main(int argc, char const *argv[])
{
    int *ret = ft_range(6,2);
    for (size_t i = 0; i < 5; i++)
    {
        printf("%d\n",ret[i]);
    }
    
    return 0;
}
#endif