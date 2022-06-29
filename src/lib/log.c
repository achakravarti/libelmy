#include "../../include/log.h"

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

}


elmy_log_t *elmy_log_copy(const elmy_log_t *ctx)
{

}


elmy_log_t *elmy_log_clone(const elmy_log_t *ctx)
{

}


elmy_log_t *elmy_log_t_free__(const elmy_log_t *ctx)
{

}


const cy_utf8_t *elmy_log_ts(const elmy_log_t *ctx)
{

}


const cy_utf8_t *elmy_log_ts_event(const elmy_log_t *ctx)
{

}


enum cy_log_facility elmy_log_facility(const elmy_log_t *ctx)
{

}


enum cy_log_severity elmy_log_severity(const elmy_log_t *ctx)
{

}


const cy_utf8_t *elmy_log_hostname(const elmy_log_t *ctx)
{

}


const cy_utf8_t *elmy_log_tag(const elmy_log_t *ctx)
{

}


const cy_utf8_t *elmy_log_message(const elmy_log_t *ctx)
{

}


cy_utf8_t *elmy_log_print(const elmy_log_t *ctx, enum elmy_log_format fmt)
{

}
