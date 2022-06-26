#include "all.c"
#include "facility.c"
#include "hostname.c"
#include "message.c"
#include "meta.c"
#include "misc.c"
#include "severity.c"
#include "tag.c"


#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>


struct opt_select {
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
};


// ^[0-7](,\s?[0-7]){0,7}$
// ^\b([0-9]|1[0-9]|2[0123])\b(,\s?\b([0-9]|1[0-9]|2[0123])\b{0,23}$
// https://www.freebsd.org/cgi/man.cgi?getopt_long(3)
void opt_parse(int argc, char **argv, struct opt_select *sel)
{
        printf("ARGC = %d\n", argc);

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
                        sel->count = true;
                        break;

                case 'i':
                        sel->initial = true;
                        break;

                case 'l':
                        sel->last = true;
                        break;

                case 'a':
                        sel->all = true;
                        break;

                case 'f':
                        sel->facility = true;
                        break;

                case 's':
                        sel->severity = true;
                        break;

                case 'n':
                        sel->hostname = true;
                        break;

                case 't':
                        sel->tag = true;
                        break;

                case 'm':
                        sel->message = true;
                        break;

                case 'p':
                        sel->message = true;
                        break;

                case 'h':
                        sel->help = true;
                        break;

                case 'v':
                        sel->version = true;
                        break;

                case '?':
                        sel->error = true;
                        break;

                default:
                        sel->error = true;
                        break;
                }
        }

        argc -= optind;
        argv += optind;
}

// https://stackoverflow.com/questions/18079340/u
int opt_proc(int argc, char **argv)
{
        struct opt_select s = (struct opt_select) { false };
        opt_parse(argc, argv, &s);

        if (s.error) {
                misc_help();
                return EXIT_FAILURE;
        }

        if (argc > 3) {
                fprintf(stderr, "%s: unknown argument combination\n", argv[0]);
                misc_help();
                return EXIT_FAILURE;
        }

        if (argc == 3 && !s.paged
            && (s.all || s.facility || s.severity || s.hostname || s.tag
                || s.message)) {
                fprintf(stderr, "%s: unknown argument combination\n", argv[0]);
                misc_help();
                return EXIT_FAILURE;
        }

        if (argc > 2
            && (s.help || s.version || s.count || s.initial || s.last)) {
                fprintf(stderr, "%s: unknown argument combination\n", argv[0]);
                misc_help();
                return EXIT_FAILURE;
        }

        if (s.help) {
                misc_help();
                return EXIT_SUCCESS;
        }

        if (s.version) {
                misc_version();
                return EXIT_SUCCESS;
        }

        if (s.count)
                return meta_count();

        if (s.initial)
                return meta_initial();

        if (s.last)
                return meta_last();

        if (s.all)
                return s.paged ? all_paged() : all();

        if (s.facility)
                return s.paged ? facility_paged() : facility();

        if (s.severity)
                return s.paged ? severity_paged() : severity();

        if (s.hostname)
                return s.paged ? hostname_paged() : hostname();

        if (s.tag)
                return s.paged ? tag_paged() : tag();

        if (s.message)
                return s.paged ? message_paged() : message();

        return EXIT_SUCCESS;
}
