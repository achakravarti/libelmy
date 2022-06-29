#include "../../include/log.h"

#include <libchrysalid/hptr.h>

#include <assert.h>


struct elmy_log__ {
        enum cy_log_facility    facility;
        enum cy_log_severity    severity;
        cy_utf8_t               *ts;
        cy_utf8_t               *evts;
        cy_utf8_t               *hostname;
        cy_utf8_t               *tag;
        cy_utf8_t               *message;
};


elmy_log_t *elmy_log_new(const char *ts, const char *evts,
                         enum cy_log_facility facility,
                         enum cy_log_severity severity, const char *hostname,
                         const char *tag, const char *message)
{
        assert(ts != NULL && *ts != '\0');
        assert(evts != NULL && *evts != '\0');
        assert(hostname != NULL && *hostname != '\0');
        assert(tag != NULL && *tag != '\0');

        elmy_log_t *ctx = cy_hptr_new(sizeof *ctx);
        ctx->facility = facility;
        ctx->severity = severity;
        ctx->ts = cy_utf8_new(ts);
        ctx->evts = cy_utf8_new(evts);
        ctx->hostname = cy_utf8_new(hostname);
        ctx->tag = cy_utf8_new(tag);
        ctx->message = cy_utf8_new(message);

        return ctx;
}


elmy_log_t *elmy_log_copy(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return cy_hptr_copy((cy_hptr_t *) ctx);
}


elmy_log_t *elmy_log_clone(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return elmy_log_new(ctx->ts, ctx->evts, ctx->facility, ctx->severity,
                            ctx->hostname, ctx->tag, ctx->message);
}


void elmy_log_t_free__(elmy_log_t **ctx)
{
        elmy_log_t *l;

        if (CY_LIKELY(ctx && (l = *ctx))) {
                if (cy_hptr_refc((const cy_hptr_t *) l) == 1) {
                        cy_utf8_free(&l->ts);
                        cy_utf8_free(&l->evts);
                        cy_utf8_free(&l->hostname);
                        cy_utf8_free(&l->tag);
                        cy_utf8_free(&l->message);
                }

                cy_hptr_free((cy_hptr_t **) ctx);
        }
}


const cy_utf8_t *elmy_log_ts(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->ts;
}


const cy_utf8_t *elmy_log_ts_event(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->evts;
}


enum cy_log_facility elmy_log_facility(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->facility;
}


enum cy_log_severity elmy_log_severity(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->severity;
}


const cy_utf8_t *elmy_log_hostname(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->hostname;
}


const cy_utf8_t *elmy_log_tag(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->tag;
}


const cy_utf8_t *elmy_log_message(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->message;
}


cy_utf8_t *elmy_log_print(const elmy_log_t *ctx, enum elmy_log_format fmt)
{
        assert(ctx != NULL);

        if (fmt == ELMY_LOG_FORMAT_CSV) {
                return cy_utf8_new_empty();
        }

        if (fmt == ELMY_LOG_FORMAT_CSV_HDR) {
                return cy_utf8_new_empty();
        }

        if (fmt == ELMY_LOG_FORMAT_JSON) {
                return cy_utf8_new_empty();
        }

        return cy_utf8_new_empty();
}
