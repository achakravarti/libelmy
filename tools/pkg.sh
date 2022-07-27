. "$(dirname "0")/../tools/msg.sh"
. "$(dirname "0")/../tools/os.sh"
. "$(dirname "0")/../tools/su.sh"


pkg_remove()
{
        msg_info "looking for package $1"

        os_query

        _emsg="failed to remove package $1"
        _omsg="package $1 not found, skipping"
        _imsg="removing package $1, this may take a while"

        case "$OS_DISTRO" in
        Alpine)
                msg_fail "not implemented"
                ;;

        Arch)
                if ! pacman -Qi | tr -s ' ' | grep -wq "Name : $1\$"; then
                        msg_ok "$_omsg"
                        return
                fi

                msg_info "$_imsg"
                yay -Rns --noconfirm "$1" || msg_warn "$_emsg"; return
                ;;

        FreeBSD)
                msg_fail "not implemented"
                ;;

        *)
                msg_fail "not implemented"
                ;;
        esac

        msg_ok "remove package $1"
}

pkg_install()
{
        msg_info "looking for package $1"

        os_query

        _emsg="failed to install package $1"
        _omsg="package $1 found, skipping"
        _imsg="installing package $1, this may take a while"

        case "$OS_DISTRO" in
        Alpine)
                if apk info | grep -wq "$1"; then
                        msg_ok "$_omsg"
                        return
                fi

                msg_info "$_imsg"
                $SU apk add "$1" || msg_fail "$_emsg"
                ;;

        Arch)
                if pacman -Qi | tr -s ' ' | grep -wq "$1\$"; then
                        msg_ok "$_omsg"
                        return
                fi

                msg_info "$_imsg"
                yay -S --needed --noconfirm "$1" || msg_fail "$_emsg"
                ;;

        FreeBSD)
                if pkg info | grep -wq "$1"; then
                        msg_ok "$_omsg"
                        return
                fi

                msg_info "$_imsg"
                $SU pkg install -y "$1" || msg_fail "$_emsg"
                ;;

        *)
                if dpkg -l | grep -wq "$1"l; then
                        msg_ok "$_omsg"
                        return
                fi

                msg_info "$_imsg"
                $SU apt install -y "$1" || msg_fail "$_emsg"
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
                ($SU apk update && $SU apk upgrade) || msg_fail "$_emsg"
                ;;

        Arch)
                $SU pacman -Syu --noconfirm || msg_fail "$_emsg"
                ;;

        FreeBSD)
                ($SU pkg update && $SU pkg upgrade -y) || msg_fail "$_emsg"
                ;;

        *)
                ($SU apt update -y && $SU apt upgrade -y) || msg_fail "$_emsg"
                ;;
        esac

        msg_ok "upgraded packages"
}
