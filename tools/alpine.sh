#!/bin/sh


. "$(dirname "0")/../external/shflags/shflags"
. "$(dirname "0")/../tools/os.sh"
. "$(dirname "0")/../tools/pkg.sh"
. "$(dirname "0")/../tools/msg.sh"


main_setup()
{
        DEFINE_boolean 'man' true 'generate man pages' 'm'
        DEFINE_boolean 'upgrade' false 'upgrade package manager' 'u'
        DEFINE_boolean 'check' true 'support check tools' 'c'
        DEFINE_boolean 'database' true 'generate database' 'd'
        DEFINE_boolean 'release' false 'build for release' 'r'
        DEFINE_boolean 'force' false 'force regeneration' 'f'

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"

        os_query
}


main_exec()
{
        if [ "$FLAGS_upgrade" -eq "$FLAGS_TRUE" ]; then
                pkg_upgrade
        else
                msg_warn "--noupgrade set, skipping package upgrades"
        fi

        pkg_install gcc
        pkg_install build-base
        pkg_install make
        pkg_install postgresql
        pkg_install libpq-dev
        pkg_install rsyslog

        _wmsg="--man set, but not supported in Alpine Linux"
        [ "$FLAGS_man" -eq "$FLAGS_TRUE" ] && msg_warn _wmsg

        if [ "$FLAGS_check" -eq "$FLAGS_TRUE" ]; then
                pkg_install valgrind
                pkg_install git
                pkg_install libgit2
                pkg_install libgit2-dev
                pkg_install libffi-dev
                pkg_install pkgconf
                pkg_install ninja
                pkg_install meson
                pkg_install cmake

                _repo=https://github.com/Snaipe/Criterion
                _emsg="failed to clone criterion repository"
                git clone --recursive "$_repo" || msg_fail _emsg

                _emsg="failed to build criterion"
                cd build || msg_fail _emsg
                meson build || msg_fail _emsg
                ninja -C build || msg_fail _emsg

                _emsg="failed to install criterion"
                sudo ninja -C build install || msg_fail _emsg
                sudo ldconfig || msg_fail _emsg
        else
                msg_warn "--nocheck set, skipping support for check tools"
        fi
}


main_setup "$@"
main_exec
