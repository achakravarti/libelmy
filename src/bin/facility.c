#include <stdio.h>
#include <stdlib.h>


static int facility(const char *);
static int facility_paged(const char *);


int facility(const char *arg)
{
        printf("facility = %s\n", arg);
        return EXIT_SUCCESS;
}


int facility_paged(const char *arg)
{
        printf("facility = %s\n", arg);
        return EXIT_SUCCESS;
}
