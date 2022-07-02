#include "../../include/error.h"
#include "../../include/rule.h"

#include <libchrysalid/include/ext.h>
#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/log.h>
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


size_t elmy_rule_count(void)
{
        PGconn *c = db_connect();
        PGresult *r = db_exec(c, "SELECT * FROM logs_count();");
        size_t res = strtoumax(PQgetvalue(r, 0, 0), NULL, 10);

        PQclear(r);
        PQfinish(c);

        return res;
}


cy_utf8_t *elmy_rule_initial(const char *tz)
{
        assert(tz != NULL && *tz != '\0');

        const char *p[] = {tz};
        const char *s = "SELECT * FROM logs_ts_first($1);";

        PGconn *c = db_connect();
        PGresult *r = db_execp(c, s, p, sizeof (p) / sizeof (*p));
        cy_utf8_t *res = cy_utf8_new(PQgetvalue(r, 0, 0));

        PQclear(r);
        PQfinish(c);

        return res;
}


cy_utf8_t *elmy_rule_last(const char *tz)
{
        assert(tz != NULL && *tz != '\0');

        const char *p[] = {tz};
        const char *s = "SELECT * FROM logs_ts_last($1);";

        PGconn *c = db_connect();
        PGresult *r = db_execp(c, s, p, sizeof (p) / sizeof (*p));
        cy_utf8_t *res = cy_utf8_new(PQgetvalue(r, 0, 0));

        PQclear(r);
        PQfinish(c);

        return res;
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
