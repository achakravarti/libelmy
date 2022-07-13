#ifndef __LIBELMY_BIN_SRC_SHOW_H__
#define __LIBELMY_BIN_SRC_SHOW_H__


#include "opt.h"
#include "../../include/error.h"
#include "../../include/logs.h"


extern void show_usage(void);
extern void show_version(void);
extern CY_PSAFE int show_invalid(char *[]);
extern CY_PSAFE int show_missing(char *[]);
extern CY_PSAFE int show_error(const elmy_error_t *);
extern CY_PSAFE int show_logs(const elmy_logs_t *, const struct opt *);


#endif /* !__LIBELMY_BIN_SRC_SHOW_H__ */
