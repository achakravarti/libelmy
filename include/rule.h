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


extern size_t
elmy_rule_count(void);

extern CY_PSAFE cy_utf8_t *
elmy_rule_initial(const char *);

extern CY_PSAFE cy_utf8_t *
elmy_rule_last(const char *);

extern CY_SAFE elmy_logs_t *
elmy_rule_all(const char *);

extern CY_SAFE elmy_logs_t *
elmy_rule_all_paged(const char *, const struct elmy_page *);

extern int
elmy_rule_facility(const char *, const struct elmy_page *,
                   enum cy_log_facility [], size_t, elmy_logs_t **,
                   cy_utf8_t **);

extern int
elmy_rule_severity(const char *, const struct elmy_page *,
                   enum cy_log_severity [], size_t, elmy_logs_t **,
                   cy_utf8_t **);

extern int
elmy_rule_hostname(const char *, const struct elmy_page *, const char *,
                   elmy_logs_t **, cy_utf8_t **);

extern int
elmy_rule_tag(const char *, const struct elmy_page *, const char *,
              elmy_logs_t **, cy_utf8_t **);

extern int
elmy_rule_message(const char *, const struct elmy_page *, const char *,
                  elmy_logs_t **, cy_utf8_t **);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_RULE_H__ */
