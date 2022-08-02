
# MESSAGE FUNCTIONS


msg_ok()
{
        _ts="\033[0;33m]$(date +'%b %d %H:%M:%S')\033[0m]"
        printf '[\033[0;32m OK \033[0m] %s: %s...\n' "$_ts" "$1"
}


msg_info()
{
        _ts="\033[0;33m]$(date +'%b %d %H:%M:%S')\033[0m]"
        printf '[\033[0;34mINFO\033[0m] %s: %s...\n' "$_ts" "$1"
}


msg_warn()
{
        _ts="\033[0;33m]$(date +'%b %d %H:%M:%S')\033[0m]"
        printf '[\033[1;33mWARN\033[0m] %s: %s...\n' "$_ts" "$1"
}


msg_fail()
{
        _ts="\033[0;33m]$(date +'%b %d %H:%M:%S')\033[0m]"
        printf '[\033[1;31mFAIL\033[0m] %s: %s, exiting...\n' "$_ts" "$1"

        exit 1
}
