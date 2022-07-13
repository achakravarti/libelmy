/* Local dependencies */
#include "opt.h"
#include "show.h"

/* Non-standard libary dependencies */
#include "../../include/rule.h"
#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>

/* Standard library dependencies */
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Prototypes for private support functions */

static CY_PSAFE int cmd_count(const struct opt *, int, char *[]);
static CY_PSAFE int cmd_all(const struct opt *, char *[]);
static CY_PSAFE int cmd_facility(const struct opt *, char *[]);
static CY_PSAFE int cmd_severity(const struct opt *, char *[]);

static CY_PSAFE int csv_array(const char *, const char *, int, int **, size_t *);

typedef enum elmy_status (rule_ts_f)(const char *, cy_utf8_t **, elmy_error_t **);
typedef enum elmy_status (rule_fstr_f)(const char *, const char *, const elmy_page_t *, elmy_logs_t **, elmy_error_t **);


static CY_PSAFE int run_ts(rule_ts_f *rule, const struct opt *o, char *argv[]);
static CY_PSAFE int run_fstr(rule_fstr_f *rule, const struct opt *o, char *argv[]);


/* Implementation of public function */


/*                                                               %func:rule_exec
 * __NAME__
 *      rule_exec() - executes user-specified rule
 *
 * __RETURN__
 *      The {{rule_exec()}} functions returns one of the following {{int}}
 *      status codes:
 *
 *        - {{EXIT_SUCCESS}} if the rule executed successfully
 *        - {{EXIT_FAILURE: if a command line-related error occurrs
 *        - {{ELMY_STATUS_DBCONN}} if a database connection error occurs
 *        - {{ELMY_STATUS_DBQRY}} if a database query error occurs
 */
int cmd_exec(const struct opt *o, int argc, char *argv[])
{
        if (argc == 1 || o->error)
                return show_invalid(argv);

        if (o->help) {
                show_usage();
                return EXIT_SUCCESS;
        }

        if (o->version) {
                show_version();
                return EXIT_SUCCESS;
        }

        const char *rule = argv[argc - 1];

        if (!strcmp(rule, "count"))
                return cmd_count(o, argc, argv);

        if (!strcmp(rule, "initial"))
                return run_ts(elmy_rule_initial, o, argv);

        if (!strcmp(rule, "last"))
                return run_ts(elmy_rule_last, o, argv);

        if (!strcmp(rule, "all"))
                return cmd_all(o, argv);

        if (!strcmp(rule, "facility"))
                return cmd_facility(o, argv);

        if (!strcmp(rule, "severity"))
                return cmd_severity(o, argv);

        if (!strcmp(rule, "hostname"))
                return run_fstr(elmy_rule_hostname, o, argv);

        if (!strcmp(rule, "tag"))
                return run_fstr(elmy_rule_tag, o, argv);

        if (!strcmp(rule, "message"))
                return run_fstr(elmy_rule_message, o, argv);

        return show_invalid(argv);
}


/* Implementation of private functions */


/*                                                              %func:rule_count
 * __NAME__
 *      rule_count() - executes the "count" rule
 *
 * __RETURN__
 *      The {{rule_exec()}} functions returns one of the following {{int}}
 *      status codes:
 *
 *        - {{EXIT_SUCCESS}} if the rule executed successfully
 *        - {{EXIT_FAILURE: if a command line-related error occurrs
 *        - {{ELMY_STATUS_DBCONN}} if a database connection error occurs
 *        - {{ELMY_STATUS_DBQRY}} if a database query error occurs
 */
int cmd_count(const struct opt *o, int argc, char *argv[])
{
        if (CY_UNLIKELY(argc > 2))
                return show_invalid(argv);

        size_t res;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_count(&res, &err)))
                return show_error(err);

        printf("%zu\n", res);
        return EXIT_SUCCESS;
}

int run_ts(rule_ts_f *rule, const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(
            o->help || o->json || o->unpaged || o->version || *o->filter
            || *o->sortcol || *o->sortdir || *o->rowstart || *o->rowcount))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone))
                return show_missing(argv);

        CY_AUTO(cy_utf8_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(rule(o->timezone, &res, &err)))
                return show_error(err);

        printf("%s\n", res);
        return EXIT_SUCCESS;
}


