#include "hnd.c"

#include <libchrysalid/utf8.h>

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
        cy_utf8_t *facility_arg;
        cy_utf8_t *severity_arg;
        cy_utf8_t *hostname_arg;
        cy_utf8_t *tag_arg;
        cy_utf8_t *message_arg;
};


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
                        data->facility_arg = cy_utf8_new(optarg);
                        break;

                case 's':
                        data->severity = true;
                        data->severity_arg = cy_utf8_new(optarg);
                        break;

                case 'n':
                        data->hostname = true;
                        data->hostname_arg = cy_utf8_new(optarg);
                        break;

                case 't':
                        data->tag = true;
                        data->tag_arg = cy_utf8_new(optarg);
                        break;

                case 'm':
                        data->message = true;
                        data->message_arg = cy_utf8_new(optarg);
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
                hnd_help();
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
        hnd_help();
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
                return hnd_help();

        if (d.version)
                return hnd_version();

        if (d.count)
                return hnd_count();

        if (d.initial)
                return hnd_initial();

        if (d.last)
                return hnd_last();

        if (d.all)
                return d.paged ? hnd_all_paged() : hnd_all();

        if (d.facility)
                return d.paged ? hnd_facility_paged(d.facility_arg)
                               : hnd_facility(d.facility_arg);

        if (d.severity)
                return d.paged ? hnd_severity_paged(d.severity_arg)
                               : hnd_severity(d.severity_arg);

        if (d.hostname)
                return d.paged ? hnd_hostname_paged(d.hostname_arg)
                               : hnd_hostname(d.hostname_arg);

        if (d.tag)
                return d.paged ? hnd_tag_paged(d.tag_arg) : hnd_tag(d.tag_arg);

        if (d.message)
                return d.paged ? hnd_message_paged(d.message_arg)
                               : hnd_message(d.message_arg);

        cy_utf8_free(&d.facility_arg);
        cy_utf8_free(&d.severity_arg);
        cy_utf8_free(&d.hostname_arg);
        cy_utf8_free(&d.tag_arg);
        cy_utf8_free(&d.message_arg);

        return EXIT_SUCCESS;
}
