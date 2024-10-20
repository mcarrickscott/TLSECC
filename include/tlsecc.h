// TLSECC Library
// A small, fast, portable library for implementation of Elliptic curve algorithms as required by TLS
//
#ifndef TLSECC_H
#define TLSECC_H

/**** X25519 ****/

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
    @return true for success
 */
extern int X25519_SHARED_SECRET(char *SK,char *PK,char *SS);


/**** NIST256 *****/

/** @brief Generate an NIST256 key pair
 *
    @param compress set true for point compression
    @param SK an input random 32-byte secret key
    @param PK an output random 33 or 65 byte public key
 */
extern void NIST256_KEY_PAIR(int compress,char *SK,char *PK);

/** @brief Generate an NIST256 shared secret
 *
    @param SK an input random 32-byte secret key
    @param PK an input random public key, 33-byte compressed or 65 bytes uncompressed
    @param SS an output shared 32-byte secret
    @return true for success
 */
extern int NIST256_SHARED_SECRET(char *SK,char *PK,char *SS);


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
    @return true if signature is good, else false
 */
extern int NIST256_VERIFY(char *PK,int mlen,char *m,char *sig);


/**** NIST384 *****/

/** @brief Generate an NIST384 key pair
 *
    @param compress set true for point compression
    @param SK an input random 48-byte secret key
    @param PK an output 49 or 97 byte public key
 */
extern void NIST384_KEY_PAIR(int compress,char *SK,char *PK);

/** @brief Generate an NIST384 shared secret
 *
    @param SK an input random 48-byte secret key
    @param PK an input random public key, 49-byte compressed or 97 bytes uncompressed
    @param SS an output shared 48-byte secret
    @return true for success
 */
extern int NIST384_SHARED_SECRET(char *SK,char *PK,char *SS);


/** @brief Generate an NIST384 ECDSA signature
 *
    @param SK an input random 48-byte secret key
    @param ran an input random 56 bytes
    @param mlen the length of the input
    @param m an input message
    @param sig an output 96 byte signature
 */
extern void NIST384_SIGN(char *SK,char *ran,int mlen,char *m,char *sig);

/** @brief Generate an NIST384 shared secret
 *
    @param PK an input random public key, 49-byte compressed or 97 bytes uncompressed
    @param mlen the length of the input
    @param m an input message
    @param sig an input 96 byte signature
    @return true if signature is good, else false
 */
extern int NIST384_VERIFY(char *PK,int mlen,char *m,char *sig);



/**** NIST521 *****/

/** @brief Generate an NIST521 key pair
 *
    @param compress set true for point compression
    @param SK an input random 66-byte secret key
    @param PK an output 67 or 133 byte public key
 */
extern void NIST521_KEY_PAIR(int compress,char *SK,char *PK);

/** @brief Generate an NIST521 shared secret
 *
    @param SK an input random 66-byte secret key
    @param PK an input random public key, 67-byte compressed or 133 bytes uncompressed
    @param SS an output shared 66-byte secret
    @return true for success
 */
extern int NIST521_SHARED_SECRET(char *SK,char *PK,char *SS);


/** @brief Generate an NIST521 ECDSA signature
 *
    @param SK an input random 66-byte secret key
    @param ran an input random 74 bytes
    @param mlen the length of the input
    @param m an input message
    @param sig an output 132 byte signature
 */
extern void NIST521_SIGN(char *SK,char *ran,int mlen,char *m,char *sig);

/** @brief Generate an NIST521 shared secret
 *
    @param PK an input random public key, 67-byte compressed or 133 bytes uncompressed
    @param mlen the length of the input
    @param m an input message
    @param sig an input 132 byte signature
    @return true if signature is good, else false
 */
extern int NIST521_VERIFY(char *PK,int mlen,char *m,char *sig);
#endif