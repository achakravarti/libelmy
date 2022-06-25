#include <stdio.h>


static void facility(void);
static void facility_paged(void);


void
facility(void)
{
        printf("-f (--facility) handled\n");
}


void
facility_paged(void)
{
        printf("-f -p (--facility --paged) handled\n");
}
