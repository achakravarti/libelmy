#include "../../include/logs.h"

#include <assert.h>


struct elmy_logs__ {
        size_t        len;
        elmy_log_t  **items;
};


elmy_logs_t *elmy_logs_new(size_t len)
{
        assert(len > 0);
}


elmy_logs_t *elmy_logs_copy(const elmy_logs_t *ctx)
{
        assert(ctx != NULL);
}


elmy_logs_t *elmy_logs_clone(const elmy_logs_t *ctx)
{
        assert(ctx != NULL);
}


void elmy_logs_t_free__(elmy_logs_t **ctx)
{

}


const elmy_log_t *elmy_logs_get(const elmy_logs_t *ctx, size_t idx)
{
        assert(ctx != NULL);
        assert(ctx->items[idx] != NULL);
}


void elmy_log_set(elmy_logs_t *ctx, size_t idx, const elmy_log_t *val)
{
        assert(ctx != NULL);
}


cy_utf8_t *elmy_logs_print(const elmy_logs_t *ctx, enum elmy_logs_format fmt)
{
        assert(ctx != NULL);
}
