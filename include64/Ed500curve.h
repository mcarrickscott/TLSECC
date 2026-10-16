
// API header file for Edwards and Weierstrass curves 

#ifndef CURVE_H
#define CURVE_H

// point.h generated from the curve.py script, and required for definition of point structure 
/*** Insert automatically generated point definition point.h here ***/

// elliptic curve point in projective coordinates
#include <stdint.h>

#ifndef ED500
#define ED500
#endif
#define WORDLENGTH 64
#define FBYTES 64
struct xyz {
	uint64_t x[9];
	uint64_t y[9];
	uint64_t z[9];
};
typedef struct xyz point;


/*** End of automatically generated code ***/

// api functions. char* parameters are big-endian integers of fixed length
extern int ecn_ed500_get(point *P,char *x, char *y);  // extract from point
extern void ecn_ed500_set(int s,const char *x,const char *y,point *P); // set point
extern void ecn_ed500_inf(point *P);  // set point-at-infinity 
extern int ecn_ed500_isinf(point *P); // check for point-at-infinity
extern void ecn_ed500_neg(point *P);  // negate point
extern void ecn_ed500_add(point *Q,point *P); // add Q to P 
extern void ecn_ed500_sub(point *Q,point *P); // subtract Q from P
extern void ecn_ed500_dbl(point *P);          // double P
extern void ecn_ed500_gen(point *P);          // create generator point
extern void ecn_ed500_ran(int r,point *P);    // randomize projective point (side channel noise)
extern void ecn_ed500_mul(const char *e,point *P); // multiply P by e
extern void ecn_ed500_mul2(const char *e,point *P,const char *f,point *Q,point *R); // R=eP+fQ
extern int ecn_ed500_cmp(point *P,point *Q);  // compare points for equality
extern void ecn_ed500_affine(point *P);       // convert from projective (x,y,z) to (x,y,1)
extern void ecn_ed500_cpy(point *Q,point *P); // copy Q to P
extern void ecn_ed500_cof(point *P);  // multiply point by small curve co-factor

#endif