#ifndef __LIBELMY_SRC_BIN_OPT_H__
#define __LIBELMY_SRC_BIN_OPT_H__

#include "../../include/page.h"

#include <libchrysalid/include/utf8.h>


struct bin_opt {
        bool             error;
        bool             help;
        bool             json;
        bool             unpaged;
        bool             version;
        cy_utf8_t       *timezone;
        cy_utf8_t       *filter;
        cy_utf8_t       *sortcol;
        cy_utf8_t       *sortdir;
        cy_utf8_t       *rowstart;
        cy_utf8_t       *rowcount;
};


extern CY_SAFE struct bin_opt *bin_opt_new(int, char *[]);
extern void bin_opt_free(struct bin_opt **);
extern CY_SAFE elmy_page_t *bin_opt_page(const struct bin_opt *o);

#endif /* !__LIBELMY_SRC_BIN_OPT_H__ */
