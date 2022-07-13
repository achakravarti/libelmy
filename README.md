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


## Supported Platforms

  - Alpine Linux
  - Arch Linux
  - CentOS
  - Debian
  - Fedora
  - FreeBSD
  - Ubuntu

## Build Options

  - As library only (devel/prod)
    - make
    - make doc
    - make clean
    - make check
    - make install
    - make uninstall
  - As library + command line binary (devel/release)
    - make
    - make doc
    - make clean
    - make check
    - make install
    - make uninstall
  - As containerised library + command line binary (devel/release)
    - make
    - make doc
    - make clean
    - make check
    - make install
    - make uninstall
    
## Future build options
  - As library + FastCGI binary
  - As containerised libary + FastCGI binary
