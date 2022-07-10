#include "opt.h"
#include "show.h"

#include "../../include/rule.h"
#include <libchrysalid/include/utf8.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int rule_count(const struct opt *o, int argc, char *argv[]);
static int rule_initial(const struct opt *o, char *argv[]);
static int rule_last(const struct opt *o, char *argv[]);
static int rule_all(const struct opt *o, char *argv[]);


int rule_exec(const struct opt *o, int argc, char *argv[])
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

        return show_invalid(argv);
}


int rule_count(const struct opt *o, int argc, char *argv[])
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


int rule_initial(const struct opt *o, char *argv[])
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


int rule_last(const struct opt *o, char *argv[])
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


int rule_all(const struct opt *o, char *argv[])
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
