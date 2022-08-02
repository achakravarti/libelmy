#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"
. "$(dirname "0")/../tools/msg.sh"
. "$(dirname "0")/../tools/os.sh"
. "$(dirname "0")/../tools/su.sh"
. "$(dirname "0")/../tools/pkg.sh"
. "$(dirname "0")/../tools/srv.sh"


# MAIN FUNCTIONS


main_setup()
{
        DEFINE_string 'dbhost' '127.0.0.1' 'database host address' 1001
        DEFINE_integer 'dbport' '5432' 'database port' 1002
        DEFINE_string 'dbname' 'syslog' 'database name' 1003
        DEFINE_string 'dbuser' 'rsyslog' 'database user' 1004
        DEFINE_string 'dbpass' 'rsyslog' 'database user password' 1005
        DEFINE_string 'dbpgpass' '' 'database postgres password' 1006
        DEFINE_boolean 'reset' false 'reset database' 1007

        FLAGS "$@" || exit $?
        eval set -- "$FLAGS_ARGV"
}


main_exec()
{
        infra_server
        infra_client
        db_user
        db_schema
}


# INFRASTRUCTURE FUNCTIONS


infra_server()
{
        if ! [ "$FLAGS_dbhost" = '127.0.0.1' ]; then
                msg_warn '--dbhost set to remote, skipping server installation'
                return
        fi

        case "$OS_DISTRO" in
        Alpine)
                _pgdir=/var/lib/postgresql/data
                _pgsrv=postgresql

                pkg_install postgresql
                pkg_install postgresql-contrib;;

        Arch)
                _pgdir=/var/lib/postgres/data
                _pgsrv=postgresql.service

                pkg_install postgresql;;

        FreeBSD)
                _pgdir=/var/db/postgres/data14
                _pgsrv=postgresql

                pkg_install postgresql14-server
                pkg_install postgresql14-contrib;;

        *)
                _pgdir=/var/lib/postgres/14/main
                _pgsrv=postgresql.service

                pkg_install postgresql
                pkg_install postgresql-contrib;;
        esac

        msg_info 'checking postgres cluster'

        if ! [ -d "$_pgdir" ]   \
            || [ -z "$($SU -u postgres ls -A "$_pgdir")" ]; then
                    echo "DEBUG"
                $SU -u postgres initdb          \
                    --locale=en_US.UTF-8        \
                    --encoding=UTF8             \
                    -D "$_pgdir"                \
                    || msg_fail 'failed to initialise postgres cluster'
        fi

        srv_enable "$_pgsrv"
        srv_start "$_pgsrv"
        msg_ok 'postgres cluster initialised'
}


infra_client()
{
        case "$OS_DISTRO" in
        Alpine)
                pkg_install libpq-dev;;

        Arch)
                pkg_install postgresql-libs;;

        FreeBSD)
                pkg_install postgresql14-client
                pkg_install postgresql-libpgeasy;;

        *)
                pkg_install libpq-dev;;
        esac
}


# DATABASE FUNCTIONS


db_cmd()
{
        PGPASSWORD="$FLAGS_dbpgpass" psql       \
            -h "$FLAGS_dbhost"                  \
            -p "$FLAGS_dbport"                  \
            -U postgres                         \
            -tAqc "$1" || msg_fail 'failed to execute SQL command'
}


db_script()
{
        PGPASSWORD=$FLAGS_dbpass psql   \
            -h "$FLAGS_dbhost"          \
            -p "$FLAGS_dbport"          \
            -d "$FLAGS_dbname"          \
            -U "$FLAGS_dbuser"          \
            -tAqf init.sql || msg_fail 'failed to execute SQL script'
}


db_user()
{
        msg_info "looking for database user $FLAGS_dbuser"

        _sql="SELECT 1 FROM PG_ROLES WHERE ROLNAME='$FLAGS_dbuser'"
        if db_cmd "$_sql" | grep -q 1; then
                msg_ok "user $FLAGS_dbuser found, skipping"
                return
        fi

        msg_info "user $FLAGS_dbuser not found, creating"

        db_cmd "CREATE USER $FLAGS_dbuser WITH PASSWORD '$FLAGS_dbpass';"
        db_cmd "ALTER ROLE $FLAGS_dbuser SET CLIENT_ENCODING TO 'UTF8';"

        _sql="ALTER ROLE $FLAGS_dbuser SET DEFAULT_TRANSACTION_ISOLATION"
        _sql="$_sql TO 'READ COMMITTED';"
        db_cmd "$_sql"

        db_cmd "ALTER ROLE $FLAGS_dbuser SET TIMEZONE TO 'UTC'"
}


db_schema()
{
        if [ "$FLAGS_reset" -eq "$FLAGS_TRUE" ]; then
                msg_warn "--reset set, dropping database $FLAGS_dbname"

                db_cmd "DROP DATABASE IF EXISTS $FLAGS_dbname WITH (FORCE);"
                msg_ok "dropped database $FLAGS_dbname"
        fi

        msg_info "looking for database $FLAGS_dbname"

        _sql="SELECT 1 FROM PG_DATABASE WHERE DATNAME='$FLAGS_dbname'"
        if db_cmd "$_sql" | grep -q 1; then
                msg_ok "database $FLAGS_dbname found, skipping"
                return
        fi

        db_cmd "CREATE DATABASE $FLAGS_dbname;"
        db_cmd "GRANT ALL PRIVILEGES ON DATABASE $FLAGS_dbname TO $FLAGS_dbuser;"
        db_script init.sql

        msg_ok "created database "$FLAGS_dbname"
}


# MAIN ENTRY POINT


main_setup "$@"
main_exec
