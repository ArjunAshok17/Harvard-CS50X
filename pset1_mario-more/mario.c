#include <cs50.h>
#include <stdio.h>

void spaces(int n);
void bricks(int n);

int main(void)
{
    // get valid height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // print the bricks
    for (int i = 0; i < height; i++)
    {
        // print spaces before bricks
        spaces(height - i - 1);

        // print left set of bricks
        bricks(i + 1);

        // print space between bricks
        printf("  ");

        // print right set of bricks
        bricks(i + 1);

        // next line
        printf("\n");
    }
}

void spaces(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}

void bricks(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}