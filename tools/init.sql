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
        , (23, 'local7');


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
        , (7, 'debug');


CREATE OR REPLACE FUNCTION count_logs()
RETURNS INT
LANGUAGE SQL AS
$$
    SELECT count(id)
    FROM systemevents;
$$;


CREATE OR REPLACE FUNCTION list_all_logs()
RETURNS TABLE (
    id INT
    , logged_at TIMESTAMPTZ
    , facility_id SMALLINT
    , facility_keyword TEXT
    , severity_id SMALLINT
    , severity_keyword TEXT
    , hostname TEXT
    , log_message TEXT
    , application_tag TEXT
) LANGUAGE SQL AS
$$
    SELECT
        e.id
        , e.devicereportedtime
        , e.facility
        , f.keyword
        , e.priority
        , s.keyword
        , e.fromhost
        , e.message
        , e.syslogtag
    FROM systemevents AS e
    INNER JOIN facilities AS f ON e.facility = f.id
    INNER JOIN severities AS s ON e.priority = s.id
    ORDER BY e.receivedat DESC;
$$;


CREATE OR REPLACE FUNCTION list_logs_subset(
    _start_row      INT
    , _row_count    INT
    , _sort_row     INT
    , _sort_asc     BOOLEAN
)
RETURNS TABLE (
    id INT
    , logged_at TIMESTAMPTZ
    , facility_id SMALLINT
    , facility_keyword TEXT
    , severity_id SMALLINT
    , severity_keyword TEXT
    , hostname TEXT
    , log_message TEXT
    , application_tag TEXT
) LANGUAGE SQL AS
$$
    SELECT
        e.id AS id
        , e.devicereportedtime AS logged_at
        , e.facility AS facility_id
        , f.keyword AS facility_keyword
        , e.priority AS severity_id
        , s.keyword AS severity_keyword
        , e.fromhost AS hostname
        , e.message AS log_message
        , e.syslogtag AS application_tag
    FROM systemevents AS e
    INNER JOIN facilities AS f ON e.facility = f.id
    INNER JOIN severities AS s ON e.priority = s.id
    ORDER BY
          (CASE WHEN _sort_row = 0 AND _sort_asc = TRUE THEN e.id END) ASC
          , (CASE WHEN _sort_row = 0 AND _sort_asc = FALSE THEN e.id END) DESC
          , (CASE WHEN
                _sort_row = 1 AND _sort_asc = TRUE
             THEN e.devicereportedtime
             END) ASC
          , (CASE WHEN
                _sort_row = 1 AND _sort_asc = FALSE
             THEN e.devicereportedtime
             END) DESC
          , (CASE WHEN _sort_row = 2 AND _sort_asc = TRUE THEN e.facility END)
            ASC
          , (CASE WHEN _sort_row = 2 AND _sort_asc = FALSE THEN e.facility END)
            DESC
          , (CASE WHEN _sort_row = 3 AND _sort_asc = TRUE THEN f.keyword END)
            ASC
          , (CASE WHEN _sort_row = 3 AND _sort_asc = FALSE THEN f.keyword END)
            DESC
          , (CASE WHEN _sort_row = 4 AND _sort_asc = TRUE THEN e.priority END)
            ASC
          , (CASE WHEN _sort_row = 4 AND _sort_asc = FALSE THEN e.priority END)
            DESC
          , (CASE WHEN _sort_row = 5 AND _sort_asc = TRUE THEN s.keyword END)
            ASC
          , (CASE WHEN _sort_row = 5 AND _sort_asc = FALSE THEN s.keyword END)
            DESC
          , (CASE WHEN _sort_row = 6 AND _sort_asc = TRUE THEN e.fromhost END)
            ASC
          , (CASE WHEN _sort_row = 6 AND _sort_asc = FALSE THEN e.fromhost END)
            DESC
          , (CASE WHEN _sort_row = 7 AND _sort_asc = TRUE THEN e.message END)
            ASC
          , (CASE WHEN _sort_row = 7 AND _sort_asc = FALSE THEN e.message END)
            DESC
          , (CASE WHEN _sort_row = 8 AND _sort_asc = TRUE THEN e.syslogtag END)
            ASC
          , (CASE WHEN _sort_row = 8 AND _sort_asc = FALSE THEN e.syslogtag END)
            DESC
    LIMIT _row_count OFFSET _start_row - 1;
$$;
