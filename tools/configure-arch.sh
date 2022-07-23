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
        msg_info "executing ./configure-arch.sh"

        if [ "$FLAGS_upgrade" -eq "$FLAGS_TRUE" ]; then
                pkg_upgrade
        else
                msg_warn "--noupgrade set, skipping package upgrades"
        fi

        pkg_install base-devel
        pkg_install postgresql
        pkg_install postgresql-libs
        pkg_install rsyslog

        if [ "$FLAGS_man" -eq "$FLAGS_TRUE" ]; then
                pkg_install pandoc
        else
                msg_warn "--noman set, skipping support for man pages"
        fi

        if [ "$FLAGS_check" -eq "$FLAGS_TRUE" ]; then
                pkg_install valgrind
                pkg_install criterion
        else
                msg_warn "--nocheck set, skipping support for check tools"
        fi
}


main_setup "$@"
main_exec
