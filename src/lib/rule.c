#include "../../include/rule.h"

#include <libchrysalid/log.h>
#include <libpq-fe.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>


static PGconn *db_connect(void)
{
        PGconn *c = PQconnectdb("user=rsyslog password=rsyslog dbname=syslog");

        if (PQstatus(c) == CONNECTION_BAD) {
                const char *e = PQerrorMessage(c);

                cy_log_err(e);
                fprintf(stderr, "Failed to connect to database: %s\n", e);

                PQfinish(c);
                abort();
        }

        return c;
}


bool elmy_rule_count(size_t *res, cy_utf8_t **err)
{
        assert(res);

        PGconn *c = db_connect();
        PGresult *r = PQexec(c, "SELECT * FROM logs_count();");

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
                fprintf(stderr, "Failed to execute logs_count()\n");

                PQclear(r);
                PQfinish(c);
                return false;
        }

        *res = strtoumax(PQgetvalue(r, 0, 0), NULL, 10);
        if (*res == UINTMAX_MAX && errno == ERANGE) {
                *res = 0;

                PQclear(r);
                PQfinish(c);
                return false;
        }

        PQclear(r);
        PQfinish(c);
        return true;
}


bool elmy_rule_initial(const char *tz, cy_utf8_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        const char *params[1] = {tz};
        const char *sql = "SELECT * FROM logs_ts_first($1);";
        PGresult *r = PQexecParams(c, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
                fprintf(stderr, "Failed to execute logs_ts_first()\n");

                PQclear(r);
                PQfinish(c);
                return false;
        }

        *res = cy_utf8_new(PQgetvalue(r, 0, 0));

        PQclear(r);
        PQfinish(c);
        return true;
}


bool elmy_rule_last(const char *tz, cy_utf8_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        const char *params[1] = {tz};
        const char *sql = "SELECT * FROM logs_ts_last($1);";
        PGresult *r = PQexecParams(c, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
                fprintf(stderr, "Failed to execute logs_ts_first()\n");

                PQclear(r);
                PQfinish(c);
                return false;
        }

        *res = cy_utf8_new(PQgetvalue(r, 0, 0));

        PQclear(r);
        PQfinish(c);
        return true;
}


bool elmy_rule_all(const char *tz, const struct elmy_page *pg,
                   elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        PQfinish(c);

        return false;
}


bool elmy_rule_facility(const char *tz, const struct elmy_page *pg,
                        enum cy_log_facility filter[], size_t nfilter,
                        elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        PQfinish(c);

        return false;
}


bool elmy_rule_severity(const char *tz, const struct elmy_page *pg,
                        enum cy_log_severity filter[], size_t nfilter,
                        elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        PQfinish(c);

        return false;
}


bool elmy_rule_hostname(const char *tz, const struct elmy_page *pg,
                        const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        PQfinish(c);

        return false;
}


bool elmy_rule_tag(const char *tz, const struct elmy_page *pg,
                   const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        PQfinish(c);

        return false;
}


bool elmy_rule_message(const char *tz, const struct elmy_page *pg,
                       const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        PGconn *c = db_connect();
        PQfinish(c);

        return false;
}
