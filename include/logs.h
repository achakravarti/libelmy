/* Header guard */
#ifndef __LIBELMY_INCLUDE_LOGS_H__
#define __LIBELMY_INCLUDE_LOGS_H__

#include "log.h"

#include <libchrysalid/log.h>
#include <libchrysalid/utf8.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


typedef struct elmy_logs__ elmy_logs_t;


enum elmy_logs_format {
        ELMY_LOGS_FORMAT_DEFAULT,
        ELMY_LOGS_FORMAT_CSV,
        ELMY_LOGS_FORMAT_CSV_HDR,
        ELMY_LOGS_FORMAT_JSON
};


extern CY_HOT CY_SAFE elmy_logs_t *
elmy_logs_new(size_t);

extern CY_HOT CY_SAFE elmy_logs_t *
elmy_logs_copy(const elmy_logs_t *);

extern CY_SAFE elmy_logs_t *
elmy_logs_clone(const elmy_logs_t *);

extern CY_HOT void
elmy_logs_t_free__(elmy_logs_t **);

#define elmy_logs_free(ctx) \
elmy_logs_t_free__(ctx)

extern CY_SAFE const elmy_log_t *
elmy_logs_get(const elmy_logs_t *, size_t);

extern CY_PSAFE void
elmy_logs_set(elmy_logs_t *, size_t, const elmy_log_t *);

extern CY_SAFE cy_utf8_t *
elmy_logs_print(const elmy_logs_t *, enum elmy_logs_format);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_LOGS_H__ */
