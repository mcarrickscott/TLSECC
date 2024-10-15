// TLSECC Library
// A small, fast, portable library for implementation of Elliptic curve algorithms as required by TLS
//
#ifndef TLSECC_H
#define TLSECC_H

/** @brief Generate an X25519 key pair
 *
    @param SK an input random 32-byte secret key
    @param PK an output random 32-byte public key
 */
extern void X25519_KEY_PAIR(char *SK,char *PK);

/** @brief Generate an X25519 shared secret
 *
    @param SK an input random 32-byte secret key
    @param PK an input random 32-byte public key
    @param SS an output shared 32-byte secret
 */
extern void X25519_SHARED_SECRET(char *SK,char *PK,char *SS);


/** @brief Generate an NIST256 key pair
 *
    @param SK an input random 32-byte secret key
    @param PK an output random 32-byte public key
 */
extern void NIST256_KEY_PAIR(char *SK,char *PK);

/** @brief Generate an NIST256 shared secret
 *
    @param SK an input random 32-byte secret key
    @param PK an input random public key, 33-byte compressed or 65 bytes uncompressed
    @param SS an output shared 32-byte secret
 */
extern void NIST256_SHARED_SECRET(char *SK,char *PK,char *SS);


/** @brief Generate an NIST256 ECDSA signature
 *
    @param SK an input random 32-byte secret key
    @param ran an input random 40 bytes
    @param mlen the length of the input
    @param m an input message
    @param sig an output 64 byte signature
 */
extern void NIST256_SIGN(char *SK,char *ran,int mlen,char *m,char *sig);

/** @brief Generate an NIST256 shared secret
 *
    @param PK an input random public key, 33-byte compressed or 65 bytes uncompressed
    @param mlen the length of the input
    @param m an input message
    @param sig an input 64 byte signature
    @return 1 if signature is good, else 0
 */
extern int NIST256_VERIFY(char *PK,int mlen,char *m,char *sig);

#endif