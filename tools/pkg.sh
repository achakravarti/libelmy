. "$(dirname "0")/../tools/msg.sh"
. "$(dirname "0")/../tools/os.sh"


pkg_install()
{
        msg_info "installing package $1, this may take a while"

        os_query
        _emsg="failed to install package $1"
        _omsg="package $1 found, skipping"

        case "$OS_DISTRO" in
        Alpine)
                if apk info | grep "$1" >/dev/null 2>&1; then
                        msg_ok "$_omsg"
                        return
                fi

                ! sudo apk add "$1" && msg_fail "$_emsg"
                ;;

        Arch)
                if pacman -Qi | grep "$1" >/dev/null 2>&1; then
                        msg_ok "$_omsg"
                        return
                fi

                ! yay -S --needed --noconfirm "$1" && msg_fail "$_emsg"
                ;;

        FreeBSD)
                if pkg info | grep "$1" >/dev/null 2>&1; then
                        msg_ok "$_omsg"
                        return
                fi

                ! sudo pkg install -y "$1" && msg_fail "$_emsg"
                ;;

        *)
                if dpkg -l | grep "$1" >/dev/null 2>&1; then
                        msg_ok "$_omsg"
                        return
                fi

                ! sudo apt install -y "$1" && msg_fail "$_emsg"
                ;;
        esac

        msg_ok "installed package $1"
}


pkg_upgrade()
{
        msg_info "upgrading packages, this may take a while"

        os_query
        _emsg="failed to upgrade packages"

        case "$OS_DISTRO" in
        Alpine)
                ! sudo apk update && apk upgrade && msg_fail "$_emsg"
                ;;

        Arch)
                ! sudo pacman -Syu --noconfirm && msg_fail "$_emsg"
                ;;

        FreeBSD)
                ! sudo pkg update && pkg upgrade -y && msg_fail "$_emsg"
                ;;

        *)
                ! sudo apt update -y && apt upgrade -y && msg_fail "$_emsg"
                ;;
        esac

        msg_ok "upgraded packages"
}
