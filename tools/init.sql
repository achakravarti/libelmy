CREATE EXTENSION IF NOT EXISTS pg_trgm;


--
-- TABLE DEFINITIONS
--


--                                                           %table:systemevents
-- __NAME__
--      systemevents - holds system logs
--
CREATE TABLE IF NOT EXISTS systemevents (
    id                      SERIAL
    , customerid            BIGINT
    , receivedat            TIMESTAMPTZ
    , devicereportedtime    TIMESTAMPTZ
    , facility              SMALLINT
    , priority              SMALLINT
    , fromhost              TEXT
    , message               TEXT
    , ntseverity            INT
    , importance            INT
    , eventsource           TEXT
    , eventuser             TEXT
    , eventcategory         INT
    , eventid               INT
    , eventbinarydata       TEXT
    , maxavailable          INT
    , currusage             INT
    , minusage              INT
    , maxusage              INT
    , infounitid            INT
    , syslogtag             TEXT
    , eventlogtype          TEXT
    , genericfilename       TEXT
    , systemid              INT
    , PRIMARY KEY           (id)
);


CREATE INDEX idx_systemevents__devicereportedtime
       ON systemevents
       USING BTREE (devicereportedtime);

CREATE INDEX idx_systemevents__facility
       ON systemevents
       USING BTREE (facility);

CREATE INDEX idx_systemevents__priority
       ON systemevents
       USING BTREE (priority);

CREATE INDEX idx_systemevents__fromhost
       ON systemevents
       USING GIN (fromhost GIN_TRGM_OPS);

CREATE INDEX idx_systemevents__syslogtag
       ON systemevents
       USING GIN (syslogtag GIN_TRGM_OPS);

CREATE INDEX idx_systemevents__message
       ON systemevents
       USING GIN (message GIN_TRGM_OPS);


--                                                             %table:facilities
-- __NAME__
--      facilities - defines syslog facilities
CREATE TABLE IF NOT EXISTS facilities (
       id               SMALLINT NOT NULL
       , keyword        TEXT NOT NULL
       , PRIMARY KEY    (id)
);

INSERT INTO facilities (id, keyword)
VALUES
        (0, 'kern')
        , (1, 'user')
        , (2, 'mail')
        , (3, 'daemon')
        , (4, 'auth')
        , (5, 'syslog')
        , (6, 'lpr')
        , (7, 'news')
        , (8, 'uucp')
        , (9, 'cron')
        , (10, 'authpriv')
        , (11, 'ftp')
        , (12, 'ntp')
        , (13, 'security')
        , (14, 'console')
        , (15, 'solaris-cron')
        , (16, 'local0')
        , (17, 'local1')
        , (18, 'local2')
        , (19, 'local3')
        , (20, 'local4')
        , (21, 'local5')
        , (22, 'local6')
        , (23, 'local7')
ON CONFLICT DO NOTHING;


--                                                             %table:severities
-- __NAME__
--      severities - defines syslog severities
CREATE TABLE IF NOT EXISTS severities (
       id               SMALLINT NOT NULL
       , keyword        TEXT NOT NULL
       , PRIMARY KEY    (id)
);

INSERT INTO severities (id, keyword)
VALUES
        (0, 'emerg')
        , (1, 'alert')
        , (2, 'crit')
        , (3, 'err')
        , (4, 'warning')
        , (5, 'notice')
        , (6, 'info')
        , (7, 'debug')
ON CONFLICT DO NOTHING;


--
-- FUNCTION DEFINITIONS
--


--                                                              %func:count_logs
-- __NAME__
--      count_logs() - counts number of log entries
CREATE OR REPLACE FUNCTION count_logs()
RETURNS INT
LANGUAGE SQL STABLE AS
$$
    SELECT count(id)
    FROM systemevents;
$$;


