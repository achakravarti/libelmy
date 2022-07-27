. "$(dirname "0")/../tools/msg.sh"
. "$(dirname "0")/../tools/os.sh"
. "$(dirname "0")/../tools/su.sh"


srv_restart()
{
        _emsg="failed to restart service $1"
        _imsg="restarting service $1"
        _omsg="service $1 restarted"

        msg_info "$_imsg"

        if [ "$OS_DISTRO" = "FreeBSD" ] ; then
                $SU service "$1" restart || msg_fail "$_emsg"
        else
                $SU systemctl restart "$1" || msg_fail "$_emsg"
        fi

        msg_ok "$_omsg"
}


srv_enable()
{
        _emsg="failed to enable service $1"
        _imsg="enabling service $1"
        _omsg="service $1 enabled"

        msg_info "$_imsg"

        if [ "$OS_DISTRO" = "FreeBSD" ] ; then
                if ! grep -q "$1_enable=\"YES\"" /etc/rc.conf; then
                        $SU sysrc "$1_enable=\"YES\"" || msg_fail "$_emsg"
                fi

                if ! service "$1" status | grep -q "is running"; then
                        $SU service "$1" start || msg_fail "$_emsg"
                fi
        else
                if ! systemctl is-enabled "$1" | grep -q enabled; then
                        $SU systemctl enable "$1" || msg_fail "$_emsg"
                fi

                if ! systemctl is-active --quiet "$1"; then
                        $SU systemctl start "$1" || msg_fail "$_emsg"
                fi
        fi

        msg_ok "$_omsg"
}
