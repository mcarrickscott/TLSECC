# TLSECC

Portable C and Rust libraries for supporting Elliptic curve functionality for TLS1.3 on 32 or 64 bit architectures

# Build

For a 64 bit build, copy the contents of the *c64* and *include* directories into a working directory. Then

	gcc -I. -O2 -c *.c
	ar rc tlsecc.a *.o

The library is built in *tlsecc.a*, to be used in conjuction with *tlsecc.h*
