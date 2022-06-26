#include <stdio.h>
#include <stdlib.h>


static int hostname(const char *);
static int hostname_paged(const char *);


int hostname(const char *arg)
{
        printf("hostname = %s\n", arg);
        return EXIT_SUCCESS;
}


int hostname_paged(const char *arg)
{
        printf("hostname = %s\n", arg);
        return EXIT_SUCCESS;
}