/*                                                                %func:rule_all
 * __NAME__
 *      rule_all() - executes the "all" rule
 *
 * __RETURN__
 *      The {{rule_exec()}} functions returns one of the following {{int}}
 *      status codes:
 *
 *        - {{EXIT_SUCCESS}} if the rule executed successfully
 *        - {{EXIT_FAILURE}} if a command line-related error occurrs
 *        - {{ELMY_STATUS_DBCONN}} if a database connection error occurs
 *        - {{ELMY_STATUS_DBQRY}} if a database query error occurs
 */
int cmd_all(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version || *o->filter))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone))
                return show_missing(argv);

        CY_AUTO(elmy_page_t) *pg = CY_UNLIKELY(o->unpaged)
            ? elmy_page_new_disabled()
            : elmy_page_new_parse(
                o->rowstart, o->rowcount, o->sortcol, o->sortdir);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_all(o->timezone, pg, &res, &err)))
                return show_error(err);

        return show_logs(res, o);
}

int run_fstr(rule_fstr_f *rule, const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone || !*o->filter))
                return show_missing(argv);

        CY_AUTO(elmy_page_t) *pg = CY_UNLIKELY(o->unpaged)
            ? elmy_page_new_disabled()
            : elmy_page_new_parse(
                o->rowstart, o->rowcount, o->sortcol, o->sortdir);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(rule(o->filter, o->timezone, pg, &res, &err)))
                return show_error(err);

        return show_logs(res, o);
}


int cmd_facility(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone || !*o->filter))
                return show_missing(argv);

        CY_AUTO(elmy_page_t) *pg = CY_UNLIKELY(o->unpaged)
            ? elmy_page_new_disabled()
            : elmy_page_new_parse(
                o->rowstart, o->rowcount, o->sortcol, o->sortdir);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        const char *regex
            = "^\\b([0-9]|1[0-9]|2[0123])\\b(,\\b([0-9]|1[0-9]|2[0123])\\b)"
                "{0,23}$";

        int *facilities = NULL;
        size_t len = 0;

        if (CY_UNLIKELY(csv_array(
            o->filter, regex, __CY_LOG_FACILITY_LEN__, &facilities, &len)))
                return show_invalid(argv);

        if (CY_UNLIKELY(elmy_rule_facility(
            (enum cy_log_facility *) facilities, len, o->timezone, pg, &res,
            &err)))
                return show_error(err);

        return show_logs(res, o);
}


int cmd_severity(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone || !*o->filter))
                return show_missing(argv);

        CY_AUTO(elmy_page_t) *pg = CY_UNLIKELY(o->unpaged)
            ? elmy_page_new_disabled()
            : elmy_page_new_parse(
                o->rowstart, o->rowcount, o->sortcol, o->sortdir);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        const char *regex = "^[0-7](,[0-7]){0,7}$";
        int *severities = NULL;
        size_t len = 0;

        if (CY_UNLIKELY(csv_array(
            o->filter, regex, __CY_LOG_SEVERITY_LEN__, &severities, &len)))
                return show_invalid(argv);

        if (CY_UNLIKELY(elmy_rule_severity(
            (enum cy_log_severity *) severities, len, o->timezone, pg, &res,
            &err)))
                return show_error(err);

        return show_logs(res, o);
}



int csv_array(
    const char *csv, const char *regex, int max, int **array, size_t *len)
{
        assert(!*array);
        assert(*regex);
        assert(max >= 0 && max <= __CY_LOG_FACILITY_LEN__);

        if (CY_UNLIKELY(!*csv))
                return EXIT_FAILURE;

        CY_AUTO(cy_utf8_t) *s = cy_utf8_new(csv);

        if (CY_UNLIKELY(!cy_utf8_match(s, regex))) {
                printf("MATCH FAILED\n");
                return EXIT_FAILURE;
        }

        register char *t;
        char *r = NULL;
        register int i = 0;
        int *a = cy_hptr_new(sizeof (int) * __CY_LOG_FACILITY_LEN__);

        for (t = strtok_r(s, ",", &r); t != NULL; t = strtok_r(NULL, ",", &r)) {
                a[i] = strtoumax(t, NULL, 10);

                if (CY_UNLIKELY(a[i] < 0 || a[i++] > max))
                        return EXIT_FAILURE;
        }

        if (CY_UNLIKELY(i > max))
                return EXIT_FAILURE;

        *array = a;
        *len = i;
        return EXIT_SUCCESS;
}
