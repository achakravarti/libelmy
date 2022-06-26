#include <stdio.h>
#include <stdlib.h>


static int hostname(void);
static int hostname_paged(void);


int hostname(void)
{
        printf("-n (--hostname) handled\n");
        return EXIT_SUCCESS;
}


int hostname_paged(void)
{
        printf("-n -p (--hostname --paged) handled\n");
        return EXIT_SUCCESS;
}
