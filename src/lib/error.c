#include "../../include/error.h"

#include <libchrysalid/include/hptr.h>

#include <assert.h>
#include <libchrysalid/include/utf8.h>


void elmy_error_dbconn(enum elmy_error erno, const char *msg)
{
        fprintf(stderr, "Error %d: Failed to connect to database: %s\n",
                erno, msg);
        cy_log_err("Error %d: Failed to connect to database: %s\n", erno, msg);
        exit(EXIT_FAILURE);
}

void elmy_error_dbqry(enum elmy_error erno, const char *msg)
{
        fprintf(stderr, "Error %d: Failed to execute query: %s\n", erno, msg);
        cy_log_err("Error %d: Failed to execute query: %s\n", erno, msg);
        exit(EXIT_FAILURE);
}


struct elmy_error__ {
        enum elmy_status     status;
        cy_utf8_t           *rule;
        cy_utf8_t           *msg;
};


elmy_error_t *elmy_error_new(enum elmy_status status, const char *rule,
                             const char *msg)
{
        assert(rule != NULL && *rule != '\0');
        assert(msg != NULL && *msg != '\0');

        elmy_error_t *ctx = cy_hptr_new(sizeof *ctx);
        ctx->status = status;
        ctx->rule = cy_utf8_new(rule);
        ctx->msg = cy_utf8_new(msg);

        return ctx;
}


elmy_error_t *elmy_error_copy(const elmy_error_t *ctx)
{
        assert(ctx != NULL);

        return cy_hptr_copy((cy_hptr_t *) ctx);
}


elmy_error_t *elmy_error_clone(const elmy_error_t *ctx)
{
        assert(ctx != NULL);

        return elmy_error_new(ctx->status, ctx->rule, ctx->msg);
}


void elmy_error_t_free__(elmy_error_t **ctx)
{
        elmy_error_t *e;

        if (CY_LIKELY(ctx && (e = *ctx))) {
                if (cy_hptr_refc((cy_hptr_t *) e) == 1) {
                        cy_utf8_free(&e->rule);
                        cy_utf8_free(&e->msg);
                }

                cy_hptr_free((cy_hptr_t **) ctx);
        }
}


enum elmy_status elmy_error_status(const elmy_error_t *ctx)
{
        assert(ctx != NULL);

        return ctx->status;
}


const cy_utf8_t *elmy_error_rule(const elmy_error_t *ctx)
{
        assert(ctx != NULL);

        return ctx->rule;
}


const cy_utf8_t *elmy_error_msg(const elmy_error_t *ctx)
{
        assert(ctx != NULL);

        return ctx->msg;
}


cy_utf8_t *elmy_error_str(const elmy_error_t *ctx)
{
        assert(ctx != NULL);

        return cy_utf8_new_fmt("Status: %zu\nRule: %s\nMessage: %s",
                               ctx->status, ctx->rule, ctx->msg);
}


cy_json_t *elmy_error_json(const elmy_error_t *ctx)
{
        assert(ctx != NULL);

        const char *fmt = "{\"status\":%zu,\"rule\":%s,\"msg\":%s}";
        CY_AUTO(cy_utf8_t) *s = cy_utf8_new_fmt(fmt, ctx->status, ctx->rule,
                                                ctx->msg);

        return cy_json_new(s);
}
