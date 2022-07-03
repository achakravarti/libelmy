#include "db.h"

#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>
#include <libpq-fe.h>

#include <string.h>


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

enum elmy_status db_exec(db_t *ctx)
{
        ctx->res = PQexec(ctx->conn, ctx->sql);

        if (CY_UNLIKELY(PQresultStatus(ctx->res) != PGRES_TUPLES_OK)) {
                ctx->status = ELMY_STATUS_ERR_DBQRY;
                ctx->err = elmy_error_new(ELMY_STATUS_ERR_DBQRY, ctx->rule,
                                          PQerrorMessage(ctx->conn));
                return ELMY_STATUS_ERR_DBQRY;
        }

        return ELMY_STATUS_OK;
}


enum elmy_status db_exec_param(db_t *ctx, const char *params[])
{
        register size_t nparams = 0;
        register char *s = ctx->sql;

        while ((s = strstr(s, "$"))) {
                s++;
                nparams++;
        }

        ctx->res = PQexecParams(ctx->conn, ctx->sql, nparams, NULL, params,
                                NULL, NULL, 0);

        if (CY_UNLIKELY(PQresultStatus(ctx->res) != PGRES_TUPLES_OK)) {
                ctx->status = ELMY_STATUS_ERR_DBQRY;
                ctx->err = elmy_error_new(ELMY_STATUS_ERR_DBQRY, ctx->rule,
                                          PQerrorMessage(ctx->conn));
                return ELMY_STATUS_ERR_DBQRY;
        }

        return ELMY_STATUS_OK;
}


PGresult *db_result(const db_t *ctx)
{
        return ctx->res;
}


elmy_error_t *db_error(const db_t *ctx)
{
        return elmy_error_copy(ctx->err);
}
