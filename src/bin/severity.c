#include <stdio.h>
#include <stdlib.h>


static int severity(const char *);
static int severity_paged(const char *);


int severity(const char *arg)
{
        printf("severity = %s\n", arg);
        return EXIT_SUCCESS;
}


int severity_paged(const char *arg)
{
        printf("severity = %s\n", arg);
        return EXIT_SUCCESS;
}
