#include "../../libchrysalid/include/ext.h"

#include <stdbool.h>
#include <stddef.h>


enum elmy_sort {
        ELMY_SORT_TS = 1,
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
