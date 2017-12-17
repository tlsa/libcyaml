LibCYAML: Schema-based YAML parsing and serialisation
=====================================================

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
