#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// For example: cargo run --release --features NIST384 --bin EC384
// python curve_rust.py 64 NIST384
// This completes weierstrass.rs for this curve
// ECDSA Implementation for curve P-384
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf

use crate::nist384;
use crate::nist384::*;
use crate::nist384::ECP;

use crate::hash;
use crate::hash::*;

/*** Insert code automatically generated in group.rs here ***/
/* Note that much of this code is not needed and can be deleted */

type SPINT = u64;
type SSPINT = i64;
type UDPINT = u128;
type DPINT = u128;
//propagate carries
#[allow(unused_variables)]
#[inline]
fn prop(n: &mut [SPINT]) -> SPINT {
    let mask = ((1 as SPINT) << 56) - 1;
    let mut carry = (n[0] as SSPINT) >> 56;
    n[0] &= mask;
    for i in 1..6 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 56;
    }
    n[6] += carry as SPINT;
    return ((n[6] as SSPINT) >> 63) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 56;
    let carry = prop(n);
    n[0] += (0xec196accc52973 as SPINT) & (carry as SPINT);
    n[1] += (0xdb248b0a77aec as SPINT) & (carry as SPINT);
    n[2] += (0x81f4372ddf581a as SPINT) & (carry as SPINT);
    n[3] += (0xffffffffc7634d as SPINT) & (carry as SPINT);
    n[4] -= 1 & (carry as SPINT);
    n[6] += (0x1000000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
#[inline]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 56;
    n[0] -= 0xec196accc52973 as SPINT;
    n[1] -= 0xdb248b0a77aec as SPINT;
    n[2] -= 0x81f4372ddf581a as SPINT;
    n[3] -= 0xffffffffc7634d as SPINT;
    n[4] += 1;
    n[6] -= 0x1000000000000 as SPINT;
    return flatten(n);
}

