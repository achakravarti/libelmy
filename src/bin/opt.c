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
        }
}
