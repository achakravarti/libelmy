#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"

# -t,--target (default arch)
# -p,--pandoc (default true)
# -u,--update (default true)
#
# ./init.sh --target arch --pandoc --update


# MAIN FUNCTIONS


main_flags()
{
        DEFINE_string 'target' 'arch' 'target platform' 't'
        DEFINE_boolean 'pandoc' true 'install pandoc' 'p'
        DEFINE_boolean 'update' true 'update package manager' 'u'

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


main_run()
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
                arch_init
        else
                echo "Unsupported target platform, exiting..."
                exit 1
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
        if ! sudo systemctl start "$1"; then
                echo "Failed to start $1, exiting..."
                exit 1
        fi

        if ! sudo systemctl enable "$1"; then
                echo "Failed to enable $1, exiting..."
                exit 1
        fi
}



# ARCH LINUX FUNCTIONS


arch_update()
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


arch_install()
{
        if ! pacman -Qi | grep "$1" >/dev/null 2>&1; then
                echo "Package $1 not found, installing..."
                arch_update

                if [ "$1" -eq 0 ]; then
                        if ! sudo pacman -S --noconfirm "$1"; then
                                echo "Failed to install package $1, exiting..."
                                exit 1
                        fi
                else
                        if ! yay -S --noconfirm "$1"; then
                                echo "Failed to install package $1, exiting..."
                                exit 1
                        fi
                fi
        fi
}


arch_init()
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

        arch_install gcc 0
        arch_install make 0
        arch_install valgrind 0
        arch_install postgresql 0
        arch_install postgresql-libs 0

        if [ "$FLAGS_pandoc" -eq "$FLAGS_TRUE" ]; then
                arch_install pandoc 0
        fi

        arch_install criterion 1
        arch_install rsyslog 1

        systemd_enabled postgresql.service
        if [ "$rv" -ne 0 ]; then
                if ! sudo -u postgres \
                    initdb --locale en_US.UTF-8 -D '/var/lib/postgres/data';
                then
                        echo "Failed to initialise postgres cluster, exiting..."
                        exit 1
                fi

                systemd_enable postgresql.service
        fi
}


# MAIN ENTRY POINT


main_flags "$@"
main_run
