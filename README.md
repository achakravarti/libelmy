# libelmy

## C Interface

``` C

enum elmy_sortcol {
    ELMY_SORTCOL_TS,
    ELMY_SORTCOL_FACILITY,
    ELMY_SORTCOL_SEVERITY,
    ELMY_SORTCOL_HOSTNAME,
    ELMY_SORTCOL_TAG,
    ELMY_SORTCOL_MESSAGE
};


struct elmy_pagination {
   size_t row_start;
   size_t row_count;
   enum elmy_sortcol sort_col;
   bool sort_asc;
};

struct elmy_log_t {
    cy_utf8_t *event_at;
    cy_utf8_t *logged_at;
    cy_utf8_t *facility_id;
    cy_utf8_t *facility_keyword;
    cy_utf8_t *severity_id;
    cy_utf8_t *severity_keyword;
    cy_utf8_t *hostname;
    cy_utf8_t *tag;
    cy_utf8_t *message;
};

int elmy_logs_count()

cy_utf8_t *elmy_logs_ts_first(const char *tz)

cy_utf8_t *elmy_logs_ts_last(const char *tz)

elmy_logs_t *elmy_logs_all(const char *tz)

elmy_logs_t *elmy_logs_all_paged(const char *tz, const struct elmy_pagination *)

elmy_logs_t *elmy_logs_facility(const char *tz, enum cy_log_facility filter[], int len)

elmy_logs_t *elmy_logs_facility_paged(const char *tz, enum cy_log_facility filter[], int len, const struct elmy_pagination *)

elmy_logs_t *elmy_logs_severity(const char *tz)

elmy_logs_t *elmy_logs_severity_paged(const char *tz)

elmy_logs_t *elmy_logs_hostname(const char *tz, const char *filter)

elmy_logs_t *elmy_logs_hostname_paged(const char *tz, const char *filter, const struct elmy_pagination *)

elmy_logs_t *elmy_logs_tag(const char *tz)

elmy_logs_t *elmy_logs_tag_paged(const char *tz)

elmy_logs_t *elmy_logs_message(const char *tz)

elmy_logs_t *elmy_logs_message_paged(const char *tz)

```

elmy -c --count
elmy -i --initial
elmy -l --last
elmy -a --all
elmy -f --facility=1,2,5
elmy -s --severity=0,3
elmy -h --hostname
elmy -t --tag
elmy -m --message
elmy -p --paged
elmy -h --help
elmy -v --version