--                                                 %func:get_first_log_timestamp
-- __NAME__
--      get_first_log_timestamp() - gets timestamp of first log
CREATE OR REPLACE FUNCTION get_first_log_timestamp(
    _timezone   TEXT
) RETURNS TIMESTAMP
LANGUAGE SQL STABLE AS
$$
    SELECT receivedat AT TIME ZONE _timezone
    FROM systemevents
    ORDER BY receivedat ASC
    LIMIT 1;
$$;


--                                                  %func:get_last_log_timestamp
-- __NAME__
--      get_last_log_timestamp() - gets timestamp of last log
CREATE OR REPLACE FUNCTION get_last_log_timestamp(
    _timezone   TEXT
) RETURNS TIMESTAMP
LANGUAGE SQL STABLE AS
$$
    SELECT receivedat AT TIME ZONE _timezone
    FROM systemevents
    ORDER BY receivedat DESC
    LIMIT 1;
$$;


--                                                           %func:list_all_logs
-- __NAME__
--      list_all_logs() - lists all logs
CREATE OR REPLACE FUNCTION list_all_logs(
    _timezone   TEXT
)
RETURNS TABLE (
    event_at            TIMESTAMP
    , logged_at         TIMESTAMP
    , facility_id       SMALLINT
    , facility_keyword  TEXT
    , severity_id       SMALLINT
    , severity_keyword  TEXT
    , hostname          TEXT
    , application_tag   TEXT
    , log_message       TEXT
) LANGUAGE SQL STABLE AS
$$
    SELECT
        e.devicereportedtime AT TIME ZONE _timezone
        , e.receivedat AT TIME ZONE _timezone
        , e.facility
        , f.keyword
        , e.priority
        , s.keyword
        , e.fromhost
        , e.syslogtag
        , e.message
    FROM systemevents AS e
    INNER JOIN facilities AS f ON e.facility = f.id
    INNER JOIN severities AS s ON e.priority = s.id
    ORDER BY e.devicereportedtime DESC;
$$;


--                                                        %func:list_logs_subset
-- __NAME__
--      list_logs_subset() - paginates logs
CREATE OR REPLACE FUNCTION list_logs_subset(
    _row_start   INT
    , _row_count INT
    , _sort_col TEXT
    , _sort_dir TEXT
    , _timezone TEXT
) RETURNS TABLE (
    event_at            TIMESTAMP
    , logged_at         TIMESTAMP
    , facility_id       SMALLINT
    , facility_keyword  TEXT
    , severity_id       SMALLINT
    , severity_keyword  TEXT
    , hostname          TEXT
    , tag               TEXT
    , message           TEXT
) LANGUAGE PLPGSQL STABLE AS
$$
BEGIN
    RETURN QUERY EXECUTE format(
        'SELECT
            e.devicereportedtime AT TIME ZONE %L
            , e.receivedat AT TIME ZONE %L
            , e.facility
            , f.keyword
            , e.priority
            , s.keyword
            , e.fromhost
            , e.syslogtag
            , e.message
        FROM systemevents AS e
        INNER JOIN facilities AS f ON e.facility = f.id
        INNER JOIN severities AS s ON e.priority = s.id
        ORDER BY %I %s
        LIMIT %s OFFSET %s'
        , _timezone
        , _timezone
        , _sort_col
        , _sort_dir
        , _row_count
        , _row_start
    );
END
$$;