//Modular addition - reduce less than 2p
#[allow(unused_variables)]
#[inline]
fn modadd(b: &[SPINT], n: &mut [SPINT]) {
    n[0] = n[0] + b[0];
    n[1] = n[1] + b[1];
    n[2] = n[2] + b[2];
    n[3] = n[3] + b[3];
    n[4] = n[4] + b[4];
    n[5] = n[5] + b[5];
    n[6] = n[6] + b[6];
    n[0] -= 0x1d832d5998a52e6 as SPINT;
    n[1] -= 0x1b6491614ef5d8 as SPINT;
    n[2] -= 0x103e86e5bbeb034 as SPINT;
    n[3] -= 0x1ffffffff8ec69a as SPINT;
    n[4] += 2;
    n[6] -= 0x2000000000000 as SPINT;
    let carry = prop(n);
    n[0] += (0x1d832d5998a52e6 as SPINT) & (carry as SPINT);
    n[1] += (0x1b6491614ef5d8 as SPINT) & (carry as SPINT);
    n[2] += (0x103e86e5bbeb034 as SPINT) & (carry as SPINT);
    n[3] += (0x1ffffffff8ec69a as SPINT) & (carry as SPINT);
    n[4] -= 2 & (carry as SPINT);
    n[6] += (0x2000000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

//Modular subtraction - reduce less than 2p
#[allow(unused_variables)]
#[inline]
fn modsub(b: &[SPINT], n: &mut [SPINT]) {
    n[0] = n[0] - b[0];
    n[1] = n[1] - b[1];
    n[2] = n[2] - b[2];
    n[3] = n[3] - b[3];
    n[4] = n[4] - b[4];
    n[5] = n[5] - b[5];
    n[6] = n[6] - b[6];
    let carry = prop(n);
    n[0] += (0x1d832d5998a52e6 as SPINT) & (carry as SPINT);
    n[1] += (0x1b6491614ef5d8 as SPINT) & (carry as SPINT);
    n[2] += (0x103e86e5bbeb034 as SPINT) & (carry as SPINT);
    n[3] += (0x1ffffffff8ec69a as SPINT) & (carry as SPINT);
    n[4] -= 2 & (carry as SPINT);
    n[6] += (0x2000000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

//Modular negation
#[allow(unused_variables)]
#[inline]
fn modneg(n: &mut [SPINT]) {
    n[0] = (0 as SPINT) - n[0];
    n[1] = (0 as SPINT) - n[1];
    n[2] = (0 as SPINT) - n[2];
    n[3] = (0 as SPINT) - n[3];
    n[4] = (0 as SPINT) - n[4];
    n[5] = (0 as SPINT) - n[5];
    n[6] = (0 as SPINT) - n[6];
    let carry = prop(n);
    n[0] += (0x1d832d5998a52e6 as SPINT) & (carry as SPINT);
    n[1] += (0x1b6491614ef5d8 as SPINT) & (carry as SPINT);
    n[2] += (0x103e86e5bbeb034 as SPINT) & (carry as SPINT);
    n[3] += (0x1ffffffff8ec69a as SPINT) & (carry as SPINT);
    n[4] -= 2 & (carry as SPINT);
    n[6] += (0x2000000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
#[inline]
fn modmli(_b: usize, _c: &mut [SPINT]) {
    return;
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 49260895723021306548442156017426496
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
#[inline]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0xec196accc52973 as SPINT;
    let p1 = 0xdb248b0a77aec as SPINT;
    let p2 = 0x81f4372ddf581a as SPINT;
    let p3 = 0xffffffffc7634d as SPINT;
    let q = (1 as SPINT) << 56; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0xd46089e88fdc45 as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as DPINT) * (p0 as DPINT);
    t >>= 56;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p1 as DPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as DPINT) * (p0 as DPINT);
    t >>= 56;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p2 as DPINT);
    t += (v1 as DPINT) * (p1 as DPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as DPINT) * (p0 as DPINT);
    t >>= 56;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v1 as DPINT) * (p2 as DPINT);
    t += (v2 as DPINT) * (p1 as DPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as DPINT) * (p0 as DPINT);
    t >>= 56;
    t += (c[0] as DPINT) * (b[4] as DPINT);
    t += (c[1] as DPINT) * (b[3] as DPINT);
    t += (c[2] as DPINT) * (b[2] as DPINT);
    t += (c[3] as DPINT) * (b[1] as DPINT);
    t += (c[4] as DPINT) * (b[0] as DPINT);
    t += (q - v0) as DPINT;
    t += (v1 as DPINT) * (p3 as DPINT);
    t += (v2 as DPINT) * (p2 as DPINT);
    t += (v3 as DPINT) * (p1 as DPINT);
    let v4 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v4 as DPINT) * (p0 as DPINT);
    t >>= 56;
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    s -= v1;
    t += (v2 as DPINT) * (p3 as DPINT);
    t += (v3 as DPINT) * (p2 as DPINT);
    t += (v4 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as DPINT) * (p0 as DPINT);
    t >>= 56;
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    t += (v0 as DPINT) << 48;
    s -= v2;
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as DPINT) * (p0 as DPINT);
    t >>= 56;
    t += (c[1] as DPINT) * (b[6] as DPINT);
    t += (c[2] as DPINT) * (b[5] as DPINT);
    t += (c[3] as DPINT) * (b[4] as DPINT);
    t += (c[4] as DPINT) * (b[3] as DPINT);
    t += (c[5] as DPINT) * (b[2] as DPINT);
    t += (c[6] as DPINT) * (b[1] as DPINT);
    s = mask as SPINT;
    t += (v1 as DPINT) << 48;
    s -= v3;
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    t += (c[2] as DPINT) * (b[6] as DPINT);
    t += (c[3] as DPINT) * (b[5] as DPINT);
    t += (c[4] as DPINT) * (b[4] as DPINT);
    t += (c[5] as DPINT) * (b[3] as DPINT);
    t += (c[6] as DPINT) * (b[2] as DPINT);
    s = mask as SPINT;
    t += (v2 as DPINT) << 48;
    s -= v4;
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += s as DPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    t += (c[3] as DPINT) * (b[6] as DPINT);
    t += (c[4] as DPINT) * (b[5] as DPINT);
    t += (c[5] as DPINT) * (b[4] as DPINT);
    t += (c[6] as DPINT) * (b[3] as DPINT);
    s = mask as SPINT;
    t += (v3 as DPINT) << 48;
    s -= v5;
    t += (v6 as DPINT) * (p3 as DPINT);
    t += s as DPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    t += (c[4] as DPINT) * (b[6] as DPINT);
    t += (c[5] as DPINT) * (b[5] as DPINT);
    t += (c[6] as DPINT) * (b[4] as DPINT);
    s = mask as SPINT;
    t += (v4 as DPINT) << 48;
    s -= v6;
    t += s as DPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    t += (c[5] as DPINT) * (b[6] as DPINT);
    t += (c[6] as DPINT) * (b[5] as DPINT);
    s = mask as SPINT;
    t += (v5 as DPINT) << 48;
    t += s as DPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    t += (c[6] as DPINT) * (b[6] as DPINT);
    s = mask as SPINT;
    t += (v6 as DPINT) << 48;
    t += s as DPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    t -= 1;
    c[6] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
#[inline]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let p0 = 0xec196accc52973 as SPINT;
    let p1 = 0xdb248b0a77aec as SPINT;
    let p2 = 0x81f4372ddf581a as SPINT;
    let p3 = 0xffffffffc7634d as SPINT;
    let q = (1 as SPINT) << 56; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0xd46089e88fdc45 as SPINT;
    tot = (c[0] as UDPINT) * (c[0] as UDPINT);
    t = tot;
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as UDPINT) * (p0 as UDPINT);
    t >>= 56;
    tot = (c[0] as UDPINT) * (c[1] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p1 as UDPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as UDPINT) * (p0 as UDPINT);
    t >>= 56;
    tot = (c[0] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    tot += (c[1] as UDPINT) * (c[1] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p2 as UDPINT);
    t += (v1 as UDPINT) * (p1 as UDPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as UDPINT) * (p0 as UDPINT);
    t >>= 56;
    tot = (c[0] as UDPINT) * (c[3] as UDPINT);
    tot += (c[1] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p3 as UDPINT);
    t += (v1 as UDPINT) * (p2 as UDPINT);
    t += (v2 as UDPINT) * (p1 as UDPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as UDPINT) * (p0 as UDPINT);
    t >>= 56;
    tot = (c[0] as UDPINT) * (c[4] as UDPINT);
    tot += (c[1] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    tot += (c[2] as UDPINT) * (c[2] as UDPINT);
    t += tot;
    t += (q - v0) as UDPINT;
    t += (v1 as UDPINT) * (p3 as UDPINT);
    t += (v2 as UDPINT) * (p2 as UDPINT);
    t += (v3 as UDPINT) * (p1 as UDPINT);
    let v4 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v4 as UDPINT) * (p0 as UDPINT);
    t >>= 56;
    tot = (c[0] as UDPINT) * (c[5] as UDPINT);
    tot += (c[1] as UDPINT) * (c[4] as UDPINT);
    tot += (c[2] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s -= v1;
    t += (v2 as UDPINT) * (p3 as UDPINT);
    t += (v3 as UDPINT) * (p2 as UDPINT);
    t += (v4 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as UDPINT) * (p0 as UDPINT);
    t >>= 56;
    tot = (c[0] as UDPINT) * (c[6] as UDPINT);
    tot += (c[1] as UDPINT) * (c[5] as UDPINT);
    tot += (c[2] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    tot += (c[3] as UDPINT) * (c[3] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v0 as UDPINT) << 48;
    s -= v2;
    t += (v3 as UDPINT) * (p3 as UDPINT);
    t += (v4 as UDPINT) * (p2 as UDPINT);
    t += (v5 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as UDPINT) * (p0 as UDPINT);
    t >>= 56;
    tot = (c[1] as UDPINT) * (c[6] as UDPINT);
    tot += (c[2] as UDPINT) * (c[5] as UDPINT);
    tot += (c[3] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v1 as UDPINT) << 48;
    s -= v3;
    t += (v4 as UDPINT) * (p3 as UDPINT);
    t += (v5 as UDPINT) * (p2 as UDPINT);
    t += (v6 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    tot = (c[2] as UDPINT) * (c[6] as UDPINT);
    tot += (c[3] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    tot += (c[4] as UDPINT) * (c[4] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v2 as UDPINT) << 48;
    s -= v4;
    t += (v5 as UDPINT) * (p3 as UDPINT);
    t += (v6 as UDPINT) * (p2 as UDPINT);
    t += s as UDPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    tot = (c[3] as UDPINT) * (c[6] as UDPINT);
    tot += (c[4] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v3 as UDPINT) << 48;
    s -= v5;
    t += (v6 as UDPINT) * (p3 as UDPINT);
    t += s as UDPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    tot = (c[4] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    tot += (c[5] as UDPINT) * (c[5] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v4 as UDPINT) << 48;
    s -= v6;
    t += s as UDPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    tot = (c[5] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v5 as UDPINT) << 48;
    t += s as UDPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    tot = (c[6] as UDPINT) * (c[6] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v6 as UDPINT) << 48;
    t += s as UDPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 56;
    t -= 1;
    c[6] = t as SPINT;
    return;
}

//copy
#[inline]
fn modcpy(a: &[SPINT], c: &mut [SPINT]) {
    for i in 0..7 {
        c[i] = a[i];
    }
    return;
}

//square n times
fn modnsqr(a: &mut [SPINT], n: isize) {
    for _i in 0..n {
        modsqr(a);
    }
}

//Calculate progenitor
fn modpro(w: &[SPINT], r: &mut [SPINT]) {
    let mut x: [SPINT; 7] = [0; 7];
    let mut z: [SPINT; 7] = [0; 7];
    let mut t0: [SPINT; 7] = [0; 7];
    let mut t1: [SPINT; 7] = [0; 7];
    let mut t2: [SPINT; 7] = [0; 7];
    let mut t3: [SPINT; 7] = [0; 7];
    let mut t4: [SPINT; 7] = [0; 7];
    let mut t5: [SPINT; 7] = [0; 7];
    let mut t6: [SPINT; 7] = [0; 7];
    let mut t7: [SPINT; 7] = [0; 7];
    let mut t8: [SPINT; 7] = [0; 7];
    let mut t9: [SPINT; 7] = [0; 7];
    let mut t10: [SPINT; 7] = [0; 7];
    modcpy(w, &mut x);
    modcpy(&x, &mut t3);
    modsqr(&mut t3);
    modcpy(&x, &mut t1);
    modmul(&t3, &mut t1);
    modcpy(&t3, &mut t0);
    modmul(&t1, &mut t0);
    modcpy(&t3, &mut t2);
    modmul(&t0, &mut t2);
    modcpy(&t3, &mut t4);
    modmul(&t2, &mut t4);
    modcpy(&t3, &mut z);
    modmul(&t4, &mut z);
    modcpy(&t3, &mut t5);
    modmul(&z, &mut t5);
    modmul(&t5, &mut t3);
    modcpy(&t3, &mut t6);
    modsqr(&mut t6);
    modmul(&x, &mut t6);
    modcpy(&t6, &mut t8);
    modnsqr(&mut t8, 2);
    modcpy(&t8, &mut t9);
    modsqr(&mut t9);
    modcpy(&t9, &mut t7);
    modsqr(&mut t7);
    modcpy(&t7, &mut t10);
    modnsqr(&mut t10, 5);
    modmul(&t10, &mut t7);
    modcpy(&t7, &mut t10);
    modnsqr(&mut t10, 10);
    modmul(&t10, &mut t7);
    modcpy(&t7, &mut t10);
    modnsqr(&mut t10, 4);
    modmul(&t10, &mut t9);
    modnsqr(&mut t9, 21);
    modmul(&t9, &mut t7);
    modcpy(&t7, &mut t9);
    modnsqr(&mut t9, 3);
    modmul(&t9, &mut t8);
    modnsqr(&mut t8, 47);
    modmul(&t8, &mut t7);
    modcpy(&t7, &mut t8);
    modnsqr(&mut t8, 95);
    modmul(&t8, &mut t7);
    modmul(&t3, &mut t7);
    modnsqr(&mut t7, 6);
    modmul(&t2, &mut t7);
    modnsqr(&mut t7, 3);
    modmul(&t1, &mut t7);
    modnsqr(&mut t7, 7);
    modmul(&t5, &mut t7);
    modnsqr(&mut t7, 6);
    modmul(&t5, &mut t7);
    modsqr(&mut t7);
    modmul(&x, &mut t7);
    modnsqr(&mut t7, 11);
    modmul(&t6, &mut t7);
    modnsqr(&mut t7, 2);
    modmul(&x, &mut t7);
    modnsqr(&mut t7, 8);
    modmul(&t5, &mut t7);
    modnsqr(&mut t7, 2);
    modmul(&t1, &mut t7);
    modnsqr(&mut t7, 6);
    modmul(&z, &mut t7);
    modnsqr(&mut t7, 4);
    modmul(&t2, &mut t7);
    modnsqr(&mut t7, 6);
    modmul(&t7, &mut t6);
    modnsqr(&mut t6, 5);
    modmul(&z, &mut t6);
    modnsqr(&mut t6, 10);
    modmul(&t5, &mut t6);
    modnsqr(&mut t6, 9);
    modmul(&t6, &mut t5);
    modnsqr(&mut t5, 4);
    modmul(&z, &mut t5);
    modnsqr(&mut t5, 6);
    modmul(&t5, &mut t4);
    modnsqr(&mut t4, 3);
    modmul(&x, &mut t4);
    modnsqr(&mut t4, 7);
    modmul(&z, &mut t4);
    modnsqr(&mut t4, 7);
    modmul(&t0, &mut t4);
    modnsqr(&mut t4, 5);
    modmul(&t2, &mut t4);
    modnsqr(&mut t4, 5);
    modmul(&t4, &mut t3);
    modnsqr(&mut t3, 5);
    modmul(&z, &mut t3);
    modnsqr(&mut t3, 4);
    modmul(&z, &mut t3);
    modnsqr(&mut t3, 5);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 3);
    modmul(&t1, &mut t2);
    modnsqr(&mut t2, 7);
    modmul(&t1, &mut t2);
    modnsqr(&mut t2, 6);
    modmul(&z, &mut t2);
    modnsqr(&mut t2, 4);
    modmul(&t0, &mut t2);
    modnsqr(&mut t2, 3);
    modmul(&t1, &mut t2);
    modnsqr(&mut t2, 4);
    modmul(&t1, &mut t2);
    modnsqr(&mut t2, 4);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 6);
    modmul(&t0, &mut t1);
    modnsqr(&mut t1, 5);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 6);
    modmul(&t0, &mut z);
    modsqr(&mut z);
    modmul(&x, &mut z);
    modnsqr(&mut z, 2);
    modcpy(&z, r);
    return;
}

//calculate inverse, provide progenitor h if available
fn modinv(h: Option<&[SPINT]>, z: &mut [SPINT]) {
    let mut s: [SPINT; 7] = [0; 7];
    let mut t: [SPINT; 7] = [0; 7];
    if let Some(hint) = h {
        modcpy(&hint, &mut t);
    } else {
        modpro(&z, &mut t);
    }
    modcpy(&z, &mut s);
    modnsqr(&mut t, 2);
    modcpy(&t, z);
    modmul(&s, z);
    return;
}

//Convert n to n-residue form, n=nres(m)
fn nres(n: &mut [SPINT]) {
    let c: [SPINT; 7] = [
        0x35fd09360e3cb4,
        0x73c4e1290eb460,
        0x43a6addff2dbd2,
        0x1cc5bf02d164ee,
        0x95d40d49174aab,
        0x3fb05b7a282668,
        0xee012b39bf21,
    ];
    modmul(&c, n);
    return;
}

//Convert m back to normal form, m=redc(n)
fn redc(m: &mut [SPINT]) {
    let mut c: [SPINT; 7] = [0; 7];
    c[0] = 1;
    modmul(&c, m);
    modfsb(m);
    return;
}

//is unity?
fn modis1(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 7] = [0; 7];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 1..7 {
        d |= c[i] as SSPINT;
    }
    let c0 = c[0] as SSPINT;
    return (1 & ((d - 1) >> 56) & (((c0 ^ 1) - 1) >> 56)) != 0;
}

//is zero?
fn modis0(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 7] = [0; 7];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 0..7 {
        d |= c[i] as SSPINT;
    }
    return (1 & ((d - 1) >> 56)) != 0;
}

