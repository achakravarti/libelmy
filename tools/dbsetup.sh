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
