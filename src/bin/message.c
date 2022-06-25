#include <stdio.h>


static void message(void);
static void message_paged(void);


void
message(void)
{
        printf("-m (--message) handled\n");
}


void
message_paged(void)
{
        printf("-m -p (--message --paged) handled\n");
}
