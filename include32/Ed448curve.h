// API header file for Edwards and Weierstrass curves 

#ifndef Ed448CURVE_H
#define Ed448CURVE_H

// point.h generated from the curve.py script, and required for definition of point structure 
/*** Insert automatically generated point definition point.h here ***/

// elliptic curve point in projective coordinates
#include <stdint.h>

#ifndef ED448
#define ED448
#endif
#define WORDLENGTH 32
struct xyz {
	uint32_t x[16];
	uint32_t y[16];
	uint32_t z[16];
};
typedef struct xyz point;

/*** End of automatically generated code ***/

// api functions. char* parameters are big-endian integers of fixed length
extern int ecn448get(point *P,char *x, char *y);  // extract from point
extern void ecn448set(int s,const char *x,const char *y,point *P); // set point
extern void ecn448inf(point *P);  // set point-at-infinity 
extern int ecn448isinf(point *P); // check for point-at-infinity
extern void ecn448neg(point *P);  // negate point
extern void ecn448add(point *Q,point *P); // add Q to P 
extern void ecn448sub(point *Q,point *P); // subtract Q from P
extern void ecn448dbl(point *P);          // double P
extern void ecn448gen(point *P);          // create generator point
extern void ecn448mul(const char *e,point *P); // multiply P by e
extern void ecn448mul2(const char *e,point *P,const char *f,point *Q,point *R); // R=eP+fQ
extern int ecn448cmp(point *P,point *Q);  // compare points for equality
extern void ecn448affine(point *P);       // convert from projective (x,y,z) to (x,y,1)
extern void ecn448cpy(point *Q,point *P); // copy Q to P
extern void ecn448cof(point *P);  // multiply point by small curve co-factor

#endif