#include "../../include/logs.h"

#include <libchrysalid/hptr.h>

#include <assert.h>
#include <libchrysalid/utf8.h>


struct elmy_logs__ {
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

                        cy_hptr_free((cy_hptr_t **) l->items);
                }

                cy_hptr_free((cy_hptr_t **) ctx);
        }
}


const elmy_log_t *elmy_logs_get(const elmy_logs_t *ctx, size_t idx)
{
        assert(ctx != NULL);
        assert(ctx->items[idx] != NULL);

        return ctx->items[idx];
}


void elmy_log_set(elmy_logs_t *ctx, size_t idx, const elmy_log_t *val)
{
        assert(ctx != NULL);
        assert(val != NULL);

        elmy_log_free(&ctx->items[idx]);
        ctx->items[idx] = elmy_log_copy(val);
}


cy_utf8_t *elmy_logs_print(const elmy_logs_t *ctx, enum elmy_logs_format fmt)
{
        assert(ctx != NULL);

        return cy_utf8_new_empty();
}
