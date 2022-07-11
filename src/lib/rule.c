/* Local dependencies */
#include "../../include/error.h"
#include "../../include/rule.h"
#include "db.h"

/* External dependencies */
#include <libchrysalid/include/ext.h>
#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/log.h>
#include <libchrysalid/include/utf8.h>
#include <libpq-fe.h>

/* Standard library dependencies */
#include <assert.h>
#include <inttypes.h>


/* Prototypes for support functions */

static CY_RSAFE cy_utf8_t *enums_csv(int enums[], size_t len);

static CY_PSAFE enum elmy_status rule_ts(
    const char *, const char *, cy_utf8_t **, elmy_error_t **);

static CY_PSAFE enum elmy_status rule_fint(
    const char *, int [], size_t, const char *, const elmy_page_t *,
    elmy_logs_t **, elmy_error_t **);

static CY_PSAFE enum elmy_status rule_fstr(
    const char *, const char *, const char *, const elmy_page_t *,
    elmy_logs_t **, elmy_error_t **);


/* Implementations of public functions */


/*                                                         %func:elmy_rule_count
 * __NAME__
 *      {{elmy_rule_count()}} - rule to count number of logs
 *
 * __SYNOPSIS__
 *      #include <libelmy/libelmy.h>
 *      extern CY_PSAFE enum elmy_status elmy_rule_count(
 *          size_t *res, elmy_error_t **err);
 *
 * __PARAMETERS__
 *      - {{res}}: handle to rule result
 *      - {{err}}: handle to error
 *
 * __RETURN__
 *      - {{ELMY_STATUS_OK}} if no error occurred
 *      - Any other relevant {{elmy_status}} enumerator if an error occured
 */
enum elmy_status elmy_rule_count(size_t *res, elmy_error_t **err)
{
        assert(res != NULL);
        assert(err != NULL && *err == NULL);

        CY_AUTO(db_t) *db = db_new("count", "SELECT * FROM logs_count();");

        if (CY_UNLIKELY(db_exec(db))) {
                *res = 0;
                *err = db_error(db);
                return elmy_error_status(*err);
        }

        *res = strtoumax(PQgetvalue(db_result(db), 0, 0), NULL, 10);
        return ELMY_STATUS_OK;
}


/*                                                       %func:elmy_rule_initial
 * __NAME__
 *      {{elmy_rule_initial()}} - rule to get timestamp of first log entry
 *
 * __SYNOPSIS__
 *      #include <libelmy/libelmy.h>
 *      extern CY_PSAFE enum elmy_status elmy_rule_initial(
 *          const char *tz, cy_utf8_t **res, elmy_error_t **err);
 *
 * __PARAMETERS__
 *      - {{res}}: handle to rule result
 *      - {{err}}: handle to error
 *
 * __RETURN__
 *      - {{ELMY_STATUS_OK}} if no error occurred
 *      - Any other relevant {{elmy_status}} enumerator if an error occured
 */
enum elmy_status elmy_rule_initial(
    const char *tz, cy_utf8_t **res, elmy_error_t **err)
{
        return rule_ts("ts_first", tz, res, err);
}


enum elmy_status elmy_rule_last(const char *tz, cy_utf8_t **res,
                                elmy_error_t **err)
{
        return rule_ts("ts_first", tz, res, err);
}


enum elmy_status elmy_rule_all(const char *tz, const elmy_page_t *pg,
                               elmy_logs_t **res, elmy_error_t **err)
{
        assert(*tz);
        assert(!*res);

        CY_AUTO(db_t) *db;
        enum elmy_status rc;

        if (elmy_page_disabled(pg)) {
                db = db_new("all", "SELECT * FROM logs_all($1);");
                rc = db_exec_param(db, (const char *[1]) {tz});
        } else {
                const char *p[] = {elmy_page_start(pg), elmy_page_count(pg),
                                   elmy_page_col(pg), elmy_page_dir(pg), tz};
                db = db_new(
                    "all", "SELECT * FROM logs_all_paged($1,$2,$3,$4,$5);");
                rc = db_exec_param(db, p);
        }

        if (CY_UNLIKELY(rc)) {
                *res = elmy_logs_new_empty();
                *err = db_error(db);
                return rc;
        }

        *res = elmy_logs_new_parse__(db_result(db));
        return ELMY_STATUS_OK;
}


enum elmy_status elmy_rule_facility(
    enum cy_log_facility filter[], size_t nfilter, const char *tz,
    const elmy_page_t *pg, elmy_logs_t **res, elmy_error_t **err)
{
        return rule_fint("facility", (int *) filter, nfilter, tz, pg, res, err);
}


enum elmy_status elmy_rule_severity(
    enum cy_log_severity filter[], size_t nfilter, const char *tz,
    const elmy_page_t *pg, elmy_logs_t **res, elmy_error_t **err)
{
        return rule_fint("severity", (int *) filter, nfilter, tz, pg, res, err);
}


