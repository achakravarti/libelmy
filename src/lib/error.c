#include "../../include/error.h"

#include <assert.h>


void elmy_error_dbconn(enum elmy_error erno, const char *msg)
{
        fprintf(stderr, "Error %d: Failed to connect to database: %s\n",
                erno, msg);
        cy_log_err("Error %d: Failed to connect to database: %s\n", erno, msg);
        exit(EXIT_FAILURE);
}

void elmy_error_dbqry(enum elmy_error erno, const char *msg)
{
        fprintf(stderr, "Error %d: Failed to execute query: %s\n", erno, msg);
        cy_log_err("Error %d: Failed to execute query: %s\n", erno, msg);
        exit(EXIT_FAILURE);
}


struct elmy_error__ {
        enum elmy_status     status;
        cy_utf8_t           *msg;
        cy_utf8_t           *imsg;
};


elmy_error_t *elmy_error_new(enum elmy_status status, const char *msg,
                             const char *imsg)
{
        return 0;
}


elmy_error_t *elmy_error_copy(const elmy_error_t *ctx)
{
        return 0;
}


elmy_error_t *elmy_error_clone(const elmy_error_t *ctx)
{
        return 0;
}


void elmy_error_t_free__(elmy_error_t **ctx)
{
}


enum elmy_status elmy_error_status(const elmy_error_t *ctx)
{
        return 0;
}


cy_utf8_t *elmy_error_msg(const elmy_error_t *ctx)
{
        return 0;
}


cy_utf8_t *elmy_error_msg_inner(const elmy_error_t *ctx)
{
        return 0;
}


cy_utf8_t *elmy_error_str(const elmy_error_t *ctx)
{
        return 0;
}


cy_json_t *elmy_error_json(const elmy_error_t *ctx)
{
        return 0;
}
