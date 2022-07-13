/* Local dependencies */
#include "cmd.h"
#include "opt.h"
#include "print.h"

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


/* Private callback functions  */

typedef enum elmy_status (cmd_ts_f)(
    const char *, cy_utf8_t **, elmy_error_t **);

typedef enum elmy_status (cmd_fstr_f)(
    const char *, const char *, const elmy_page_t *, elmy_logs_t **,
    elmy_error_t **);


/* Prototypes for private command processing routines */

static CY_PSAFE int cmd_count(const struct opt *, int, char *[]);
static CY_PSAFE int cmd_all(const struct opt *, char *[]);
static CY_PSAFE int cmd_facility(const struct opt *, char *[]);
static CY_PSAFE int cmd_severity(const struct opt *, char *[]);
static CY_PSAFE int cmd_ts(cmd_ts_f *, const struct opt *, char *[]);
static CY_PSAFE int cmd_fstr(cmd_fstr_f *, const struct opt *, char *[]);

/* Prototype for private support function */

static CY_PSAFE int csv_array(const char *, const char *, int, int **, size_t *);





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
int bin_cmd_exec(const struct opt *o, int argc, char *argv[])
{
        if (argc == 1 || o->error)
                return print_invalid(argv);

        if (o->help) {
                print_usage();
                return EXIT_SUCCESS;
        }

        if (o->version) {
                print_version();
                return EXIT_SUCCESS;
        }

        const char *rule = argv[argc - 1];

        if (!strcmp(rule, "count"))
                return cmd_count(o, argc, argv);

        if (!strcmp(rule, "initial"))
                return cmd_ts(elmy_rule_initial, o, argv);

        if (!strcmp(rule, "last"))
                return cmd_ts(elmy_rule_last, o, argv);

        if (!strcmp(rule, "all"))
                return cmd_all(o, argv);

        if (!strcmp(rule, "facility"))
                return cmd_facility(o, argv);

        if (!strcmp(rule, "severity"))
                return cmd_severity(o, argv);

        if (!strcmp(rule, "hostname"))
                return cmd_fstr(elmy_rule_hostname, o, argv);

        if (!strcmp(rule, "tag"))
                return cmd_fstr(elmy_rule_tag, o, argv);

        if (!strcmp(rule, "message"))
                return cmd_fstr(elmy_rule_message, o, argv);

        return print_invalid(argv);
}


/* Implementation of private command processing functions */


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
                return print_invalid(argv);

        size_t res;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_count(&res, &err)))
                return print_error(err);

        printf("%zu\n", res);
        return EXIT_SUCCESS;
}


int cmd_ts(cmd_ts_f *rule, const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(
            o->help || o->json || o->unpaged || o->version || *o->filter
            || *o->sortcol || *o->sortdir || *o->rowstart || *o->rowcount))
                return print_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone))
                return print_missing(argv);

        CY_AUTO(cy_utf8_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(rule(o->timezone, &res, &err)))
                return print_error(err);

        printf("%s\n", res);
        return EXIT_SUCCESS;
}


int cmd_fstr(cmd_fstr_f *rule, const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version))
                return print_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone || !*o->filter))
                return print_missing(argv);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;
        CY_AUTO(elmy_page_t) *pg = opt_page(o);

        if (CY_UNLIKELY(rule(o->filter, o->timezone, pg, &res, &err)))
                return print_error(err);

        return print_logs(res, o);
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
                return print_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone))
                return print_missing(argv);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;
        CY_AUTO(elmy_page_t) *pg = opt_page(o);

        if (CY_UNLIKELY(elmy_rule_all(o->timezone, pg, &res, &err)))
                return print_error(err);

        return print_logs(res, o);
}

int cmd_facility(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version))
                return print_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone || !*o->filter))
                return print_missing(argv);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;
        CY_AUTO(elmy_page_t) *pg = opt_page(o);

        const char *regex
            = "^\\b([0-9]|1[0-9]|2[0123])\\b(,\\b([0-9]|1[0-9]|2[0123])\\b)"
                "{0,23}$";

        int *facilities = NULL;
        size_t len = 0;

        if (CY_UNLIKELY(csv_array(
            o->filter, regex, __CY_LOG_FACILITY_LEN__, &facilities, &len)))
                return print_invalid(argv);

        if (CY_UNLIKELY(elmy_rule_facility(
            (enum cy_log_facility *) facilities, len, o->timezone, pg, &res,
            &err)))
                return print_error(err);

        return print_logs(res, o);
}


int cmd_severity(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version))
                return print_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone || !*o->filter))
                return print_missing(argv);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;
        CY_AUTO(elmy_page_t) *pg = opt_page(o);

        const char *regex = "^[0-7](,[0-7]){0,7}$";
        int *severities = NULL;
        size_t len = 0;

        if (CY_UNLIKELY(csv_array(
            o->filter, regex, __CY_LOG_SEVERITY_LEN__, &severities, &len)))
                return print_invalid(argv);

        if (CY_UNLIKELY(elmy_rule_severity(
            (enum cy_log_severity *) severities, len, o->timezone, pg, &res,
            &err)))
                return print_error(err);

        return print_logs(res, o);
}


/* Implementation of private support function */


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
