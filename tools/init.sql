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
        , e.receivedat
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
    start_row   INT
    , row_count INT
    , sort_row  INT
    , sort_asc  BOOLEAN
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
        e.id
        , e.receivedat
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
