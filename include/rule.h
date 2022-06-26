#include "log.h"
#include "page.h"

#include "../../libchrysalid/include/utf8.h"

#include <stddef.h>


typedef struct elmy_rule_count__ elmy_rule_count_t;

extern elmy_rule_count_t *elmy_rule_count_new(void);
extern elmy_rule_count_t *elmy_rule_count_copy(const elmy_rule_count_t *);
extern elmy_rule_count_t *elmy_rule_count_clone(const elmy_rule_count_t *);
extern void elmy_rule_count_t_free__(elmy_rule_count_t **);
extern bool elmy_rule_count_valid(const elmy_rule_count_t *);
extern size_t elmy_rule_count_exec(const elmy_rule_count_t *);


typedef struct elmy_rule_first__ elmy_rule_first_t;

extern elmy_rule_first_t *elmy_rule_first_new(const char *);
extern void elmy_rule_first_t_free__(elmy_rule_first_t **);
extern elmy_rule_first_t *elmy_rule_first_copy(const elmy_rule_first_t *);
extern elmy_rule_first_t *elmy_rule_first_clone(const elmy_rule_first_t *);
extern bool elmy_rule_first_valid(const elmy_rule_first_t *);
extern cy_utf8_t *elmy_rule_first_exec(const elmy_rule_first_t *);


typedef struct elmy_rule_last__ elmy_rule_last_t;

extern elmy_rule_last_t *elmy_rule_last_new(const char *);
extern void elmy_rule_last_t_free__(elmy_rule_last_t **);
extern elmy_rule_last_t *elmy_rule_last_copy(const elmy_rule_last_t *);
extern elmy_rule_last_t *elmy_rule_last_clone(const elmy_rule_last_t *);
extern bool elmy_rule_last_valid(const elmy_rule_last_t *);
extern cy_utf8_t *elmy_rule_last_exec(const elmy_rule_last_t *);


typedef struct elmy_rule_all__ elmy_rule_all_t;

extern elmy_rule_all_t *elmy_rule_all_new(const char *, const struct elmy_page *);
extern void elmy_rule_all_t_free__(elmy_rule_all_t **);
extern elmy_rule_all_t *elmy_rule_all_copy(const elmy_rule_all_t *);
extern elmy_rule_all_t *elmy_rule_all_clone(const elmy_rule_all_t *);
extern bool elmy_rule_all_valid(const elmy_rule_all_t *);
extern elmy_logs_t *elmy_rule_all_exec(const elmy_rule_all_t *);


typedef struct elmy_rule_facility__ elmy_rule_facility_t;

extern elmy_rule_facility_t *elmy_rule_facility_new(const char *, const struct elmy_page *, int[], size_t);
extern void elmy_rule_facility_t_free__(elmy_rule_facility_t **);
extern elmy_rule_facility_t *elmy_rule_facility_copy(const elmy_rule_facility_t *);
extern elmy_rule_facility_t *elmy_rule_facility_clone(const elmy_rule_facility_t *);
extern bool elmy_rule_facility_valid(const elmy_rule_facility_t *);
extern elmy_logs_t *elmy_rule_facility_exec(const elmy_rule_facility_t *);


typedef struct elmy_rule_severity__ elmy_rule_severity_t;

extern elmy_rule_severity_t *elmy_rule_severity_new(const char *, const struct elmy_page *, int[], size_t);
extern void elmy_rule_severity_t_free__(elmy_rule_severity_t **);
extern elmy_rule_severity_t *elmy_rule_severity_copy(const elmy_rule_severity_t *);
extern elmy_rule_severity_t *elmy_rule_severity_clone(const elmy_rule_severity_t *);
extern bool elmy_rule_severity_valid(const elmy_rule_severity_t *);
extern elmy_logs_t *elmy_rule_severity_exec(const elmy_rule_severity_t *);


typedef struct elmy_rule_hostname__ elmy_rule_hostname_t;

extern elmy_rule_hostname_t *elmy_rule_hostname_new(const char *, const struct elmy_page *, const char *);
extern void elmy_rule_hostname_t_free__(elmy_rule_hostname_t **);
extern elmy_rule_hostname_t *elmy_rule_hostname_copy(const elmy_rule_hostname_t *);
extern elmy_rule_hostname_t *elmy_rule_hostname_clone(const elmy_rule_hostname_t *);
extern bool elmy_rule_hostname_valid(const elmy_rule_hostname_t *);
extern elmy_logs_t *elmy_rule_hostname_exec(const elmy_rule_hostname_t *);


typedef struct elmy_rule_tag__ elmy_rule_tag_t;

extern elmy_rule_tag_t *elmy_rule_tag_new(const char *, const struct elmy_page *, const char *);
extern void elmy_rule_tag_t_free__(elmy_rule_tag_t **);
extern elmy_rule_tag_t *elmy_rule_tag_copy(const elmy_rule_tag_t *);
extern elmy_rule_tag_t *elmy_rule_tag_clone(const elmy_rule_tag_t *);
extern bool elmy_rule_tag_valid(const elmy_rule_tag_t *);
extern elmy_logs_t *elmy_rule_tag_exec(const elmy_rule_tag_t *);


typedef struct elmy_rule_message__ elmy_rule_message_t;

extern elmy_rule_message_t *elmy_rule_message_new(const char *, const struct elmy_page *, const char *);
extern void elmy_rule_message_t_free__(elmy_rule_message_t **);
extern elmy_rule_message_t *elmy_rule_message_copy(const elmy_rule_message_t *);
extern elmy_rule_message_t *elmy_rule_message_clone(const elmy_rule_message_t *);
extern bool elmy_rule_message_valid(const elmy_rule_message_t *);
extern elmy_logs_t *elmy_rule_message_exec(const elmy_rule_message_t *);
