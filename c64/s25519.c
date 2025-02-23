// Elliptic curve based digital signature template
// See for example https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
//

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "Ed25519curve.h" // elliptic curve API

/*** Insert code automatically generated from group.c here ***/
/* Note that much of this code is not needed and can be deleted */


// Command line : python monty.py 64 ed25519

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 5
#define Radix 51
#define Nbits 253
#define Nbytes 32

#define MONTGOMERY
#define ED25519

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 51u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 51u;
  n[0] &= mask;
  for (i = 1; i < 4; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 51u;
  }
  n[4] += (spint)carry;
  return -((n[4] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0x2631a5cf5d3edu) & carry;
  n[1] += ((spint)0x3d45ef39acb02u) & carry;
  n[2] += ((spint)0x537be7u) & carry;
  n[4] += ((spint)0x1000000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  n[0] -= (spint)0x2631a5cf5d3edu;
  n[1] -= (spint)0x3d45ef39acb02u;
  n[2] -= (spint)0x537be7u;
  n[4] -= (spint)0x1000000000000u;
  return flatten(n);
}

// Modular addition - reduce less than 2p
static void inline modadd(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[0] -= (spint)0x4c634b9eba7dau;
  n[1] -= (spint)0x7a8bde7359604u;
  n[2] -= (spint)0xa6f7ceu;
  n[4] -= (spint)0x2000000000000u;
  carry = prop(n);
  n[0] += ((spint)0x4c634b9eba7dau) & carry;
  n[1] += ((spint)0x7a8bde7359604u) & carry;
  n[2] += ((spint)0xa6f7ceu) & carry;
  n[4] += ((spint)0x2000000000000u) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void inline modsub(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] - b[0];
  n[1] = a[1] - b[1];
  n[2] = a[2] - b[2];
  n[3] = a[3] - b[3];
  n[4] = a[4] - b[4];
  carry = prop(n);
  n[0] += ((spint)0x4c634b9eba7dau) & carry;
  n[1] += ((spint)0x7a8bde7359604u) & carry;
  n[2] += ((spint)0xa6f7ceu) & carry;
  n[4] += ((spint)0x2000000000000u) & carry;
  (void)prop(n);
}

// Modular negation
static void inline modneg(const spint *b, spint *n) {
  spint carry;
  n[0] = (spint)0 - b[0];
  n[1] = (spint)0 - b[1];
  n[2] = (spint)0 - b[2];
  n[3] = (spint)0 - b[3];
  n[4] = (spint)0 - b[4];
  carry = prop(n);
  n[0] += ((spint)0x4c634b9eba7dau) & carry;
  n[1] += ((spint)0x7a8bde7359604u) & carry;
  n[2] += ((spint)0xa6f7ceu) & carry;
  n[4] += ((spint)0x2000000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 29927135956970752720703206450479
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0x2631a5cf5d3edu;
  spint p1 = 0x3d45ef39acb02u;
  spint p2 = 0x537be7u;
  spint q = ((spint)1 << 51u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x51da312547e1bu;
  t += (dpint)a[0] * b[0];
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * (dpint)p0;
  t >>= 51;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  t += (dpint)v0 * (dpint)p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * (dpint)p0;
  t >>= 51;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)v0 * (dpint)p2;
  t += (dpint)v1 * (dpint)p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * (dpint)p0;
  t >>= 51;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)v1 * (dpint)p2;
  t += (dpint)v2 * (dpint)p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * (dpint)p0;
  t >>= 51;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)(udpint)((udpint)v0 << 48u);
  t += (dpint)v2 * (dpint)p2;
  t += (dpint)v3 * (dpint)p1;
  spint v4 = (((spint)t * ndash) & mask);
  t += (dpint)v4 * (dpint)p0;
  t >>= 51;
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)(udpint)((udpint)v1 << 48u);
  t += (dpint)v3 * (dpint)p2;
  t += (dpint)v4 * (dpint)p1;
  c[0] = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)(udpint)((udpint)v2 << 48u);
  t += (dpint)v4 * (dpint)p2;
  c[1] = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)(udpint)((udpint)v3 << 48u);
  c[2] = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[4] * b[4];
  t += (dpint)(udpint)((udpint)v4 << 48u);
  c[3] = ((spint)t & mask);
  t >>= 51;
  c[4] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0x2631a5cf5d3edu;
  spint p1 = 0x3d45ef39acb02u;
  spint p2 = 0x537be7u;
  spint q = ((spint)1 << 51u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x51da312547e1bu;
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (udpint)v0 * p0;
  t >>= 51;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (udpint)v1 * p0;
  t >>= 51;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v0 * p2;
  t += (udpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (udpint)v2 * p0;
  t >>= 51;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v1 * p2;
  t += (udpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (udpint)v3 * p0;
  t >>= 51;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)v0 << 48u;
  t += (udpint)v2 * p2;
  t += (udpint)v3 * p1;
  spint v4 = (((spint)t * ndash) & mask);
  t += (udpint)v4 * p0;
  t >>= 51;
  tot = (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v1 << 48u;
  t += (udpint)v3 * p2;
  t += (udpint)v4 * p1;
  c[0] = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v2 << 48u;
  t += (udpint)v4 * p2;
  c[1] = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v3 << 48u;
  c[2] = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[4] * a[4];
  t += tot;
  t += (udpint)v4 << 48u;
  c[3] = ((spint)t & mask);
  t >>= 51;
  c[4] = (spint)t;
}

// copy
static void inline modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 5; i++) {
    c[i] = a[i];
  }
}

