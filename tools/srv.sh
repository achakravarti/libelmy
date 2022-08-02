. "$(dirname "0")/../tools/msg.sh"
. "$(dirname "0")/../tools/os.sh"
. "$(dirname "0")/../tools/su.sh"


srv_start()
{

        msg_info "starting service $1"

        _emsg="failed to start service $1"
        if [ "$OS_DISTRO" = "FreeBSD" ] ; then
                if ! service "$1" status | grep -q "is running"; then
                        $SU service "$1" start || msg_fail "$_emsg"
                fi
        else
                if ! systemctl is-active --quiet "$1"; then
                        $SU systemctl start "$1" || msg_fail "$_emsg"
                fi
        fi

        msg_ok "service $1 started"
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


srv_enable()
{
        msg_info "enabling service $1"

        _emsg="failed to enable service $1"
        if [ "$OS_DISTRO" = "FreeBSD" ] ; then
                if ! grep -q "$1_enable=\"YES\"" /etc/rc.conf; then
                        $SU sysrc "$1_enable=\"YES\"" || msg_fail "$_emsg"
                fi
        else
                if ! systemctl is-enabled "$1" | grep -q enabled; then
                        $SU systemctl enable "$1" || msg_fail "$_emsg"
                fi
        fi

        msg_ok "service $1 enabled"
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
