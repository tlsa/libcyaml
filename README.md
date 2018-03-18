LibCYAML: Schema-based YAML parsing and serialisation
=====================================================

[![Build Status](https://travis-ci.org/tlsa/libcyaml.svg?branch=master)](https://travis-ci.org/tlsa/libcyaml) [![Code Coverage](https://codecov.io/gh/tlsa/libcyaml/branch/master/graph/badge.svg)](https://codecov.io/gh/tlsa/libcyaml)

LibCYAML is a C library for reading and writing structured YAML documents.
It is written in ISO C11 and licensed under the ISC licence.

Overview
--------

The fundamental idea behind CYAML is to allow applications to construct
schemas which describe both the permissible structure of the YAML documents
to read/write, and the C data structure(s) in which the loaded data is
arranged in memory.

### Goals

* Make it easy to load YAML into client's custom C data structures.
* Good compromise between flexibility and simplicity.

### Features

* Load, Save and Free functions.
* Reads and writes arbitrary client data structures.
* Schema-driven, allowing control over permitted YAML, for example:
    - Required / optional mapping fields.
    - Allowed / disallowed values.
    - Minimum / maximum sequence entry count.
    - etc...
* Enumerations and flag words.
* YAML backtraces make it simple for users to fix their YAML to
  conform to your schema.
* Uses standard `libyaml` library for low-level YAML read / write.

Status
------

Under development.  Until a release is made, the API may change, and
various features may not yet be implemented.  Feedback welcome.

Building
--------

To build the library (debug mode), run:

    make

To build a release version:

    make VARIANT=release

To run the tests, run any of the following, which generate various
levels of output verbosity (optionally setting `VARIANT=release`):

    make test
    make test-quiet
    make test-verbose
    make test-debug

To run the tests under `valgrind`, a similar set of targets is available:

    make valgrind
    make valgrind-quiet
    make valgrind-verbose
    make valgrind-debug

To generate a test coverage report, `gcovr` is required:

    make coverage

To generate both public API documentation, and documentation of CYAML's
internals, `doxygen` is required:

    make docs
