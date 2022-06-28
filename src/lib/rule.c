#include "../../include/rule.h"

#include <libchrysalid/log.h>

#include <assert.h>


bool elmy_rule_count(size_t *res, cy_utf8_t **err)
{
        assert(res);

        return false;
}


bool elmy_rule_first(const char *tz, cy_utf8_t **res, cy_utf8_t **err)
{
        return false;
}


bool elmy_rule_last(const char *tz, cy_utf8_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        return false;
}


bool elmy_rule_all(const char *tz, const struct elmy_page *pg,
                   elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        return false;
}


bool elmy_rule_facility(const char *tz, const struct elmy_page *pg,
                        enum cy_log_facility filter[], size_t nfilter,
                        elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        return false;
}


bool elmy_rule_severity(const char *tz, const struct elmy_page *pg,
                        enum cy_log_severity filter[], size_t nfilter,
                        elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        return false;
}


bool elmy_rule_hostname(const char *tz, const struct elmy_page *pg,
                        const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        return false;
}


bool elmy_rule_tag(const char *tz, const struct elmy_page *pg,
                   const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        return false;
}


bool elmy_rule_message(const char *tz, const struct elmy_page *pg,
                       const char *filter, elmy_logs_t **res, cy_utf8_t **err)
{
        assert(tz);
        assert(*tz);
        assert(res);
        assert(!*res);

        return false;
}
