#define BASEL "0123456789abcdef"
#define BASEU "0123456789ABCDEF"
#ifdef TEST
#include <stdio.h>
#endif
int next_digit(char ch,int base)
{
    int i = 0;
    while(i<base)
    {
    //printf("%s,%d\n",__func__,i);
        if (ch == BASEL[i] || ch == BASEU[i])
            return i;
        i++;
    }

    return -1;
}





int     ft_atoi_base(const char *str, int str_base)
{
    int ret = 0;
    int sign = 1;

    if (!str || !*str)
        return 0;
    if (*str == '-')
    {
        str++;
        sign = -1;
    }
    int next = next_digit(*str++,str_base);
    while(next>=0)
    {
        ret *= str_base;
        ret += next;
        next = next_digit(*str++,str_base);
    }
    return ret * sign;
}

#ifdef TEST

int main(int argc, char const *argv[])
{
printf("%d %d %d\n",ft_atoi_base("12345",10),ft_atoi_base("-9F8",16),ft_atoi_base("0",2));
    return 0;
}

#endif