// #include


char *ft_strcpy(char * dst, const char * src)
{
    char *srcp = (char *)src;
    char *dstp = dst;
    while(*srcp)
    {   
        *dstp++ = *srcp++;
    }
    *dstp = 0;
    return dst;
}


#ifdef TEST
#include <string.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
    char dst[100] = "Hello Nice to meet you!!!\n";
    char *src = "Welcome to my channell ^^!!! copy done\n";
    ft_strcpy(dst,src);

    #define ft_strcpy strcpy
    ft_strcpy(dst,src);
    printf("%s",dst);
    return 0;
}
#endif