// square n times
static void modnsqr(spint *a, int n) {
  int i;
  for (i = 0; i < n; i++) {
    modsqr(a, a);
  }
}

// Calculate progenitor
static void modpro(const spint *w, spint *z) {
  spint x[5];
  spint t0[5];
  spint t1[5];
  spint t2[5];
  spint t3[5];
  spint t4[5];
  spint t5[5];
  spint t6[5];
  spint t7[5];
  spint t8[5];
  spint t9[5];
  spint t10[5];
  spint t11[5];
  modcpy(w, x);
  modsqr(x, t1);
  modmul(x, t1, z);
  modsqr(z, t4);
  modmul(t1, t4, t8);
  modmul(z, t8, t6);
  modmul(x, t6, t3);
  modmul(t8, t3, t2);
  modmul(t4, t2, t0);
  modmul(z, t0, z);
  modmul(t4, z, t4);
  modmul(t3, t4, t10);
  modmul(t0, t10, t5);
  modmul(t8, t5, t9);
  modmul(t1, t9, t0);
  modmul(t2, t0, t7);
  modmul(t1, t7, t3);
  modmul(t1, t3, t1);
  modmul(t8, t1, t2);
  modmul(t8, t2, t8);
  modmul(t10, t9, t11);
  modnsqr(t11, 127);
  modmul(t0, t11, t11);
  modnsqr(t11, 8);
  modmul(t8, t11, t11);
  modnsqr(t11, 7);
  modmul(t2, t11, t11);
  modnsqr(t11, 6);
  modmul(t10, t11, t10);
  modnsqr(t10, 8);
  modmul(t9, t10, t9);
  modnsqr(t9, 8);
  modmul(t8, t9, t8);
  modnsqr(t8, 7);
  modmul(t7, t8, t7);
  modnsqr(t7, 9);
  modmul(t1, t7, t7);
  modnsqr(t7, 6);
  modmul(t6, t7, t6);
  modnsqr(t6, 13);
  modmul(t5, t6, t5);
  modnsqr(t5, 6);
  modmul(t4, t5, t4);
  modnsqr(t4, 10);
  modmul(t3, t4, t3);
  modnsqr(t3, 8);
  modmul(t2, t3, t2);
  modnsqr(t2, 7);
  modmul(t1, t2, t1);
  modnsqr(t1, 7);
  modmul(t0, t1, t0);
  modnsqr(t0, 5);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  int i;
  spint s[5];
  spint t[5];
  if (h == NULL) {
    modpro(x, t);
  } else {
    modcpy(h, t);
  }
  modcpy(x, s);
  for (i = 0; i < (2 - 1); i++) {
    modsqr(s, s);
    modmul(s, x, s);
  }
  modnsqr(t, 3);
  modmul(s, t, z);
}

