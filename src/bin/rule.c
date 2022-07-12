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

static CY_PSAFE int rule_count(const struct opt *, int, char *[]);

static CY_PSAFE int rule_initial(const struct opt *, char *[]);

static CY_PSAFE int rule_last(const struct opt *, char *[]);

static CY_PSAFE int rule_all(const struct opt *, char *[]);

static CY_PSAFE int rule_facility(const struct opt *, char *[]);

static CY_PSAFE int rule_hostname(const struct opt *, char *[]);

static CY_PSAFE int rule_tag(const struct opt *, char *[]);

static CY_PSAFE int rule_message(const struct opt *, char *[]);


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
int
rule_exec(const struct opt *o, int argc, char *argv[])
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
                return rule_count(o, argc, argv);

        if (!strcmp(rule, "initial"))
                return rule_initial(o, argv);

        if (!strcmp(rule, "last"))
                return rule_last(o, argv);

        if (!strcmp(rule, "all"))
                return rule_all(o, argv);

        if (!strcmp(rule, "facility"))
                return rule_facility(o, argv);

        if (!strcmp(rule, "hostname"))
                return rule_hostname(o, argv);

        if (!strcmp(rule, "tag"))
                return rule_tag(o, argv);

        if (!strcmp(rule, "message"))
                return rule_message(o, argv);

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
int
rule_count(const struct opt *o, int argc, char *argv[])
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


/*                                                            %func:rule_initial
 * __NAME__
 *      rule_initial() - executes the "initial" rule
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
int
rule_initial(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->json || o->unpaged || o->version
                        || *o->filter || *o->sortcol || *o->sortdir
                        || *o->rowstart || *o->rowcount))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone))
                return show_missing(argv);

        CY_AUTO(cy_utf8_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_initial(o->timezone, &res, &err)))
                return show_error(err);

        printf("%s\n", res);
        return EXIT_SUCCESS;
}


/*                                                               %func:rule_last
 * __NAME__
 *      rule_last() - executes the "last" rule
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
int
rule_last(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->json || o->unpaged || o->version
                        || *o->filter || *o->sortcol || *o->sortdir
                        || *o->rowstart || *o->rowcount))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone))
                return show_missing(argv);

        CY_AUTO(cy_utf8_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_last(o->timezone, &res, &err)))
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
int
rule_all(const struct opt *o, char *argv[])
{
        if (CY_UNLIKELY(o->help || o->version || *o->filter))
                return show_invalid(argv);

        if (CY_UNLIKELY(!*o->timezone))
                return show_missing(argv);

        CY_AUTO(elmy_page_t) *pg = CY_UNLIKELY(o->unpaged)
                                   ? elmy_page_new_disabled()
                                   : elmy_page_new_parse(o->rowstart,
                                                         o->rowcount,
                                                         o->sortcol,
                                                         o->sortdir);
        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_all(o->timezone, pg, &res, &err)))
                return show_error(err);

        if (o->json) {
                CY_AUTO(cy_json_t) *j = elmy_logs_json(res);
                CY_AUTO(cy_utf8_t) *s = cy_json_print(j, true);
                printf("%s\n", s);
        } else {
                CY_AUTO(cy_utf8_t) *s = elmy_logs_str(res);
                printf("%s", s);
        }

        return EXIT_SUCCESS;
}


int rule_hostname(const struct opt *o, char *argv[])
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

        if (CY_UNLIKELY(
            elmy_rule_hostname(o->filter, o->timezone, pg, &res, &err)))
                return show_error(err);

        if (o->json) {
                CY_AUTO(cy_json_t) *j = elmy_logs_json(res);
                CY_AUTO(cy_utf8_t) *s = cy_json_print(j, true);
                printf("%s\n", s);
        } else {
                CY_AUTO(cy_utf8_t) *s = elmy_logs_str(res);
                printf("%s", s);
        }

        return EXIT_SUCCESS;
}


int rule_tag(const struct opt *o, char *argv[])
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

        if (CY_UNLIKELY(
            elmy_rule_tag(o->filter, o->timezone, pg, &res, &err)))
                return show_error(err);

        if (o->json) {
                CY_AUTO(cy_json_t) *j = elmy_logs_json(res);
                CY_AUTO(cy_utf8_t) *s = cy_json_print(j, true);
                printf("%s\n", s);
        } else {
                CY_AUTO(cy_utf8_t) *s = elmy_logs_str(res);
                printf("%s", s);
        }

        return EXIT_SUCCESS;
}


int rule_message(const struct opt *o, char *argv[])
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

        if (CY_UNLIKELY(
            elmy_rule_message(o->filter, o->timezone, pg, &res, &err)))
                return show_error(err);

        if (o->json) {
                CY_AUTO(cy_json_t) *j = elmy_logs_json(res);
                CY_AUTO(cy_utf8_t) *s = cy_json_print(j, true);
                printf("%s\n", s);
        } else {
                CY_AUTO(cy_utf8_t) *s = elmy_logs_str(res);
                printf("%s", s);
        }

        return EXIT_SUCCESS;
}


int rule_facility(const struct opt *o, char *argv[])
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
            = "^\\b([0-9]|1[0-9]|2[0123])\\b(,\\b([0-9]|1[0-9]|2[0123])\\b){0,23}$";

        int *facilities = NULL;
        size_t len = 0;

        if (CY_UNLIKELY(csv_array(
            o->filter, regex, __CY_LOG_FACILITY_LEN__, &facilities, &len)))
                return show_invalid(argv);

        if (CY_UNLIKELY(elmy_rule_facility(
            (enum cy_log_facility *) facilities, len, o->timezone, pg, &res,
            &err)))
                return show_error(err);

        if (o->json) {
                CY_AUTO(cy_json_t) *j = elmy_logs_json(res);
                CY_AUTO(cy_utf8_t) *s = cy_json_print(j, true);
                printf("%s\n", s);
        } else {
                CY_AUTO(cy_utf8_t) *s = elmy_logs_str(res);
                printf("%s", s);
        }

        return EXIT_SUCCESS;
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
