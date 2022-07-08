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


static void proc_usage(void)
{
        printf("Usage: elmy [-c,--count] [-i,--initial] [-l,--last]\n"
                "\t[-a,--all [-p,--paged]] [-f,--facility csv [-p,--paged]]\n"
                "\t\t[-n,--hostname string [-p,--paged]]"
                " [-s,--severity csv [-p,--paged]]\n"
                "\t\t[-m,--message string [-p,--paged]]"
                " [-t,--tag string [-p,--paged]]\n"
                "\t[-h,--help] [-v,--version]\n");
}


static int proc_error(const struct opt *o, int argc, char *argv[])
{
        if (argc == 1) {
                fprintf(stderr, "%s: missing argument or option\n", argv[0]);
                proc_usage();
                return EXIT_FAILURE;
        }

        if (o->error) {
                fprintf(stderr, "%s: invalid argument or option(s)\n", argv[0]);
                proc_usage();
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}


static int proc_help(const struct opt *o, int argc, char *argv[])
{
        if (o->help) {
                if (argc > 2) {
                        fprintf(stderr, "%s: invalid argument or option(s)\n",
                                argv[0]);
                        proc_usage();
                        return EXIT_FAILURE;
                }

                proc_usage();
        }

        return EXIT_SUCCESS;
}


static int proc_version(const struct opt *o, int argc, char *argv[])
{
        if (o->version) {
                if (argc > 2) {
                        fprintf(stderr, "%s: invalid argument or option(s)\n",
                                argv[0]);
                        proc_usage();
                        return EXIT_FAILURE;
                }

                printf("(lib)elmy 0.0.1 -- easy log monitoring\n"
                       "Copyright (c) 2022 Abhishek Chakravarti\n"
                       "Released under the BSD 2-Clause License\n");
        }

        return EXIT_SUCCESS;
}


static int proc_count(const struct opt *o, int argc, char *argv[])
{
        if (!strcmp(argv[argc - 1], "count")) {
                if (argc > 2) {
                        fprintf(stderr, "%s: invalid argument or option(s)\n",
                                argv[0]);
                        proc_usage();
                        return EXIT_FAILURE;
                }

                size_t res;
                CY_AUTO(elmy_error_t) *err = NULL;

                if (CY_UNLIKELY(elmy_rule_count(&res, &err))) {
                        elmy_error_str(err);
                        return elmy_error_status(err);
                }

                printf("%zu\n", res);
        }

        return EXIT_SUCCESS;
}


static int run_rule(const struct opt *o, int argc, char *argv[])
{
        if (proc_error(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_help(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_version(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_count(o, argc, argv))
                return EXIT_FAILURE;

        /*const char *rule = argv[argc - 1];

        if (!strcmp(rule, "count"))
                return hnd_count();

        if (!strcmp(rule, "initial"))
                return hnd_initial();

        if (!strcmp(rule, "last"))
                return hnd_last();

                return EXIT_FAILURE;*/

        return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
        //int rc = opt_proc(argc, argv);
        struct opt *o = opt_new(argc, argv);

        int rc = run_rule(o, argc, argv);
        opt_free(&o);

        return rc;
}
