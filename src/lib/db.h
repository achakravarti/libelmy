/* Header guard */
#ifndef __LIBELMY_SRC_LIB_DB_H__
#define __LIBELMY_SRC_LIB_DB_H__

#include "../../include/error.h"

#include <libpq-fe.h>


typedef struct {
        enum elmy_status     status;
        elmy_error_t        *err;
        PGconn              *conn;
        PGresult            *res;
        cy_utf8_t           *rule;
        cy_utf8_t           *sql;
} db_t;


extern CY_SAFE db_t *
db_new(const char *, const char *);

extern void
db_t_free__(db_t **);

#define db_free(ctx) db_t_free__(ctx)

extern CY_PSAFE enum elmy_status
db_exec(db_t *);

extern CY_PSAFE enum elmy_status
db_exec_param(db_t *, const char *[]);

extern CY_PSAFE PGresult *
db_result(const db_t *);

extern CY_SAFE elmy_error_t *
db_error(const db_t *);


/* Header guard */
#endif /* !__LIBELMY_SRC_LIB_DB_H__ */
