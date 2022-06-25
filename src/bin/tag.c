#include <stdio.h>


static void tag(void);
static void tag_paged(void);


void
tag(void)
{
        printf("-t (--tag) handled\n");
}


void
tag_paged(void)
{
        printf("-t -p (--tag --paged) handled\n");
}