// Convert m to n-residue form, n=nres(m)
static void nres(const spint *m, spint *n) {
  const spint c[5] = {0x74ecc96df62b2u, 0x21a8b634e5561u, 0x7f5be660eab09u,
                      0x347b9dd8e7a42u, 0xce65046df0c2u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[5];
  c[0] = 1;
  for (i = 1; i < 5; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[5];
  for (i = 0; i < 5; i++) {
    b[i] = n[i];
    t[i] = n[i + 5];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[5];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 5; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 51u) &
          (((c0 ^ (spint)1) - (spint)1) >> 51u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[5];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 5; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 51u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 5; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 5; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 5; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[5];
  if (h == NULL) {
    modpro(x, r);
    modsqr(r, r);
  } else {
    modsqr(h, r);
  }
  modmul(r, x, r);
  modnsqr(r, 1);
  return modis1(r) | modis0(x);
}

// conditional move g to f if d=1
static void modcmv(int d, const spint *g, volatile spint *f) {
  int i;
  spint c0,c1,s,t;
  spint r=0x3cc3c33c5aa5a55a;
  c0=(~d)&(r+1);
  c1=d+r;
  for (i = 0; i < 5; i++) {
      s=g[i]; t=f[i];
      f[i] =c0*t+c1*s;
      f[i]-=r*(t+s); 
  }
}

// conditional swap g and f if d=1
static void modcsw(int d, volatile spint *g, volatile spint *f) {
  int i;
  spint c0,c1,s,t,w;
  spint r=0x3cc3c33c5aa5a55a;
  c0=(~d)&(r+1);
  c1=d+r;
  for (i = 0; i < 5; i++) {
      s=g[i]; t=f[i];
      w=r*(t+s);
      f[i] =c0*t+c1*s;
      f[i]-=w;  
      g[i] =c0*s+c1*t;
      g[i]-=w;   
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  int k;
  spint t[5];
  spint b[5];
  spint v[5];
  spint z[5] = {0x775dfebbe07d4u, 0x26859d07fd7d0u, 0x70709e83bc159u,
                0xf3be9eb6b055u, 0x94a7310e0798u};
  spint s[5];
  spint y[5];
  if (h == NULL) {
    modpro(x, y);
  } else {
    modcpy(h, y);
  }
  modmul(y, x, s);
  modmul(s, y, t);
  nres(z, z);
  for (k = 2; k > 1; k--) {
    modcpy(t, b);
    modnsqr(b, k - 2);
    int d = 1 - modis1(b);
    modmul(s, z, v);
    modcmv(d, v, s);
    modsqr(z, z);
    modmul(t, z, v);
    modcmv(d, v, t);
  }
  modcpy(s, r);
}

// shift left by less than a word
static void modshl(unsigned int n, spint *a) {
  int i;
  a[4] = ((a[4] << n)) | (a[3] >> (51u - n));
  for (i = 3; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x7ffffffffffff) | (a[i - 1] >> (51u - n));
  }
  a[0] = (a[0] << n) & (spint)0x7ffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 4; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (51u - n)) & (spint)0x7ffffffffffff);
  }
  a[4] = a[4] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 51u;
  unsigned int m = r % 51u;
  modzer(a);
  if (r >= 32 * 8)
    return;
  a[n] = 1;
  a[n] <<= m;
  nres(a, a);
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[5];
  redc(a, c);
  for (i = 31; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr(8, c);
  }
}

// import from byte array
// returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
  int i, res;
  for (i = 0; i < 5; i++) {
    a[i] = 0;
  }
  for (i = 0; i < 32; i++) {
    modshl(8, a);
    a[0] += (spint)(unsigned char)b[i];
  }
  res = modfsb(a);
  nres(a, a);
  return res;
}

// determine sign
static int modsign(const spint *a) {
  spint c[5];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[5], d[5];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 5; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 51) & 1;
  }
  return eq;
}

/*** End of automatically generated code ***/

typedef spint gel[Nlimbs];  // group element definition
#define BYTES Nbytes        // Number of bytes in group element

// some general purpose functions

// reverse bytes of buff - for endian conversion
static void reverse(char *buff) {
    int n=BYTES;
    for (int i = 0; i < n/2; i++) { 
        char ch = buff[i]; 
        buff[i] = buff[n - i - 1]; 
        buff[n - i - 1] = ch; 
    } 
}

