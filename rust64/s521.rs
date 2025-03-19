#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// For example: cargo run --release --features NIST521 --bin EC521
// python curve_rust.py 64 NIST521
// This completes weierstrass.rs for this curve
// ECDSA Implementation for curve P-521
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf

use crate::nist521;
use crate::nist521::*;
use crate::nist521::ECP;

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
    let mask = ((1 as SPINT) << 59) - 1;
    let mut carry = (n[0] as SSPINT) >> 59;
    n[0] &= mask;
    for i in 1..8 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 59;
    }
    n[8] += carry as SPINT;
    return ((n[8] as SSPINT) >> 63) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 59;
    let carry = prop(n);
    n[0] += (0x36fb71e91386409 as SPINT) & (carry as SPINT);
    n[1] += (0x6b937113388f5d7 as SPINT) & (carry as SPINT);
    n[2] += (0x523dc269740ee as SPINT) & (carry as SPINT);
    n[3] += (0x3c1df97cb35bfe6 as SPINT) & (carry as SPINT);
    n[4] += (0x7ffffffffa51868 as SPINT) & (carry as SPINT);
    n[5] -= 1 & (carry as SPINT);
    n[8] += (0x2000000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
#[inline]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 59;
    n[0] -= 0x36fb71e91386409 as SPINT;
    n[1] -= 0x6b937113388f5d7 as SPINT;
    n[2] -= 0x523dc269740ee as SPINT;
    n[3] -= 0x3c1df97cb35bfe6 as SPINT;
    n[4] -= 0x7ffffffffa51868 as SPINT;
    n[5] += 1;
    n[8] -= 0x2000000000000 as SPINT;
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
    n[7] = n[7] + b[7];
    n[8] = n[8] + b[8];
    n[0] -= 0x6df6e3d2270c812 as SPINT;
    n[1] -= 0xd726e226711ebae as SPINT;
    n[2] -= 0xa47b84d2e81dc as SPINT;
    n[3] -= 0x783bf2f966b7fcc as SPINT;
    n[4] -= 0xfffffffff4a30d0 as SPINT;
    n[5] += 2;
    n[8] -= 0x4000000000000 as SPINT;
    let carry = prop(n);
    n[0] += (0x6df6e3d2270c812 as SPINT) & (carry as SPINT);
    n[1] += (0xd726e226711ebae as SPINT) & (carry as SPINT);
    n[2] += (0xa47b84d2e81dc as SPINT) & (carry as SPINT);
    n[3] += (0x783bf2f966b7fcc as SPINT) & (carry as SPINT);
    n[4] += (0xfffffffff4a30d0 as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[8] += (0x4000000000000 as SPINT) & (carry as SPINT);
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
    n[7] = n[7] - b[7];
    n[8] = n[8] - b[8];
    let carry = prop(n);
    n[0] += (0x6df6e3d2270c812 as SPINT) & (carry as SPINT);
    n[1] += (0xd726e226711ebae as SPINT) & (carry as SPINT);
    n[2] += (0xa47b84d2e81dc as SPINT) & (carry as SPINT);
    n[3] += (0x783bf2f966b7fcc as SPINT) & (carry as SPINT);
    n[4] += (0xfffffffff4a30d0 as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[8] += (0x4000000000000 as SPINT) & (carry as SPINT);
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
    n[7] = (0 as SPINT) - n[7];
    n[8] = (0 as SPINT) - n[8];
    let carry = prop(n);
    n[0] += (0x6df6e3d2270c812 as SPINT) & (carry as SPINT);
    n[1] += (0xd726e226711ebae as SPINT) & (carry as SPINT);
    n[2] += (0xa47b84d2e81dc as SPINT) & (carry as SPINT);
    n[3] += (0x783bf2f966b7fcc as SPINT) & (carry as SPINT);
    n[4] += (0xfffffffff4a30d0 as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[8] += (0x4000000000000 as SPINT) & (carry as SPINT);
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
// maximum possible = 3902326505693672769824936250097569004
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
#[inline]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0x36fb71e91386409 as SPINT;
    let p1 = 0x6b937113388f5d7 as SPINT;
    let p2 = 0x523dc269740ee as SPINT;
    let p3 = 0x3c1df97cb35bfe6 as SPINT;
    let p4 = 0x7ffffffffa51868 as SPINT;
    let q = (1 as SPINT) << 59; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x52f5ccd79a995c7 as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p1 as DPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p2 as DPINT);
    t += (v1 as DPINT) * (p1 as DPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v1 as DPINT) * (p2 as DPINT);
    t += (v2 as DPINT) * (p1 as DPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[4] as DPINT);
    t += (c[1] as DPINT) * (b[3] as DPINT);
    t += (c[2] as DPINT) * (b[2] as DPINT);
    t += (c[3] as DPINT) * (b[1] as DPINT);
    t += (c[4] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p4 as DPINT);
    t += (v1 as DPINT) * (p3 as DPINT);
    t += (v2 as DPINT) * (p2 as DPINT);
    t += (v3 as DPINT) * (p1 as DPINT);
    let v4 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v4 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    t += (q - v0) as DPINT;
    t += (v1 as DPINT) * (p4 as DPINT);
    t += (v2 as DPINT) * (p3 as DPINT);
    t += (v3 as DPINT) * (p2 as DPINT);
    t += (v4 as DPINT) * (p1 as DPINT);
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    s -= v1;
    t += (v2 as DPINT) * (p4 as DPINT);
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[7] as DPINT);
    t += (c[1] as DPINT) * (b[6] as DPINT);
    t += (c[2] as DPINT) * (b[5] as DPINT);
    t += (c[3] as DPINT) * (b[4] as DPINT);
    t += (c[4] as DPINT) * (b[3] as DPINT);
    t += (c[5] as DPINT) * (b[2] as DPINT);
    t += (c[6] as DPINT) * (b[1] as DPINT);
    t += (c[7] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    s -= v2;
    t += (v3 as DPINT) * (p4 as DPINT);
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[0] as DPINT) * (b[8] as DPINT);
    t += (c[1] as DPINT) * (b[7] as DPINT);
    t += (c[2] as DPINT) * (b[6] as DPINT);
    t += (c[3] as DPINT) * (b[5] as DPINT);
    t += (c[4] as DPINT) * (b[4] as DPINT);
    t += (c[5] as DPINT) * (b[3] as DPINT);
    t += (c[6] as DPINT) * (b[2] as DPINT);
    t += (c[7] as DPINT) * (b[1] as DPINT);
    t += (c[8] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    t += (v0 as DPINT) << 49;
    s -= v3;
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += (v7 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as DPINT) * (p0 as DPINT);
    t >>= 59;
    t += (c[1] as DPINT) * (b[8] as DPINT);
    t += (c[2] as DPINT) * (b[7] as DPINT);
    t += (c[3] as DPINT) * (b[6] as DPINT);
    t += (c[4] as DPINT) * (b[5] as DPINT);
    t += (c[5] as DPINT) * (b[4] as DPINT);
    t += (c[6] as DPINT) * (b[3] as DPINT);
    t += (c[7] as DPINT) * (b[2] as DPINT);
    t += (c[8] as DPINT) * (b[1] as DPINT);
    s = mask as SPINT;
    t += (v1 as DPINT) << 49;
    s -= v4;
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t += (c[2] as DPINT) * (b[8] as DPINT);
    t += (c[3] as DPINT) * (b[7] as DPINT);
    t += (c[4] as DPINT) * (b[6] as DPINT);
    t += (c[5] as DPINT) * (b[5] as DPINT);
    t += (c[6] as DPINT) * (b[4] as DPINT);
    t += (c[7] as DPINT) * (b[3] as DPINT);
    t += (c[8] as DPINT) * (b[2] as DPINT);
    s = mask as SPINT;
    t += (v2 as DPINT) << 49;
    s -= v5;
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    t += s as DPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t += (c[3] as DPINT) * (b[8] as DPINT);
    t += (c[4] as DPINT) * (b[7] as DPINT);
    t += (c[5] as DPINT) * (b[6] as DPINT);
    t += (c[6] as DPINT) * (b[5] as DPINT);
    t += (c[7] as DPINT) * (b[4] as DPINT);
    t += (c[8] as DPINT) * (b[3] as DPINT);
    s = mask as SPINT;
    t += (v3 as DPINT) << 49;
    s -= v6;
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    t += s as DPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t += (c[4] as DPINT) * (b[8] as DPINT);
    t += (c[5] as DPINT) * (b[7] as DPINT);
    t += (c[6] as DPINT) * (b[6] as DPINT);
    t += (c[7] as DPINT) * (b[5] as DPINT);
    t += (c[8] as DPINT) * (b[4] as DPINT);
    s = mask as SPINT;
    t += (v4 as DPINT) << 49;
    s -= v7;
    t += (v8 as DPINT) * (p4 as DPINT);
    t += s as DPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t += (c[5] as DPINT) * (b[8] as DPINT);
    t += (c[6] as DPINT) * (b[7] as DPINT);
    t += (c[7] as DPINT) * (b[6] as DPINT);
    t += (c[8] as DPINT) * (b[5] as DPINT);
    s = mask as SPINT;
    t += (v5 as DPINT) << 49;
    s -= v8;
    t += s as DPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t += (c[6] as DPINT) * (b[8] as DPINT);
    t += (c[7] as DPINT) * (b[7] as DPINT);
    t += (c[8] as DPINT) * (b[6] as DPINT);
    s = mask as SPINT;
    t += (v6 as DPINT) << 49;
    t += s as DPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t += (c[7] as DPINT) * (b[8] as DPINT);
    t += (c[8] as DPINT) * (b[7] as DPINT);
    s = mask as SPINT;
    t += (v7 as DPINT) << 49;
    t += s as DPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t += (c[8] as DPINT) * (b[8] as DPINT);
    s = mask as SPINT;
    t += (v8 as DPINT) << 49;
    t += s as DPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t -= 1;
    c[8] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
#[inline]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let p0 = 0x36fb71e91386409 as SPINT;
    let p1 = 0x6b937113388f5d7 as SPINT;
    let p2 = 0x523dc269740ee as SPINT;
    let p3 = 0x3c1df97cb35bfe6 as SPINT;
    let p4 = 0x7ffffffffa51868 as SPINT;
    let q = (1 as SPINT) << 59; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x52f5ccd79a995c7 as SPINT;
    tot = (c[0] as UDPINT) * (c[0] as UDPINT);
    t = tot;
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[1] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p1 as UDPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    tot += (c[1] as UDPINT) * (c[1] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p2 as UDPINT);
    t += (v1 as UDPINT) * (p1 as UDPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[3] as UDPINT);
    tot += (c[1] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p3 as UDPINT);
    t += (v1 as UDPINT) * (p2 as UDPINT);
    t += (v2 as UDPINT) * (p1 as UDPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[4] as UDPINT);
    tot += (c[1] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    tot += (c[2] as UDPINT) * (c[2] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p4 as UDPINT);
    t += (v1 as UDPINT) * (p3 as UDPINT);
    t += (v2 as UDPINT) * (p2 as UDPINT);
    t += (v3 as UDPINT) * (p1 as UDPINT);
    let v4 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v4 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[5] as UDPINT);
    tot += (c[1] as UDPINT) * (c[4] as UDPINT);
    tot += (c[2] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    t += tot;
    t += (q - v0) as UDPINT;
    t += (v1 as UDPINT) * (p4 as UDPINT);
    t += (v2 as UDPINT) * (p3 as UDPINT);
    t += (v3 as UDPINT) * (p2 as UDPINT);
    t += (v4 as UDPINT) * (p1 as UDPINT);
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[6] as UDPINT);
    tot += (c[1] as UDPINT) * (c[5] as UDPINT);
    tot += (c[2] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    tot += (c[3] as UDPINT) * (c[3] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s -= v1;
    t += (v2 as UDPINT) * (p4 as UDPINT);
    t += (v3 as UDPINT) * (p3 as UDPINT);
    t += (v4 as UDPINT) * (p2 as UDPINT);
    t += (v5 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[7] as UDPINT);
    tot += (c[1] as UDPINT) * (c[6] as UDPINT);
    tot += (c[2] as UDPINT) * (c[5] as UDPINT);
    tot += (c[3] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s -= v2;
    t += (v3 as UDPINT) * (p4 as UDPINT);
    t += (v4 as UDPINT) * (p3 as UDPINT);
    t += (v5 as UDPINT) * (p2 as UDPINT);
    t += (v6 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[0] as UDPINT) * (c[8] as UDPINT);
    tot += (c[1] as UDPINT) * (c[7] as UDPINT);
    tot += (c[2] as UDPINT) * (c[6] as UDPINT);
    tot += (c[3] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    tot += (c[4] as UDPINT) * (c[4] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v0 as UDPINT) << 49;
    s -= v3;
    t += (v4 as UDPINT) * (p4 as UDPINT);
    t += (v5 as UDPINT) * (p3 as UDPINT);
    t += (v6 as UDPINT) * (p2 as UDPINT);
    t += (v7 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as UDPINT) * (p0 as UDPINT);
    t >>= 59;
    tot = (c[1] as UDPINT) * (c[8] as UDPINT);
    tot += (c[2] as UDPINT) * (c[7] as UDPINT);
    tot += (c[3] as UDPINT) * (c[6] as UDPINT);
    tot += (c[4] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v1 as UDPINT) << 49;
    s -= v4;
    t += (v5 as UDPINT) * (p4 as UDPINT);
    t += (v6 as UDPINT) * (p3 as UDPINT);
    t += (v7 as UDPINT) * (p2 as UDPINT);
    t += (v8 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    tot = (c[2] as UDPINT) * (c[8] as UDPINT);
    tot += (c[3] as UDPINT) * (c[7] as UDPINT);
    tot += (c[4] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    tot += (c[5] as UDPINT) * (c[5] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v2 as UDPINT) << 49;
    s -= v5;
    t += (v6 as UDPINT) * (p4 as UDPINT);
    t += (v7 as UDPINT) * (p3 as UDPINT);
    t += (v8 as UDPINT) * (p2 as UDPINT);
    t += s as UDPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    tot = (c[3] as UDPINT) * (c[8] as UDPINT);
    tot += (c[4] as UDPINT) * (c[7] as UDPINT);
    tot += (c[5] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v3 as UDPINT) << 49;
    s -= v6;
    t += (v7 as UDPINT) * (p4 as UDPINT);
    t += (v8 as UDPINT) * (p3 as UDPINT);
    t += s as UDPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    tot = (c[4] as UDPINT) * (c[8] as UDPINT);
    tot += (c[5] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    tot += (c[6] as UDPINT) * (c[6] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v4 as UDPINT) << 49;
    s -= v7;
    t += (v8 as UDPINT) * (p4 as UDPINT);
    t += s as UDPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    tot = (c[5] as UDPINT) * (c[8] as UDPINT);
    tot += (c[6] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v5 as UDPINT) << 49;
    s -= v8;
    t += s as UDPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    tot = (c[6] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    tot += (c[7] as UDPINT) * (c[7] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v6 as UDPINT) << 49;
    t += s as UDPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    tot = (c[7] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v7 as UDPINT) << 49;
    t += s as UDPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    tot = (c[8] as UDPINT) * (c[8] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v8 as UDPINT) << 49;
    t += s as UDPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 59;
    t -= 1;
    c[8] = t as SPINT;
    return;
}

//copy
#[inline]
fn modcpy(a: &[SPINT], c: &mut [SPINT]) {
    for i in 0..9 {
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
    let mut x: [SPINT; 9] = [0; 9];
    let mut z: [SPINT; 9] = [0; 9];
    let mut t0: [SPINT; 9] = [0; 9];
    let mut t1: [SPINT; 9] = [0; 9];
    let mut t2: [SPINT; 9] = [0; 9];
    let mut t3: [SPINT; 9] = [0; 9];
    let mut t4: [SPINT; 9] = [0; 9];
    let mut t5: [SPINT; 9] = [0; 9];
    let mut t6: [SPINT; 9] = [0; 9];
    let mut t7: [SPINT; 9] = [0; 9];
    let mut t8: [SPINT; 9] = [0; 9];
    let mut t9: [SPINT; 9] = [0; 9];
    let mut t10: [SPINT; 9] = [0; 9];
    let mut t11: [SPINT; 9] = [0; 9];
    let mut t12: [SPINT; 9] = [0; 9];
    let mut t13: [SPINT; 9] = [0; 9];
    let mut t14: [SPINT; 9] = [0; 9];
    let mut t15: [SPINT; 9] = [0; 9];
    let mut t16: [SPINT; 9] = [0; 9];
    let mut t17: [SPINT; 9] = [0; 9];
    let mut t18: [SPINT; 9] = [0; 9];
    let mut t19: [SPINT; 9] = [0; 9];
    let mut t20: [SPINT; 9] = [0; 9];
    modcpy(w, &mut x);
    modcpy(&x, &mut t2);
    modsqr(&mut t2);
    modcpy(&x, &mut t13);
    modmul(&t2, &mut t13);
    modcpy(&x, &mut t4);
    modmul(&t13, &mut t4);
    modcpy(&t13, &mut t0);
    modmul(&t4, &mut t0);
    modcpy(&x, &mut t5);
    modmul(&t0, &mut t5);
    modcpy(&t13, &mut t16);
    modmul(&t5, &mut t16);
    modcpy(&t2, &mut t10);
    modmul(&t16, &mut t10);
    modcpy(&t2, &mut t18);
    modmul(&t10, &mut t18);
    modcpy(&t2, &mut t1);
    modmul(&t18, &mut t1);
    modcpy(&t2, &mut t12);
    modmul(&t1, &mut t12);
    modcpy(&t4, &mut t15);
    modmul(&t12, &mut t15);
    modcpy(&t2, &mut z);
    modmul(&t15, &mut z);
    modcpy(&t2, &mut t3);
    modmul(&z, &mut t3);
    modcpy(&t2, &mut t6);
    modmul(&t3, &mut t6);
    modcpy(&t5, &mut t11);
    modmul(&t6, &mut t11);
    modcpy(&t4, &mut t14);
    modmul(&t11, &mut t14);
    modcpy(&t4, &mut t9);
    modmul(&t14, &mut t9);
    modcpy(&t2, &mut t17);
    modmul(&t9, &mut t17);
    modcpy(&t4, &mut t7);
    modmul(&t17, &mut t7);
    modmul(&t7, &mut t4);
    modcpy(&t2, &mut t8);
    modmul(&t4, &mut t8);
    modcpy(&t2, &mut t5);
    modmul(&t8, &mut t5);
    modmul(&t5, &mut t2);
    modcpy(&t2, &mut t19);
    modnsqr(&mut t19, 3);
    modmul(&t19, &mut t15);
    modcpy(&t15, &mut t19);
    modsqr(&mut t19);
    modcpy(&t19, &mut t20);
    modnsqr(&mut t20, 8);
    modmul(&t15, &mut t20);
    modnsqr(&mut t20, 10);
    modmul(&t20, &mut t19);
    modcpy(&t19, &mut t20);
    modnsqr(&mut t20, 8);
    modmul(&t15, &mut t20);
    modnsqr(&mut t20, 28);
    modmul(&t20, &mut t19);
    modcpy(&t19, &mut t20);
    modnsqr(&mut t20, 63);
    modmul(&t20, &mut t19);
    modcpy(&t19, &mut t20);
    modnsqr(&mut t20, 8);
    modmul(&t15, &mut t20);
    modnsqr(&mut t20, 127);
    modmul(&t20, &mut t19);
    modmul(&x, &mut t19);
    modnsqr(&mut t19, 7);
    modmul(&t11, &mut t19);
    modnsqr(&mut t19, 5);
    modmul(&t13, &mut t19);
    modnsqr(&mut t19, 8);
    modmul(&t10, &mut t19);
    modnsqr(&mut t19, 8);
    modmul(&t18, &mut t19);
    modnsqr(&mut t19, 11);
    modmul(&t5, &mut t19);
    modnsqr(&mut t19, 4);
    modmul(&t19, &mut t18);
    modnsqr(&mut t18, 8);
    modmul(&t18, &mut t17);
    modnsqr(&mut t17, 6);
    modmul(&t11, &mut t17);
    modnsqr(&mut t17, 5);
    modmul(&t12, &mut t17);
    modnsqr(&mut t17, 5);
    modmul(&t17, &mut t16);
    modnsqr(&mut t16, 10);
    modmul(&t16, &mut t15);
    modnsqr(&mut t15, 4);
    modmul(&t13, &mut t15);
    modnsqr(&mut t15, 15);
    modmul(&t15, &mut t14);
    modnsqr(&mut t14, 9);
    modmul(&t2, &mut t14);
    modnsqr(&mut t14, 2);
    modmul(&t14, &mut t13);
    modnsqr(&mut t13, 9);
    modmul(&t13, &mut t12);
    modnsqr(&mut t12, 7);
    modmul(&t12, &mut t11);
    modnsqr(&mut t11, 4);
    modmul(&t11, &mut t10);
    modnsqr(&mut t10, 12);
    modmul(&t5, &mut t10);
    modnsqr(&mut t10, 6);
    modmul(&t10, &mut t9);
    modnsqr(&mut t9, 7);
    modmul(&t9, &mut t8);
    modnsqr(&mut t8, 8);
    modmul(&t4, &mut t8);
    modnsqr(&mut t8, 8);
    modmul(&t1, &mut t8);
    modnsqr(&mut t8, 8);
    modmul(&t8, &mut t7);
    modnsqr(&mut t7, 5);
    modmul(&t1, &mut t7);
    modnsqr(&mut t7, 9);
    modmul(&t2, &mut t7);
    modnsqr(&mut t7, 6);
    modmul(&t7, &mut t6);
    modnsqr(&mut t6, 7);
    modmul(&t6, &mut t5);
    modnsqr(&mut t5, 7);
    modmul(&t5, &mut t4);
    modnsqr(&mut t4, 5);
    modmul(&t4, &mut t3);
    modnsqr(&mut t3, 4);
    modmul(&t0, &mut t3);
    modnsqr(&mut t3, 9);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 7);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 5);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 9);
    modmul(&t0, &mut z);
    modnsqr(&mut z, 6);
    modcpy(&z, r);
    return;
}

//calculate inverse, provide progenitor h if available
fn modinv(h: Option<&[SPINT]>, z: &mut [SPINT]) {
    let mut s: [SPINT; 9] = [0; 9];
    let mut t: [SPINT; 9] = [0; 9];
    if let Some(hint) = h {
        modcpy(&hint, &mut t);
    } else {
        modpro(&z, &mut t);
    }
    modcpy(&z, &mut s);
    for _i in 0..3 - 1 {
        modsqr(&mut s);
        modmul(&z, &mut s);
    }
    modnsqr(&mut t, 4);
    modcpy(&t, z);
    modmul(&s, z);
    return;
}

//Convert n to n-residue form, n=nres(m)
fn nres(n: &mut [SPINT]) {
    let c: [SPINT; 9] = [
        0x5765fcd15cf17db,
        0x6c812fbca5b2553,
        0x46d5e1be513b608,
        0x2c0753b44a31127,
        0x33fcf450ac6f345,
        0x5e636b0d472b3de,
        0xe03d1492d0d455,
        0x475fd1888707a5b,
        0x80e468e696d6,
    ];
    modmul(&c, n);
    return;
}

//Convert m back to normal form, m=redc(n)
fn redc(m: &mut [SPINT]) {
    let mut c: [SPINT; 9] = [0; 9];
    c[0] = 1;
    modmul(&c, m);
    modfsb(m);
    return;
}

//is unity?
fn modis1(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 9] = [0; 9];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 1..9 {
        d |= c[i] as SSPINT;
    }
    let c0 = c[0] as SSPINT;
    return (1 & ((d - 1) >> 59) & (((c0 ^ 1) - 1) >> 59)) != 0;
}

//is zero?
fn modis0(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 9] = [0; 9];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 0..9 {
        d |= c[i] as SSPINT;
    }
    return (1 & ((d - 1) >> 59)) != 0;
}

//set to zero
fn modzer(a: &mut [SPINT]) {
    for i in 0..9 {
        a[i] = 0;
    }
    return;
}

//set to one
fn modone(a: &mut [SPINT]) {
    a[0] = 1;
    for i in 1..9 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//set to integer
fn modint(x: usize, a: &mut [SPINT]) {
    a[0] = x as SPINT;
    for i in 1..9 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//Test for quadratic residue
fn modqr(h: Option<&[SPINT]>, x: &[SPINT]) -> bool {
    let mut r: [SPINT; 9] = [0; 9];
    if let Some(hint) = h {
        modcpy(&hint, &mut r);
    } else {
        modpro(&x, &mut r);
    }
    modsqr(&mut r);
    modmul(&x, &mut r);
    modnsqr(&mut r, 2);
    return modis1(&r) | modis0(x);
}

//conditional move g to f if d=1
#[inline(never)]
fn modcmv(d: usize, g: &[SPINT], f: &mut [SPINT]) {
    let dd=d as SPINT;
    static mut R:SPINT=0;
    let r:SPINT;
    unsafe {
        R+=0x3cc3c33c5aa5a55a;
        r=R;
    }
    let c0=(!dd)&(r+1);
    let c1=dd+r;
    for i in 0..9 {
        let s = g[i];
        let t = f[i];
        unsafe{core::ptr::write_volatile(&mut f[i],c0*t+c1*s)}  
        f[i]-=r*(t+s);
    }
    return;
}

//conditional swap g and f if d=1
#[inline(never)]
fn modcsw(d: usize, g: &mut [SPINT], f: &mut [SPINT]) {
    let dd=d as SPINT;
    static mut R:SPINT=0;
    let r:SPINT;
    unsafe {
        R+=0x3cc3c33c5aa5a55a;
        r=R;
    }
    let c0=(!dd)&(r+1);
    let c1=dd+r;
    for i in 0..9 {
        let s = g[i];
        let t = f[i];
        let w=r*(t+s);
        unsafe{core::ptr::write_volatile(&mut f[i],c0*t+c1*s)}  
        f[i]-=w;
        unsafe{core::ptr::write_volatile(&mut g[i],c0*s+c1*t)} 
        g[i]-=w;
    }
    return;
}

//Modular square root, provide progenitor h if available, NULL if not
fn modsqrt(x: &[SPINT], h: Option<&[SPINT]>, r: &mut [SPINT]) {
    let mut t: [SPINT; 9] = [0; 9];
    let mut b: [SPINT; 9] = [0; 9];
    let mut v: [SPINT; 9] = [0; 9];
    let mut z: [SPINT; 9] = [
        0x7f9c7b7b63e2205,
        0x6567b50c3590214,
        0x2de0d991f4e9c91,
        0x4ff9c8ee6de6ccc,
        0x3a3055276d535f7,
        0x25b5f525f98ace,
        0x2313ef1f980035f,
        0x460bf9eb845697d,
        0x9a0a650d44b2,
    ];
    let mut y: [SPINT; 9] = [0; 9];
    let mut s: [SPINT; 9] = [0; 9];
    if let Some(hint) = h {
        modcpy(&hint, &mut y);
    } else {
        modpro(&x, &mut y);
    }
    modcpy(&x, &mut s);
    modmul(&y, &mut s);
    modcpy(&s, &mut t);
    modmul(&y, &mut t);
    nres(&mut z);
    for k in (2..=3).rev() {
        modcpy(&t, &mut b);
        modnsqr(&mut b, k - 2);
        let d = 1 - (modis1(&b) as usize);
        modcpy(&z, &mut v);
        modmul(&s, &mut v);
        modcmv(d, &v, &mut s);
        modsqr(&mut z);
        modcpy(&z, &mut v);
        modmul(&t, &mut v);
        modcmv(d, &v, &mut t);
    }
    modcpy(&s, r);
    return;
}

//shift left by less than a word
fn modshl(n: isize, a: &mut [SPINT]) {
    a[9 - 1] = (a[9 - 1] << n) | (a[9 - 2] >> (59 - n));
    for i in (1..9 - 1).rev() {
        a[i] = ((a[i] << n) & 0x7ffffffffffffff) | (a[i - 1] >> (59 - n));
    }
    a[0] = (a[0] << n) & 0x7ffffffffffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..9 - 1 {
        a[i] = (a[i] >> n) | ((a[i + 1] << (59 - n)) & 0x7ffffffffffffff);
    }
    a[9 - 1] = a[9 - 1] >> n;
    return r as isize;
}

//set a= 2^r
fn mod2r(r: usize, a: &mut [SPINT]) {
    let n = r / 59;
    let m = r % 59;
    modzer(a);
    if r >= 66 * 8 {
        return;
    }
    a[n] = 1;
    a[n] <<= m;
    nres(a);
}

//export to byte array
fn modexp(a: &[SPINT], b: &mut [u8]) {
    let mut c: [SPINT; 9] = [0; 9];
    modcpy(a, &mut c);
    redc(&mut c);
    for i in (0..66).rev() {
        b[i] = (c[0] & 0xff) as u8;
        modshr(8, &mut c);
    }
    return;
}

//import from byte array
fn modimp(b: &[u8], a: &mut [SPINT]) -> bool {
    for i in 0..9 {
        a[i] = 0;
    }
    for i in 0..66 {
        modshl(8, a);
        a[0] += b[i] as SPINT;
    }
    let res = modfsb(a);
    nres(a);
    return res;
}

//determine sign
fn modsign(a: &[SPINT]) -> usize {
    let mut c: [SPINT; 9] = [0; 9];
    modcpy(a, &mut c);
    redc(&mut c);
    return (c[0] % 2) as usize;
}

//return true if equal
fn modcmp(a: &[SPINT], b: &[SPINT]) -> bool {
    let mut c: [SPINT; 9] = [0; 9];
    let mut d: [SPINT; 9] = [0; 9];
    let mut eq = 1;
    modcpy(a, &mut c);
    redc(&mut c);
    modcpy(b, &mut d);
    redc(&mut d);
    for i in 0..9 {
        eq &= (((c[i] ^ d[i]) - 1) >> 59) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 9;
const RADIX: usize = 59;
const NBITS: usize = 521;
const NBYTES: usize = 66;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;

/*** End of automatically generated code ***/

// number of limbs and bytes in representation
pub const BYTES:usize = NBYTES;
const LIMBS:usize= NLIMBS;
type GEL = [SPINT; LIMBS];

const PREHASHED:bool = false;  // true only for test vector


// reduce 74 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^520.x + y, where x and y < q (x is top 9 bytes, y is bottom 65 bytes)
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


// Input private key - 66 random bytes
// Output public key - 133 bytes (0x04<x>|<y>), or 67 if compressed (0x02<x>.. or 0x03<x>)
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
    if sha==64 {
        let mut sh512 = SHA512::new();
        for i in 0..m.len() {
            sh512.process(m[i]);
        }
        let h=sh512.hash();
        thm[0]=0; thm[1]=0;
        for i in 0..64 {
            thm[i+2]=h[i];
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
