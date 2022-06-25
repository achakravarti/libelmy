#include <stdio.h>


static void all(void);
static void all_paged(void);


void
all(void)
{
        printf("-a (--all) handled\n");
}


void
all_paged(void)
{
        printf("-a -p (--all --paged) handled\n");
}
