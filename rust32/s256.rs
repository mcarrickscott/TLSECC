#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// For example: cargo run --release --features NIST256 --bin EC256
// python curve_rust.py 64 NIST256
// This completes weierstrass.rs for this curve
// ECDSA Implementation for curve P-256
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf

use crate::nist256;
use crate::nist256::*;
use crate::nist256::ECP;

use crate::hash;
use crate::hash::*;

/*** Insert code automatically generated in group.rs here ***/
/* Note that much of this code is not needed and can be deleted */

type SPINT = u32;
type SSPINT = i32;
type UDPINT = u64;
type DPINT = u64;
//propagate carries
#[allow(unused_variables)]
#[inline]
fn prop(n: &mut [SPINT]) -> SPINT {
    let mask = ((1 as SPINT) << 29) - 1;
    let mut carry = (n[0] as SSPINT) >> 29;
    n[0] &= mask;
    for i in 1..8 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 29;
    }
    n[8] += carry as SPINT;
    return ((n[8] as SSPINT) >> 31) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 29;
    let carry = prop(n);
    n[0] += (0x1c632551 as SPINT) & (carry as SPINT);
    n[1] += (0x1dce5617 as SPINT) & (carry as SPINT);
    n[2] += (0x5e7a13c as SPINT) & (carry as SPINT);
    n[3] += (0xdf55b4e as SPINT) & (carry as SPINT);
    n[4] += (0x1ffffbce as SPINT) & (carry as SPINT);
    n[5] -= 1 & (carry as SPINT);
    n[6] += (0x40000 as SPINT) & (carry as SPINT);
    n[7] += (0x1fe00000 as SPINT) & (carry as SPINT);
    n[8] += (0xffffff as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 29;
    n[0] -= 0x1c632551 as SPINT;
    n[1] -= 0x1dce5617 as SPINT;
    n[2] -= 0x5e7a13c as SPINT;
    n[3] -= 0xdf55b4e as SPINT;
    n[4] -= 0x1ffffbce as SPINT;
    n[5] += 1;
    n[6] -= 0x40000 as SPINT;
    n[7] -= 0x1fe00000 as SPINT;
    n[8] -= 0xffffff as SPINT;
    return flatten(n);
}

