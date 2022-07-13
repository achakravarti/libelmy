/* Header guard */
#ifndef __LIBELMY_INCLUDE_PAGE_H__
#define __LIBELMY_INCLUDE_PAGE_H__

#include <libchrysalid/include/ext.h>

#include <stdbool.h>
#include <stddef.h>

/* C++ compatiblity */
#ifdef __cplusplus
extern "C" {
#endif


enum elmy_sort {
        ELMY_SORT_TS = 1,
        ELMY_SORT_TS_EVENT,
        ELMY_SORT_FACILITY,
        ELMY_SORT_SEVERITY,
        ELMY_SORT_HOSTNAME,
        ELMY_SORT_TAG,
        ELMY_SORT_MESSAGE
};


struct elmy_page {
        size_t          row_start;
        size_t          row_count;
        enum elmy_sort  sort_col;
        bool            sort_asc;
};


typedef struct elmy_page__  elmy_page_t;


extern CY_RSAFE elmy_page_t *
elmy_page_new(size_t, size_t, enum elmy_sort, bool);

extern CY_RSAFE elmy_page_t *
elmy_page_new_disabled(void);

extern CY_SAFE elmy_page_t *
elmy_page_new_parse(const char *, const char *, const char *, const char *);

extern CY_SAFE elmy_page_t *
elmy_page_copy(const elmy_page_t *);

extern CY_SAFE elmy_page_t *
elmy_page_clone(const elmy_page_t *);

extern void
elmy_page_t_free__(elmy_page_t **);

#define elmy_page_free(ctx) \
elmy_page_t_free__(ctx)

extern CY_PSAFE bool
elmy_page_disabled(const elmy_page_t *);

extern CY_SAFE const char *
elmy_page_start(const elmy_page_t *);

extern CY_SAFE const char *
elmy_page_count(const elmy_page_t *);

extern CY_SAFE const char *
elmy_page_col(const elmy_page_t *);

extern CY_SAFE const char *
elmy_page_dir(const elmy_page_t *);


/* C++ compatiblity */
#ifdef __cplusplus
}
#endif

/* Header guard */
#endif /* !__LIBELMY_INCLUDE_PAGE_H__ */
