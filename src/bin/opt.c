#include "all.c"
#include "facility.c"
#include "hostname.c"
#include "message.c"
#include "meta.c"
#include "misc.c"
#include "severity.c"
#include "tag.c"


#include <getopt.h>
#include <stdlib.h>


void opt_parse(int argc, char **argv)
{
        struct option opts[] = {
                {"help", no_argument, NULL, 'h'},
                {"version", no_argument, NULL, 'v'},
                { 0 }
        };

        register int o;

        while (1) {
                o = getopt_long(argc, argv, "hv", opts, 0);

                if (o == -1)
                        break;

                switch (o) {
                case 'h':
                        misc_help();
                        break;

                case 'v':
                        misc_version();
                        break;

                case '?':
                        misc_help();
                        break;

                default:
                        break;
                }
        }
}
