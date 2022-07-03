#include "../../include/error.h"
#include "../../include/rule.h"

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


static PGconn *db_connect(void)
{
        PGconn *c = PQconnectdb("user=rsyslog password=rsyslog dbname=syslog");

        if (CY_UNLIKELY(PQstatus(c) == CONNECTION_BAD)) {
                CY_AUTO(cy_utf8_t) *err = cy_utf8_new(PQerrorMessage(c));
                PQfinish(c);
                elmy_error_dbconn(ELMY_ERROR_DBCONN, err);
        }

        return c;
}


static PGresult *db_exec(PGconn *conn, const char *sql)
{
        PGresult *r = PQexec(conn, sql);

        if (CY_UNLIKELY(PQresultStatus(r) != PGRES_TUPLES_OK)) {
                CY_AUTO(cy_utf8_t) *err = cy_utf8_new(PQerrorMessage(conn));
                PQclear(r);
                PQfinish(conn);
                elmy_error_dbqry(ELMY_ERROR_DBQRY, err);
        }

        return r;
}


static PGconn *db_connect2(const char *rule, elmy_error_t **err)
{
        PGconn *c = PQconnectdb("user=rsyslog password=rsyslog dbname=syslog");

        if (CY_LIKELY(PQstatus(c) == CONNECTION_OK))
                return c;

        CY_AUTO(cy_utf8_t) *msg = cy_utf8_new(PQerrorMessage(c));
        *err = elmy_error_new(ELMY_STATUS_ERR_DBCONN, rule, msg);
        PQfinish(c);

        return NULL;
}


static PGresult *db_exec2(PGconn *conn, const char *sql, const char *rule,
                          elmy_error_t **err)
{
        PGresult *r = PQexec(conn, sql);

        if (CY_LIKELY(PQresultStatus(r) == PGRES_TUPLES_OK))
                return r;

        CY_AUTO(cy_utf8_t) *msg = cy_utf8_new(PQerrorMessage(conn));
        *err = elmy_error_new(ELMY_STATUS_ERR_DBQRY, rule, msg);
        PQclear(r);
        PQfinish(conn);

        return NULL;
}


static PGresult *db_execp(PGconn *conn, const char *sql, const char *params[],
                          size_t nparams)
{
        PGresult *r = PQexecParams(conn, sql, nparams, NULL, params, NULL, NULL,
                                   0);

        if (CY_UNLIKELY(PQresultStatus(r) != PGRES_TUPLES_OK)) {
                CY_AUTO(cy_utf8_t) *err = cy_utf8_new(PQerrorMessage(conn));
                PQclear(r);
                PQfinish(conn);
                elmy_error_dbqry(ELMY_ERROR_DBQRY, err);
        }

        return r;
}


static PGresult *db_execp2(PGconn *conn, const char *sql, const char *params[],
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

        PGconn *c = db_connect2(rule, err);
        if (CY_UNLIKELY(!c)) {
                *res = 0;
                return elmy_error_status(*err);
        }

        PGresult *r = db_exec2(c, sql, rule, err);
        if (CY_UNLIKELY(!r)) {
                *res = 0;
                return elmy_error_status(*err);
        }

        *res = strtoumax(PQgetvalue(r, 0, 0), NULL, 10);
        PQclear(r);
        PQfinish(c);

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
        const size_t nparams = 1;

        PGconn *c = db_connect2(rule, err);
        if (CY_UNLIKELY(!c)) {
                *res = cy_utf8_new_empty();
                return elmy_error_status(*err);
        }

        PGresult *r = db_execp2(c, sql, params, nparams, rule, err);
        if (CY_UNLIKELY(!r)) {
                *res = cy_utf8_new_empty();
                return elmy_error_status(*err);
        }

        *res = cy_utf8_new(PQgetvalue(r, 0, 0));
        PQclear(r);
        PQfinish(c);

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
        const size_t nparams = 1;

        PGconn *c = db_connect2(rule, err);
        if (CY_UNLIKELY(!c)) {
                *res = cy_utf8_new_empty();
                return elmy_error_status(*err);
        }

        PGresult *r = db_execp2(c, sql, params, nparams, rule, err);
        if (CY_UNLIKELY(!r)) {
                *res = cy_utf8_new_empty();
                return elmy_error_status(*err);
        }

        *res = cy_utf8_new(PQgetvalue(r, 0, 0));
        PQclear(r);
        PQfinish(c);

        return ELMY_STATUS_OK;
}


elmy_logs_t *elmy_rule_all(const char *tz)
{
        assert(tz != NULL && *tz != '\0');

        const char *p[] = {tz};
        const char *s = "SELECT * FROM logs_all($1);";

        PGconn *c = db_connect();
        PGresult *r = db_execp(c, s, p, sizeof (p) / sizeof (*p));
        elmy_logs_t *res = elmy_logs_parse__(r);

        PQclear(r);
        PQfinish(c);

        return res;
}


elmy_logs_t *elmy_rule_all_paged(const char *tz, const struct elmy_page *pg)
{
        assert(tz != NULL && *tz != '\0');
        assert(pg != NULL && pg->row_start > 0 && pg->row_count > 0);

        const char *col = sort_col(pg->sort_col);
        const char *dir = pg->sort_asc ? "asc" : "desc";
        char *start = sort_val(pg->row_start);
        char *count = sort_val(pg->row_count);
        const char *p[] = {start, count, col, dir, tz};
        const char *s = "SELECT * FROM logs_all_paged($1,$2,$3,$4,$5);";

        PGconn *c = db_connect();
        PGresult *r = db_execp(c, s, p, sizeof (p) / sizeof (*p));
        elmy_logs_t *res = elmy_logs_parse__(r);

        PQclear(r);
        PQfinish(c);
        cy_hptr_free((cy_hptr_t **) &start);
        cy_hptr_free((cy_hptr_t **) &count);

        return res;
}


int elmy_rule_facility(const char *tz, const struct elmy_page *pg,
                       enum cy_log_facility filter[], size_t nfilter,
                       elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        PQfinish(c);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_severity(const char *tz, const struct elmy_page *pg,
                       enum cy_log_severity filter[], size_t nfilter,
                       elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        PQfinish(c);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_hostname(const char *tz, const struct elmy_page *pg,
                       const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        PQfinish(c);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_tag(const char *tz, const struct elmy_page *pg,
                  const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        PQfinish(c);

        return ELMY_STATUS_FAIL;
}


int elmy_rule_message(const char *tz, const struct elmy_page *pg,
                      const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        PQfinish(c);

        return ELMY_STATUS_FAIL;
}
