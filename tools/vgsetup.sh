#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"


# -b,--box (default arch)
# -k,--sshkey (default $HOME/.ssh/id_rsa)
# -u,--update (default true)
# -d,--doc (default true)
#
# ./vgsetup -b arch -k ~/.ssh/id_rsa -u -d


flag_setup()
{
        DEFINE_string 'box' 'arch' 'Vagrant box' 'b'
        DEFINE_string 'sshkey' "$HOME/.ssh/id_rsa" 'GitHub SSH key' 'k'
        DEFINE_boolean 'update' true 'update Vagrant box on provisioning' 'u'
        DEFINE_boolean 'doc' true 'install Pandoc on provisioning' 'd'

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


check_vagrant()
{
        if ! vagrant -v >/dev/null 2>&1; then
                echo  "vagrant command not found; install it first"
                exit 1
        fi
}


vgfile_write()
{
        if [ ${FLAGS_box} = "arch" ]; then
                vgfile_arch
        elif [ ${FLAGS_box} = "alpine" ]; then
                vgfile_alpine
        elif [ ${FLAGS_box} = "debian" ]; then
                vgfile_debian
        elif [ ${FLAGS_box} = "centos" ]; then
                vgfile_centos
        elif [ ${FLAGS_box} = "freebsd" ]; then
                vgfile_freebsd
        fi
}


vgfile_arch()
{
        echo "not implemented"
        exit 1
}


vgfile_alpine()
{
        echo "not implemented"
        exit 1
}


vgfile_debian()
{
        echo "not implemented"
        exit 1
}


vgfile_centos()
{
        echo "not implemented"
        exit 1
}


vgfile_freebsd()
{
        echo "not implemented"
        exit 1
}


flag_setup "$@"
check_vagrant
vgfile_write
