#include "hnd.c"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>


struct opt_data {
        bool count;
        bool initial;
        bool last;
        bool all;
        bool facility;
        bool severity;
        bool hostname;
        bool tag;
        bool message;
        bool paged;
        bool help;
        bool version;
        bool error;
        char *facility_arg;
        char *severity_arg;
        char *hostname_arg;
        char *tag_arg;
        char *message_arg;
};


// TEMPORARY UNTIL CHRYSALID INSTALLED
char *opt_arg(void)
{
        size_t len = optarg ? strlen(optarg) : 0;
        char *bfr = malloc(len + 1);

        if (!bfr)
                abort();

        strcpy(bfr, optarg);
        bfr[len] = '\0';

        return bfr;
}

// ^[0-7](,\s?[0-7]){0,7}$
// ^\b([0-9]|1[0-9]|2[0123])\b(,\s?\b([0-9]|1[0-9]|2[0123])\b{0,23}$
// https://www.freebsd.org/cgi/man.cgi?getopt_long(3)
void opt_parse(int argc, char **argv, struct opt_data *data)
{
        struct option opts[] = {
                {"count", no_argument, NULL, 'c'},
                {"initial", no_argument, NULL, 'i'},
                {"last", no_argument, NULL, 'l'},
                {"all", no_argument, NULL, 'a'},
                {"facility", required_argument, NULL, 'f'},
                {"severity", required_argument, NULL, 's'},
                {"hostname", required_argument, NULL, 'n'},
                {"tag", required_argument, NULL, 't'},
                {"message", required_argument, NULL, 'm'},
                {"paged", no_argument, NULL, 'p'},
                {"help", no_argument, NULL, 'h'},
                {"version", no_argument, NULL, 'v'},
                { 0 }
        };

        register int o;

        while (1) {
                o = getopt_long(argc, argv, "cilaf:s:n:t:m:phv", opts, NULL);

                if (o == -1)
                        return;

                switch (o) {
                case 'c':
                        data->count = true;
                        break;

                case 'i':
                        data->initial = true;
                        break;

                case 'l':
                        data->last = true;
                        break;

                case 'a':
                        data->all = true;
                        break;

                case 'f':
                        data->facility = true;
                        data->facility_arg = opt_arg();
                        break;

                case 's':
                        data->severity = true;
                        data->severity_arg = opt_arg();
                        break;

                case 'n':
                        data->hostname = true;
                        data->hostname_arg = opt_arg();
                        break;

                case 't':
                        data->tag = true;
                        data->tag_arg = opt_arg();
                        break;

                case 'm':
                        data->message = true;
                        data->message_arg = opt_arg();
                        break;

                case 'p':
                        data->paged = true;
                        break;

                case 'h':
                        data->help = true;
                        break;

                case 'v':
                        data->version = true;
                        break;

                case '?':
                        data->error = true;
                        break;

                default:
                        data->error = true;
                        break;
                }
        }

        argc -= optind;
        argv += optind;
}


bool opt_check(int argc, char **argv, const struct opt_data *data)
{
        if (data->error) {
                misc_help();
                return false;
        }

        if (argc > 3)
                goto combo_error;

        if (argc == 3 && !data->paged
            && (data->all || data->facility || data->severity || data->hostname
                || data->tag || data->message))
                goto combo_error;

        if (argc > 2
            && (data->help || data->version || data->count || data->initial
                || data->last))
                goto combo_error;

        return true;

combo_error:
        fprintf(stderr, "%s: unknown argument combination\n", argv[0]);
        misc_help();
        return false;
}


// https://stackoverflow.com/questions/18079340/u
int opt_proc(int argc, char **argv)
{
        struct opt_data d = (struct opt_data) { 0 };
        opt_parse(argc, argv, &d);

        if (!opt_check(argc, argv, &d))
                return EXIT_FAILURE;

        if (d.help)
                return misc_help();

        if (d.version)
                return misc_version();

        if (d.count)
                return meta_count();

        if (d.initial)
                return meta_initial();

        if (d.last)
                return meta_last();

        if (d.all)
                return d.paged ? all_paged() : all();

        if (d.facility)
                return d.paged ? facility_paged(d.facility_arg)
                               : facility(d.facility_arg);

        if (d.severity)
                return d.paged ? severity_paged(d.severity_arg)
                               : severity(d.severity_arg);

        if (d.hostname)
                return d.paged ? hostname_paged(d.hostname_arg)
                               : hostname(d.hostname_arg);

        if (d.tag)
                return d.paged ? tag_paged(d.tag_arg) : tag(d.tag_arg);

        if (d.message)
                return d.paged ? message_paged(d.message_arg)
                               : message(d.message_arg);

        // TEMPORARY CODE
        free(d.facility_arg);
        free(d.severity_arg);
        free(d.hostname_arg);
        free(d.tag_arg);
        free(d.message_arg);

        return EXIT_SUCCESS;
}
