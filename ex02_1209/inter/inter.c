#include <unistd.h>
void build(int argc, char const *argv[])
{
    char *left = argv[1];
    char *right = argv[2];
    char *ptr = left;
    while(*ptr)
    {
        char *right_ptr = right;
        int flag = 1;
        char *ptr2 = left;
        while(ptr2<ptr)
        {
            if (*ptr2==*ptr)
            {
                flag = 0;
            }
            ptr2++;
        }
        if (flag == 0)
        {
            ptr++;
            continue;
        }
        while(*right_ptr)
        {
            if (*ptr == *right_ptr)
            {
                write(1,ptr,1);
                break;
            }
            right_ptr++;
        }
        ptr++;

    }

}

int main(int argc, char const *argv[])
{
    if (argc==3)
    {
        build(argc,argv);
    }
    write(1,"\n",1);
    return 0;
}