static int char2int(char input)
{
    if ((input >= '0') && (input <= '9'))
        return input - '0';
    if ((input >= 'A') && (input <= 'F'))
        return input - 'A' + 10;
    if ((input >= 'a') && (input <= 'f'))
        return input - 'a' + 10;
    return 0;
}

// Convert from a hex string to byte array 
static void fromHex(int ilen, const char *src, char *dst)
{
    int i,lz,len=0;
    char pad[128];
    while (src[len]!=0) len++;
    lz=2*ilen-len;
    if (lz<0) lz=0;
    for (i=0;i<lz;i++) pad[i]='0';  // pad with leading zeros
    for (i=lz;i<2*ilen;i++) pad[i]=src[i-lz];

    for (i=0;i<ilen;i++)
    {
        dst[i] = (char2int(pad[2*i]) * 16) + char2int(pad[2*i + 1]);
    }
}

static void byte2hex(char *ptr,unsigned char ch)
{
    int t=ch/16;
    int b=ch%16;
    if (t<10)
    	ptr[0]='0'+t;
    else
    	ptr[0]='a'+(t-10);
    if (b<10)
    	ptr[1]='0'+b;
    else
    	ptr[1]='a'+(b-10);    	
}

// Convert a byte array to a hex string 
static void toHex(int len, const char *src, char *dst)
{
    int i;
    for (i = 0; i < len; i++)
    {
        unsigned char ch = src[i];
        byte2hex(&dst[i * 2],ch);
    }
    dst[2*len]='\0';
}


// reduce 64 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^248.(2^248.x + y)  + z, where x,y and z < q (z is bottom 31 bytes, y is next 31 bytes, x is top 2 bytes)
// Important that x,y and z < q, 31 bytes = 248 bits, q is 256 bits
static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,z,c;

    mod2r(248,c);
   
    for (i=0;i<31;i++)  // bottom 31 bytes
        buff[i]=h[i];
    buff[31]=0;
    reverse(buff);
    modimp(buff,z);  
    
    for (i=0;i<31;i++)  // middle 31 bytes
        buff[i]=h[i+31];
    buff[31]=0;
    reverse(buff);
    modimp(buff,y);  
    
    for (i=0;i<2;i++)  // top 4 bytes
       buff[i]=h[62+i];   
    for (i=2;i<32;i++)
        buff[i]=0;
    reverse(buff);
    modimp(buff,x);    
    
    modmul(x,c,x); 
    modadd(x,y,x); 
    modmul(x,c,x); 
    modadd(x,z,r);  
}

// general purpose HASH512 hash function
// Input ilen bytes, output 64 bytes
static void H(int ilen,char *s,char *digest)
{
    hash512 sh512;
    HASH512_init(&sh512);
    for (int i=0;i<ilen;i++) 
        HASH512_process(&sh512,s[i]);
    HASH512_hash(&sh512,digest); 
}

// API implementation

// Input private key
// Output public key
void ED25519_KEY_PAIR(char *prv,char *pub)
{
    int sign;
    point G;
    ecn25519gen(&G);  // get curve generator point
    char s[2*BYTES];
    H(BYTES,prv,s);
    s[0]&=0xF8;
    s[BYTES-1]&=0x7F; s[BYTES-1]|=0x40;
    reverse(s);
    ecn25519mul(s,&G); 
    sign=ecn25519get(&G,NULL,pub);  // get y coordinate and sign
    reverse(pub);              // big endian to little endian  
    pub[BYTES-1]|=(char)(sign<<7);    
}

// Note that a per message "random" number usually generated by a Cryptographically Secure Random Number Generator (CSRNG) is often required, and if so is calculated externally and passed into the signature function
// Methods are
// 1. non-deterministic - a non-biased high entropy random number in the range from 0 - q (the group order) - recommended ii CSRNG is good
// 2. deterministic - derived from a hash of the message to be signed and the secret key (must be done like this for EdDSA, optional for ECDSA)
// 3. hedged - a combination of methods 1 and 2 - recommended if CSRNG is poor
// Note that this functionality is out-of-scope for this exercise (except for EdDSA). For examples see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf and RFC6979

