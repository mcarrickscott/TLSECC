
// API header file for Edwards and Weierstrass curves 

#ifndef NIST256CURVE_H
#define NIST256CURVE_H

// point.h generated from the curve.py script, and required for definition of point structure 
/*** Insert automatically generated point definition point.h here ***/

// elliptic curve point in projective coordinates
#include <stdint.h>

#ifndef NIST256
#define NIST256
#endif
#define WORDLENGTH 32
struct xyz {
	uint32_t x[9];
	uint32_t y[9];
	uint32_t z[9];
};
typedef struct xyz point;

/*** End of automatically generated code ***/

// api functions. char* parameters are big-endian integers of fixed length
extern int ecn256get(point *P,char *x, char *y);  // extract from point
extern void ecn256set(int s,const char *x,const char *y,point *P); // set point
extern void ecn256inf(point *P);  // set point-at-infinity 
extern int ecn256isinf(point *P); // check for point-at-infinity
extern void ecn256neg(point *P);  // negate point
extern void ecn256add(point *Q,point *P); // add Q to P 
extern void ecn256sub(point *Q,point *P); // subtract Q from P
extern void ecn256dbl(point *P);          // double P
extern void ecn256gen(point *P);          // create generator point
extern void ecn256mul(const char *e,point *P); // multiply P by e
extern void ecn256mul2(const char *e,point *P,const char *f,point *Q,point *R); // R=eP+fQ
extern int ecn256cmp(point *P,point *Q);  // compare points for equality
extern void ecn256affine(point *P);       // convert from projective (x,y,z) to (x,y,1)
extern void ecn256cpy(point *Q,point *P); // copy Q to P
extern void ecn256cof(point *P);  // multiply point by small curve co-factor

#endif