enum elmy_status elmy_rule_hostname(
    const char *filter, const char *tz, const elmy_page_t *pg, elmy_logs_t **res,
    elmy_error_t **err)
{
        return rule_fstr("hostname", filter, tz, pg, res, err);
}


enum elmy_status elmy_rule_tag(
    const char *filter, const char *tz, const elmy_page_t *pg, elmy_logs_t **res,
    elmy_error_t **err)
{
        return rule_fstr("tag", filter, tz, pg, res, err);
}


enum elmy_status elmy_rule_message(
    const char *filter, const char *tz, const elmy_page_t *pg, elmy_logs_t **res,
    elmy_error_t **err)
{
        return rule_fstr("message", filter, tz, pg, res, err);
}


/* Implementations of support functions */


// https://stackoverflow.com/questions/1745811
cy_utf8_t *enums_csv(int enums[], size_t len)
{
        const size_t bfrlen = 80;
        char bfr[bfrlen];

        register size_t off = 1;

        for (register size_t i = 0; i < len; i++)
                off += snprintf(
                    bfr + off, bfrlen - off, i > 0 ? ",%d" : "%u", enums[i]);

        bfr[0] = '{';
        bfr[off] = '}';
        bfr[off + 1] = '\0';

        return cy_utf8_new(bfr);
}


enum elmy_status rule_ts(
    const char *rule, const char *tz, cy_utf8_t **res, elmy_error_t **err)
{
        assert(*rule);
        assert(*tz);
        assert(!*err);

        CY_AUTO(cy_utf8_t) *sql = cy_utf8_new_fmt(
            "SELECT * FROM logs_%s($1);", rule);

        db_t *db = db_new(rule, sql);

        if (CY_UNLIKELY(db_exec_param(db, (const char *[1]) {tz}))) {
                *res = 0;
                *err = db_error(db);
                return elmy_error_status(*err);
        }

        *res = cy_utf8_new(PQgetvalue(db_result(db), 0, 0));
        return ELMY_STATUS_OK;
}


enum elmy_status rule_fint(
    const char *rule, int filter[], size_t nfilter, const char *tz,
    const elmy_page_t *pg, elmy_logs_t **res, elmy_error_t **err)
{
        assert(*rule);
        assert(nfilter > 0);
        assert(*tz);
        assert(!*res);
        assert(!*err);

        enum elmy_status rc;
        CY_AUTO(db_t) *db;
        CY_AUTO(cy_utf8_t) *csv = enums_csv(filter, nfilter);

        if (CY_UNLIKELY(elmy_page_disabled(pg))) {
                CY_AUTO(cy_utf8_t) *sql = cy_utf8_new_fmt(
                    "SELECT * FROM logs_%s($1,$2);", rule);

                db = db_new(rule, sql);
                rc = db_exec_param(db, (const char *[2]) {csv, tz});
        } else {
                CY_AUTO(cy_utf8_t) *sql = cy_utf8_new_fmt(
                    "SELECT * FROM logs_%s_paged($1,$2,$3,$4,$5,$6);", rule);

                const char *p[] = {
                    csv, elmy_page_start(pg), elmy_page_count(pg),
                    elmy_page_col(pg), elmy_page_dir(pg), tz};

                db = db_new(rule, sql);
                rc = db_exec_param(db, p);
        }

        if (CY_UNLIKELY(rc)) {
                *res = elmy_logs_new_empty();
                *err = db_error(db);
                return rc;
        }

        *res = elmy_logs_new_parse__(db_result(db));
        return ELMY_STATUS_OK;
}


enum elmy_status rule_fstr(
    const char *rule, const char *filter, const char *tz, const elmy_page_t *pg,
    elmy_logs_t **res, elmy_error_t **err)
{
        assert(*rule);
        assert(*filter);
        assert(*tz);
        assert(!*res);
        assert(!*err);

        enum elmy_status rc;
        CY_AUTO(db_t) *db;


        if (CY_UNLIKELY(elmy_page_disabled(pg))) {
                CY_AUTO(cy_utf8_t) *sql = cy_utf8_new_fmt(
                    "SELECT * FROM logs_%s($1,$2);", rule);

                db = db_new(rule, sql);
                rc = db_exec_param(db, (const char *[2]) {filter, tz});
        } else {
                CY_AUTO(cy_utf8_t) *sql = cy_utf8_new_fmt(
                    "SELECT * FROM logs_%s_paged($1,$2,$3,$4,$5,$6);", rule);

                const char *p[] = {
                    filter, elmy_page_start(pg), elmy_page_count(pg),
                    elmy_page_col(pg), elmy_page_dir(pg), tz};

                db = db_new(rule, sql);
                rc = db_exec_param(db, p);
        }

        if (CY_UNLIKELY(rc)) {
                *res = elmy_logs_new_empty();
                *err = db_error(db);
                return rc;
        }

        *res = elmy_logs_new_parse__(db_result(db));
        return ELMY_STATUS_OK;
}
