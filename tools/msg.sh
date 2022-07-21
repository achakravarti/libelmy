
# MESSAGE FUNCTIONS


msg_ok()
{
        printf '[\033[0;32m OK \033[0m] %s...\n' "$1"
}


msg_info()
{
        printf '[\033[0;34mINFO\033[0m] %s...\n' "$1"
}


msg_warn()
{
        printf '[\033[1;33mWARN\033[0m] %s...\n' "$1"
}


msg_fail()
{
        printf '[\033[1;33mFAIL\033[0m] %s, exiting...\n' "$1"
        exit 1
}
