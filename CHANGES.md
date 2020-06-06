LibCYAML: Change Log
====================

## LibCYAML v1.1.0

* **Loading**:
  * Significantly optimised handling of aliases and anchors.
  * Fixed handling of duplicate mapping keys.
* **Saving**:
  * Increased precision for double precision floating point values.
* **General**:
  * Fixed data handling on big endian systems.

No changes are required for client applications to upgrade.


## LibCYAML v1.0.2

* **Loading**:
  * Fixed invalid read on error path for bitfield handling.
* **Buildsystem**:
  * Fixed to link against libraries after listing objects.
  * Added `check` target as alias for `test`.

No changes are required for client applications to upgrade.


## LibCYAML v1.0.1

* **Loading**:
  * Fixed mapping and sequence values with `CYAML_FLAG_POINTER_NULL`.
* **Buildsystem**:
  * Installation: Explicitly create leading directories.

No changes are required for client applications to upgrade.


## LibCYAML v1.0.0

* Initial release.
