// API header file for Edwards and Weierstrass curves 

#ifndef NIST384CURVE_H
#define NIST384CURVE_H

// point.h generated from the curve.py script, and required for definition of point structure 
/*** Insert automatically generated point definition point.h here ***/

// elliptic curve point in projective coordinates
#include <stdint.h>

#ifndef NIST384
#define NIST384
#endif
#define WORDLENGTH 32
struct xyz {
	uint32_t x[14];
	uint32_t y[14];
	uint32_t z[14];
};
typedef struct xyz point;


/*** End of automatically generated code ***/

// api functions. char* parameters are big-endian integers of fixed length
extern int ecn384get(point *P,char *x, char *y);  // extract from point
extern void ecn384set(int s,const char *x,const char *y,point *P); // set point
extern void ecn384inf(point *P);  // set point-at-infinity 
extern int ecn384isinf(point *P); // check for point-at-infinity
extern void ecn384neg(point *P);  // negate point
extern void ecn384add(point *Q,point *P); // add Q to P 
extern void ecn384sub(point *Q,point *P); // subtract Q from P
extern void ecn384dbl(point *P);          // double P
extern void ecn384gen(point *P);          // create generator point
extern void ecn384mul(const char *e,point *P); // multiply P by e
extern void ecn384mul2(const char *e,point *P,const char *f,point *Q,point *R); // R=eP+fQ
extern int ecn384cmp(point *P,point *Q);  // compare points for equality
extern void ecn384affine(point *P);       // convert from projective (x,y,z) to (x,y,1)
extern void ecn384cpy(point *Q,point *P); // copy Q to P
extern void ecn384cof(point *P);  // multiply point by small curve co-factor

#endif