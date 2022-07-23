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
        pkg_install llvm
        pkg_install postgresql14-server
        pkg_install postgresql14-contrib
        pkg_install postgresql-libpgeasy
        pkg_install rsyslog

        if [ "$FLAGS_man" -eq "$FLAGS_TRUE" ]; then
                pkg_install hs-pandoc
                pkg_install hs-pandoc-crossref
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
