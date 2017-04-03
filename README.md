# YARC: Yet Another Resource Compiler

Let's face it: modern, cross-platform, embeddable and flexible resource compiling is still lacking nowadays.

Traditionally, resource compilers have the following limitations:

* Resources can only be linked in executables, not libraries
* The resource compiler and corresponding API is platform-specific
* Maintenance of non-portable resource scripts is often necessary
* Resource compression, when supported, makes embedding more complex

## Compiler Usage

`yarc [options] <input files>`

Options:
 * `-o` <output file>  output file (default is "resources.c")
 * `-p` <prefix>       name prefix (default is "yarc")
 * `-b` <bundle>       bundle name (default is "default")
 * `-w` <width>        hex dump width (default is 16)
 * `-a` <padding>      append zero padding (default is 2)
 * `-u`                use uppercase hex (default is lowercase)
 * `-s`                use static keyword on resources
 * `-h`                print help
 * `-v`                print version (1.0.0)
 * `-V`                verbose mode

