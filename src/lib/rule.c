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

#define SQL_COUNT       "SELECT * FROM logs_count();"
#define SQL_INITIAL     "SELECT * FROM logs_ts_first($1);"
#define SQL_LAST        "SELECT * FROM logs_ts_last($1);"
#define SQL_ALL         "SELECT * FROM logs_all($1);"
#define SQL_ALL_PAGED   "SELECT * FROM logs_all_paged($1,$2,$3,$4,$5);"


enum elmy_status elmy_rule_count(size_t *res, elmy_error_t **err)
{
        assert(res != NULL);
        assert(err != NULL && *err == NULL);

        CY_AUTO(db_t) *db = db_new(RULE_COUNT, SQL_COUNT);

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

        const char *params[] = {tz};
        CY_AUTO(db_t) *db = db_new(RULE_INITIAL, SQL_INITIAL);

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

        const char *sql = "SELECT * FROM logs_ts_last($1);";
        const char *params[] = {tz};
        CY_AUTO(db_t) *db = db_new(RULE_LAST, SQL_LAST);

        if (CY_UNLIKELY(db_exec_param(db, params))) {
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


        CY_AUTO(db_t) *db = db_new(RULE_ALL, elmy_page_disabled(pg)
                                   ? SQL_ALL : SQL_ALL_PAGED);

        enum elmy_status rc;
        if (elmy_page_disabled(pg)) {
                const char *params[] = {tz};
                rc = db_exec_param(db, params);
        } else {
                const char *params[] = {
                        elmy_page_start(pg), elmy_page_count(pg),
                        elmy_page_col(pg), elmy_page_dir(pg), tz
                };
                rc = db_exec_param(db, params);
        }

        if (CY_UNLIKELY(rc)) {
                *res = NULL;
                *err = db_error(db);
                return rc;
        }

        *res = elmy_logs_parse__(db_result(db));
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
