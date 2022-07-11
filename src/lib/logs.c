#include "../../include/logs.h"

#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>
#include <libpq-fe.h>

#include <assert.h>
#include <inttypes.h>
#include <malloc.h>
#include <string.h>


struct elmy_logs__ {
        size_t        sz;
        size_t        len;
        elmy_log_t  **items;
};


elmy_logs_t *elmy_logs_new(size_t len)
{
        assert(len > 0);

        elmy_logs_t *ctx = cy_hptr_new(sizeof *ctx);
        ctx->len = len;
        ctx->items = cy_hptr_new(sizeof (elmy_log_t *) * len);

        return ctx;
}


elmy_logs_t *elmy_logs_new_empty(void)
{
        elmy_logs_t *ctx = cy_hptr_new(sizeof *ctx);
        return ctx;
}


elmy_logs_t *elmy_logs_new_parse__(void *res)
{
        assert(res != NULL);

        PGresult *r = (PGresult *) res;

        register size_t len = PQntuples(r);

        if (CY_UNLIKELY(!len))
                return elmy_logs_new_empty();

        elmy_logs_t *ctx = elmy_logs_new(len);
        elmy_log_t *log;

        for (register size_t i = 0; i < len; i++) {
                log = elmy_log_new(PQgetvalue(r, i, 1), PQgetvalue(r, i, 0),
                                   strtoumax(PQgetvalue(r, i, 2), NULL, 10),
                                   PQgetvalue(r, i, 3),
                                   strtoumax(PQgetvalue(r, i, 4), NULL, 10),
                                   PQgetvalue(r, i, 5), PQgetvalue(r, i, 6),
                                   PQgetvalue(r, i, 7), PQgetvalue(r, i, 8));

                ctx->items[i] = log;
                ctx->sz += elmy_log_sz(log);
        }

        return ctx;
}


elmy_logs_t *elmy_logs_copy(const elmy_logs_t *ctx)
{
        assert(ctx != NULL);

        return cy_hptr_copy((cy_hptr_t *) ctx);
}


elmy_logs_t *elmy_logs_clone(const elmy_logs_t *ctx)
{
        assert(ctx != NULL);

        elmy_logs_t *cp = elmy_logs_new(ctx->len);

        for (register size_t i = 0; i < ctx->len; i++)
                cp->items[i] = elmy_log_clone(ctx->items[i]);

        return cp;
}


void elmy_logs_t_free__(elmy_logs_t **ctx)
{
        elmy_logs_t *l;

        if (CY_LIKELY (ctx && (l = *ctx))) {
                if (cy_hptr_refc((cy_hptr_t *) l) == 1) {
                        for (register size_t i = 0; i < l->len; i++)
                                elmy_log_free(&l->items[i]);

                        cy_hptr_free((cy_hptr_t **) &l->items);
                }

                cy_hptr_free((cy_hptr_t **) ctx);
        }
}


size_t elmy_logs_len(const elmy_logs_t *ctx)
{
        assert(ctx != NULL);

        return ctx->len;
}


const elmy_log_t *elmy_logs_get(const elmy_logs_t *ctx, size_t idx)
{
        assert(ctx != NULL);
        assert(idx < ctx->len);
        assert(ctx->items[idx] != NULL);

        return ctx->items[idx];
}


void elmy_logs_set(elmy_logs_t *ctx, size_t idx, const elmy_log_t *val)
{
        assert(ctx != NULL);
        assert(val != NULL);

        if (CY_LIKELY(ctx->items[idx])) {
                ctx->sz -= elmy_log_sz(ctx->items[idx]);
                elmy_log_free(&ctx->items[idx]);
        }

        ctx->items[idx] = elmy_log_copy(val);
        ctx->sz += elmy_log_sz(val);
}


cy_utf8_t *elmy_logs_str(const elmy_logs_t *ctx)
{
        assert(ctx != NULL);

        if (CY_UNLIKELY(!elmy_logs_len(ctx)))
                return cy_utf8_new_empty();

        char *bfr = cy_hptr_new(ctx->sz * 2); /* approximation */
        register char *b = bfr;

        for (register size_t i = 0; i < ctx->len; i++) {
                CY_AUTO(cy_utf8_t) *s = elmy_log_str(ctx->items[i]);
                size_t len = strlen(s);

                memcpy(b, s, len);
                b += len;
                memcpy(b++, "\n", 1);
        }

        *b = '\0';
        return bfr;
}


cy_json_t *elmy_logs_json(const elmy_logs_t *ctx)
{
        assert(ctx != NULL);

        if (CY_UNLIKELY(!elmy_logs_len(ctx)))
                return cy_json_new("{\"logs\":null}");

        char *bfr = cy_hptr_new(ctx->sz * 2); /* approximation */
        register char *b = bfr;

        const char *hdr = "{\"logs\":[";
        size_t len = strlen(hdr);
        strncpy(bfr, hdr, len);
        b += len;


        for (register size_t i = 0; i < ctx->len; i++) {
                CY_AUTO(cy_json_t) *j = elmy_log_json(ctx->items[i]);
                CY_AUTO(cy_utf8_t) *s = cy_json_print(j, false);

                len = strlen(s);
                memcpy(b, s, len);

                if (i < ctx->len - 1) {
                        memcpy(b + len, ",", 1);
                        b += (len + 1);
                } else {
                        b += len;
                }
        }

        memcpy(b, "]}", 2);
        cy_json_t *j = cy_json_new(bfr);
        cy_hptr_free((cy_hptr_t **) &bfr);

        return j;
}
