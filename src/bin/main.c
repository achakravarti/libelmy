#include "opt.h"
#include "show.h"
#include "rule.h"

#include "../../include/rule.h"

#include <libchrysalid/include/hptr.h>
#include <libchrysalid/include/utf8.h>
#include <getopt.h>

#include <stdlib.h>
#include <string.h>


static int proc_error(const struct opt *o, int argc, char *argv[])
{
        if (argc == 1 || o->error)
                return show_invalid(argv);

        return EXIT_SUCCESS;
}


static int proc_help(const struct opt *o, int argc, char *argv[])
{
        if (o->help) {
                if (argc > 2)
                        return show_invalid(argv);

                show_usage();
        }

        return EXIT_SUCCESS;
}


static int proc_version(const struct opt *o, int argc, char *argv[])
{
        if (o->version) {
                if (argc > 2)
                        return show_invalid(argv);

                printf("(lib)elmy 0.0.1 -- easy log monitoring\n"
                       "Copyright (c) 2022 Abhishek Chakravarti\n"
                       "Released under the BSD 2-Clause License\n");
        }

        return EXIT_SUCCESS;
}


static int proc_count(const struct opt *o, int argc, char *argv[])
{
        if (!strcmp(argv[argc - 1], "count")) {
                if (CY_UNLIKELY(argc > 2))
                        return show_invalid(argv);

                size_t res;
                CY_AUTO(elmy_error_t) *err = NULL;

                if (CY_UNLIKELY(elmy_rule_count(&res, &err)))
                        return show_error(err);

                printf("%zu\n", res);
        }

        return EXIT_SUCCESS;
}


static int proc_initial(const struct opt *o, int argc, char *argv[])
{
        if (!strcmp(argv[argc - 1], "initial")) {
                if (CY_UNLIKELY(argc != 3))
                        return show_invalid(argv);

                if (CY_UNLIKELY(cy_utf8_empty(o->timezone)))
                        return show_missing(argv);

                CY_AUTO(cy_utf8_t) *res = NULL;
                CY_AUTO(elmy_error_t) *err = NULL;

                if (CY_UNLIKELY(elmy_rule_initial(o->timezone, &res, &err)))
                        return show_error(err);

                printf("%s\n", res);
        }

        return EXIT_SUCCESS;
}


static int proc_last(const struct opt *o, int argc, char *argv[])
{
        if (!strcmp(argv[argc - 1], "last")) {
                if (CY_UNLIKELY(argc != 3))
                        return show_invalid(argv);

                if (CY_UNLIKELY(cy_utf8_empty(o->timezone)))
                        return show_missing(argv);

                CY_AUTO(cy_utf8_t) *res = NULL;
                CY_AUTO(elmy_error_t) *err = NULL;

                if (CY_UNLIKELY(elmy_rule_last(o->timezone, &res, &err)))
                        return show_error(err);

                printf("%s\n", res);
        }

        return EXIT_SUCCESS;
}


static int proc_all(const struct opt *o, int argc, char *argv[])
{
        if (!strcmp(argv[argc - 1], "all")) {
                if (CY_UNLIKELY(argc < 3 && argc > 7))
                        return show_invalid(argv);

                if (CY_UNLIKELY(cy_utf8_empty(o->timezone)))
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
        }

        return EXIT_SUCCESS;
}


static int run_rule(const struct opt *o, int argc, char *argv[])
{
        if (proc_error(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_help(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_version(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_count(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_initial(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_last(o, argc, argv))
                return EXIT_FAILURE;

        if (proc_all(o, argc, argv))
                return EXIT_FAILURE;

        /*const char *rule = argv[argc - 1];

        if (!strcmp(rule, "count"))
                return hnd_count();

        if (!strcmp(rule, "initial"))
                return hnd_initial();

        if (!strcmp(rule, "last"))
                return hnd_last();

                return EXIT_FAILURE;*/

        return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
        struct opt *o = opt_new(argc, argv);
        //int rc = run_rule(o, argc, argv);
        int rc = rule_exec(o, argc, argv);
        opt_free(&o);

        return rc;
}
