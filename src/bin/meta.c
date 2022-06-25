#include <stdio.h>
#include <stdlib.h>


static int meta_count(void);
static int meta_initial(void);
static int meta_last(void);


int meta_count(void)
{
        printf("-c (--count) handled\n");
        return EXIT_SUCCESS;
}


int meta_initial(void)
{
        printf("-i (--initial) handled\n");
        return EXIT_SUCCESS;
}


int meta_last(void)
{
        printf("-l (--last) handled\n");
        return EXIT_SUCCESS;
}
