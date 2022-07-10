#ifndef __LIBELMY_BIN_SRC_SHOW_H__
#define __LIBELMY_BIN_SRC_SHOW_H__

#include "../../include/error.h"


extern void show_usage(void);
extern void show_version(void);
extern CY_PSAFE int show_invalid(char *[]);
extern CY_PSAFE int show_missing(char *[]);
extern CY_PSAFE int show_error(const elmy_error_t *);


#endif /* !__LIBELMY_BIN_SRC_SHOW_H__ */
