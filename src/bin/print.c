#include "print.h"


void print_usage(void)
{
        printf("Usage: elmy [-c,--count] [-i,--initial] [-l,--last]\n"
                "\t[-a,--all [-p,--paged]] [-f,--facility csv [-p,--paged]]\n"
                "\t\t[-n,--hostname string [-p,--paged]]"
                " [-s,--severity csv [-p,--paged]]\n"
                "\t\t[-m,--message string [-p,--paged]]"
                " [-t,--tag string [-p,--paged]]\n"
                "\t[-h,--help] [-v,--version]\n");
}


void print_version(void)
{
        printf("(lib)elmy 0.0.1 -- easy log monitoring\n"
                "Copyright (c) 2022 Abhishek Chakravarti\n"
                "Released under the BSD 2-Clause License\n");
}


int print_invalid(char *argv[])
{
        fprintf(stderr, "%s: invalid argument or option(s)\n", argv[0]);
        print_usage();

        return EXIT_FAILURE;
}


int print_missing(char *argv[])
{
        fprintf(stderr, "%s: missing option(s) for argument\n", argv[0]);
        print_usage();
        return EXIT_FAILURE;
}


int print_error(const elmy_error_t *err)
{
        CY_AUTO(cy_utf8_t) *emsg = elmy_error_str(err);
        fprintf(stderr, "%s\n", emsg);

        return elmy_error_status(err);
}


int print_logs(const elmy_logs_t *logs, const struct opt *o)
{
        if (o->json) {
                CY_AUTO(cy_json_t) *j = elmy_logs_json(logs);
                CY_AUTO(cy_utf8_t) *s = cy_json_print(j, true);
                printf("%s\n", s);
        } else {
                CY_AUTO(cy_utf8_t) *s = elmy_logs_str(logs);
                printf("%s", s);
        }

        return EXIT_SUCCESS;
}
