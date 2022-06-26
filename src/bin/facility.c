#include <stdio.h>
#include <stdlib.h>


static int facility(void);
static int facility_paged(void);


int facility(void)
{
        printf("-f (--facility) handled\n");
        return EXIT_SUCCESS;
}


int facility_paged(void)
{
        printf("-f -p (--facility --paged) handled\n");
        return EXIT_SUCCESS;
}
