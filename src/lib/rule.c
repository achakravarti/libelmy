#include "../../include/error.h"
#include "../../include/rule.h"
#include "db.h"

#include <libchrysalid/include/ext.h>
#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/log.h>
#include <libchrysalid/include/utf8.h>
#include <libpq-fe.h>

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>


#define RULE_COUNT      "count"
#define RULE_INITIAL    "initial"
#define RULE_LAST       "last"
#define RULE_ALL        "all"

#define SQL_COUNT               "SELECT * FROM logs_count();"
#define SQL_INITIAL             "SELECT * FROM logs_ts_first($1);"
#define SQL_LAST                "SELECT * FROM logs_ts_last($1);"
#define SQL_ALL                 "SELECT * FROM logs_all($1);"
#define SQL_ALL_PAGED           "SELECT * FROM logs_all_paged($1,$2,$3,$4,$5);"


enum elmy_status elmy_rule_count(size_t *res, elmy_error_t **err)
{
        assert(res != NULL);
        assert(err != NULL && *err == NULL);

        CY_AUTO(db_t) *db = db_new(RULE_COUNT, SQL_COUNT);

        if (CY_UNLIKELY(db_exec(db)))
                goto error;

        *res = strtoumax(PQgetvalue(db_result(db), 0, 0), NULL, 10);
        return ELMY_STATUS_OK;

error:
        *res = 0;
        *err = db_error(db);
        return elmy_error_status(*err);
}


