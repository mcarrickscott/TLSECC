# TLSECC

Portable, performant, constant time C and Rust libraries for supporting Elliptic curve functionality for TLS1.3 on 32 or 64 bit architectures.

Code in large part automatically generated from python scripts - see https://github.com/mcarrickscott/modarith

# Build

For a 64 bit C build, copy the contents of the *c64*, *include* and *include64* directories into a working directory. Then

	gcc -I. -O2 -c *.c
	ar rc tlsecc.a *.o

The library is built in *tlsecc.a*, to be used in conjunction with *tlsecc.h*

To run a test program copy from the *test* directory into the working directory and 

	gcc -O2 tests256.c tlsecc.a -o tests256


For a 64-bit Rust build, first create a library

	cargo new --lib tlsecc

Then copy into the *src* directory all files from the *rust64* directory. Build the library

	cargo build --release

To run a test programs, copy *libtlsecc.rlib* from the *target/release* directory into a working directory, along with the rust test programs from the *test* directory.

	rustc tests256.rs --extern tlsecc=libtlsecc.rlib