--                                                    %func:filter_logs_facility
-- __NAME__
--      filter_logs_facility() - filters logs by facility
CREATE OR REPLACE FUNCTION filter_logs_facility(
    _filter      SMALLINT[]
    , _row_start INT
    , _row_count INT
    , _sort_col TEXT
    , _sort_dir TEXT
    , _timezone TEXT
) RETURNS TABLE (
    event_at            TIMESTAMP
    , logged_at         TIMESTAMP
    , facility_id       SMALLINT
    , facility_keyword  TEXT
    , severity_id       SMALLINT
    , severity_keyword  TEXT
    , hostname          TEXT
    , tag               TEXT
    , message           TEXT
) LANGUAGE PLPGSQL STABLE AS
$$
BEGIN
    RETURN QUERY EXECUTE format(
        'SELECT
            e.devicereportedtime AT TIME ZONE %L
            , e.receivedat AT TIME ZONE %L
            , e.facility
            , f.keyword
            , e.priority
            , s.keyword
            , e.fromhost
            , e.syslogtag
            , e.message
        FROM systemevents AS e
        INNER JOIN facilities AS f ON e.facility = f.id
        INNER JOIN severities AS s ON e.priority = s.id
        WHERE e.facility = ANY (%L)
        ORDER BY %I %s
        LIMIT %s OFFSET %s'
        , _timezone
        , _timezone
        , _filter
        , _sort_col
        , _sort_dir
        , _row_count
        , _row_start
    );
END
$$;


--                                                    %func:filter_logs_severity
-- __NAME__
--      filter_logs_severity() - filters logs by severity
CREATE OR REPLACE FUNCTION filter_logs_severity(
    _filter      SMALLINT[]
    , _row_start INT
    , _row_count INT
    , _sort_col TEXT
    , _sort_dir TEXT
    , _timezone TEXT
) RETURNS TABLE (
    event_at            TIMESTAMP
    , logged_at         TIMESTAMP
    , facility_id       SMALLINT
    , facility_keyword  TEXT
    , severity_id       SMALLINT
    , severity_keyword  TEXT
    , hostname          TEXT
    , tag               TEXT
    , message           TEXT
) LANGUAGE PLPGSQL STABLE AS
$$
BEGIN
    RETURN QUERY EXECUTE format(
        'SELECT
            e.devicereportedtime AT TIME ZONE %L
            , e.receivedat AT TIME ZONE %L
            , e.facility
            , f.keyword
            , e.priority
            , s.keyword
            , e.fromhost
            , e.syslogtag
            , e.message
        FROM systemevents AS e
        INNER JOIN facilities AS f ON e.facility = f.id
        INNER JOIN severities AS s ON e.priority = s.id
        WHERE e.priority = ANY (%L)
        ORDER BY %I %s
        LIMIT %s OFFSET %s'
        , _timezone
        , _timezone
        , _filter
        , _sort_col
        , _sort_dir
        , _row_count
        , _row_start
    );
END
$$;


--                                                    %func:filter_logs_hostname
-- __NAME__
--      filter_logs_hostname() - filters logs by hostname
CREATE OR REPLACE FUNCTION filter_logs_hostname(
    _filter      TEXT
    , _row_start INT
    , _row_count INT
    , _sort_col TEXT
    , _sort_dir TEXT
    , _timezone TEXT
) RETURNS TABLE (
    event_at            TIMESTAMP
    , logged_at         TIMESTAMP
    , facility_id       SMALLINT
    , facility_keyword  TEXT
    , severity_id       SMALLINT
    , severity_keyword  TEXT
    , hostname          TEXT
    , tag               TEXT
    , message           TEXT
) LANGUAGE PLPGSQL STABLE AS
$$
BEGIN
    RETURN QUERY EXECUTE format(
        'SELECT
            e.devicereportedtime AT TIME ZONE %L
            , e.receivedat AT TIME ZONE %L
            , e.facility
            , f.keyword
            , e.priority
            , s.keyword
            , e.fromhost
            , e.syslogtag
            , e.message
        FROM systemevents AS e
        INNER JOIN facilities AS f ON e.facility = f.id
        INNER JOIN severities AS s ON e.priority = s.id
        WHERE e.fromhost ILIKE ''%%'' || %L || ''%%''
        ORDER BY %I %s
        LIMIT %s OFFSET %s'
        , _timezone
        , _timezone
        , _filter
        , _sort_col
        , _sort_dir
        , _row_count
        , _row_start
    );
END
$$;


