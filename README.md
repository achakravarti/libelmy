# libelmy

libelmy - easy log monitoring

A simple microservice-type binary that handles querying application logs from a
centralised round-robin PostgreSQL database populated by rsyslog. Meant to be a
lightweight replacement for much more compute-intensive logging stacks such as
ELK (Elasticsearch Logstash Kibana).

Can be used either as a shared library (with C/C++ clients) or as a binary (e.g.
with Python glue code). The binary uses the shared libary. The shared library
code is in the `include/` and `src/lib` directories, whereas the binary code is
in the `src/bin` directory. The database schema and setup scripts are available
int the `tools` directory.


## Build Options

  - As library only
  - As library + command line binary
  - As library + FastCGI binary (future)
