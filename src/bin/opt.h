#ifndef __LIBELMY_SRC_BIN_OPT_H__
#define __LIBELMY_SRC_BIN_OPT_H__

#include "../../include/page.h"

#include <libchrysalid/include/utf8.h>


struct opt {
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


extern CY_SAFE struct opt *opt_new(int, char *[]);
extern void opt_free(struct opt **);
extern elmy_page_t *opt_page(const struct opt *o);


#endif /* !__LIBELMY_SRC_BIN_OPT_H__ */
