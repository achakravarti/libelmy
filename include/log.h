/* Header guard */
#ifndef __LIBELMY_INCLUDE_LOG_H__
#define __LIBELMY_INCLUDE_LOG_H__

#include <libchrysalid/log.h>
#include <libchrysalid/utf8.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


typedef struct elmy_log__ elmy_log_t;

enum elmy_log_format {
        ELMY_LOG_FORMAT_DEFAULT,
        ELMY_LOG_FORMAT_CSV,
        ELMY_LOG_FORMAT_CSV_HDR,
        EMLY_LOG_FORMAT_JSON
};


extern CY_HOT CY_SAFE elmy_log_t *
elmy_log_new(const char *, const char *, enum cy_log_facility,
             enum cy_log_severity, const char *, const char *, const char *);

extern CY_HOT CY_SAFE elmy_log_t *
elmy_log_copy(const elmy_log_t *);

extern CY_HOT CY_SAFE elmy_log_t *
elmy_log_clone(const elmy_log_t *);

extern CY_HOT elmy_log_t *
elmy_log_clone(const elmy_log_t *);

extern CY_HOT elmy_log_t *
elmy_log_t_free__(const elmy_log_t *);

#define emly_log_free(ctx)  \
emly_log_t_free__(ctx)

extern CY_SAFE const cy_utf8_t *
elmy_log_ts(const elmy_log_t *);

extern CY_SAFE const cy_utf8_t *
elmy_log_ts_event(const elmy_log_t *);

extern CY_PSAFE enum cy_log_facility
elmy_log_facility(const elmy_log_t *);

extern CY_PSAFE enum cy_log_severity
elmy_log_severity(const elmy_log_t *);

extern CY_SAFE const cy_utf8_t *
elmy_log_hostname(const elmy_log_t *);

extern CY_SAFE const cy_utf8_t *
elmy_log_tag(const elmy_log_t *);

extern CY_SAFE const cy_utf8_t *
elmy_log_message(const elmy_log_t *);

extern CY_RSAFE cy_utf8_t *
elmy_log_print(enum elmy_log_format);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_LOG_H__ */
