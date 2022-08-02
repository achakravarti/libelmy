. "$(dirname "0")/../tools/msg.sh"
. "$(dirname "0")/../tools/os.sh"
. "$(dirname "0")/../tools/su.sh"


srv_enable()
{
        msg_info "checking if service $1 enabled"

        _emsg="failed to enable service $1"
        _omsg1="service $1 already enabled, skipping"
        _omsg2="service $1 enabled"

        case "$OS_DISTRO" in
        Alpine)
                msg_fail 'not implemented';;

        FreeBSD)
                if grep -q "$1_enable=\"YES\"" /etc/rc.conf; then
                        msg_ok "$_omsg1"
                else
                        $SU sysrc "$1_enable=\"YES\"" || msg_fail "$_emsg"
                        msg_ok "$_omsg2"
                fi;;

        *)
                if systemctl is-enabled "$1" | grep -q enabled; then
                        msg_ok "$_omsg1"
                else
                        $SU systemctl enable "$1" || msg_fail "$_emsg"
                        msg_ok "$_omsg2"
                fi;;
        esac
}


srv_start()
{
        msg_info "checking if service $1 started"

        _emsg="failed to start service $1"
        _omsg1="service $1 already started, skipping"
        _omsg2="service $1 started"

        case "$OS_DISTRO" in
        Alpine)
                msg_fail 'not implemented';;

        FreeBSD)
                if service "$1" status | grep -q 'is running'; then
                        msg_ok "$_omsg1"
                else
                        $SU service "$1" start || msg_fail "$_emsg"
                        msg_ok "$_omsg2"
                fi;;

        *)
                if systemctl is-active --quiet "$1"; then
                        msg_ok "$_omsg1"
                else
                        $SU systemctl start "$1" || msg_fail "$_emsg"
                        msg_ok "$_omsg2"
                fi;;
        esac
}


srv_restart()
{
        msg_info "restarting service $1"

        _emsg="failed to restart service $1"
        if [ "$OS_DISTRO" = "FreeBSD" ] ; then
                $SU service "$1" restart || msg_fail "$_emsg"
        else
                $SU systemctl restart "$1" || msg_fail "$_emsg"
        fi

        msg_ok "service $1 restarted"
}


srv_stop()
{
        msg_info "stopping service $1"

        _emsg="failed to stop service $1"
        if [ "$OS_DISTRO" = "FreeBSD" ] ; then
                $SU service "$1" stop || msg_fail "$_emsg"
        else
                $SU systemctl stop "$1" || msg_fail "$_emsg"
        fi

        msg_ok "service $1 stopped"
}
