#include "cmd.h"
#include "opt.h"


int main(int argc, char *argv[])
{
        struct bin_opt *o = bin_opt_new(argc, argv);
        int rc = bin_cmd_exec(o, argc, argv);
        bin_opt_free(&o);

        return rc;
}
