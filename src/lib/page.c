#include "../../include/page.h"

#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>


struct elmy_page__ {
        cy_utf8_t   *start;
        cy_utf8_t   *count;
        cy_utf8_t   *col;
        cy_utf8_t   *dir;
};


static cy_utf8_t *size_str(size_t val)
{
        size_t sz = snprintf(NULL, 0, "%zu", val) + 1;
        char *bfr = cy_hptr_new(sz);
        snprintf(bfr, sz, "%zu", val);
        bfr[sz] = '\0';

        return bfr;
}


static cy_utf8_t *col_str(enum elmy_sort sort)
{
        switch (sort) {
        case ELMY_SORT_TS:
                return cy_utf8_new("receivedat");
                break;

        case ELMY_SORT_FACILITY:
                return cy_utf8_new("facility");
                break;

        case ELMY_SORT_SEVERITY:
                return cy_utf8_new("priority");
                break;

        case ELMY_SORT_HOSTNAME:
                return cy_utf8_new("fromhost");
                break;

        case ELMY_SORT_TAG:
                return cy_utf8_new("syslogtag");
                break;

        case ELMY_SORT_MESSAGE:
                return cy_utf8_new("message");
                break;

        case ELMY_SORT_TS_EVENT:
        default:
                return cy_utf8_new("devicereportedtime");
                break;
        }
}


static cy_utf8_t *parse_size(const char *src, const char *def)
{
        if (CY_UNLIKELY(!*src|| *src == '0'
                        || strspn(src, "0123456789") != strlen(src)))
                return cy_utf8_new(def);

        return cy_utf8_new(src);
}


static cy_utf8_t *parse_dir(const char *src)
{
        if (CY_UNLIKELY(!*src || strcmp(src, "asc")))
                return cy_utf8_new("desc");

        return cy_utf8_new("asc");
}


static cy_utf8_t *parse_col(const char *src)
{
        if (CY_UNLIKELY(!*src))
                return cy_utf8_new("receivedat");

        if (!strcmp(src, "facility"))
                return cy_utf8_new("facility");

        if (!strcmp(src, "severity"))
                return cy_utf8_new("priority");

        if (!strcmp(src, "hostname"))
                return cy_utf8_new("fromhost");

        if (!strcmp(src, "tag"))
                return cy_utf8_new("syslogtag");

        if (!strcmp(src, "message"))
                return cy_utf8_new("message");

        if (!strcmp(src, "tslog"))
                return cy_utf8_new("receivedat");

        return cy_utf8_new("devicereportedtime");
}



elmy_page_t *elmy_page_new(size_t start, size_t count, enum elmy_sort col,
                           bool asc)
{
        assert(start > 0);
        assert(count > 0);

        elmy_page_t *ctx = cy_hptr_new(sizeof *ctx);
        ctx->start = size_str(start);
        ctx->count = size_str(count);
        ctx->col = col_str(col);
        ctx->dir = cy_utf8_new(asc ? "asc" : "desc");

        return ctx;
}


elmy_page_t *elmy_page_new_disabled(void)
{
        elmy_page_t *ctx = cy_hptr_new(sizeof *ctx);
        ctx->start = cy_utf8_new_empty();
        ctx->count = cy_utf8_new_empty();
        ctx->col = cy_utf8_new_empty();
        ctx->dir = cy_utf8_new_empty();

        return ctx;
}


elmy_page_t *elmy_page_new_parse(const char *start, const char *count,
                                 const char *col, const char *dir)
{
        assert(start != NULL);
        assert(count != NULL);
        assert(col != NULL);
        assert(dir != NULL);

        elmy_page_t *ctx = cy_hptr_new(sizeof *ctx);
        ctx->start = parse_size(start, "1");
        ctx->count = parse_size(count, "25");
        ctx->col = parse_col(col);
        ctx->dir = parse_dir(dir);

        return ctx;
}


elmy_page_t *elmy_page_copy(const elmy_page_t *ctx)
{
        assert(ctx != NULL);

        return cy_hptr_copy((cy_hptr_t *) ctx);
}


elmy_page_t *elmy_page_clone(const elmy_page_t *ctx)
{
        assert(ctx != NULL);

        elmy_page_t *cp = cy_hptr_new(sizeof *ctx);
        cp->start = cy_utf8_clone(ctx->start);
        cp->count = cy_utf8_clone(ctx->count);
        cp->col = cy_utf8_clone(ctx->col);
        cp->dir = cy_utf8_clone(ctx->dir);

        return cp;
}


void elmy_page_t_free__(elmy_page_t **ctx)
{
        elmy_page_t *p;

        if (CY_LIKELY(ctx && (p = *ctx))) {
                if (cy_hptr_refc((cy_hptr_t *) p) == 1) {
                        cy_utf8_free(&p->start);
                        cy_utf8_free(&p->count);
                        cy_utf8_free(&p->col);
                        cy_utf8_free(&p->dir);
                }

                cy_hptr_free((cy_hptr_t **) ctx);
        }
}


bool elmy_page_disabled(const elmy_page_t *ctx)
{
        assert(ctx != NULL);

        return cy_utf8_empty(ctx->count);
}


const char *elmy_page_start(const elmy_page_t *ctx)
{
        assert(ctx != NULL);

        return ctx->start;
}


const char *elmy_page_count(const elmy_page_t *ctx)
{
        assert(ctx != NULL);

        return ctx->count;
}


const char *elmy_page_col(const elmy_page_t *ctx)
{
        assert(ctx != NULL);

        return ctx->col;
}


const char *elmy_page_dir(const elmy_page_t *ctx)
{
        assert(ctx != NULL);

        return ctx->dir;
}
