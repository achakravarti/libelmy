/* Header guard */
#ifndef __LIBELMY_INCLUDE_LOGS_H__
#define __LIBELMY_INCLUDE_LOGS_H__

#include <libchrysalid/log.h>
#include <libchrysalid/utf8.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


typedef struct elmy_logs__ elmy_logs_t;


enum elmy_logs_format {
        ELMY_LOG_FORMAT_DEFAULT,
        EMLY_LOG_FORMAT_JSON
};


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_LOGS_H__ */
