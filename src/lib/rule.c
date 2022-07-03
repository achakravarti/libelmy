#include "../../include/error.h"
#include "../../include/rule.h"
#include "db.c"

#include <libchrysalid/include/ext.h>
#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/log.h>
#include <libchrysalid/include/utf8.h>
#include <libpq-fe.h>

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>


static const char *sort_col(enum elmy_sort sort)
{
        switch (sort) {
        case ELMY_SORT_TS:
                return "receivedat";
                break;

        case ELMY_SORT_FACILITY:
                return "facility";
                break;

        case ELMY_SORT_SEVERITY:
                return "priority";
                break;

        case ELMY_SORT_HOSTNAME:
                return "fromhost";
                break;

        case ELMY_SORT_TAG:
                return "syslogtag";
                break;

        case ELMY_SORT_MESSAGE:
                return "message";
                break;

        case ELMY_SORT_TS_EVENT:
        default:
                return "devicereportedtime";
                break;
        }
}


static char *sort_val(size_t val)
{
        size_t sz = snprintf(NULL, 0, "%zu", val) + 1;
        char *bfr = cy_hptr_new(sz);
        snprintf(bfr, sz, "%zu", val);

        return bfr;
}


static PGconn *db_connect(const char *rule, elmy_error_t **err)
{
        PGconn *c = PQconnectdb("user=rsyslog password=rsyslog dbname=syslog");

        if (CY_LIKELY(PQstatus(c) == CONNECTION_OK))
                return c;

        CY_AUTO(cy_utf8_t) *msg = cy_utf8_new(PQerrorMessage(c));
        *err = elmy_error_new(ELMY_STATUS_ERR_DBCONN, rule, msg);
        PQfinish(c);

        return NULL;
}


static PGresult *db_execp(PGconn *conn, const char *sql, const char *params[],
                          size_t nparams, const char *rule, elmy_error_t **err)
{
        PGresult *r = PQexecParams(conn, sql, nparams, NULL, params, NULL, NULL,
                                   0);

        if (CY_LIKELY(PQresultStatus(r) == PGRES_TUPLES_OK))
                return r;

        CY_AUTO(cy_utf8_t) *msg = cy_utf8_new(PQerrorMessage(conn));
        *err = elmy_error_new(ELMY_STATUS_ERR_DBQRY, rule, msg);
        PQclear(r);
        PQfinish(conn);

        return NULL;
}


enum elmy_status elmy_rule_count(size_t *res, elmy_error_t **err)
{
        assert(res != NULL);
        assert(err != NULL && *err == NULL);

        const char *sql = "SELECT * FROM logs_count();";
        const char *rule = "count";
        CY_AUTO(db_t) *db = db_new(rule, sql);

        if (CY_UNLIKELY(db_exec(db))) {
                *res = 0;
                *err = db_error(db);
                return elmy_error_status(*err);
        }

        *res = strtoumax(PQgetvalue(db_result(db), 0, 0), NULL, 10);
        return ELMY_STATUS_OK;
}


enum elmy_status elmy_rule_initial(const char *tz, cy_utf8_t **res,
                                   elmy_error_t **err)
{
        assert(tz != NULL && *tz != '\0');
        assert(res != NULL && *res == NULL);
        assert(err != NULL && *err == NULL);

        const char *rule = "initial";
        const char *sql = "SELECT * FROM logs_ts_first($1);";
        const char *params[] = {tz};
        CY_AUTO(db_t) *db = db_new(rule, sql);

        if (CY_UNLIKELY(db_exec_param(db, params))) {
                *res = cy_utf8_new_empty();
                *err = db_error(db);
                return elmy_error_status(*err);
        }

        *res = cy_utf8_new(PQgetvalue(db_result(db), 0, 0));
        return ELMY_STATUS_OK;
}


enum elmy_status elmy_rule_last(const char *tz, cy_utf8_t **res,
                                elmy_error_t **err)
{
        assert(tz != NULL && *tz != '\0');
        assert(res != NULL && *res == NULL);
        assert(err != NULL && *err == NULL);

        const char *rule = "initial";
        const char *sql = "SELECT * FROM logs_ts_last($1);";
        const char *params[] = {tz};
        CY_AUTO(db_t) *db = db_new(rule, sql);

        if (CY_UNLIKELY(db_exec_param(db, params))) {
                *res = cy_utf8_new_empty();
                *err = db_error(db);
                return elmy_error_status(*err);
        }

        *res = cy_utf8_new(PQgetvalue(db_result(db), 0, 0));
        return ELMY_STATUS_OK;
}


enum elmy_status elmy_rule_all(const char *tz, const struct elmy_page *pg,
                               elmy_logs_t **res, elmy_error_t **err)
{
        assert(tz != NULL && *tz != '\0');
        assert(pg != NULL);
        assert(res != NULL && *res == NULL);
        assert(err != NULL && *err == NULL);

        const char *rule = "all";

        if (CY_UNLIKELY(!pg->row_start || !pg->row_count)) {
                const char *params[] = {tz};
                const char *sql = "SELECT * FROM logs_all($1);";
                CY_AUTO(db_t) *db = db_new(rule, sql);

                if (CY_UNLIKELY(db_exec_param(db, params))) {
                        *res = NULL;
                        *err = db_error(db);
                        return elmy_error_status(*err);
                }

                *res = elmy_logs_parse__(db_result(db));
                return ELMY_STATUS_OK;
        }

        const char *col = sort_col(pg->sort_col);
        const char *dir = pg->sort_asc ? "asc" : "desc";
        char *start = sort_val(pg->row_start);
        char *count = sort_val(pg->row_count);
        const char *params[] = {start, count, col, dir, tz};
        const char *sql = "SELECT * FROM logs_all_paged($1,$2,$3,$4,$5);";
        CY_AUTO(db_t) *db = db_new(rule, sql);

        if (CY_UNLIKELY(db_exec_param(db, params))) {
                *res = NULL;
                *err = db_error(db);
                return elmy_error_status(*err);
        }


        *res = elmy_logs_parse__(db_result(db));
        cy_hptr_free((cy_hptr_t **) &start);
        cy_hptr_free((cy_hptr_t **) &count);

        return ELMY_STATUS_OK;
}


int elmy_rule_facility(const char *tz, const struct elmy_page *pg,
                       enum cy_log_facility filter[], size_t nfilter,
                       elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_severity(const char *tz, const struct elmy_page *pg,
                       enum cy_log_severity filter[], size_t nfilter,
                       elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_hostname(const char *tz, const struct elmy_page *pg,
                       const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_tag(const char *tz, const struct elmy_page *pg,
                  const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_message(const char *tz, const struct elmy_page *pg,
                      const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        return ELMY_STATUS_FAIL;
}
