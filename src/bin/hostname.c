#include <stdio.h>


static void hostname(void);
static void hostname_paged(void);


void
hostname(void)
{
        printf("-n (--hostname) handled\n");
}


void
hostname_paged(void)
{
        printf("-n -p (--hostname --paged) handled\n");
}
