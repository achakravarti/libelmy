#include "../../include/error.h"
#include "../../include/rule.h"

#include <libchrysalid/ext.h>
#include <libchrysalid/log.h>
#include <libpq-fe.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>


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
        assert(tz && *tz);

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
        assert(tz && *tz);

        const char *p[] = {tz};
        const char *s = "SELECT * FROM logs_ts_last($1);";

        PGconn *c = db_connect();
        PGresult *r = db_execp(c, s, p, sizeof (p) / sizeof (*p));
        cy_utf8_t *res = cy_utf8_new(PQgetvalue(r, 0, 0));

        PQclear(r);
        PQfinish(c);

        return res;
}


elmy_logs_t *elmy_rule_all(const char *tz, const struct elmy_page *pg)
{
        assert(tz != NULL && *tz != '\0');

        (void) pg;
        const char *p[] = {tz};
        const char *s = "SELECT * FROM logs_all($1);";

        PGconn *c = db_connect();
        PGresult *r = db_execp(c, s, p, sizeof (p) / sizeof (*p));
        register size_t len = PQntuples(r);
        elmy_logs_t *res = elmy_logs_new(len);

        elmy_log_t *log;
        for (register size_t i = 0; i < len; i++) {
                log = elmy_log_new(PQgetvalue(r, i, 1), PQgetvalue(r, i, 0),
                                   strtoumax(PQgetvalue(r, i, 2), NULL, 10),
                                   PQgetvalue(r, i, 3),
                                   strtoumax(PQgetvalue(r, i, 4), NULL, 10),
                                   PQgetvalue(r, i, 5), PQgetvalue(r, i, 6),
                                   PQgetvalue(r, i, 7), PQgetvalue(r, i, 8));

                elmy_logs_set(res, i, log);
                elmy_log_free(&log);
        }

        PQclear(r);
        PQfinish(c);

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