// To create such a per message random number, consider the method reduce() from Ed448.c, which reduces a much larger number modulo q, as required to minimize bias

// input private key, per-message random number (or public key), message length, message to be signed
// Output signature.
void ED25519_SIGN(char *prv,char *pub,int mlen,char *m,char *sig)
{
    int i,sign;
    point G;
    gel r,s,d;     // some group elements..
    hash512 sh512;
    char h[2*BYTES];   
    char ipub[BYTES];
    ecn25519gen(&G);  // get curve generator point
 
    if (pub!=NULL)
    {
        for (i=0;i<BYTES;i++)
            ipub[i]=pub[i];
    } else {
        ED25519_KEY_PAIR(prv,ipub);
    }

    HASH512_init(&sh512);
    H(BYTES,prv,h);
    
    h[0]&=0xF8;
    h[BYTES-1]&=0x7F; h[BYTES-1]|=0x40;
    reverse(h);
    modimp(h,s);
    
    for (i=BYTES;i<2*BYTES;i++) 
    	HASH512_process(&sh512,h[i]); 
    for (i=0;i<mlen;i++)
        HASH512_process(&sh512,m[i]);    
    HASH512_hash(&sh512,h);
    
    reduce(h,r);
    modexp(r,h);  // convert to big endian array
    ecn25519mul(h,&G);
    
    sign=ecn25519get(&G,NULL,sig);  // get y coordinate and sign
    reverse(sig);              // big endian to little endian
    sig[BYTES-1]|=(char)(sign<<7); // first part of signature
    
    HASH512_init(&sh512);
    for (i=0;i<BYTES;i++ )
        HASH512_process(&sh512,sig[i]);  // R
    for (i=0;i<BYTES;i++)
        HASH512_process(&sh512,ipub[i]);  // Q
    for (i=0;i<mlen;i++)
        HASH512_process(&sh512,m[i]);   // M 
    HASH512_hash(&sh512,h);
    reduce(h,d);
    modmul(d,s,d);
    modadd(d,r,d);

    modexp(d,&sig[BYTES]);
    reverse(&sig[BYTES]); // second part of signature         
}

// input public key, message length message and signature. 
// Return 1 for success, 0 for failure
int ED25519_VERIFY(char *pub,int mlen,char *m,char *sig) 
{
    int i,sign;
    char buff[BYTES]; 
    char h[2*BYTES];        
    point G,R,Q;
    gel u;     // some group elements..
    hash512 sh512;
        
    ecn25519gen(&G);  // get curve generator point

// reconstruct point R
    sign=(sig[BYTES-1]>>7)&1;
    for (i=0;i<BYTES;i++)
        buff[i]=sig[i];
    buff[BYTES-1]&=0x7f;        
    reverse(buff);
    ecn25519set(sign,NULL,buff,&R);  
    if (ecn25519isinf(&R)) return 0;
   
// reconstruct point Q 
    sign=(pub[BYTES-1]>>7)&1;
    for (i=0;i<BYTES;i++)
        buff[i]=pub[i];
    buff[BYTES-1]&=0x7f;
    reverse(buff);
    ecn25519set(sign,NULL,buff,&Q); 
    if (ecn25519isinf(&Q)) return 0;
   
    for (i=0;i<BYTES;i++)
        buff[i]=sig[i+BYTES];
    reverse(buff);  
    
    HASH512_init(&sh512);
    for (i=0;i<BYTES;i++ )
        HASH512_process(&sh512,sig[i]);  // R
    for (i=0;i<BYTES;i++)
        HASH512_process(&sh512,pub[i]);  // Q
    for (i=0;i<mlen;i++)
        HASH512_process(&sh512,m[i]);   // M 
    HASH512_hash(&sh512,h);     
    reduce(h,u); modneg(u,u); modexp(u,h);

    if (!modimp(buff,u)) return 0;  // out of range

    ecn25519cof(&G); ecn25519cof(&R); ecn25519cof(&Q);
    ecn25519mul2(buff,&G,h,&Q,&Q);

    if (ecn25519cmp(&R,&Q))
        return 1;
    return 0;   
}