//Modular addition - reduce less than 2p
#[allow(unused_variables)]
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
    n[0] -= 0x38c64aa2 as SPINT;
    n[1] -= 0x3b9cac2e as SPINT;
    n[2] -= 0xbcf4278 as SPINT;
    n[3] -= 0x1beab69c as SPINT;
    n[4] -= 0x3ffff79c as SPINT;
    n[5] += 2;
    n[6] -= 0x80000 as SPINT;
    n[7] -= 0x3fc00000 as SPINT;
    n[8] -= 0x1fffffe as SPINT;
    let carry = prop(n);
    n[0] += (0x38c64aa2 as SPINT) & (carry as SPINT);
    n[1] += (0x3b9cac2e as SPINT) & (carry as SPINT);
    n[2] += (0xbcf4278 as SPINT) & (carry as SPINT);
    n[3] += (0x1beab69c as SPINT) & (carry as SPINT);
    n[4] += (0x3ffff79c as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[6] += (0x80000 as SPINT) & (carry as SPINT);
    n[7] += (0x3fc00000 as SPINT) & (carry as SPINT);
    n[8] += (0x1fffffe as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

//Modular subtraction - reduce less than 2p
#[allow(unused_variables)]
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
    n[0] += (0x38c64aa2 as SPINT) & (carry as SPINT);
    n[1] += (0x3b9cac2e as SPINT) & (carry as SPINT);
    n[2] += (0xbcf4278 as SPINT) & (carry as SPINT);
    n[3] += (0x1beab69c as SPINT) & (carry as SPINT);
    n[4] += (0x3ffff79c as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[6] += (0x80000 as SPINT) & (carry as SPINT);
    n[7] += (0x3fc00000 as SPINT) & (carry as SPINT);
    n[8] += (0x1fffffe as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

//Modular negation
#[allow(unused_variables)]
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
    n[0] += (0x38c64aa2 as SPINT) & (carry as SPINT);
    n[1] += (0x3b9cac2e as SPINT) & (carry as SPINT);
    n[2] += (0xbcf4278 as SPINT) & (carry as SPINT);
    n[3] += (0x1beab69c as SPINT) & (carry as SPINT);
    n[4] += (0x3ffff79c as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[6] += (0x80000 as SPINT) & (carry as SPINT);
    n[7] += (0x3fc00000 as SPINT) & (carry as SPINT);
    n[8] += (0x1fffffe as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
fn modmli(_b: usize, _c: &mut [SPINT]) {
    return;
}

// Overflow limit   = 18446744073709551616
// maximum possible = 3881626228440796233
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0x1c632551 as SPINT;
    let p1 = 0x1dce5617 as SPINT;
    let p2 = 0x5e7a13c as SPINT;
    let p3 = 0xdf55b4e as SPINT;
    let p4 = 0x1ffffbce as SPINT;
    let p7 = 0x1fe00000 as SPINT;
    let p8 = 0xffffff as SPINT;
    let q = (1 as SPINT) << 29; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0xe00bc4f as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as DPINT) * (p0 as DPINT);
    t >>= 29;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p1 as DPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as DPINT) * (p0 as DPINT);
    t >>= 29;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p2 as DPINT);
    t += (v1 as DPINT) * (p1 as DPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as DPINT) * (p0 as DPINT);
    t >>= 29;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v1 as DPINT) * (p2 as DPINT);
    t += (v2 as DPINT) * (p1 as DPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as DPINT) * (p0 as DPINT);
    t >>= 29;
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
    t >>= 29;
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
    t >>= 29;
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    t += (v0 as DPINT) << 18;
    s -= v1;
    t += (v2 as DPINT) * (p4 as DPINT);
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as DPINT) * (p0 as DPINT);
    t >>= 29;
    t += (c[0] as DPINT) * (b[7] as DPINT);
    t += (c[1] as DPINT) * (b[6] as DPINT);
    t += (c[2] as DPINT) * (b[5] as DPINT);
    t += (c[3] as DPINT) * (b[4] as DPINT);
    t += (c[4] as DPINT) * (b[3] as DPINT);
    t += (c[5] as DPINT) * (b[2] as DPINT);
    t += (c[6] as DPINT) * (b[1] as DPINT);
    t += (c[7] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    t += (v0 as DPINT) * (p7 as DPINT);
    t += (v1 as DPINT) << 18;
    s -= v2;
    t += (v3 as DPINT) * (p4 as DPINT);
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as DPINT) * (p0 as DPINT);
    t >>= 29;
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
    t += (v0 as DPINT) * (p8 as DPINT);
    t += (v1 as DPINT) * (p7 as DPINT);
    t += (v2 as DPINT) << 18;
    s -= v3;
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += (v7 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as DPINT) * (p0 as DPINT);
    t >>= 29;
    t += (c[1] as DPINT) * (b[8] as DPINT);
    t += (c[2] as DPINT) * (b[7] as DPINT);
    t += (c[3] as DPINT) * (b[6] as DPINT);
    t += (c[4] as DPINT) * (b[5] as DPINT);
    t += (c[5] as DPINT) * (b[4] as DPINT);
    t += (c[6] as DPINT) * (b[3] as DPINT);
    t += (c[7] as DPINT) * (b[2] as DPINT);
    t += (c[8] as DPINT) * (b[1] as DPINT);
    s = mask as SPINT;
    t += (v1 as DPINT) * (p8 as DPINT);
    t += (v2 as DPINT) * (p7 as DPINT);
    t += (v3 as DPINT) << 18;
    s -= v4;
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[2] as DPINT) * (b[8] as DPINT);
    t += (c[3] as DPINT) * (b[7] as DPINT);
    t += (c[4] as DPINT) * (b[6] as DPINT);
    t += (c[5] as DPINT) * (b[5] as DPINT);
    t += (c[6] as DPINT) * (b[4] as DPINT);
    t += (c[7] as DPINT) * (b[3] as DPINT);
    t += (c[8] as DPINT) * (b[2] as DPINT);
    s = mask as SPINT;
    t += (v2 as DPINT) * (p8 as DPINT);
    t += (v3 as DPINT) * (p7 as DPINT);
    t += (v4 as DPINT) << 18;
    s -= v5;
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    t += s as DPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[3] as DPINT) * (b[8] as DPINT);
    t += (c[4] as DPINT) * (b[7] as DPINT);
    t += (c[5] as DPINT) * (b[6] as DPINT);
    t += (c[6] as DPINT) * (b[5] as DPINT);
    t += (c[7] as DPINT) * (b[4] as DPINT);
    t += (c[8] as DPINT) * (b[3] as DPINT);
    s = mask as SPINT;
    t += (v3 as DPINT) * (p8 as DPINT);
    t += (v4 as DPINT) * (p7 as DPINT);
    t += (v5 as DPINT) << 18;
    s -= v6;
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    t += s as DPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[4] as DPINT) * (b[8] as DPINT);
    t += (c[5] as DPINT) * (b[7] as DPINT);
    t += (c[6] as DPINT) * (b[6] as DPINT);
    t += (c[7] as DPINT) * (b[5] as DPINT);
    t += (c[8] as DPINT) * (b[4] as DPINT);
    s = mask as SPINT;
    t += (v4 as DPINT) * (p8 as DPINT);
    t += (v5 as DPINT) * (p7 as DPINT);
    t += (v6 as DPINT) << 18;
    s -= v7;
    t += (v8 as DPINT) * (p4 as DPINT);
    t += s as DPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[5] as DPINT) * (b[8] as DPINT);
    t += (c[6] as DPINT) * (b[7] as DPINT);
    t += (c[7] as DPINT) * (b[6] as DPINT);
    t += (c[8] as DPINT) * (b[5] as DPINT);
    s = mask as SPINT;
    t += (v5 as DPINT) * (p8 as DPINT);
    t += (v6 as DPINT) * (p7 as DPINT);
    t += (v7 as DPINT) << 18;
    s -= v8;
    t += s as DPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[6] as DPINT) * (b[8] as DPINT);
    t += (c[7] as DPINT) * (b[7] as DPINT);
    t += (c[8] as DPINT) * (b[6] as DPINT);
    s = mask as SPINT;
    t += (v6 as DPINT) * (p8 as DPINT);
    t += (v7 as DPINT) * (p7 as DPINT);
    t += (v8 as DPINT) << 18;
    t += s as DPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[7] as DPINT) * (b[8] as DPINT);
    t += (c[8] as DPINT) * (b[7] as DPINT);
    s = mask as SPINT;
    t += (v7 as DPINT) * (p8 as DPINT);
    t += (v8 as DPINT) * (p7 as DPINT);
    t += s as DPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[8] as DPINT) * (b[8] as DPINT);
    s = mask as SPINT;
    t += (v8 as DPINT) * (p8 as DPINT);
    t += s as DPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t -= 1;
    c[8] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let p0 = 0x1c632551 as SPINT;
    let p1 = 0x1dce5617 as SPINT;
    let p2 = 0x5e7a13c as SPINT;
    let p3 = 0xdf55b4e as SPINT;
    let p4 = 0x1ffffbce as SPINT;
    let p7 = 0x1fe00000 as SPINT;
    let p8 = 0xffffff as SPINT;
    let q = (1 as SPINT) << 29; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0xe00bc4f as SPINT;
    tot = (c[0] as UDPINT) * (c[0] as UDPINT);
    t = tot;
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[0] as UDPINT) * (c[1] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p1 as UDPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[0] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    tot += (c[1] as UDPINT) * (c[1] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p2 as UDPINT);
    t += (v1 as UDPINT) * (p1 as UDPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[0] as UDPINT) * (c[3] as UDPINT);
    tot += (c[1] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p3 as UDPINT);
    t += (v1 as UDPINT) * (p2 as UDPINT);
    t += (v2 as UDPINT) * (p1 as UDPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
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
    t >>= 29;
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
    t >>= 29;
    tot = (c[0] as UDPINT) * (c[6] as UDPINT);
    tot += (c[1] as UDPINT) * (c[5] as UDPINT);
    tot += (c[2] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    tot += (c[3] as UDPINT) * (c[3] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v0 as UDPINT) << 18;
    s -= v1;
    t += (v2 as UDPINT) * (p4 as UDPINT);
    t += (v3 as UDPINT) * (p3 as UDPINT);
    t += (v4 as UDPINT) * (p2 as UDPINT);
    t += (v5 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[0] as UDPINT) * (c[7] as UDPINT);
    tot += (c[1] as UDPINT) * (c[6] as UDPINT);
    tot += (c[2] as UDPINT) * (c[5] as UDPINT);
    tot += (c[3] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v0 as UDPINT) * (p7 as UDPINT);
    t += (v1 as UDPINT) << 18;
    s -= v2;
    t += (v3 as UDPINT) * (p4 as UDPINT);
    t += (v4 as UDPINT) * (p3 as UDPINT);
    t += (v5 as UDPINT) * (p2 as UDPINT);
    t += (v6 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[0] as UDPINT) * (c[8] as UDPINT);
    tot += (c[1] as UDPINT) * (c[7] as UDPINT);
    tot += (c[2] as UDPINT) * (c[6] as UDPINT);
    tot += (c[3] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    tot += (c[4] as UDPINT) * (c[4] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v0 as UDPINT) * (p8 as UDPINT);
    t += (v1 as UDPINT) * (p7 as UDPINT);
    t += (v2 as UDPINT) << 18;
    s -= v3;
    t += (v4 as UDPINT) * (p4 as UDPINT);
    t += (v5 as UDPINT) * (p3 as UDPINT);
    t += (v6 as UDPINT) * (p2 as UDPINT);
    t += (v7 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[1] as UDPINT) * (c[8] as UDPINT);
    tot += (c[2] as UDPINT) * (c[7] as UDPINT);
    tot += (c[3] as UDPINT) * (c[6] as UDPINT);
    tot += (c[4] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v1 as UDPINT) * (p8 as UDPINT);
    t += (v2 as UDPINT) * (p7 as UDPINT);
    t += (v3 as UDPINT) << 18;
    s -= v4;
    t += (v5 as UDPINT) * (p4 as UDPINT);
    t += (v6 as UDPINT) * (p3 as UDPINT);
    t += (v7 as UDPINT) * (p2 as UDPINT);
    t += (v8 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[2] as UDPINT) * (c[8] as UDPINT);
    tot += (c[3] as UDPINT) * (c[7] as UDPINT);
    tot += (c[4] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    tot += (c[5] as UDPINT) * (c[5] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v2 as UDPINT) * (p8 as UDPINT);
    t += (v3 as UDPINT) * (p7 as UDPINT);
    t += (v4 as UDPINT) << 18;
    s -= v5;
    t += (v6 as UDPINT) * (p4 as UDPINT);
    t += (v7 as UDPINT) * (p3 as UDPINT);
    t += (v8 as UDPINT) * (p2 as UDPINT);
    t += s as UDPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[3] as UDPINT) * (c[8] as UDPINT);
    tot += (c[4] as UDPINT) * (c[7] as UDPINT);
    tot += (c[5] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v3 as UDPINT) * (p8 as UDPINT);
    t += (v4 as UDPINT) * (p7 as UDPINT);
    t += (v5 as UDPINT) << 18;
    s -= v6;
    t += (v7 as UDPINT) * (p4 as UDPINT);
    t += (v8 as UDPINT) * (p3 as UDPINT);
    t += s as UDPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[4] as UDPINT) * (c[8] as UDPINT);
    tot += (c[5] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    tot += (c[6] as UDPINT) * (c[6] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v4 as UDPINT) * (p8 as UDPINT);
    t += (v5 as UDPINT) * (p7 as UDPINT);
    t += (v6 as UDPINT) << 18;
    s -= v7;
    t += (v8 as UDPINT) * (p4 as UDPINT);
    t += s as UDPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[5] as UDPINT) * (c[8] as UDPINT);
    tot += (c[6] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v5 as UDPINT) * (p8 as UDPINT);
    t += (v6 as UDPINT) * (p7 as UDPINT);
    t += (v7 as UDPINT) << 18;
    s -= v8;
    t += s as UDPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[6] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    tot += (c[7] as UDPINT) * (c[7] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v6 as UDPINT) * (p8 as UDPINT);
    t += (v7 as UDPINT) * (p7 as UDPINT);
    t += (v8 as UDPINT) << 18;
    t += s as UDPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[7] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v7 as UDPINT) * (p8 as UDPINT);
    t += (v8 as UDPINT) * (p7 as UDPINT);
    t += s as UDPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[8] as UDPINT) * (c[8] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v8 as UDPINT) * (p8 as UDPINT);
    t += s as UDPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t -= 1;
    c[8] = t as SPINT;
    return;
}

