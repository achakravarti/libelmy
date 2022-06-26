#include <stdio.h>
#include <stdlib.h>


static int all(void);
static int all_paged(void);


int
all(void)
{
        printf("-a (--all) handled\n");
        return EXIT_SUCCESS;
}


int
all_paged(void)
{
        printf("-a -p (--all --paged) handled\n");
        return EXIT_SUCCESS;
}
