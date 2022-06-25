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


// https://www.freebsd.org/cgi/man.cgi?getopt_long(3)
void opt_parse(int argc, char **argv, struct opt_select *sel)
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

        if (s.help)
                misc_help();

        if (s.version)
                misc_version();

        if (s.count)
                meta_count();

        if (s.initial)
                meta_initial();

        if (s.last)
                meta_last();

        if (s.all && !s.paged)
                all();

        if (s.all && s.paged)
                all_paged();

        if (s.facility && !s.paged) {
                facility();
        }

        if (s.facility && s.paged) {
                facility_paged();
        }

        if (s.severity && !s.paged) {
                severity();
        }

        if (s.severity && s.paged) {
                severity_paged();
        }

        if (s.hostname && !s.paged) {
                hostname();
        }

        if (s.hostname && s.paged) {
                hostname_paged();
        }

        if (s.tag && !s.paged) {
                tag();
        }

        if (s.tag && s.paged) {
                tag_paged();
        }

        if (s.message && !s.paged) {
                tag();
        }

        if (s.message && s.paged) {
                message_paged();
        }

        return EXIT_SUCCESS;
}
