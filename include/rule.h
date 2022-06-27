/* Header guard */
#ifndef __LIBELMY_INCLUDE_RULE_H__
#define __LIBELMY_INCLUDE_RULE_H__

#include "log.h"
#include "page.h"

#include "../../libchrysalid/include/utf8.h"

#include <stddef.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


extern bool
elmy_rule_count(size_t *, cy_utf8_t **);

extern bool
elmy_rule_first(const char *, cy_utf8_t **, cy_utf8_t **);

extern bool
elmy_rule_last(const char *, cy_utf8_t **, cy_utf8_t **);

extern bool
elmy_rule_all(const char *, const struct elmy_page *, elmy_logs_t **,
              cy_utf8_t **);

extern bool
elmy_rule_facility(const char *, const struct elmy_page *, int [], size_t,
                   elmy_logs_t **, cy_utf8_t **);

extern bool
elmy_rule_severity(const char *, const struct elmy_page *, int [], size_t,
                   elmy_logs_t **, cy_utf8_t **);

extern bool
elmy_rule_hostname(const char *, const struct elmy_page *, const char *,
                   elmy_logs_t **, cy_utf8_t **);

extern bool
elmy_rule_tag(const char *, const struct elmy_page *, const char *,
              elmy_logs_t **, cy_utf8_t **);

extern bool
elmy_rule_message(const char *, const struct elmy_page *, const char *,
                  elmy_logs_t **, cy_utf8_t **);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_RULE_H__ */
