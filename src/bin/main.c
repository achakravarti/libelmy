#include "all.c"
#include "facility.c"
#include "hostname.c"
#include "message.c"
#include "meta.c"
#include "misc.c"
#include "opt.c"
#include "severity.c"
#include "tag.c"

#include <stdlib.h>


int main(int argc, char **argv)
{
        opt_parse(argc, argv);

        return EXIT_SUCCESS;
}
