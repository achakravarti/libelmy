#include <stdio.h>
#include <stdlib.h>


static int severity(void);
static int severity_paged(void);


int severity(void)
{
        printf("-s (--severity) handled\n");
}


int severity_paged(void)
{
        printf("-s -p (--severity --paged) handled\n");
}