//set to zero
fn modzer(a: &mut [SPINT]) {
    for i in 0..7 {
        a[i] = 0;
    }
    return;
}

//set to one
fn modone(a: &mut [SPINT]) {
    a[0] = 1;
    for i in 1..7 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//set to integer
fn modint(x: usize, a: &mut [SPINT]) {
    a[0] = x as SPINT;
    for i in 1..7 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//Test for quadratic residue
fn modqr(h: Option<&[SPINT]>, x: &[SPINT]) -> bool {
    let mut r: [SPINT; 7] = [0; 7];
    if let Some(hint) = h {
        modcpy(&hint, &mut r);
    } else {
        modpro(&x, &mut r);
    }
    modsqr(&mut r);
    modmul(&x, &mut r);
    return modis1(&r);
}

//conditional move g to f if d=1
fn modcmv(d: usize, g: &[SPINT], f: &mut [SPINT]) {
    let r0 = f[0] ^ g[1];
    let r1 = f[1] ^ g[0];
    let dd = d as SPINT;
    let c0=1-(dd-r0);
    let c1=dd+r1;
    for i in 0..7 {
        let t = f[i];
        unsafe{core::ptr::write_volatile(&mut f[i],c0 * t + c1 * g[i])}
        f[i] -= r0 * t + r1 * g[i];
    }
    return;
}

//conditional swap g and f if d=1
fn modcsw(d: usize, g: &mut [SPINT], f: &mut [SPINT]) {
    let r0 = f[0] ^ g[1];
    let r1 = f[1] ^ g[0];
    let dd = d as SPINT;
    let c0=1-(dd-r0);
    let c1=dd+r1;
    for i in 0..7 {
        let t = f[i];
        let s = g[i];
        unsafe{core::ptr::write_volatile(&mut f[i],c0 * t + c1 * s);}
        unsafe{core::ptr::write_volatile(&mut g[i],c0 * s + c1 * t);}
        f[i] -= r0 * t + r1 * s;
        g[i] -= r0 * s + r1 * t;
    }
    return;
}

//Modular square root, provide progenitor h if available, NULL if not
fn modsqrt(x: &[SPINT], h: Option<&[SPINT]>, r: &mut [SPINT]) {
    let mut y: [SPINT; 7] = [0; 7];
    let mut s: [SPINT; 7] = [0; 7];
    if let Some(hint) = h {
        modcpy(&hint, &mut y);
    } else {
        modpro(&x, &mut y);
    }
    modcpy(&x, &mut s);
    modmul(&y, &mut s);
    modcpy(&s, r);
    return;
}

//shift left by less than a word
fn modshl(n: isize, a: &mut [SPINT]) {
    a[7 - 1] = (a[7 - 1] << n) | (a[7 - 2] >> (56 - n));
    for i in (1..7 - 1).rev() {
        a[i] = ((a[i] << n) & 0xffffffffffffff) | (a[i - 1] >> (56 - n));
    }
    a[0] = (a[0] << n) & 0xffffffffffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..7 - 1 {
        a[i] = (a[i] >> n) | ((a[i + 1] << (56 - n)) & 0xffffffffffffff);
    }
    a[7 - 1] = a[7 - 1] >> n;
    return r as isize;
}

//set a= 2^r
fn mod2r(r: usize, a: &mut [SPINT]) {
    let n = r / 56;
    let m = r % 56;
    modzer(a);
    if r >= 48 * 8 {
        return;
    }
    a[n] = 1;
    a[n] <<= m;
    nres(a);
}

//export to byte array
fn modexp(a: &[SPINT], b: &mut [u8]) {
    let mut c: [SPINT; 7] = [0; 7];
    modcpy(a, &mut c);
    redc(&mut c);
    for i in (0..48).rev() {
        b[i] = (c[0] & 0xff) as u8;
        modshr(8, &mut c);
    }
    return;
}

//import from byte array
fn modimp(b: &[u8], a: &mut [SPINT]) -> bool {
    for i in 0..7 {
        a[i] = 0;
    }
    for i in 0..48 {
        modshl(8, a);
        a[0] += b[i] as SPINT;
    }
    let res = modfsb(a);
    nres(a);
    return res;
}

//determine sign
fn modsign(a: &[SPINT]) -> usize {
    let mut c: [SPINT; 7] = [0; 7];
    modcpy(a, &mut c);
    redc(&mut c);
    return (c[0] % 2) as usize;
}

//return true if equal
fn modcmp(a: &[SPINT], b: &[SPINT]) -> bool {
    let mut c: [SPINT; 7] = [0; 7];
    let mut d: [SPINT; 7] = [0; 7];
    let mut eq = 1;
    modcpy(a, &mut c);
    redc(&mut c);
    modcpy(b, &mut d);
    redc(&mut d);
    for i in 0..7 {
        eq &= (((c[i] ^ d[i]) - 1) >> 56) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 7;
const RADIX: usize = 56;
const NBITS: usize = 384;
const NBYTES: usize = 48;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;

/*** End of automatically generated code ***/

// number of limbs and bytes in representation
pub const BYTES:usize = NBYTES;
const LIMBS:usize= NLIMBS;
type GEL = [SPINT; LIMBS];

const PREHASHED:bool = false;  // true only for test vector


// reduce 56 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^376.x + y, where x and y < q (x is top 9 bytes, y is bottom 47 bytes)
// Important that x and y < q
fn reduce(h:&[u8],r:&mut [SPINT]) {
    let mut buff:[u8;BYTES]=[0;BYTES];    
    let mut x:GEL=[0;LIMBS];
    let mut y:GEL=[0;LIMBS];
    let mut c:GEL=[0;LIMBS];

    mod2r(8*(BYTES-1),&mut c);

    for i in 0..BYTES-1 {
        buff[i]=h[i];
    }
    buff[BYTES-1]=0;
    buff.reverse();
    modimp(&buff,&mut y);

    for i in 0..9 {
        buff[i]=h[BYTES-1+i];
    }
    for i in 9..BYTES {
        buff[i]=0;
    }
    buff.reverse();
    modimp(&buff,&mut x);

    modmul(&c,&mut x); 
    modcpy(&x,r); modadd(&y,r);
}


// Input private key - 48 random bytes
// Output public key - 97 bytes (0x04<x>|<y>), or 33 if compressed (0x02<x>.. or 0x03<x>)
pub fn KEY_PAIR(compress: bool,prv: &[u8],public: &mut [u8]) {
    let mut P=ECP::new();
    let mut x:[u8;BYTES]=[0;BYTES];
    let mut y:[u8;BYTES]=[0;BYTES];
    let fb:u8;
    ecngen(&mut P);

    ecnmul(prv,&mut P); 

    if compress {
        fb=0x02+ecnget(&mut P,&mut x,None) as u8; // 0x02 or 0x03
        for i in 0..BYTES {
            public[1+i]=x[i];
        }
    } else {
        fb=0x04; // no compression
        ecnget(&mut P,&mut x,Some(&mut y));  // get x and y
        for i in 0..BYTES {
            public[1+i]=x[i];
            public[1+i+BYTES]=y[i];
        }
    }
    public[0]=fb;
}

pub fn PREHASH(sha: usize,m: &[u8]) -> [u8; BYTES] {
    let mut thm: [u8; BYTES] = [0; BYTES];
    if sha==48 {
        let mut sh384 = SHA384::new();
        for i in 0..m.len() {
            sh384.process(m[i]);
        }
        let h=sh384.hash();
        for i in 0..BYTES {
            thm[i]=h[i];
        }
    }
    if sha==64 {
        let mut sh512 = SHA512::new();
        for i in 0..m.len() {
            sh512.process(m[i]);
        }
        let h=sh512.hash();
        for i in 0..BYTES {
            thm[i]=h[i];
        } 
    }
    return thm;
}

pub fn SIGN(prv: &[u8],ran: &[u8],thm:&[u8],sig: &mut [u8]) {
    let mut rb:[u8;BYTES]=[0;BYTES];
    let mut sb:[u8;BYTES]=[0;BYTES];
    let mut R=ECP::new();
    let mut e:GEL=[0;LIMBS];
    let mut r:GEL=[0;LIMBS];
    let mut s:GEL=[0;LIMBS];
    let mut k:GEL=[0;LIMBS];

    modimp(thm,&mut e);

    ecngen(&mut R);
    modimp(prv,&mut s);

    reduce(ran,&mut k);
    modexp(&k,&mut rb);
    ecnmul(&rb,&mut R);
    modinv(None,&mut k);

    ecnget(&mut R,&mut rb,None);
    modimp(&rb,&mut r);

    modmul(&r,&mut s);
    modadd(&e,&mut s);
    modmul(&k,&mut s);
    modzer(&mut k);

    modexp(&r,&mut rb);
    modexp(&s,&mut sb);

    for i in 0..BYTES {
        sig[i]=rb[i];
        sig[BYTES+i]=sb[i];
    }
}

// input public key, message and signature
pub fn VERIFY(public: &[u8],thm:&[u8],sig:&[u8]) -> bool {
    let mut G=ECP::new();
    let mut Q=ECP::new();

    let mut rb:[u8;BYTES]=[0;BYTES];
    let mut sb:[u8;BYTES]=[0;BYTES];
    let mut u:[u8;BYTES]=[0;BYTES];
    let mut v:[u8;BYTES]=[0;BYTES];
    
    let mut e:GEL=[0;LIMBS];
    let mut r:GEL=[0;LIMBS];
    let mut s:GEL=[0;LIMBS];
    let mut rds:GEL=[0;LIMBS];
    
    modimp(thm,&mut e);

    ecngen(&mut G);

// import from signature
    for i in 0..BYTES {
        rb[i]=sig[i];
        sb[i]=sig[BYTES+i];
    }    

    if !modimp(&rb,&mut r) {
        return false; // if not in range
    }
    if !modimp(&sb,&mut s) {
        return false;
    }

    if modis0(&r) || modis0(&s) {
        return false;
    }
    modinv(None,&mut s); modcpy(&r,&mut rds);
    modmul(&s,&mut rds); modexp(&rds,&mut v);  // export to byte array
    modmul(&e,&mut s); modexp(&s,&mut u); 

    for i in 0..BYTES {
        rb[i]=public[1+i];
        sb[i]=public[1+i+BYTES];
    }

    if public[0]==0x04 {
        ecnset(0,&rb,Some(&sb),&mut Q);
    } else {
        ecnset((public[0]&1) as usize,&rb,None,&mut Q);
    }

    let mut R=ecnmul2(&u,&G,&v,&Q);
    if ecnisinf(&R) {
        return false;
    }
    ecnget(&mut R,&mut rb,None);

    modimp(&rb,&mut e);
    if modcmp(&r,&e) {
        return true;
    }
    return false;
}

pub fn SHARED_SECRET(prv: &[u8],public: &[u8],ss: &mut[u8]) -> bool {
    let mut P=ECP::new();
    let mut xb:[u8;BYTES]=[0;BYTES];

    for i in 0..BYTES {
        xb[i]=public[1+i];
    }
    if public[0]==0x04 {
        let mut yb:[u8;BYTES]=[0;BYTES];
        for i in 0..BYTES {
            yb[i]=public[1+i+BYTES];
        }
        ecnset(0,&xb,Some(&yb),&mut P);
    } else {
        ecnset((public[0]&1) as usize,&xb,None,&mut P);
    }

    ecnmul(prv,&mut P);
    ecnget(&mut P,ss,None);
    if ecnisinf(&P) {
        return false;
    }
    return true;
}
