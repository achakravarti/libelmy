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
);
