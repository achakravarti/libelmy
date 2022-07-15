#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"


# -b,--box (default arch)
# -k,--sshkey (default $HOME/.ssh/id_rsa)
# -u,--update (default true)
# -d,--doc (default true)
# -p,--purge (default false)
#
# ./vgsetup -b arch -k ~/.ssh/id_rsa -u -d -p


flag_setup()
{
        DEFINE_string 'box' 'arch' 'Vagrant box' 'b'
        DEFINE_string 'sshkey' "$HOME/.ssh/id_rsa" 'GitHub SSH key' 'k'
        DEFINE_boolean 'update' true 'update Vagrant box on provisioning' 'u'
        DEFINE_boolean 'doc' true 'install Pandoc on provisioning' 'd'
        DEFINE_boolean 'purge' false 'purge existing Vagrant box' 'p'

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


vgbin_check()
{
        if ! vagrant -v >/dev/null 2>&1; then
                echo  "vagrant command not found; install it first"
                exit 1
        fi
}


vgbox_boot()
{
        if ! eval "$(ssh-agent -s)"; then
                echo "Failed to forward SSH key, exiting..."
                exit 1
        fi

        if ! ssh-add "$FLAGS_sshkey"; then
                echo "Failed to forward SSH key, exiting..."
                exit 1
        fi

        echo "Forwarded SSH key..."

        if ! vagrant up; then
                echo "Failed to boot Vagrant box, exiting..."
                exit 1
        fi

        if ! vagrant ssh -c \
            'ssh -o StrictHostKeyChecking=no -T git@github.com'; then
                echo "Failed to authenticate with GitHub, exiting..."
                exit 1
        fi

        if "$1" -eq 0; then
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

                echo "Git clone successful, configure and make as required..."
        fi

        echo "Vagrant box boot successful, happy coding :)"
        exit 0
}

vgfile_check()
{
        if [ -e Vagrantfile ]; then
                if [ "$FLAGS_purge" = "$FLAGS_TRUE" ]; then
                        vagrant halt >/dev/null 2>&1
                        vagrant destroy -f >/dev/null 2>&1
                        rm Vagrantfile
                        echo "Purged existing Vagrant box..."

                elif vagrant status | grep "is running"; then
                        echo "Vagrant box already running, skipping..."
                        exit 0

                else
                        vgbox_boot 1
                fi
        fi
}


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


vgfile_arch()
{
        echo "Vagrant.configure(\"2\") do |config|" > Vagrantfile
        {
                echo "  config.vm.box = \"generic/arch\"";
                echo "  config.ssh.forward_agent = true";
                echo "  config.vm.provision \"shell\", inline: <<-SHELL";
                echo "    pacman -Sy --noconfirm"
        } >> Vagrantfile

        if [ "$FLAGS_update" -eq "$FLAGS_TRUE" ]; then
                echo "    pacman -Su --noconfirm" >> Vagrantfile
        fi

        if [ "$FLAGS_doc " -eq "$FLAGS_TRUE" ]; then
                echo "    pacman -S pandoc --noconfirm" >> Vagrantfile
        fi

        {
                echo "    pacman -S gcc make git --noconfirm";
                echo "    pacman -S postgresql postgresql-libs --noconfirm";
                printf "    su - postgres -c \"initidb --locale en_US.UTF-8";
                echo "     -D '/var/lib/postgres/data'\"";
                echo "    systemctl start postgresql.service";
                echo "    systemctl enable postgresql.service";
                echo "  SHELL";
                echo "end";
        } >> Vagrantfile
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
vgbin_check
vgfile_check
vgfile_write
