#!/bin/sh

. "$(dirname "0")/../external/shflags/shflags"


# -h host (default 127.0.0.1)
# -p port (default 5433)
# -d database (default syslog)
# -u user (default rsyslog)
# -w password (watchword) (default rsyslog)
# -r reset
#
# ./dbsetup -h 127.0.0.1 -p 5433 -d libelmy -u libelmy -w my-secret password


# Script sets up the database where rsyslog writes its data
# Creates database and user if it does not exist
# Creates tables and triggers and sequences if database exists
# Deletes and recreates everything if reset


setup_flags() {
    DEFINE_string 'host' '127.0.0.1' 'database host address' 'a'
    DEFINE_integer 'port' '5432' 'database port' 'p'
    DEFINE_string 'database' 'syslog' 'database name' 'd'
    DEFINE_string 'user' 'rsyslog' 'database user' 'u'
    DEFINE_string 'password' 'rsyslog' 'database user password' 'w'
    DEFINE_string 'pgpassword' '' 'database postgres password' 'x'
    DEFINE_boolean 'reset' false 'reset database' 'r'

    FLAGS "$@" || exit $?
    eval set -- "$FLAGS_ARGV"
}


pg_cmd()
{
    PGPASSWORD="$FLAGS_pgpassword" psql \
        -h "$FLAGS_host"                \
        -p "$FLAGS_port"                \
        -U postgres                     \
        -tAqc "$1"
}


pg_file()
{
    PGPASSWORD=$FLAGS_password psql \
        -h "$FLAGS_host"            \
        -p "$FLAGS_port"            \
        -d "$FLAGS_database"        \
        -U "$FLAGS_user"            \
        -tAqf init.sql
}




check_sudo_exists() {
    if ! sudo -V >/dev/null 2>&1; then
        echo "sudo command not found; install it first"
        exit 1
    fi
}

check_psql_exists() {
    if ! psql -V >/dev/null 2>&1; then
        echo "psql command not found; install it first"
        exit 1
    fi
}

check_postgres_running() {
    if [ "$FLAGS_host" = "localhost" ] || [ "$FLAGS_host" = "127.0.0.1" ]; then
        if ! systemctl  is-active --quiet postgresql.service; then
            echo "postgresql.service not running; start it first"
            exit 1
        fi
    else
        if ! nc -w 1 "$FLAGS_host" "$FLAGS_port"; then
            echo "no response from $FLAGS_host:$FLAGS_port; check if running"
            exit 1
        fi
    fi
}

setup_db_user() {
    _sql="select 1 from pg_roles where rolname='$FLAGS_user'"

    if ! pg_cmd "$_sql" | grep -q 1; then
        echo "User $FLAGS_user not found, creating..."
        create_db_user
    else
        echo "User $FLAGS_user found, skipping..."
    fi
}


setup_db()
{
    _sql="select 1 from pg_database where datname='$FLAGS_database'"

    if ! pg_cmd "$_sql" | grep -q 1; then
        echo "Database $FLAGS_database not found, creating..."
        create_db
    else
        echo "Database $FLAGS_database found, skipping..."
    fi
}


create_db()
{
    pg_cmd "create database $FLAGS_database;"
    pg_cmd "grant all privileges on database $FLAGS_database to $FLAGS_user;"
    pg_file init.sql

    if rsyslogd -v >/dev/null 2>&1; then
        sudo systemctl start rsyslog.service >/dev/null 2>&1
    fi
}


create_db_user()
{
    pg_cmd "create user $FLAGS_user with password '$FLAGS_password';"
    pg_cmd "alter role $FLAGS_user set client_encoding to 'utf8';"

    _sql="alter role $FLAGS_user set default_transaction_isolation"
    _sql="$_sql to 'read committed';"
    pg_cmd "$_sql"

    pg_cmd "alter role $FLAGS_user set timezone to 'UTC'"
}


drop_db()
{
    if [ "$FLAGS_reset" -eq "$FLAGS_TRUE" ]; then
        if systemctl is-active --quiet rsyslog.service; then
            sudo systemctl stop rsyslog.service >/dev/null 2>&1
        fi

        pg_cmd "DROP DATABASE IF EXISTS $FLAGS_database WITH (FORCE);"
        echo "Dropped database $FLAGS_database..."
    fi
}


setup_flags "$@"

check_sudo_exists
check_psql_exists
check_postgres_running

setup_db_user
drop_db
setup_db
