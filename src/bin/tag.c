#include <stdio.h>
#include <stdlib.h>


static int tag(const char *);
static int tag_paged(const char *);


int tag(const char *arg)
{
        printf("tag = %s\n", arg);
        return EXIT_SUCCESS;
}


int tag_paged(const char *arg)
{
        printf("tag = %s\n", arg);
        return EXIT_SUCCESS;
}