//copy
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
    modcpy(w, &mut x);
    modcpy(&x, &mut z);
    modsqr(&mut z);
    modcpy(&x, &mut t0);
    modmul(&z, &mut t0);
    modcpy(&t0, &mut t5);
    modsqr(&mut t5);
    modcpy(&z, &mut t1);
    modmul(&t5, &mut t1);
    modcpy(&x, &mut t4);
    modmul(&t1, &mut t4);
    modcpy(&t4, &mut t2);
    modsqr(&mut t2);
    modcpy(&x, &mut z);
    modmul(&t2, &mut z);
    modcpy(&x, &mut t6);
    modmul(&z, &mut t6);
    modcpy(&t1, &mut t0);
    modmul(&t6, &mut t0);
    modmul(&t0, &mut z);
    modmul(&z, &mut t2);
    modcpy(&t0, &mut t3);
    modmul(&t2, &mut t3);
    modcpy(&t4, &mut t0);
    modmul(&t3, &mut t0);
    modmul(&t0, &mut z);
    modmul(&z, &mut t6);
    modmul(&t6, &mut t2);
    modcpy(&t4, &mut t7);
    modmul(&t2, &mut t7);
    modcpy(&t3, &mut t4);
    modmul(&t7, &mut t4);
    modmul(&t4, &mut t0);
    modcpy(&t2, &mut t3);
    modmul(&t0, &mut t3);
    modcpy(&t1, &mut t2);
    modmul(&t3, &mut t2);
    modcpy(&x, &mut t1);
    modmul(&t2, &mut t1);
    modmul(&t1, &mut t7);
    modmul(&t7, &mut t6);
    modmul(&t6, &mut t1);
    modmul(&t1, &mut t5);
    modmul(&t5, &mut t4);
    modmul(&t4, &mut t2);
    modmul(&t2, &mut t3);
    modmul(&t3, &mut t6);
    modmul(&t6, &mut t4);
    modmul(&t4, &mut t3);
    modmul(&t3, &mut t4);
    modmul(&t4, &mut t2);
    modmul(&t2, &mut t6);
    modmul(&t6, &mut t1);
    modmul(&t1, &mut t4);
    modmul(&t4, &mut t2);
    modmul(&t2, &mut t5);
    modmul(&t5, &mut t0);
    modmul(&t0, &mut t7);
    modcpy(&t7, &mut t8);
    modnsqr(&mut t8, 16);
    modmul(&t7, &mut t8);
    modnsqr(&mut t8, 48);
    modmul(&t7, &mut t8);
    modnsqr(&mut t8, 16);
    modmul(&t7, &mut t8);
    modnsqr(&mut t8, 16);
    modmul(&t7, &mut t8);
    modnsqr(&mut t8, 16);
    modmul(&t8, &mut t7);
    modnsqr(&mut t7, 15);
    modmul(&t7, &mut t6);
    modnsqr(&mut t6, 17);
    modmul(&t6, &mut t5);
    modnsqr(&mut t5, 16);
    modmul(&t5, &mut t4);
    modnsqr(&mut t4, 14);
    modmul(&t4, &mut t3);
    modnsqr(&mut t3, 18);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 15);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 17);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 10);
    modmul(&t0, &mut z);
    modsqr(&mut z);
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
    for _i in 0..4 - 1 {
        modsqr(&mut s);
        modmul(&z, &mut s);
    }
    modnsqr(&mut t, 5);
    modcpy(&t, z);
    modmul(&s, z);
    return;
}

