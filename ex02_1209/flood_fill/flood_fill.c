typedef struct  s_point
{
int           x;
int           y;
}               t_point;


void recursion(char **tab, t_point size, int x, int y, char fill)
{
    if (x < 0 || x >= size.x)
        return;
    if (y < 0 || y >= size.y) 
        return;
    if (tab[y][x] == fill)
    {
       tab[y][x] = 'F';
       recursion(tab,size,x+1,y,fill);
       recursion(tab,size,x-1,y,fill);
       recursion(tab,size,x,y+1,fill);
       recursion(tab,size,x,y-1,fill);
    }

}

void  flood_fill(char **tab, t_point size, t_point begin)
{
    char fill = tab[begin.y][begin.x];
    recursion(tab,size,begin.x,begin.y,fill); 
}
