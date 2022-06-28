/* Header guard */
#ifndef __LIBELMY_INCLUDE_ERROR_H__
#define __LIBELMY_INCLUDE_ERROR_H__

#include <libchrysalid/ext.h>
#include <libchrysalid/log.h>

#include <stdio.h>
#include <stdlib.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


#define ELMY_STATUS_OK 0
#define ELMY_STATUS_FAIL 1

enum elmy_error {
        ELMY_ERROR_DBCONNECT = 1
};


extern CY_WEAK void
elmy_error_dbconnect(enum elmy_error, const char *);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_ERROR_H__ */
