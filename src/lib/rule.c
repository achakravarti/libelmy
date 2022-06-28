#include "../../include/error.h"
#include "../../include/rule.h"

#include <libchrysalid/log.h>
#include <libpq-fe.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>


static PGconn *db_connect(void)
{
        PGconn *c = PQconnectdb("user=rsyslog password=rsyslog dbname=yslog");

        if (PQstatus(c) == CONNECTION_BAD) {
                elmy_error_dbconnect(ELMY_ERROR_DBCONNECT, PQerrorMessage(c));
                PQfinish(c);
                return NULL;
        }

        return c;
}


int elmy_rule_count(size_t *res, cy_utf8_t **err)
{
        assert(res);
        assert(err);

        PGconn *c = db_connect();
        PGresult *r = PQexec(c, "SELECT * FROM logs_count();");

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
                cy_utf8_free(err);
                *err = cy_utf8_new(PQerrorMessage(c));

                PQclear(r);
                PQfinish(c);

                return ELMY_STATUS_FAIL;
        }

        *res = strtoumax(PQgetvalue(r, 0, 0), NULL, 10);

        PQclear(r);
        PQfinish(c);

        return ELMY_STATUS_OK;
}


int elmy_rule_initial(const char *tz, cy_utf8_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        const char *params[1] = {tz};
        const char *sql = "SELECT * FROM logs_ts_first($1);";
        PGresult *r = PQexecParams(c, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
                cy_utf8_free(err);
                *err = cy_utf8_new(PQerrorMessage(c));

                PQclear(r);
                PQfinish(c);

                return ELMY_STATUS_FAIL;
        }

        *res = cy_utf8_new(PQgetvalue(r, 0, 0));

        PQclear(r);
        PQfinish(c);
        return ELMY_STATUS_OK;
}


int elmy_rule_last(const char *tz, cy_utf8_t **res, cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        const char *params[1] = {tz};
        const char *sql = "SELECT * FROM logs_ts_last($1);";
        PGresult *r = PQexecParams(c, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
                cy_utf8_free(err);
                *err = cy_utf8_new(PQerrorMessage(c));

                PQclear(r);
                PQfinish(c);

                return ELMY_STATUS_FAIL;
        }

        *res = cy_utf8_new(PQgetvalue(r, 0, 0));

        PQclear(r);
        PQfinish(c);
        return ELMY_STATUS_OK;
}


int elmy_rule_all(const char *tz, const struct elmy_page *pg, elmy_logs_t **res,
                  cy_utf8_t **err)
{
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        PGconn *c = db_connect();
        PQfinish(c);

        return ELMY_STATUS_FAIL;
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
