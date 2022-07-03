#include "../../include/error.h"

#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>

#include <libpq-fe.h>


typedef struct {
        enum elmy_status     status;
        elmy_error_t        *err;
        PGconn              *conn;
        PGresult            *res;
        cy_utf8_t           *rule;
        cy_utf8_t           *sql;
} db_t;


static CY_SAFE db_t *
db_new(const char *, const char *);

static void
db_t_free__(db_t **ctx);

#define db_free(ctx) db_t_free__(ctx)

static CY_PSAFE enum elmy_status
db_exec(const db_t *ctx);

static CY_PSAFE enum elmy_status
db_exec_param(const db_t *ctx, const char *params[]);

static CY_PSAFE PGresult *
db_result(const db_t *ctx);

static CY_SAFE elmy_error_t *
db_error(const db_t *ctx);


db_t *db_new(const char *rule, const char *sql)
{
        db_t *ctx = cy_hptr_new(sizeof *ctx);
        ctx->rule = cy_utf8_new(rule);
        ctx->sql = cy_utf8_new(sql);
        ctx->conn = PQconnectdb("user=rsyslog password=rsyslog dbname=syslog");

        if (CY_UNLIKELY(PQstatus(ctx->conn) == CONNECTION_BAD)) {
                ctx->status = ELMY_STATUS_ERR_DBCONN;
                ctx->err = elmy_error_new(ELMY_STATUS_ERR_DBCONN, rule,
                                          PQerrorMessage(ctx->conn));
        }

        return ctx;
}


void db_t_free__(db_t **ctx)
{
        db_t *d;

        if (CY_LIKELY(ctx && (d = *ctx))) {
                elmy_error_free(&d->err);
                cy_utf8_free(&d->rule);
                cy_utf8_free(&d->sql);

                if (CY_LIKELY(d->res))
                        PQclear(d->res);

                if (CY_LIKELY(d->conn))
                        PQfinish(d->conn);

                cy_hptr_free((cy_hptr_t **) ctx);
        }
}
