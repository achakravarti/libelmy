#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"
. "$(dirname "0")/../tools/msg.sh"
. "$(dirname "0")/../tools/os.sh"
. "$(dirname "0")/../tools/su.sh"
. "$(dirname "0")/../tools/pkg.sh"


# MAIN FUNCTIONS


main_flags()
{
        DEFINE_boolean 'check' false 'support check tools' 1001
        DEFINE_boolean 'man' false 'support manpage tools' 1002

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


main_exec()
{
        setup_base
        setup_check
        setup_man
}


# SETUP FUNCTIONS


setup_base()
{
        case "$OS_DISTRO" in
        Alpine)
                pkg_install gcc
                pkg_install make
                pkg_install git;;

        Arch)
                pkg_install base-devel
                pkg_install git;;

        FreeBSD)
                pkg_install git;;

        *)
                pkg_install build-essential
                pkg_install git;;
        esac
}


setup_man()
{
        if [ "$FLAGS_man" -eq "$FLAGS_FALSE" ]; then
                msg_warn "--man unset, skipping support for man pages"
                return
        fi

        case "$OS_DISTRO" in
        Alpine)
                msg_warn "--man set, but not supported in Alpine Linux";;

        Arch)
                pkg_install pandoc
                pkg_install pandoc-crossref;;

        FreeBSD)
                pkg_install hs-pandoc
                pkg_install hs-pandoc-crossref
                pkg_install hs-pandoc-citeproc;;

        *)
                pkg_install pandoc
                pkg_install pandoc-citeproc
                pkg_install pandoc-crossref;;
        esac
}


setup_check()
{
        if [ "$FLAGS_check" -eq "$FLAGS_FALSE" ]; then
                msg_warn "--check unset, skipping support for check tools"
                return
        fi

        case "$OS_DISTRO" in
        Alpine)
                pkg_install libgit2
                pkg_install libgit2-dev
                pkg_install libffi-dev
                pkg_install pkgconf
                pkg_install ninja
                pkg_install meson
                pkg_install cmake
                pkg_install valgrind

                _repo=https://github.com/Snaipe/Criterion
                _emsg="failed to clone criterion repository"
                git clone --recursive "$_repo" || msg_fail _emsg

                _emsg="failed to build criterion"
                cd build || msg_fail _emsg
                meson build || msg_fail _emsg
                ninja -C build || msg_fail _emsg

                _emsg="failed to install criterion"
                sudo ninja -C build install || msg_fail _emsg
                sudo ldconfig || msg_fail _emsg;;

        Arch)
                pkg_install valgrind
                pkg_install criterion;;

        FreeBSD)
                pkg_install valgrind
                pkg_install criterion;;

        *)
                pkg_install valgrind
                pkg_install libcriterion-dev;;
        esac
}


setup_alpine()
{
        pkg_install gcc
        pkg_isntall make
        pkg_install git

        if [ "$FLAGS_check" -eq "$FLAGS_FALSE" ]; then
                msg_warn '--nocheck set, skipping support for check tools'
        else
                pkg_install valgrind
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
                $SU ninja -C build install || msg_fail _emsg
                $SU ldconfig || msg_fail _emsg
        fi

        if [ "$FLAGS_man" -eq "$FLAGS_FALSE" ]; then
                msg_warn '--noman set, skipping support for man pages'
        else
                pkg_install pandoc
        fi
}


# MAIN ENTRY POINT


main_flags "$@"
main_exec
