#include "opt.c"

#include <stdlib.h>
#include <string.h>


static int run_rule(int argc, char *argv[])
{
        if (argc == 1)
                return EXIT_FAILURE;

        const char *rule = argv[argc - 1];

        if (!strcmp(rule, "count"))
                return hnd_count();

        if (!strcmp(rule, "initial"))
                return hnd_initial();

        if (!strcmp(rule, "last"))
                return hnd_last();

        return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
        //int rc = opt_proc(argc, argv);
        return run_rule(argc, argv);
}
