#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"


# -b,--box (default arch)
# -k,--sshkey (default $HOME/.ssh/id_rsa)
# -p,--purge (default false)
# -u,--update (default true)
#
# ./vgsetup -b arch -k ~/.ssh/id_rsa -p -u


#                                                               %func:flag_setup
# __NAME__
#       flag_setup() - sets up command line flags
#
flag_setup()
{
        DEFINE_string 'box' 'arch' 'Vagrant box' 'b'
        DEFINE_string 'sshkey' "$HOME/.ssh/id_rsa" 'GitHub SSH key' 'k'
        DEFINE_boolean 'purge' false 'purge existing Vagrant box' 'p'
        DEFINE_boolean 'update' false 'update package manager' 'u'

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


#                                                              %func:vgbin_check
# __NAME__
#       vgbin_check() - checks existence of vagrant command
#
vgbin_check()
{
        if ! vagrant -v >/dev/null 2>&1; then
                echo  "vagrant command not found; install it first"
                exit 1
        fi
}


#                                                               %func:vgbox_boot
# __NAME__
#       vgbox_boot() - boots Vagrant box
#
vgbox_boot()
{
        if ! vagrant up; then
                echo "Failed to boot Vagrant box, exiting..."
                exit 1
        fi

        if ! eval "$(ssh-agent -s)"; then
                echo "Failed to forward SSH key, exiting..."
                exit 1
        fi

        if ! ssh-add "$FLAGS_sshkey"; then
                echo "Failed to forward SSH key, exiting..."
                exit 1
        fi

        echo "Forwarded SSH key..."

        vagrant ssh -c 'ssh -o StrictHostKeyChecking=no -T git@github.com'
        if [ $? -gt 1 ]; then
                echo "Failed to authenticate with GitHub, exiting..."
                exit 1
        fi

        if [ "$1" -eq 0 ]; then
                _chrysalid=git@github.com:achakravarti/libchrysalid.git
                _elmy=git@github.com:achakravarti/libelmy.git

                if ! vagrant ssh -c \
                    "git clone --recurse-submodules $_chrysalid"; then
                        echo "Failed to clone $_chrysalid, exiting..."
                        exit 1;
                fi

                if ! vagrant ssh -c \
                    "git clone --recurse-submodules $_elmy"; then
                        echo "Failed to clone $_elmy, exiting..."
                        exit 1;
                fi

                echo "Git clone successful, run ./configure on each project..."
        fi

        echo "Vagrant box boot successful, happy coding :)"
        exit 0
}


#                                                             %func:vgfile_check
# __NAME__
#       vgfile_check() - checks for existing Vagrantfile
#
vgfile_check()
{
        if [ -e Vagrantfile ]; then
                if [ "$FLAGS_purge" = "$FLAGS_TRUE" ]; then
                        vagrant halt >/dev/null 2>&1
                        vagrant destroy -f >/dev/null 2>&1
                        rm Vagrantfile
                        echo "Purged existing Vagrant box..."

                elif vagrant status | grep "is running" >/dev/null; then
                        echo "Vagrant box already running, skipping..."
                        exit 0

                else
                        vgbox_boot 1
                fi
        fi
}


#                                                             %func:vgfile_write
# __NAME__
#       vgfile_write() - writes Vagrantfile
#
vgfile_write()
{
        if [ "$FLAGS_box" = arch ]; then
                vgfile_arch
        elif [ "$FLAGS_box" = alpine ]; then
                vgfile_alpine
        elif [ "$FLAGS_box" = debian ]; then
                vgfile_debian
        elif [ "$FLAGS_box" = centos ]; then
                vgfile_centos
        elif [ "$FLAGS_box" = freebsd ]; then
                vgfile_freebsd
        fi

        vgbox_boot 0
}


#                                                              %func:vgfile_arch
# __NAME__
#       vgfile_arch() - writes Vagrantfile for Arch Linux
#
vgfile_arch()
{
        echo "Vagrant.configure(\"2\") do |config|" > Vagrantfile
        {
                echo "  config.vm.box = \"generic/arch\"";
                echo "  config.ssh.forward_agent = true";
                echo "  config.vm.provision \"shell\", inline: <<-SHELL"
        } >> Vagrantfile

        if [ "$FLAGS_update" -eq "$FLAGS_TRUE" ]; then
                echo "    pacman -Syu --noconfirm" >> Vagrantfile
        else
                echo "    pacman -Sy --noconfirm" >> Vagrantfile
        fi

        {
                echo "    pacman -S base-devel git go --noconfirm --needed";
                echo "    git clone https://aur.archlinux.org/yay.git";
                echo "    chown -R vagrant:vagrant yay";
                echo "    su - vagrant -c \"cd yay; makepkg -si --noconfirm\"";
                echo "  SHELL";
                echo "end";
        } >> Vagrantfile
}


#                                                            %func:vgfile_alpine
# __NAME__
#       vgfile_alpine() - writes Vagrantfile for Alpine Linux
#
vgfile_alpine()
{
        echo "not implemented"
        exit 1
}


#                                                            %func:vgfile_debian
# __NAME__
#       vgfile_debian() - writes Vagrantfile for Debian Jessie
#
vgfile_debian()
{
        echo "not implemented"
        exit 1
}


#                                                            %func:vgfile_centos
# __NAME__
#       vgfile_centos() - writes Vagrantfile for CentOS
#
vgfile_centos()
{
        echo "not implemented"
        exit 1
}


#                                                           %func:vgfile_freebsd
# __NAME__
#       vgfile_freebsd() - writes Vagrantfile for FreeBSD
#
vgfile_freebsd()
{
        echo "not implemented"
        exit 1
}


flag_setup "$@"
vgbin_check
vgfile_check
vgfile_write