--                                                         %func:filter_logs_tag
-- __NAME__
--      filter_logs_tag() - filters logs by tag
CREATE OR REPLACE FUNCTION filter_logs_tag(
    _filter      TEXT
    , _row_start INT
    , _row_count INT
    , _sort_col TEXT
    , _sort_dir TEXT
    , _timezone TEXT
) RETURNS TABLE (
    event_at            TIMESTAMP
    , logged_at         TIMESTAMP
    , facility_id       SMALLINT
    , facility_keyword  TEXT
    , severity_id       SMALLINT
    , severity_keyword  TEXT
    , hostname          TEXT
    , tag               TEXT
    , message           TEXT
) LANGUAGE PLPGSQL STABLE AS
$$
BEGIN
    RETURN QUERY EXECUTE format(
        'SELECT
            e.devicereportedtime AT TIME ZONE %L
            , e.receivedat AT TIME ZONE %L
            , e.facility
            , f.keyword
            , e.priority
            , s.keyword
            , e.fromhost
            , e.syslogtag
            , e.message
        FROM systemevents AS e
        INNER JOIN facilities AS f ON e.facility = f.id
        INNER JOIN severities AS s ON e.priority = s.id
        WHERE e.syslogtag ILIKE ''%%'' || %L || ''%%''
        ORDER BY %I %s
        LIMIT %s OFFSET %s'
        , _timezone
        , _timezone
        , _filter
        , _sort_col
        , _sort_dir
        , _row_count
        , _row_start
    );
END
$$;


--                                                     %func:filter_logs_message
-- __NAME__
--      filter_logs_message() - filters logs by message
CREATE OR REPLACE FUNCTION filter_logs_message(
    _filter      TEXT
    , _row_start INT
    , _row_count INT
    , _sort_col TEXT
    , _sort_dir TEXT
    , _timezone TEXT
) RETURNS TABLE (
    event_at            TIMESTAMP
    , logged_at         TIMESTAMP
    , facility_id       SMALLINT
    , facility_keyword  TEXT
    , severity_id       SMALLINT
    , severity_keyword  TEXT
    , hostname          TEXT
    , tag               TEXT
    , message           TEXT
) LANGUAGE PLPGSQL STABLE AS
$$
BEGIN
    RETURN QUERY EXECUTE format(
        'SELECT
            e.devicereportedtime AT TIME ZONE %L
            , e.receivedat AT TIME ZONE %L
            , e.facility
            , f.keyword
            , e.priority
            , s.keyword
            , e.fromhost
            , e.syslogtag
            , e.message
        FROM systemevents AS e
        INNER JOIN facilities AS f ON e.facility = f.id
        INNER JOIN severities AS s ON e.priority = s.id
        WHERE e.message ILIKE ''%%'' || %L || ''%%''
        ORDER BY %I %s
        LIMIT %s OFFSET %s'
        , _timezone
        , _timezone
        , _filter
        , _sort_col
        , _sort_dir
        , _row_count
        , _row_start
    );
END
$$;


--                                                               %func:trim_logs
-- __NAME__
--      trim_logs() - truncates logs
CREATE OR REPLACE FUNCTION trim_logs(
    _size_limit INT
    , _trim_percentage INT
) RETURNS INT
LANGUAGE PLPGSQL AS
$$
DECLARE
    _deleted INT;

BEGIN
    SELECT
        CASE
            WHEN (pg_relation_size('systemevents') >= _size_limit * 1024 * 1024)
                THEN (
                    SELECT count(*) * _trim_percentage / 100
                    FROM systemevents
                )
            ELSE 0
        END
    INTO _deleted;

    IF _deleted > 0
    THEN
        DELETE FROM systemevents
        WHERE id IN (
            SELECT id
            FROM systemevents
            ORDER BY devicereportedtime DESC
            LIMIT (
                SELECT count(*) * _trim_percentage / 100
                FROM systemevents
            )
        );
    END IF;

    RETURN _deleted;
END
$$;