//Convert n to n-residue form, n=nres(m)
fn nres(n: &mut [SPINT]) {
    let c: [SPINT; 9] = [
        0x148d9ef5, 0xf4e7f75, 0x14c6a651, 0x3f8b765, 0x165861f1, 0x1256d7d8, 0x1c185b23,
        0xd4cab0f, 0x84b655,
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
    return (1 & ((d - 1) >> 29) & (((c0 ^ 1) - 1) >> 29)) != 0;
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
    return (1 & ((d - 1) >> 29)) != 0;
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
    modnsqr(&mut r, 3);
    return modis1(&r) | modis0(x);
}

//conditional move g to f if d=1
#[inline(never)]
fn modcmv(d: usize, g: &[SPINT], f: &mut [SPINT]) {
    let dd=d as SPINT;
    let r=0x5aa5a55a;
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
    let r=0x5aa5a55a;
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
        0x141e6602, 0xc96bfdd, 0xde36bc1, 0xd95a008, 0x3dfc154, 0x1d674215, 0x64aea01, 0xc54ee1,
        0xffc97f,
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
    for k in (2..=4).rev() {
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
    a[9 - 1] = (a[9 - 1] << n) | (a[9 - 2] >> (29 - n));
    for i in (1..9 - 1).rev() {
        a[i] = ((a[i] << n) & 0x1fffffff) | (a[i - 1] >> (29 - n));
    }
    a[0] = (a[0] << n) & 0x1fffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..9 - 1 {
        a[i] = (a[i] >> n) | ((a[i + 1] << (29 - n)) & 0x1fffffff);
    }
    a[9 - 1] = a[9 - 1] >> n;
    return r as isize;
}

//set a= 2^r
fn mod2r(r: usize, a: &mut [SPINT]) {
    let n = r / 29;
    let m = r % 29;
    modzer(a);
    if r >= 32 * 8 {
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
    for i in (0..32).rev() {
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
    for i in 0..32 {
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
        eq &= (((c[i] ^ d[i]) - 1) >> 29) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 9;
const RADIX: usize = 29;
const NBITS: usize = 256;
const NBYTES: usize = 32;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;

/*** End of automatically generated code ***/

// number of limbs and bytes in representation
pub const BYTES:usize = NBYTES;
const LIMBS:usize= NLIMBS;
type GEL = [SPINT; LIMBS];


const PREHASHED:bool = false;  // true only for test vector

// reduce 40 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^248.x + y, where x and y < q (x is top 9 bytes, y is bottom 31 bytes)
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


// Input private key - 32 random bytes
// Output public key - 65 bytes (0x04<x>|<y>), or 33 if compressed (0x02<x>.. or 0x03<x>)
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
    if sha==32 {
        let mut sh256 = SHA256::new();
        for i in 0..m.len() {
            sh256.process(m[i]);
        }
        let h=sh256.hash();
        for i in 0..BYTES {
            thm[i]=h[i];
        } 
    }
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
