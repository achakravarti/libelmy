#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"
. "$(dirname "0")/../tools/msg.sh"

# -t,--target (default arch)
# -p,--pandoc (default true)
# -u,--update (default true)
# -c,--check (default true)
#
# ./init.sh --target arch --pandoc --update --check


# MAIN FUNCTIONS


main_flags()
{
        DEFINE_string 'target' 'arch' 'target platform' 't'
        DEFINE_boolean 'pandoc' true 'install pandoc' 'p'
        DEFINE_boolean 'update' true 'update package manager' 'u'
        DEFINE_boolean 'check' true 'support check tools' 'u'

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


main_run()
{
        if ! id --version >/dev/null 2>&1; then
                msg_fail "id not found"
        fi

        if [ "$(id -u)" -eq 0 ]; then
                msg_fail "running as root is dangerous"
        fi

        if ! sudo -V >/dev/null 2>&1; then
                msg_fail "sudo not found"
        fi

        if ! git -v >/dev/null 2>&1; then
                msg_fail "git not found"
        fi

        if [ "$FLAGS_target" = "arch" ]; then
                arch_init
        else
                msg_fail "unsupported target platform"
        fi
}


# SYSTEMD FUNCTIONS


systemd_enabled()
{
        if systemctl is-enabled postgresql.service \
            | grep enabled >/dev/null 2>&1; then
                rv=0
        else
                rv=1
        fi
}


systemd_enable()
{
        msg_info "starting systemd service $1"

        if ! sudo systemctl start "$1"; then
                msg_fail "failed to start service $1"
        fi

        msg_ok "service $1 started"
        msg_info "enabling systemd service $1"

        if ! sudo systemctl enable "$1"; then
                msg_fail "failed to enable service $1"
        fi

        msg_ok "service $1 enabled"
}



# ARCH LINUX FUNCTIONS


arch_update()
{
        if [ "$FLAGS_update" -eq "$FLAGS_FALSE" ]; then
                    mgs_warn "--noupdate set, skipping updates"
                    return
        fi

        msg_info "checking for package updates"

        checkupdates >/dev/null 2>&1;
        rv=$?

        if [ $rv -eq 1 ]; then
                    msg_fail "failed to determine package updates"
                    exit 1
        fi

        if [ $rv -eq 2 ]; then
                    msg_ok "no package updates pending, skipping"
                    return
        fi

        msg_ok "package updates available"
        msg_info "starting update"

        if ! sudo pacman -Syu --noconfirm; then
                msg_fail "failed to update packages"
        fi

        msg_ok "package update complete"
}


arch_install()
{
        msg_info "checking package $1"

        if pacman -Qi | grep "$1" >/dev/null 2>&1; then
                msg_ok "package $1 found, skipping"
                return
        fi

        msg_info "package $1 not found, installing"

        arch_update

        if [ "$1" -eq 0 ]; then
                if ! sudo pacman -S --noconfirm "$1"; then
                        mgs_fail "failed to install package $1"
                fi
        else
                if ! yay -S --noconfirm "$1"; then
                        msg_fail "failed to install package $1"
                fi
        fi

        msg_ok "installed package $1"
}


arch_init()
{
        if ! pacman -V >/dev/null 2>&1; then
                mgs_fail "pacman not found"
        fi

        if ! checkupdates -h >/dev/null 2>&1; then
                msg_fail "checkupdates not found"
        fi

        if ! yay -V; then
                msg_fail "yay not found"
        fi

        arch_install gcc 0
        arch_install make 0
        arch_install postgresql 0
        arch_install postgresql-libs 0
        arch_install rsyslog 1

        if [ "$FLAGS_pandoc" -eq "$FLAGS_TRUE" ]; then
                arch_install pandoc 0
        else
                msg_warn "--nopandoc set, skipping pandoc installation"
        fi

        if [ "$FLAGS_check" -eq "$FLAGS_TRUE" ]; then
                arch_install valgrind 0
                arch_install criterion 1
        else
                msg_warn "--nocheck set, skipping valgrind installation"
                msg_warn "--nocheck set, skipping criterion installation"
        fi

        systemd_enabled postgresql.service
        if [ "$rv" -ne 0 ]; then
                if ! sudo -u postgres \
                    initdb --locale en_US.UTF-8 -D '/var/lib/postgres/data';
                then
                        msg_fail "failed to initialise postgres cluster"
                fi

                systemd_enable postgresql.service
        fi
}


# MAIN ENTRY POINT


main_flags "$@"
main_run
