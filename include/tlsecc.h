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


/**** X448 ****/

/** @brief Generate an X448 key pair
 *
    @param SK an input random 56-byte secret key
    @param PK an output random 56-byte public key
 */
extern void X448_KEY_PAIR(char *SK,char *PK);

/** @brief Generate an X448 shared secret
 *
    @param SK an input random 56-byte secret key
    @param PK an input random 56-byte public key
    @param SS an output shared 56-byte secret
    @return true for success
 */
extern int X448_SHARED_SECRET(char *SK,char *PK,char *SS);



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


/** @brief prehash message for ECDSA
 *
    @param sha indicates length of SHA2 hash function
    @param mlen the length of the input message
    @param m an input message
    @param thm the output truncated hash of input message
    @return true for success
 */
extern int NIST256_PREHASH(int sha,int mlen,char *m,char * thm);

/** @brief Generate an NIST256 ECDSA signature
 *
    @param SK an input random 32-byte secret key
    @param ran an input random 40 bytes
    @param thm truncated hash of input message
    @param sig an output 64 byte signature
 */
extern void NIST256_SIGN(char *SK,char *ran,char *thm,char *sig);

/** @brief Verify an NIST256 signature
 *
    @param PK an input random public key, 33-byte compressed or 65 bytes uncompressed
    @param thm truncated hash of input message
    @param sig an input 64 byte signature
    @return true if signature is good, else false
 */
extern int NIST256_VERIFY(char *PK,char *thm,char *sig);


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

/** @brief prehash message for ECDSA
 *
    @param sha indicates length of SHA2 hash function
    @param mlen the length of the input message
    @param m an input message
    @param thm the output truncated hash of input message
    @return true for success
 */
extern int NIST384_PREHASH(int sha,int mlen,char *m,char * thm);

/** @brief Generate an NIST384 ECDSA signature
 *
    @param SK an input random 48-byte secret key
    @param ran an input random 56 bytes
    @param thm truncated hash of input message
    @param sig an output 96 byte signature
 */
extern void NIST384_SIGN(char *SK,char *ran,char *thm,char *sig);

/** @brief Verify an NIST384 signature
 *
    @param PK an input random public key, 49-byte compressed or 97 bytes uncompressed
    @param thm truncated hash of input message
    @param sig an input 96 byte signature
    @return true if signature is good, else false
 */
extern int NIST384_VERIFY(char *PK,char *thm,char *sig);



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

/** @brief prehash message for ECDSA
 *
    @param sha indicates length of SHA2 hash function
    @param mlen the length of the input message
    @param m an input message
    @param thm the output truncated hash of input message
    @return true for success
 */
extern int NIST521_PREHASH(int sha,int mlen,char *m,char * thm);

/** @brief Generate an NIST521 ECDSA signature
 *
    @param SK an input random 66-byte secret key
    @param ran an input random 74 bytes
    @param thm truncated hash of input message
    @param sig an output 132 byte signature
 */
extern void NIST521_SIGN(char *SK,char *ran,char *thm,char *sig);

/** @brief Verify an NIST521 signature
 *
    @param PK an input random public key, 67-byte compressed or 133 bytes uncompressed
    @param thm truncated hash of input message
    @param sig an input 132 byte signature
    @return true if signature is good, else false
 */
extern int NIST521_VERIFY(char *PK,char *thm,char *sig);


/**** Ed448 *****/

/** @brief Generate an Ed448 key pair
 *
    @param compress set true for point compression
    @param SK an input random 57-byte secret key
    @param PK an output 57 byte public key
 */
extern void ED448_KEY_PAIR(char *SK,char *PK);

/** @brief Generate an Ed448 EdDSA signature
 *
    @param SK an input 57-byte secret key
    @param PK an input 57 byte public key
    @param mlen the length of the input
    @param m an input message
    @param sig an output 114 byte signature
 */
extern void ED448_SIGN(char *SK,char *PK,int mlen,char *m,char *sig);

/** @brief Verify an Ed448 signature
 *
    @param PK an input random public key, 57 vytes
    @param mlen the length of the input
    @param m an input message
    @param sig an input 114 byte signature
    @return true if signature is good, else false
 */
extern int ED448_VERIFY(char *PK,int mlen,char *m,char *sig);



/**** Ed25519 *****/

/** @brief Generate an Ed25519 key pair
 *
    @param compress set true for point compression
    @param SK an input random 32-byte secret key
    @param PK an output 32 byte public key
 */
extern void ED25519_KEY_PAIR(char *SK,char *PK);

/** @brief Generate an Ed25519 EdDSA signature
 *
    @param SK an input 32-byte secret key
    @param PK an input 32 byte public key
    @param mlen the length of the input
    @param m an input message
    @param sig an output 64 byte signature
 */
extern void ED25519_SIGN(char *SK,char *PK,int mlen,char *m,char *sig);

/** @brief Verify an Ed25519 signature
 *
    @param PK an input random public key, 32 vytes
    @param mlen the length of the input
    @param m an input message
    @param sig an input 64 byte signature
    @return true if signature is good, else false
 */
extern int ED25519_VERIFY(char *PK,int mlen,char *m,char *sig);

#endif