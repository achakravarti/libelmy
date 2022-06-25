#include <stdio.h>


static void severity(void);
static void severity_paged(void);


void
severity(void)
{
        printf("-s (--severity) handled\n");
}


void
severity_paged(void)
{
        printf("-s -p (--severity --paged) handled\n");
}
