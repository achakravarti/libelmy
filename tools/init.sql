CREATE EXTENSION IF NOT EXISTS pg_trgm;


CREATE TABLE IF NOT EXISTS systemevents (
    id                      SERIAL
    , customerid            BIGINT
    , receivedat            TIMESTAMPTZ --
    , devicereportedtime    TIMESTAMPTZ --
    , facility              SMALLINT --
    , priority              SMALLINT --
    , fromhost              TEXT --
    , message               TEXT --
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
    , syslogtag             TEXT --
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


CREATE TABLE IF NOT EXISTS systemeventsproperties (
    id                  SERIAL
    , systemeventid     INT
    , paramname         TEXT
    , paramvalue        TEXT
    , PRIMARY KEY       (id)
);


CREATE TABLE IF NOT EXISTS facilities (
       id               SMALLINT NOT NULL
       , keyword        TEXT NOT NULL
       , PRIMARY KEY    (id)
);

/*
CREATE TRIGGER tg_rotate_systemevents_table
    AFTER INSERT
    ON systemevents
    FOR EACH STATEMENT
    EXECUTE PROCEDURE rotate_systemevents_table();
*/


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


CREATE OR REPLACE FUNCTION count_logs()
RETURNS INT
LANGUAGE SQL STABLE AS
$$
    SELECT count(id)
    FROM systemevents;
$$;


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


--
-- _sort_row options:
-- 1 event_at
-- 2 severity_id
-- 3 facility_id
-- 4 hostname
-- 5 tag
-- 6 message
CREATE OR REPLACE FUNCTION list_logs_subset(
    _start_row      INT
    , _row_count    INT
    , _sort_row     INT
    , _sort_asc     BOOLEAN
    , _timezone     TEXT
) RETURNS TABLE (
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
    ORDER BY
          (CASE WHEN
                _sort_row = 1 AND _sort_asc = TRUE
             THEN e.devicereportedtime
           END) ASC
          , (CASE WHEN
                _sort_row = 1 AND _sort_asc = FALSE
             THEN e.devicereportedtime
             END) DESC
          , (CASE WHEN _sort_row = 2 AND _sort_asc = TRUE THEN e.priority END)
            ASC
          , (CASE WHEN _sort_row = 2 AND _sort_asc = FALSE THEN e.priority END)
            DESC
          , (CASE WHEN _sort_row = 3 AND _sort_asc = TRUE THEN e.facility END)
            ASC
          , (CASE WHEN _sort_row = 3 AND _sort_asc = FALSE THEN e.facility END)
            DESC
          , (CASE WHEN _sort_row = 4 AND _sort_asc = TRUE THEN e.fromhost END)
            ASC
          , (CASE WHEN _sort_row = 4 AND _sort_asc = FALSE THEN e.fromhost END)
            DESC
          , (CASE WHEN _sort_row = 5 AND _sort_asc = TRUE THEN e.syslogtag END)
            ASC
          , (CASE WHEN _sort_row = 5 AND _sort_asc = FALSE THEN e.syslogtag END)
            DESC
          , (CASE WHEN _sort_row = 6 AND _sort_asc = TRUE THEN e.message END)
            ASC
          , (CASE WHEN _sort_row = 6 AND _sort_asc = FALSE THEN e.message END)
            DESC
    LIMIT _row_count OFFSET _start_row - 1;
$$;


--
-- _sort_row options:
-- 1 event_at
-- 2 severity_id
-- 3 facility_id
-- 4 hostname
-- 5 tag
-- 6 message
CREATE OR REPLACE FUNCTION filter_logs_message(
    _filter         TEXT
    , _start_row    INT
    , _row_count    INT
    , _sort_row     INT
    , _sort_asc     BOOLEAN
    , _timezone     TEXT
) RETURNS TABLE (
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
    WHERE e.message ILIKE '%' || _filter || '%'
    ORDER BY
          (CASE WHEN
                _sort_row = 1 AND _sort_asc = TRUE
             THEN e.devicereportedtime
           END) ASC
          , (CASE WHEN
                _sort_row = 1 AND _sort_asc = FALSE
             THEN e.devicereportedtime
             END) DESC
          , (CASE WHEN _sort_row = 2 AND _sort_asc = TRUE THEN e.priority END)
            ASC
          , (CASE WHEN _sort_row = 2 AND _sort_asc = FALSE THEN e.priority END)
            DESC
          , (CASE WHEN _sort_row = 3 AND _sort_asc = TRUE THEN e.facility END)
            ASC
          , (CASE WHEN _sort_row = 3 AND _sort_asc = FALSE THEN e.facility END)
            DESC
          , (CASE WHEN _sort_row = 4 AND _sort_asc = TRUE THEN e.fromhost END)
            ASC
          , (CASE WHEN _sort_row = 4 AND _sort_asc = FALSE THEN e.fromhost END)
            DESC
          , (CASE WHEN _sort_row = 5 AND _sort_asc = TRUE THEN e.syslogtag END)
            ASC
          , (CASE WHEN _sort_row = 5 AND _sort_asc = FALSE THEN e.syslogtag END)
            DESC
          , (CASE WHEN _sort_row = 6 AND _sort_asc = TRUE THEN e.message END)
            ASC
          , (CASE WHEN _sort_row = 6 AND _sort_asc = FALSE THEN e.message END)
            DESC
    LIMIT _row_count OFFSET _start_row - 1;
$$;


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
