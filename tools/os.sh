
. "$(dirname "0")/../tools/msg.sh"


export OS_KERNEL
export OS_DISTRO
export OS_VERSTR
export OS_VERNUM


os_query()
{
        os_kernel__
        os_distro__
        os_version__
}


os_kernel__()
{
        msg_info "checking OS"

        OS_KERNEL=$(uname -s)

        _emsg="unsupported kernel: $OS_KERNEL"
        [ "$OS_KERNEL" != FreeBSD ]             \
            && [ "$OS_KERNEL" != Linux ]        \
            && msg_fail _emsg

        if [ "$OS_KERNEL" = Linux ]; then
                _emsg="outdated Linux kernel: $OS_KERNEL"
                [ -f /etc/os-release ] || msg_fail _emsg
        fi
}


os_distro__()
{
        if [ "$OS_KERNEL" = FreeBSD ]; then
                OS_DISTRO=FreeBSD
                return
        fi

        OS_DISTRO=$(grep 'NAME' /etc/os-release \
            | head -n 1                         \
            | cut -d '=' -f 2                   \
            | tr -d '"'                         \
            | cut -d ' ' -f 1)

        _emsg="unsupported Linux distribution: $OS_DISTRO"
        [ "$OS_DISTRO" != Alpine ]              \
            && [ "$OS_DISTRO" != Arch ]         \
            && [ "$OS_DISTRO" != Debian ]       \
            && [ "$OS_DISTRO" != Ubuntu ]       \
            && msg_fail _emsg
}


os_version__()
{
        if [ "$OS_DISTRO" = "Alpine" ] || [ "$OS_DISTRO" = "Arch" ]; then
                OS_VERSTR=rolling
                OS_VERNUM=0

                msg_ok "detected $OS_DISTRO Linux"
                return
        fi

        if [ "$OS_DISTRO" = FreeBSD ]; then
                OS_VERSTR=$(uname -r)

                OS_VERNUM=$(echo "$OS_VERSTR"   \
                    | cut -d '-' -f 1           \
                    | tr -d '.')

                _emsg="outdated FreeBSD version: $OS_VERSTR"
                [ "$OS_VERNUM" -lt 122 ] && msg_fail _emsg

                msg_ok "detected FreeBSD $OS_VERSTR"
                return
        fi

        if [ "$OS_DISTRO" = Debian ]; then
                OS_VERSTR=$(grep 'VERSION' /etc/os-release      \
                    | tr -d '"'                                 \
                    | cut -d '=' -f 2)

                OS_VERNUM=$(echo "$OS_VERSTR" | tr -d 'A-Z a-z ( ) .')

                _emsg="outdated Debian version: $OS_VERSTR"
                [ "$OS_VERNUM" -lt 10 ] && msg_fail _emsg

                msg_ok "detected Debian $OS_VERSTR"
                return
        fi

        if [ "$OS_DISTRO" = Ubuntu ]; then
                OS_VERSTR=$(grep 'VERSION_ID' /etc/os-release   \
                    | tr -d '"'                                 \
                    | cut -d '=' -f 2)

                OS_VERNUM=$(echo "$OS_VERSTR" | tr -d 'A-Z a-z ( ) .')

                _emsg="outdated Ubuntu version: $OS_VERSTR"
                [ "$OS_VERNUM" -lt 2004 ] && msg_fail _emsg

                msg_ok "detected Ubuntu $OS_VERSTR"
                return
        fi
}
