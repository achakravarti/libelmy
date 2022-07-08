#include "../../include/rule.h"

#include <libchrysalid/include/ext.h>

#include <stdio.h>
#include <stdlib.h>


static int hnd_all(void);
static int hnd_all_paged(void);


int hnd_all(void)
{
        CY_AUTO(elmy_page_t) *pg = elmy_page_new_disabled();
        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_all("asia/kolkata", pg, &res, &err))) {
                elmy_error_str(err);
                return elmy_error_status(err);
        }

        CY_AUTO(cy_utf8_t) *s = elmy_logs_print(res, ELMY_LOGS_FORMAT_JSON);
        printf("%s\n", s);
        return EXIT_SUCCESS;
}


int hnd_all_paged(void)
{
        CY_AUTO(elmy_page_t) *pg = elmy_page_new(1, 25, ELMY_SORT_TS_EVENT,
                                                 false);

        CY_AUTO(elmy_logs_t) *res = NULL;
        CY_AUTO(elmy_error_t) *err = NULL;

        if (CY_UNLIKELY(elmy_rule_all("asia/kolkata", pg, &res, &err))) {
                elmy_error_str(err);
                return elmy_error_status(err);
        }

        CY_AUTO(cy_utf8_t) *s = elmy_logs_print(res, ELMY_LOGS_FORMAT_JSON);
        printf("%s\n", s);
        return EXIT_SUCCESS;
}
