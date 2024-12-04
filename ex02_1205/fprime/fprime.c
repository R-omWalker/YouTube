#include <stdio.h>
#include <stdlib.h>




int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        //do
        int num = atoi(argv[1]);
        int div = 2;
        if (! (num < 1))
        {
            while (num!=1)
            {
                while(num % div == 0)
                {
                    num /= div;
                    printf("%d",div);
                    if (num!=1)
                        printf("*");
                }
                div++;
            }
        }
    }
    printf("\n");
    return 0;
}
