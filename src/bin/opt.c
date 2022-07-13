#include "opt.h"

#include <libchrysalid/include/hptr.h>

#include <getopt.h>


struct bin_opt *bin_opt_new(int argc, char *argv[])
{
        struct bin_opt *ctx = cy_hptr_new(sizeof *ctx);

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


void bin_opt_free(struct bin_opt **ctx)
{
        struct bin_opt *o;

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


elmy_page_t *bin_opt_page(const struct bin_opt *o)
{
        return CY_UNLIKELY(o->unpaged)
            ? elmy_page_new_disabled()
            : elmy_page_new_parse(
                o->rowstart, o->rowcount, o->sortcol, o->sortdir);
}
