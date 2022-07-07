//#include "opt.c"
#include "hnd.c"

#include <stdlib.h>
#include <string.h>

#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>
#include <getopt.h>


struct opt {
        bool             error;
        bool             help;
        bool             version;
        cy_utf8_t       *timezone;
        cy_utf8_t       *filter;
        cy_utf8_t       *sortcol;
        cy_utf8_t       *sortdir;
        cy_utf8_t       *rowstart;
        cy_utf8_t       *rowcount;
};


static struct opt *opt_new(int argc, char *argv[])
{
        struct opt *ctx = cy_hptr_new(sizeof *ctx);

        const char *opts = "t:f:c:d:s:n:hv";
        const struct option opt[] = {
                {"timezone", required_argument, NULL, 't'},
                {"filter",   required_argument, NULL, 'f'},
                {"sortcol",  required_argument, NULL, 'c'},
                {"sortdir",  required_argument, NULL, 'd'},
                {"rowstart", required_argument, NULL, 's'},
                {"rowcount", required_argument, NULL, 'n'},
                {"help",     no_argument,       NULL, 'h'},
                {"version",  no_argument,       NULL, 'v'},
                { 0 }
        };

        register int o;
        while ((o = getopt_long(argc, argv, opts, opt, NULL)) != -1) {
                switch (o) {
                case 't':
                        ctx->timezone = cy_utf8_new(optarg);
                        break;

                case 'f':
                        ctx->filter = cy_utf8_new(optarg);
                        break;

                case 'c':
                        ctx->sortcol = cy_utf8_new(optarg);
                        break;

                case 'd':
                        ctx->sortdir = cy_utf8_new(optarg);
                        break;

                case 's':
                        ctx->rowstart = cy_utf8_new(optarg);
                        break;

                case 'n':
                        ctx->rowcount = cy_utf8_new(optarg);
                        break;

                case 'h':
                        ctx->help = true;
                        break;

                case 'v':
                        ctx->version = true;
                        break;

                case '?':
                default:
                        ctx->error = true;
                        break;
                }

                argc -= optind;
                argv += optind;
        }

        return ctx;
}

static void opt_free(struct opt **ctx)
{
        struct opt *o;

        if (CY_LIKELY(ctx && (o = *ctx))) {
                cy_utf8_free(&o->timezone);
                cy_utf8_free(&o->filter);
                cy_utf8_free(&o->sortcol);
                cy_utf8_free(&o->sortdir);
                cy_utf8_free(&o->rowstart);
                cy_utf8_free(&o->rowcount);
        }

        cy_hptr_free((cy_hptr_t **) ctx);
}


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
