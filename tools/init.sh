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

        /usr/bin/check/updates >/dev/null 2>&1;
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

        if ! /usr/bin/sudo /usr/bin/pacman -Syu --noconfirm; then
                echo "Failed to update packages, exiting..."
                exit 1
        fi
}


pacman_install()
{
        if ! /usr/bin/sudo /usr/bin/pacman -S --noconfirm --needed "$1"; then
                echo "Failed to install $1, exiting..."
                exit 1
        fi
}


enable_systemd()
{
        if ! /usr/bin/systemctl start "$1"; then
                echo "Failed to start $1, exiting..."
                exit 1
        fi

        if ! /usr/bin/systemctl enable "$1"; then
                echo "Failed to enable $1, exiting..."
                exit 1
        fi
}


init_arch()
{
        if ! /usr/bin/pacman -V >/dev/null 2>&1; then
                echo "/usr/bin/pacman not found, exiting..."
                echo "Are you sure you are running Arch Linux?"
                exit 1
        fi

        if ! /usr/bin/checkupdates -h >/dev/null 2>&1; then
                echo "/usr/bin/checkupdates not found, exiting..."
                echo "Are you sure you are running Arch Linux?"
                exit 1
        fi

        if ! /usr/bin/yay -V; then
                echo "/usr/bin/yay not found, exiting..."
                echo "Are you sure you are running Arch Linux?"
                exit 1
        fi

        if ! /usr/bin/gcc -v >/dev/null 2>&1; then
                echo "/usr/bin/gcc not found, installing..."
                pacman_update
                pacman_install gcc
        fi

        if ! /usr/bin/make -v >/dev/null 2>&1; then
                echo "/usr/bin/make not found, installing..."
                pacman_update
                pacman_install make
        fi

        if ! /usr/bin/postgres -V >/dev/null 2>&1; then
                echo "/usr/bin/postgres not found, installing..."
                pacman_update
                pacman_install postgresql
                pacman_install postgresql-libs

                if ! su - postgres \
                    -c "initdb --locale en_US.UTF-8 -D '/var/lib/postgres/data'";
                then
                        echo "Failed to initialise postgres cluster, exiting..."
                        exit 1
                fi

                enable_systemd postgresql.service
        fi
}


init_run()
{
        if ! /usr/bin/id --version >/dev/null 2>&1; then
                echo "/usr/bin/id not found, exiting..."
                echo "Are you sure you are running a supported system?"
                exit 1
        fi

        if [ "$(/usr/bin/id -u)" -eq 0 ]; then
                echo "Running as root, exiting..."
                echo "Run as a non-root sudo user"
                exit 1
        fi

        if ! /usr/bin/sudo -V >/dev/null 2>&1; then
                echo "/usr/bin/sudo not found, exiting..."
                echo "Ensure sudo is installed on your system"
                exit 1
        fi

        if ! /usr/bin/git -v >/dev/null 2>&1; then
                echo "/usr/bin/git not found, exiting..."
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


flags_setup
init_run
