#include <stdio.h>
#include <stdlib.h>


static int message(void);
static int message_paged(void);


int message(void)
{
        printf("-m (--message) handled\n");
        return EXIT_SUCCESS;
}


int message_paged(void)
{
        printf("-m -p (--message --paged) handled\n");
        return EXIT_SUCCESS;
}
