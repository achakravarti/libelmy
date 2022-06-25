#include <stdio.h>


static void meta_count(void);
static void meta_initial(void);
static void meta_last(void);


void meta_count(void)
{
        printf("-c (--count) handled\n");
}


void meta_initial(void)
{
        printf("-i (--initial) handled\n");
}


void meta_last(void)
{
        printf("-l (--last) handled\n");
}
