#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>

#include <getopt.h>


struct opt {
        bool             error;
        bool             help;
        bool             json;
        bool             unpaged;
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

        ctx->timezone = cy_utf8_new_empty();
        ctx->filter = cy_utf8_new_empty();
        ctx->sortcol = cy_utf8_new_empty();
        ctx->sortdir = cy_utf8_new_empty();
        ctx->rowstart = cy_utf8_new_empty();
        ctx->rowcount = cy_utf8_new_empty();

        const char *opts = "t:f:c:d:s:n:hjvu";
        const struct option opt[] = {
                {"timezone", required_argument, NULL, 't'},
                {"filter",   required_argument, NULL, 'f'},
                {"sortcol",  required_argument, NULL, 'c'},
                {"sortdir",  required_argument, NULL, 'd'},
                {"rowstart", required_argument, NULL, 's'},
                {"rowcount", required_argument, NULL, 'n'},
                {"help",     no_argument,       NULL, 'h'},
                {"json",     no_argument,       NULL, 'j'},
                {"version",  no_argument,       NULL, 'v'},
                {"unpaged",  no_argument,       NULL, 'u'},
                { 0 }
        };

        register int o;
        while ((o = getopt_long(argc, argv, opts, opt, NULL)) != -1) {
                switch (o) {
                case 't':
                        cy_utf8_free(&ctx->timezone);
                        ctx->timezone = cy_utf8_new(optarg);
                        break;

                case 'f':
                        cy_utf8_free(&ctx->filter);
                        ctx->filter = cy_utf8_new(optarg);
                        break;

                case 'c':
                        cy_utf8_free(&ctx->sortcol);
                        ctx->sortcol = cy_utf8_new(optarg);
                        break;

                case 'd':
                        cy_utf8_free(&ctx->sortdir);
                        ctx->sortdir = cy_utf8_new(optarg);
                        break;

                case 's':
                        cy_utf8_free(&ctx->rowstart);
                        ctx->rowstart = cy_utf8_new(optarg);
                        break;

                case 'n':
                        cy_utf8_free(&ctx->rowcount);
                        ctx->rowcount = cy_utf8_new(optarg);
                        break;

                case 'h':
                        ctx->help = true;
                        break;

                case 'j':
                        ctx->json = true;
                        break;

                case 'v':
                        ctx->version = true;
                        break;

                case 'u':
                        ctx->unpaged = true;
                        break;

                case '?':
                        ctx->error = true;
                        break;
                }

                //argc -= optind;
                //argv += optind;
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
