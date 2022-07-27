. "$(dirname "0")/../tools/msg.sh"


export SU


if sudo -V >/dev/null 2>&1; then
        SU=sudo

elif doas -L >/dev/null 2>&1; then
        SU=doas

else
        msg_fail 'sudo/doas not found'
fi


id --version >/dev/null 2>&1 || msg_fail 'id not found'
[ "$(id -u)" -eq 0 ] || msg_fail 'running as root is dangerous'
