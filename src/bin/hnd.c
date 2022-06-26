#include <stdio.h>
#include <stdlib.h>


static int all(void);
static int all_paged(void);
static int facility(const char *);
static int facility_paged(const char *);
static int hostname(const char *);
static int hostname_paged(const char *);
static int message(const char *);
static int message_paged(const char *);
static int meta_count(void);
static int meta_initial(void);
static int meta_last(void);
static int misc_help(void);
static int misc_version(void);
static int severity(const char *);
static int severity_paged(const char *);
static int tag(const char *);
static int tag_paged(const char *);


int all(void)
{
        printf("-a (--all) handled\n");
        return EXIT_SUCCESS;
}


int all_paged(void)
{
        printf("-a -p (--all --paged) handled\n");
        return EXIT_SUCCESS;
}


int facility(const char *arg)
{
        printf("facility = %s\n", arg);
        return EXIT_SUCCESS;
}


int facility_paged(const char *arg)
{
        printf("facility = %s\n", arg);
        return EXIT_SUCCESS;
}


int hostname(const char *arg)
{
        printf("hostname = %s\n", arg);
        return EXIT_SUCCESS;
}


int hostname_paged(const char *arg)
{
        printf("hostname = %s\n", arg);
        return EXIT_SUCCESS;
}


int message(const char *arg)
{
        printf("message = %s\n", arg);
        return EXIT_SUCCESS;
}


int message_paged(const char *arg)
{
        printf("message = %s\n", arg);
        return EXIT_SUCCESS;
}


int meta_count(void)
{
        printf("-c (--count) handled\n");
        return EXIT_SUCCESS;
}


int meta_initial(void)
{
        printf("-i (--initial) handled\n");
        return EXIT_SUCCESS;
}


int meta_last(void)
{
        printf("-l (--last) handled\n");
        return EXIT_SUCCESS;
}


int misc_help(void)
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


int misc_version(void)
{
        printf("(lib)elmy 0.0.1 -- easy log monitoring\n"
               "Copyright (c) 2022 Abhishek Chakravarti\n"
               "Released under the BSD 2-Clause License\n");

        return EXIT_SUCCESS;
}


int severity(const char *arg)
{
        printf("severity = %s\n", arg);
        return EXIT_SUCCESS;
}


int severity_paged(const char *arg)
{
        printf("severity = %s\n", arg);
        return EXIT_SUCCESS;
}


int tag(const char *arg)
{
        printf("tag = %s\n", arg);
        return EXIT_SUCCESS;
}


int tag_paged(const char *arg)
{
        printf("tag = %s\n", arg);
        return EXIT_SUCCESS;
}
