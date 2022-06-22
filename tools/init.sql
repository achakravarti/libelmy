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


CREATE TABLE IF NOT EXISTS systemeventsproperties (
    id                  SERIAL
    , systemeventid     INT
    , paramname         TEXT
    , paramvalue        TEXT
    , PRIMARY KEY       (id)
);


CREATE TABLE IF NOT EXISTS facilities (
       id               INT NOT NULL
       , keyword        TEXT NOT NULL
       , PRIMARY KEY    (id)
)

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
        , (9, 'cron'),
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
       id               INT NOT NULL
       , keyword        TEXT NOT NULL
       , PRIMARY KEY    (id)
)

INSERT INTO severities (id, keyword, serverity)
VALUES
        (0, 'emerg')
        , (1, 'alert')
        , (2, 'crit')
        , (3, 'err')
        , (4, 'warning')
        , (5, 'notice')
        , (6, 'info')
        , (7, 'debug');
