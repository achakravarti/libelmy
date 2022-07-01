#include "../../include/error.h"


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
