#include "opt.c"
#include "rule.c"


int main(int argc, char *argv[])
{
        struct opt *o = opt_new(argc, argv);
        int rc = rule_exec(o, argc, argv);
        opt_free(&o);

        return rc;
}
