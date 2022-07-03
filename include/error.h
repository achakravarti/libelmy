/* Header guard */
#ifndef __LIBELMY_INCLUDE_ERROR_H__
#define __LIBELMY_INCLUDE_ERROR_H__

#include <libchrysalid/include/ext.h>
#include <libchrysalid/include/json.h>
#include <libchrysalid/include/log.h>
#include <libchrysalid/include/utf8.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


//#define ELMY_STATUS_OK 0
#define ELMY_STATUS_FAIL 1

enum elmy_error {
        ELMY_ERROR_DBCONN = 1,
        ELMY_ERROR_DBQRY
};


enum elmy_status {
        ELMY_STATUS_OK,
        ELMY_STATUS_ERR_DBCONN,
        ELMY_STATUS_ERR_DBQRY
};


extern CY_WEAK noreturn void
elmy_error_dbconn(enum elmy_error, const char *);

extern CY_WEAK noreturn void
elmy_error_dbqry(enum elmy_error, const char *);


typedef struct elmy_error__ elmy_error_t;

extern CY_SAFE elmy_error_t *
elmy_error_new(enum elmy_status, const char *, const char *);

extern CY_SAFE elmy_error_t *
elmy_error_copy(const elmy_error_t *);

extern CY_SAFE elmy_error_t *
elmy_error_clone(const elmy_error_t *);

extern void elmy_error_t_free__(elmy_error_t **);

#define elmy_error_free(ctx)    \
elmy_error_t_free__(ctx)

extern CY_PSAFE enum elmy_status
elmy_error_status(const elmy_error_t *);

extern CY_SAFE const cy_utf8_t *
elmy_error_rule(const elmy_error_t *);

extern CY_SAFE const cy_utf8_t *
elmy_error_msg(const elmy_error_t *);

extern CY_SAFE cy_utf8_t *
elmy_error_str(const elmy_error_t *);

extern CY_SAFE cy_json_t *
elmy_error_json(const elmy_error_t *);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_ERROR_H__ */
