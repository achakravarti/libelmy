
# MESSAGE FUNCTIONS


msg_ok()
{
        printf '[\033[0;32m OK \033[0m] \033[0;35m%s\033[0m: %s...\n'   \
            "$(date +'%b %d %H:%M:%S')"                                 \
            "$1"
}


msg_info()
{
        printf '[\033[0;34mINFO\033[0m] \033[0;35m%s\033[0m: %s...\n'   \
            "$(date +'%b %d %H:%M:%S')"                                 \
            "$1"
}


msg_warn()
{
        printf '[\033[1;33mWARN\033[0m] \033[0;35m%s\033[0m: %s...\n'   \
            "$(date +'%b %d %H:%M:%S')"                                 \
            "$1"
}


msg_fail()
{
        printf '[\033[1;31mFAIL\033[0m] \033[0;35m%s\033[0m: %s, exiting...\n'  \
            "$(date +'%b %d %H:%M:%S')"                                         \
            "$1"

        exit 1
}
