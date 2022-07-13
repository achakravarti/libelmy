#include "cmd.h"
#include "opt.h"


int main(int argc, char *argv[])
{
        struct opt *o = opt_new(argc, argv);
        int rc = bin_cmd_exec(o, argc, argv);
        opt_free(&o);

        return rc;
}