enum elmy_status elmy_rule_initial(const char *tz, cy_utf8_t **res,
                                   elmy_error_t **err)
{
        assert(tz != NULL && *tz != '\0');
        assert(res != NULL && *res == NULL);
        assert(err != NULL && *err == NULL);

        CY_AUTO(db_t) *db = db_new(RULE_INITIAL, SQL_INITIAL);

        if (CY_UNLIKELY(db_exec_param(db, (const char *[1]) {tz}))) {
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

        CY_AUTO(db_t) *db = db_new(RULE_LAST, SQL_LAST);

        if (CY_UNLIKELY(db_exec_param(db, (const char *[1]) {tz}))) {
                *res = cy_utf8_new_empty();
                *err = db_error(db);
                return elmy_error_status(*err);
        }

        *res = cy_utf8_new(PQgetvalue(db_result(db), 0, 0));
        return ELMY_STATUS_OK;
}


enum elmy_status elmy_rule_all(const char *tz, const elmy_page_t *pg,
                               elmy_logs_t **res, elmy_error_t **err)
{
        assert(tz != NULL && *tz != '\0');
        assert(pg != NULL);
        assert(res != NULL && *res == NULL);
        assert(err != NULL && *err == NULL);

        CY_AUTO(db_t) *db;
        enum elmy_status rc;

        if (elmy_page_disabled(pg)) {
                db = db_new(RULE_ALL, SQL_ALL);
                rc = db_exec_param(db, (const char *[1]) {tz});
        } else {
                const char *p[] = {elmy_page_start(pg), elmy_page_count(pg),
                                   elmy_page_col(pg), elmy_page_dir(pg), tz};
                db = db_new(RULE_ALL, SQL_ALL_PAGED);
                rc = db_exec_param(db, p);
        }

        if (CY_UNLIKELY(rc))
                goto error;

        *res = elmy_logs_new_parse__(db_result(db));
        return ELMY_STATUS_OK;

error:
        *res = NULL;
        *err = db_error(db);
        return rc;
}


// https://stackoverflow.com/questions/1745811
static cy_utf8_t *
enums_csv(int enums[], size_t len)
{
        const size_t bfrlen = 80;
        char bfr[bfrlen];

        register size_t off = 1;

        for (register size_t i = 0; i < len; i++) {
                off += snprintf(bfr + off,
                                bfrlen - off,
                                i > 0 ? ",%d" : "%u",
                                enums[i]);
        }

        bfr[0] = '{';
        bfr[off] = '}';
        bfr[off + 1] = '\0';

        return cy_utf8_new(bfr);
}


int
elmy_rule_facility(enum cy_log_facility filter[],
                   size_t nfilter,
                   const char *tz,
                   const elmy_page_t *pg,
                   elmy_logs_t **res,
                   elmy_error_t **err)
{
        assert(filter && nfilter > 0);
        assert(tz && *tz);
        assert(res && !*res);
        assert(err);

        const char *rule = "facility";
        const char *sql = "SELECT * FROM logs_facility($1,$2);";
        const char *sqlp = "SELECT * FROM logs_facility_paged($1,$2,$3,$4,$5,%6);";

        enum elmy_status rc;
        CY_AUTO(db_t) *db;
        CY_AUTO(cy_utf8_t) *csv = enums_csv((int *) filter, nfilter);

        if (CY_UNLIKELY(elmy_page_disabled(pg))) {
                db = db_new(rule, sql);
                rc = db_exec_param(db, (const char *[2]) {filter, tz});
        } else {
                const char *p[] = {csv,
                                   elmy_page_start(pg),
                                   elmy_page_count(pg),
                                   elmy_page_col(pg),
                                   elmy_page_dir(pg),
                                   tz};
                db = db_new(rule, sqlp);
                rc = db_exec_param(db, p);
        }

        if (CY_UNLIKELY(rc)) {
                *res = NULL;
                *err = db_error(db);
                return rc;
        }

        *res = elmy_logs_new_parse__(db_result(db));
        return ELMY_STATUS_OK;
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

static int
run_filter(const char            *rule,
           const char            *filter,
           const char            *tz,
           const elmy_page_t     *pg,
           elmy_logs_t          **res,
           elmy_error_t         **err)
{
        assert(rule != NULL && *rule != '\0');
        assert(filter != NULL && *filter != '\0');
        assert(tz != NULL && *tz != '\0');
        assert(pg != NULL);
        assert(res != NULL && *res == NULL);
        assert(err != NULL && *err == NULL);

        enum elmy_status rc;
        CY_AUTO(db_t) *db;
        CY_AUTO(cy_utf8_t) *sql = cy_utf8_new_fmt("SELECT * FROM logs_%s"
                                                  "($1,$2);",
                                                  rule);
        CY_AUTO(cy_utf8_t) *sqlp = cy_utf8_new_fmt("SELECT * FROM logs_%s_paged"
                                                   "($1,$2,$3,$4,$5,$6);",
                                                   rule);


        if (CY_UNLIKELY(elmy_page_disabled(pg))) {
                db = db_new(rule, sql);
                rc = db_exec_param(db, (const char *[2]) {filter, tz});
        } else {
                const char *p[] = {filter,
                                   elmy_page_start(pg),
                                   elmy_page_count(pg),
                                   elmy_page_col(pg),
                                   elmy_page_dir(pg),
                                   tz};
                db = db_new(rule, sqlp);
                rc = db_exec_param(db, p);
        }

        if (CY_UNLIKELY(rc)) {
                *res = NULL;
                *err = db_error(db);
                return rc;
        }

        *res = elmy_logs_new_parse__(db_result(db));
        return ELMY_STATUS_OK;
}


int
elmy_rule_hostname(const char            *filter,
                   const char            *tz,
                   const elmy_page_t     *pg,
                   elmy_logs_t          **res,
                   elmy_error_t         **err)
{
        return run_filter("hostname", filter, tz, pg, res, err);
}


int
elmy_rule_tag(const char         *filter,
              const char         *tz,
              const elmy_page_t  *pg,
              elmy_logs_t       **res,
              elmy_error_t      **err)
{
        return run_filter("tag", filter, tz, pg, res, err);
}


int
elmy_rule_message(const char             *filter,
                  const char             *tz,
                  const elmy_page_t      *pg,
                  elmy_logs_t           **res,
                  elmy_error_t          **err)
{
        return run_filter("message", filter, tz, pg, res, err);
}
