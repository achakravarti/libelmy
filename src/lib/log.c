#include "../../include/log.h"

#include <libchrysalid/include/hptr.h>

#include <assert.h>


struct elmy_log__ {
        enum cy_log_facility    facility;
        enum cy_log_severity    severity;
        cy_utf8_t               *facility_kw;
        cy_utf8_t               *severity_kw;
        cy_utf8_t               *ts;
        cy_utf8_t               *evts;
        cy_utf8_t               *hostname;
        cy_utf8_t               *tag;
        cy_utf8_t               *message;
};


elmy_log_t *elmy_log_new(const char *ts, const char *evts,
                         enum cy_log_facility facility, const char *facility_kw,
                         enum cy_log_severity severity, const char *severity_kw,
                         const char *hostname, const char *tag,
                         const char *message)
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
        ctx->facility_kw = cy_utf8_new(facility_kw);
        ctx->severity_kw = cy_utf8_new(severity_kw);
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

        return elmy_log_new(ctx->ts, ctx->evts, ctx->facility, ctx->facility_kw,
                            ctx->severity, ctx->severity_kw, ctx->hostname,
                            ctx->tag, ctx->message);
}


void elmy_log_t_free__(elmy_log_t **ctx)
{
        elmy_log_t *l;

        if (CY_LIKELY(ctx && (l = *ctx))) {
                if (cy_hptr_refc((const cy_hptr_t *) l) == 1) {
                        cy_utf8_free(&l->ts);
                        cy_utf8_free(&l->evts);
                        cy_utf8_free(&l->facility_kw);
                        cy_utf8_free(&l->severity_kw);
                        cy_utf8_free(&l->hostname);
                        cy_utf8_free(&l->tag);
                        cy_utf8_free(&l->message);
                }

                cy_hptr_free((cy_hptr_t **) ctx);
        }
}


size_t elmy_log_sz(const elmy_log_t *ctx)
{
        const size_t enum_sz = ctx->facility < 10 ? 2 : 3;

        return cy_hptr_sz((const cy_hptr_t *) ctx)
               + cy_utf8_sz(ctx->facility_kw) + cy_utf8_sz(ctx->severity_kw)
               + cy_utf8_sz(ctx->hostname) + cy_utf8_sz(ctx->tag)
               + cy_utf8_sz(ctx->message) + enum_sz;
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


const cy_utf8_t *elmy_log_facility_kw(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->facility_kw;
}


enum cy_log_severity elmy_log_severity(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->severity;
}


const cy_utf8_t *elmy_log_severity_kw(const elmy_log_t *ctx)
{
        assert(ctx != NULL);

        return ctx->severity_kw;
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

        const char *f;

        switch (fmt) {
        case ELMY_LOG_FORMAT_CSV:
                f = "%s,%s,%d,%s,%d,%s,%s,%s,%s";
                break;

        case ELMY_LOG_FORMAT_CSV_HDR:
                f = "log_ts,event_ts,facility,facility_kw,severity,severity_kw,"
                    "hostname,tag,message\n%s,%s,%d,%s,%d,%s,%s,%s,%s";
                break;

        case ELMY_LOG_FORMAT_JSON:
                f = "{\"log_ts\":\"%s\",\"event_ts\":\"%s\",\"facility\":%d,"
                    "\"facility_kw\":\"%s\",\"severity\":%d,"
                    "\"severity_kw\":\"%s\",\"hostname\":\"%s\",\"tag\":\"%s\","
                    "\"message\":\"%s\"}";
                break;

        default:
                f = "log_ts = %s, event_ts = %s, facility = %d,"
                    " facility_kw = %s, severity = %d, severity_kw = %s,"
                    " hostname = %s, tag = %s, message = %s";
        }

        if (fmt != ELMY_LOG_FORMAT_JSON)
                return cy_utf8_new_fmt(f, ctx->ts, ctx->evts, ctx->facility,
                                       ctx->facility_kw, ctx->severity,
                                       ctx->severity_kw, ctx->hostname,
                                       ctx->tag, ctx->message);

        CY_AUTO(cy_utf8_t) *ts = cy_utf8_escape_json(ctx->ts);
        CY_AUTO(cy_utf8_t) *evts = cy_utf8_escape_json(ctx->evts);
        CY_AUTO(cy_utf8_t) *fkw = cy_utf8_escape_json(ctx->facility_kw);
        CY_AUTO(cy_utf8_t) *skw = cy_utf8_escape_json(ctx->severity_kw);
        CY_AUTO(cy_utf8_t) *hn = cy_utf8_escape_json(ctx->hostname);
        CY_AUTO(cy_utf8_t) *tag = cy_utf8_escape_json(ctx->tag);
        CY_AUTO(cy_utf8_t) *msg = cy_utf8_escape_json(ctx->message);

        return cy_utf8_new_fmt(f, ts, evts, ctx->facility, fkw, ctx->severity,
                               skw, hn, tag, msg);
}
