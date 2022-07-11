/* Header guard */
#ifndef __LIBELMY_INCLUDE_RULE_H__
#define __LIBELMY_INCLUDE_RULE_H__

#include "logs.h"
#include "error.h"
#include "page.h"

#include <libchrysalid/include/log.h>
#include <libchrysalid/include/utf8.h>

#include <stddef.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


/* Prototypes for public rules interface */

extern CY_PSAFE enum elmy_status elmy_rule_count(size_t *, elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_initial(
    const char *, cy_utf8_t **, elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_last(
    const char *, cy_utf8_t **, elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_all(
    const char *, const elmy_page_t *, elmy_logs_t **, elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_facility(
    enum cy_log_facility[], size_t, const char *, const elmy_page_t *,
    elmy_logs_t **, elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_severity(
    enum cy_log_severity [], size_t, const char *, const elmy_page_t *,
    elmy_logs_t **, elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_hostname(
    const char *, const char *, const elmy_page_t *, elmy_logs_t **,
    elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_tag(
    const char *, const char *, const elmy_page_t *, elmy_logs_t **,
    elmy_error_t **);

extern CY_PSAFE enum elmy_status elmy_rule_message(
    const char *, const char *, const elmy_page_t *, elmy_logs_t **,
    elmy_error_t **);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_RULE_H__ */
