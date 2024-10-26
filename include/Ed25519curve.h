// API header file for Edwards and Weierstrass curves 

#ifndef Ed25519CURVE_H
#define Ed25519CURVE_H

// point.h generated from the curve.py script, and required for definition of point structure 
/*** Insert automatically generated point definition point.h here ***/

// elliptic curve point in projective coordinates
#include <stdint.h>

#ifndef ED25519
#define ED25519
#endif
#define WORDLENGTH 64
struct xyz {
	uint64_t x[5];
	uint64_t y[5];
	uint64_t z[5];
};
typedef struct xyz point;

/*** End of automatically generated code ***/

// api functions. char* parameters are big-endian integers of fixed length
extern int ecn25519get(point *P,char *x, char *y);  // extract from point
extern void ecn25519set(int s,const char *x,const char *y,point *P); // set point
extern void ecn25519inf(point *P);  // set point-at-infinity 
extern int ecn25519isinf(point *P); // check for point-at-infinity
extern void ecn25519neg(point *P);  // negate point
extern void ecn25519add(point *Q,point *P); // add Q to P 
extern void ecn25519sub(point *Q,point *P); // subtract Q from P
extern void ecn25519dbl(point *P);          // double P
extern void ecn25519gen(point *P);          // create generator point
extern void ecn25519mul(const char *e,point *P); // multiply P by e
extern void ecn25519mul2(const char *e,point *P,const char *f,point *Q,point *R); // R=eP+fQ
extern int ecn25519cmp(point *P,point *Q);  // compare points for equality
extern void ecn25519affine(point *P);       // convert from projective (x,y,z) to (x,y,1)
extern void ecn25519cpy(point *Q,point *P); // copy Q to P
extern void ecn25519cof(point *P);  // multiply point by small curve co-factor

#endif