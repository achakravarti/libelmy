#ifndef __LIBELMY_BIN_SRC_SHOW_H__
#define __LIBELMY_BIN_SRC_SHOW_H__


#include "opt.h"
#include "../../include/error.h"
#include "../../include/logs.h"

/* Prototypes for printing routines */

extern void print_usage(void);
extern void print_version(void);
extern CY_PSAFE int print_invalid(char *[]);
extern CY_PSAFE int print_missing(char *[]);
extern CY_PSAFE int print_error(const elmy_error_t *);
extern CY_PSAFE int print_logs(const elmy_logs_t *, const struct bin_opt *);


#endif /* !__LIBELMY_BIN_SRC_SHOW_H__ */
