#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"

# -t,--target (default arch)
# -d,--doc (default true)
# -u,--update (default true)
#
# ./init.sh --target arch --doc --update


flag_setup()
{
        DEFINE_string 'target' 'arch' 'target platform' 't'
        DEFINE_boolean 'doc' true 'install documentation tools' 'd'
        DEFINE_boolean 'update' true 'update package manager' 'u'

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


pacman_update()
{
        if [ "$FLAGS_update" -eq "$FLAGS_FALSE" ]; then
                echo "Skipping updates..."
                return
        fi

        checkupdates >/dev/null 2>&1;
        rv=$?

        if [ $rv -eq 1 ]; then
                echo "Failed to determine pending updates, exiting..."
                exit 1
        fi

        if [ $rv -eq 2 ]; then
                echo "No updates pending, skipping..."
                return
        fi

        echo "Updates available, syncing..."

        if ! sudo pacman -Syu --noconfirm; then
                echo "Failed to update packages, exiting..."
                exit 1
        fi
}


pacman_install()
{
        if ! pacman -Qi | grep "$1" >/dev/null 2>&1; then
                echo "Package $1 not found, installing..."
                pacman_update

                if ! sudo pacman -S --noconfirm "$1"; then
                        echo "Failed to install package $1, exiting..."
                        exit 1
                fi
        fi
}


yay_install()
{
        if ! pacman -Qi | grep "$1" >/dev/null 2>&1; then
                echo "Package $1 not found, installing..."
                pacman_update

                if ! yay -S --noconfirm "$1"; then
                        echo "Failed to install package $1, exiting..."
                        exit 1
                fi
        fi
}


enable_systemd()
{
        if ! systemctl start "$1"; then
                echo "Failed to start $1, exiting..."
                exit 1
        fi

        if ! systemctl enable "$1"; then
                echo "Failed to enable $1, exiting..."
                exit 1
        fi
}


init_arch()
{
        if ! pacman -V >/dev/null 2>&1; then
                echo "pacman not found, exiting..."
                exit 1
        fi

        if ! checkupdates -h >/dev/null 2>&1; then
                echo "checkupdates not found, exiting..."
                exit 1
        fi

        if ! yay -V; then
                echo "yay not found, exiting..."
                exit 1
        fi

        pacman_install gcc
        pacman_install make
        pacman_install valgrind
        pacman_install postgresql
        pacman_install postgresql-libs

        if ! systemctl is-enabled postgresql.service \
            | grep enabled >/dev/null 2>&1; then
                if ! sudo -iu postgres \
                    initdb --locale en_US.UTF-8 -D '/var/lib/postgres/data';
                then
                        echo "Failed to initialise postgres cluster, exiting..."
                        exit 1
                fi

                enable_systemd postgresql.service
        fi
}


init_run()
{
        if ! id --version >/dev/null 2>&1; then
                echo "/usr/bin/id not found, exiting..."
                exit 1
        fi

        if [ "$(id -u)" -eq 0 ]; then
                echo "Running as root, exiting..."
                echo "Run as a non-root sudo user"
                exit 1
        fi

        if ! sudo -V >/dev/null 2>&1; then
                echo "sudo not found, exiting..."
                echo "Ensure sudo is installed on your system"
                exit 1
        fi

        if ! git -v >/dev/null 2>&1; then
                echo "git not found, exiting..."
                echo "Ensure git is installed on your system"
                exit 1
        fi

        if [ "$FLAGS_target" = "arch" ]; then
                init_arch
        else
                echo "Unsupported target platform, exiting..."
                exit 1
        fi
}


flag_setup "$@"
init_run
