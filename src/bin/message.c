#include <stdio.h>
#include <stdlib.h>


static int message(const char *);
static int message_paged(const char *);


int message(const char *arg)
{
        printf("message = %s\n", arg);
        return EXIT_SUCCESS;
}


int message_paged(const char *arg)
{
        printf("message = %s\n", arg);
        return EXIT_SUCCESS;
}
