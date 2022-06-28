#include "../../include/rule.h"

#include <libchrysalid/ext.h>

#include <stdio.h>
#include <stdlib.h>


static int hnd_all(void);
static int hnd_all_paged(void);
static int hnd_facility(const char *);
static int hnd_facility_paged(const char *);
static int hnd_hostname(const char *);
static int hnd_hostname_paged(const char *);
static int hnd_message(const char *);
static int hnd_message_paged(const char *);
static int hnd_count(void);
static int hnd_initial(void);
static int hnd_last(void);
static int hnd_help(void);
static int hnd_version(void);
static int hnd_severity(const char *);
static int hnd_severity_paged(const char *);
static int hnd_tag(const char *);
static int hnd_tag_paged(const char *);


int hnd_all(void)
{
        printf("-a (--all) handled\n");
        return EXIT_SUCCESS;
}


int hnd_all_paged(void)
{
        printf("-a -p (--all --paged) handled\n");
        return EXIT_SUCCESS;
}


int hnd_facility(const char *arg)
{
        printf("facility = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_facility_paged(const char *arg)
{
        printf("facility = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_hostname(const char *arg)
{
        printf("hostname = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_hostname_paged(const char *arg)
{
        printf("hostname = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_message(const char *arg)
{
        printf("message = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_message_paged(const char *arg)
{
        printf("message = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_count(void)
{
        size_t sz;
        CY_AUTO(cy_utf8_t) *err = NULL;

        if (elmy_rule_count(&sz, &err)) {
                printf("size = %zu\n", sz);
                return EXIT_SUCCESS;
        }

        printf("-c (--count) failed: %s\n", err);
        return EXIT_FAILURE;
}


int hnd_initial(void)
{
        CY_AUTO(cy_utf8_t) *res = NULL;
        CY_AUTO(cy_utf8_t) *err = NULL;

        if (elmy_rule_initial("asia/kolkata", &res, &err)) {
                printf("%s\n", res);
                return EXIT_SUCCESS;
        }

        printf("-i (--initial) failed: %s\n", err);
        return EXIT_FAILURE;
}


int hnd_last(void)
{
        CY_AUTO(cy_utf8_t) *res = NULL;
        CY_AUTO(cy_utf8_t) *err = NULL;

        if (elmy_rule_last("asia/kolkata", &res, &err)) {
                printf("%s\n", res);
                return EXIT_SUCCESS;
        }

        printf("-l (--last) failed: %s\n", err);
        return EXIT_FAILURE;
}


int hnd_help(void)
{
        printf("Usage: elmy [-c,--count] [-i,--initial] [-l,--last]\n"
               "\t[-a,--all [-p,--paged]] [-f,--facility csv [-p,--paged]]\n"
               "\t\t[-n,--hostname string [-p,--paged]]"
               " [-s,--severity csv [-p,--paged]]\n"
               "\t\t[-m,--message string [-p,--paged]]"
               " [-t,--tag string [-p,--paged]]\n"
               "\t[-h,--help] [-v,--version]\n");

        return EXIT_SUCCESS;
}


int hnd_version(void)
{
        printf("(lib)elmy 0.0.1 -- easy log monitoring\n"
               "Copyright (c) 2022 Abhishek Chakravarti\n"
               "Released under the BSD 2-Clause License\n");

        return EXIT_SUCCESS;
}


int hnd_severity(const char *arg)
{
        printf("severity = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_severity_paged(const char *arg)
{
        printf("severity = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_tag(const char *arg)
{
        printf("tag = %s\n", arg);
        return EXIT_SUCCESS;
}


int hnd_tag_paged(const char *arg)
{
        printf("tag = %s\n", arg);
        return EXIT_SUCCESS;
}
