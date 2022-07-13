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
extern CY_SAFE elmy_page_t *opt_page(const struct opt *o);


typedef struct bin_opt__ bin_opt_t;

extern CY_SAFE bin_opt_t *bin_opt_new(int, char *[]);
extern void opt_free(bin_opt_t **);

extern CY_PSAFE bool bin_opt_error(const bin_opt_t *);
extern CY_PSAFE bool bin_opt_help(const bin_opt_t *);
extern CY_PSAFE bool bin_opt_json(const bin_opt_t *);
extern CY_PSAFE bool bin_opt_version(const bin_opt_t *);
extern CY_SAFE cy_utf8_t *bin_opt_tz(const bin_opt_t *);
extern CY_SAFE cy_utf8_t *bin_opt_filter(const bin_opt_t *);
extern CY_SAFE elmy_page_t *bin_opt_page(const bin_opt_t *);

#endif /* !__LIBELMY_SRC_BIN_OPT_H__ */
