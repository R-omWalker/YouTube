#include <unistd.h>
#include <stdlib.h>

int isUpper(char ch)
{
    return ('A'<=ch && ch<='Z');
}



int main(int argc, char const *argv[])
{
    if (argc!=2)
    {
        write(1,"\n",1);
        return 0;
    }

    int i = 0;
    while(argv[1][i])
    {
        char c = argv[1][i];
        if (isUpper(c))
        {
            c ^= 32; //Up <-> Low
            write(1,"_",1);

        }
        write(1,&c,1);
        i++;
    }
    
    write(1,"\n",1);
    return 0;
}
