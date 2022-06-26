#include <stdio.h>
#include <stdlib.h>


static int tag(void);
static int tag_paged(void);


int tag(void)
{
        printf("-t (--tag) handled\n");
        return EXIT_SUCCESS;
}


int tag_paged(void)
{
        printf("-t -p (--tag --paged) handled\n");
        return EXIT_SUCCESS;
}
