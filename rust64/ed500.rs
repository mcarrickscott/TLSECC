#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// Elliptic curve based digital signature template
// See for example https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf


use crate::edwards500;
use crate::edwards500::*;
use crate::edwards500::ECP;

use crate::hash;
use crate::hash::*;
//use hash::SHA256;  // choose your hash function here

/*** Insert code automatically generated in group.rs here ***/
/* Note that much of this code is not needed and can be deleted */
//Automatically generated modular arithmetic Rust code
//Command line : python monty_rust.py 64 00022095386603298957622589030453586294277462333810790687329012216734648903125732812162322745424568361198616424824173658079693047834955346648597521750624369
//Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

type SPINT = u64;
type SSPINT = i64;
type DPINT = u128;
type SDPINT = i128;
//propagate carries
#[allow(unused_variables)]
#[inline]
fn prop(n: &mut [SPINT]) -> SPINT {
    let mask = ((1 as SPINT) << 57) - 1;
    let mut carry = (n[0] as SSPINT) >> 57;
    n[0] &= mask;
    for i in 1..8 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 57;
    }
    n[8] += carry as SPINT;
    return ((n[8] as SSPINT) >> 63) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> SPINT {
    let q = (1 as SPINT) << 57;
    let carry = prop(n);
    n[0] += (0x1e5f0228c2d0871 as SPINT) & (carry as SPINT);
    n[1] += (0x8ba6c508ad6ab as SPINT) & (carry as SPINT);
    n[2] += (0xaf06fba1be3244 as SPINT) & (carry as SPINT);
    n[3] += (0x1b41960f8b5795b as SPINT) & (carry as SPINT);
    n[4] += (0x2c8858 as SPINT) & (carry as SPINT);
    n[8] += (0x6c0000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return carry & 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
#[inline]
fn modfsb(n: &mut [SPINT]) -> SPINT {
    let q = (1 as SPINT) << 57;
    n[0] -= 0x1e5f0228c2d0871 as SPINT;
    n[1] -= 0x8ba6c508ad6ab as SPINT;
    n[2] -= 0xaf06fba1be3244 as SPINT;
    n[3] -= 0x1b41960f8b5795b as SPINT;
    n[4] -= 0x2c8858 as SPINT;
    n[8] -= 0x6c0000000000 as SPINT;
    return flatten(n);
}

//Modular addition - can be used in all cases - reduce less than 2p
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
    n[0] -= 0x3cbe045185a10e2 as SPINT;
    n[1] -= 0x1174d8a115ad56 as SPINT;
    n[2] -= 0x15e0df7437c6488 as SPINT;
    n[3] -= 0x36832c1f16af2b6 as SPINT;
    n[4] -= 0x5910b0 as SPINT;
    n[8] -= 0xd80000000000 as SPINT;
    let carry = prop(n);
    n[0] += (0x3cbe045185a10e2 as SPINT) & (carry as SPINT);
    n[1] += (0x1174d8a115ad56 as SPINT) & (carry as SPINT);
    n[2] += (0x15e0df7437c6488 as SPINT) & (carry as SPINT);
    n[3] += (0x36832c1f16af2b6 as SPINT) & (carry as SPINT);
    n[4] += (0x5910b0 as SPINT) & (carry as SPINT);
    n[8] += (0xd80000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

//Fast modular addition - can be used if output is consumed ONLY by multiplication or squaring
#[allow(unused_variables)]
#[inline]
fn modaddf(b: &[SPINT], n: &mut [SPINT]) {
    n[0] = n[0] + b[0];
    n[1] = n[1] + b[1];
    n[2] = n[2] + b[2];
    n[3] = n[3] + b[3];
    n[4] = n[4] + b[4];
    n[5] = n[5] + b[5];
    n[6] = n[6] + b[6];
    n[7] = n[7] + b[7];
    n[8] = n[8] + b[8];
    return;
}

//Modular subtraction - can be used in all cases - reduce less than 2p
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
    n[0] += (0x3cbe045185a10e2 as SPINT) & (carry as SPINT);
    n[1] += (0x1174d8a115ad56 as SPINT) & (carry as SPINT);
    n[2] += (0x15e0df7437c6488 as SPINT) & (carry as SPINT);
    n[3] += (0x36832c1f16af2b6 as SPINT) & (carry as SPINT);
    n[4] += (0x5910b0 as SPINT) & (carry as SPINT);
    n[8] += (0xd80000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

//Fast modular subtraction - can be used if output is ONLY consumed by multiplication or squaring
#[allow(unused_variables)]
#[inline]
fn modsubf(b: &[SPINT], n: &mut [SPINT]) {
    n[0] = n[0] - b[0];
    n[1] = n[1] - b[1];
    n[2] = n[2] - b[2];
    n[3] = n[3] - b[3];
    n[4] = n[4] - b[4];
    n[5] = n[5] - b[5];
    n[6] = n[6] - b[6];
    n[7] = n[7] - b[7];
    n[8] = n[8] - b[8];
    n[0] += 0x3cbe045185a10e2 as SPINT;
    n[1] += 0x1174d8a115ad56 as SPINT;
    n[2] += 0x15e0df7437c6488 as SPINT;
    n[3] += 0x36832c1f16af2b6 as SPINT;
    n[4] += 0x5910b0 as SPINT;
    n[8] += 0xd80000000000 as SPINT;
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
    n[0] += (0x3cbe045185a10e2 as SPINT) & (carry as SPINT);
    n[1] += (0x1174d8a115ad56 as SPINT) & (carry as SPINT);
    n[2] += (0x15e0df7437c6488 as SPINT) & (carry as SPINT);
    n[3] += (0x36832c1f16af2b6 as SPINT) & (carry as SPINT);
    n[4] += (0x5910b0 as SPINT) & (carry as SPINT);
    n[8] += (0xd80000000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
#[inline]
fn modmli(b: usize, c: &mut [SPINT]) {
    let p0 = 0x1e5f0228c2d0871 as SPINT;
    let p1 = 0x8ba6c508ad6ab as SPINT;
    let p2 = 0xaf06fba1be3244 as SPINT;
    let p3 = 0x1b41960f8b5795b as SPINT;
    let p4 = 0x2c8858 as SPINT;
    let p8 = 0x6c0000000000 as SPINT;
    let mask = ((1 as SPINT) << 57) - 1;
    let mut t = 0 as DPINT;
    let r = 0x25ed097b425ed09 as SPINT;
    t += (c[0] as DPINT) * (b as DPINT);
    c[0] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[1] as DPINT) * (b as DPINT);
    c[1] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[2] as DPINT) * (b as DPINT);
    c[2] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[3] as DPINT) * (b as DPINT);
    c[3] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[4] as DPINT) * (b as DPINT);
    c[4] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[5] as DPINT) * (b as DPINT);
    c[5] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[6] as DPINT) * (b as DPINT);
    c[6] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[7] as DPINT) * (b as DPINT);
    c[7] = (t as SPINT) & mask;
    t = t >> 57;
    t += (c[8] as DPINT) * (b as DPINT);
    c[8] = t as SPINT;

    //Barrett-Dhem reduction
    let h = (t >> 40) as SPINT;
    let q = (((h as DPINT) * (r as DPINT)) >> 64) as SPINT;
    t = (q as DPINT) * (p0 as DPINT);
    c[0] -= (t as SPINT) & mask;
    c[1] -= (t >> 57) as SPINT;
    t = (q as DPINT) * (p1 as DPINT);
    c[1] -= (t as SPINT) & mask;
    c[2] -= (t >> 57) as SPINT;
    t = (q as DPINT) * (p2 as DPINT);
    c[2] -= (t as SPINT) & mask;
    c[3] -= (t >> 57) as SPINT;
    t = (q as DPINT) * (p3 as DPINT);
    c[3] -= (t as SPINT) & mask;
    c[4] -= (t >> 57) as SPINT;
    t = (q as DPINT) * (p4 as DPINT);
    c[4] -= (t as SPINT) & mask;
    c[5] -= (t >> 57) as SPINT;
    c[8] -= q * p8;
    prop(c);
    return;
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 231796079338595989747038106899967222
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
#[inline]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0x1e5f0228c2d0871 as SPINT;
    let p1 = 0x8ba6c508ad6ab as SPINT;
    let p2 = 0xaf06fba1be3244 as SPINT;
    let p3 = 0x1b41960f8b5795b as SPINT;
    let p4 = 0x2c8858 as SPINT;
    let p8 = 0x6c0000000000 as SPINT;
    let q = (1 as SPINT) << 57; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x172f1686e42476f as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as DPINT) * (p0 as DPINT);
    t >>= 57;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p1 as DPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as DPINT) * (p0 as DPINT);
    t >>= 57;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p2 as DPINT);
    t += (v1 as DPINT) * (p1 as DPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as DPINT) * (p0 as DPINT);
    t >>= 57;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v1 as DPINT) * (p2 as DPINT);
    t += (v2 as DPINT) * (p1 as DPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as DPINT) * (p0 as DPINT);
    t >>= 57;
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
    t >>= 57;
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    t += (v1 as DPINT) * (p4 as DPINT);
    t += (v2 as DPINT) * (p3 as DPINT);
    t += (v3 as DPINT) * (p2 as DPINT);
    t += (v4 as DPINT) * (p1 as DPINT);
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as DPINT) * (p0 as DPINT);
    t >>= 57;
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
    t += (v2 as DPINT) * (p4 as DPINT);
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as DPINT) * (p0 as DPINT);
    t >>= 57;
    t += (c[0] as DPINT) * (b[7] as DPINT);
    t += (c[1] as DPINT) * (b[6] as DPINT);
    t += (c[2] as DPINT) * (b[5] as DPINT);
    t += (c[3] as DPINT) * (b[4] as DPINT);
    t += (c[4] as DPINT) * (b[3] as DPINT);
    t += (c[5] as DPINT) * (b[2] as DPINT);
    t += (c[6] as DPINT) * (b[1] as DPINT);
    t += (c[7] as DPINT) * (b[0] as DPINT);
    t += (v3 as DPINT) * (p4 as DPINT);
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as DPINT) * (p0 as DPINT);
    t >>= 57;
    t += (c[0] as DPINT) * (b[8] as DPINT);
    t += (c[1] as DPINT) * (b[7] as DPINT);
    t += (c[2] as DPINT) * (b[6] as DPINT);
    t += (c[3] as DPINT) * (b[5] as DPINT);
    t += (c[4] as DPINT) * (b[4] as DPINT);
    t += (c[5] as DPINT) * (b[3] as DPINT);
    t += (c[6] as DPINT) * (b[2] as DPINT);
    t += (c[7] as DPINT) * (b[1] as DPINT);
    t += (c[8] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p8 as DPINT);
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += (v7 as DPINT) * (p1 as DPINT);
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as DPINT) * (p0 as DPINT);
    t >>= 57;
    t += (c[1] as DPINT) * (b[8] as DPINT);
    t += (c[2] as DPINT) * (b[7] as DPINT);
    t += (c[3] as DPINT) * (b[6] as DPINT);
    t += (c[4] as DPINT) * (b[5] as DPINT);
    t += (c[5] as DPINT) * (b[4] as DPINT);
    t += (c[6] as DPINT) * (b[3] as DPINT);
    t += (c[7] as DPINT) * (b[2] as DPINT);
    t += (c[8] as DPINT) * (b[1] as DPINT);
    t += (v1 as DPINT) * (p8 as DPINT);
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    t += (c[2] as DPINT) * (b[8] as DPINT);
    t += (c[3] as DPINT) * (b[7] as DPINT);
    t += (c[4] as DPINT) * (b[6] as DPINT);
    t += (c[5] as DPINT) * (b[5] as DPINT);
    t += (c[6] as DPINT) * (b[4] as DPINT);
    t += (c[7] as DPINT) * (b[3] as DPINT);
    t += (c[8] as DPINT) * (b[2] as DPINT);
    t += (v2 as DPINT) * (p8 as DPINT);
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    t += (c[3] as DPINT) * (b[8] as DPINT);
    t += (c[4] as DPINT) * (b[7] as DPINT);
    t += (c[5] as DPINT) * (b[6] as DPINT);
    t += (c[6] as DPINT) * (b[5] as DPINT);
    t += (c[7] as DPINT) * (b[4] as DPINT);
    t += (c[8] as DPINT) * (b[3] as DPINT);
    t += (v3 as DPINT) * (p8 as DPINT);
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    t += (c[4] as DPINT) * (b[8] as DPINT);
    t += (c[5] as DPINT) * (b[7] as DPINT);
    t += (c[6] as DPINT) * (b[6] as DPINT);
    t += (c[7] as DPINT) * (b[5] as DPINT);
    t += (c[8] as DPINT) * (b[4] as DPINT);
    t += (v4 as DPINT) * (p8 as DPINT);
    t += (v8 as DPINT) * (p4 as DPINT);
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    t += (c[5] as DPINT) * (b[8] as DPINT);
    t += (c[6] as DPINT) * (b[7] as DPINT);
    t += (c[7] as DPINT) * (b[6] as DPINT);
    t += (c[8] as DPINT) * (b[5] as DPINT);
    t += (v5 as DPINT) * (p8 as DPINT);
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    t += (c[6] as DPINT) * (b[8] as DPINT);
    t += (c[7] as DPINT) * (b[7] as DPINT);
    t += (c[8] as DPINT) * (b[6] as DPINT);
    t += (v6 as DPINT) * (p8 as DPINT);
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    t += (c[7] as DPINT) * (b[8] as DPINT);
    t += (c[8] as DPINT) * (b[7] as DPINT);
    t += (v7 as DPINT) * (p8 as DPINT);
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    t += (c[8] as DPINT) * (b[8] as DPINT);
    t += (v8 as DPINT) * (p8 as DPINT);
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    c[8] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
#[inline]
fn modsqr(c: &mut [SPINT]) {
    let mut t: DPINT;
    let mut tot: DPINT;
    let mut s: SPINT;
    let p0 = 0x1e5f0228c2d0871 as SPINT;
    let p1 = 0x8ba6c508ad6ab as SPINT;
    let p2 = 0xaf06fba1be3244 as SPINT;
    let p3 = 0x1b41960f8b5795b as SPINT;
    let p4 = 0x2c8858 as SPINT;
    let p8 = 0x6c0000000000 as SPINT;
    let q = (1 as SPINT) << 57; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x172f1686e42476f as SPINT;
    tot = (c[0] as DPINT) * (c[0] as DPINT);
    t = tot;
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[1] as DPINT);
    tot *= 2;
    t += tot;
    t += (v0 as DPINT) * (p1 as DPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[2] as DPINT);
    tot *= 2;
    tot += (c[1] as DPINT) * (c[1] as DPINT);
    t += tot;
    t += (v0 as DPINT) * (p2 as DPINT);
    t += (v1 as DPINT) * (p1 as DPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[3] as DPINT);
    tot += (c[1] as DPINT) * (c[2] as DPINT);
    tot *= 2;
    t += tot;
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v1 as DPINT) * (p2 as DPINT);
    t += (v2 as DPINT) * (p1 as DPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[4] as DPINT);
    tot += (c[1] as DPINT) * (c[3] as DPINT);
    tot *= 2;
    tot += (c[2] as DPINT) * (c[2] as DPINT);
    t += tot;
    t += (v0 as DPINT) * (p4 as DPINT);
    t += (v1 as DPINT) * (p3 as DPINT);
    t += (v2 as DPINT) * (p2 as DPINT);
    t += (v3 as DPINT) * (p1 as DPINT);
    let v4 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v4 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[5] as DPINT);
    tot += (c[1] as DPINT) * (c[4] as DPINT);
    tot += (c[2] as DPINT) * (c[3] as DPINT);
    tot *= 2;
    t += tot;
    t += (v1 as DPINT) * (p4 as DPINT);
    t += (v2 as DPINT) * (p3 as DPINT);
    t += (v3 as DPINT) * (p2 as DPINT);
    t += (v4 as DPINT) * (p1 as DPINT);
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[6] as DPINT);
    tot += (c[1] as DPINT) * (c[5] as DPINT);
    tot += (c[2] as DPINT) * (c[4] as DPINT);
    tot *= 2;
    tot += (c[3] as DPINT) * (c[3] as DPINT);
    t += tot;
    t += (v2 as DPINT) * (p4 as DPINT);
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[7] as DPINT);
    tot += (c[1] as DPINT) * (c[6] as DPINT);
    tot += (c[2] as DPINT) * (c[5] as DPINT);
    tot += (c[3] as DPINT) * (c[4] as DPINT);
    tot *= 2;
    t += tot;
    t += (v3 as DPINT) * (p4 as DPINT);
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[0] as DPINT) * (c[8] as DPINT);
    tot += (c[1] as DPINT) * (c[7] as DPINT);
    tot += (c[2] as DPINT) * (c[6] as DPINT);
    tot += (c[3] as DPINT) * (c[5] as DPINT);
    tot *= 2;
    tot += (c[4] as DPINT) * (c[4] as DPINT);
    t += tot;
    t += (v0 as DPINT) * (p8 as DPINT);
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += (v7 as DPINT) * (p1 as DPINT);
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as DPINT) * (p0 as DPINT);
    t >>= 57;
    tot = (c[1] as DPINT) * (c[8] as DPINT);
    tot += (c[2] as DPINT) * (c[7] as DPINT);
    tot += (c[3] as DPINT) * (c[6] as DPINT);
    tot += (c[4] as DPINT) * (c[5] as DPINT);
    tot *= 2;
    t += tot;
    t += (v1 as DPINT) * (p8 as DPINT);
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    tot = (c[2] as DPINT) * (c[8] as DPINT);
    tot += (c[3] as DPINT) * (c[7] as DPINT);
    tot += (c[4] as DPINT) * (c[6] as DPINT);
    tot *= 2;
    tot += (c[5] as DPINT) * (c[5] as DPINT);
    t += tot;
    t += (v2 as DPINT) * (p8 as DPINT);
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    tot = (c[3] as DPINT) * (c[8] as DPINT);
    tot += (c[4] as DPINT) * (c[7] as DPINT);
    tot += (c[5] as DPINT) * (c[6] as DPINT);
    tot *= 2;
    t += tot;
    t += (v3 as DPINT) * (p8 as DPINT);
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    tot = (c[4] as DPINT) * (c[8] as DPINT);
    tot += (c[5] as DPINT) * (c[7] as DPINT);
    tot *= 2;
    tot += (c[6] as DPINT) * (c[6] as DPINT);
    t += tot;
    t += (v4 as DPINT) * (p8 as DPINT);
    t += (v8 as DPINT) * (p4 as DPINT);
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    tot = (c[5] as DPINT) * (c[8] as DPINT);
    tot += (c[6] as DPINT) * (c[7] as DPINT);
    tot *= 2;
    t += tot;
    t += (v5 as DPINT) * (p8 as DPINT);
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    tot = (c[6] as DPINT) * (c[8] as DPINT);
    tot *= 2;
    tot += (c[7] as DPINT) * (c[7] as DPINT);
    t += tot;
    t += (v6 as DPINT) * (p8 as DPINT);
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    tot = (c[7] as DPINT) * (c[8] as DPINT);
    tot *= 2;
    t += tot;
    t += (v7 as DPINT) * (p8 as DPINT);
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
    tot = (c[8] as DPINT) * (c[8] as DPINT);
    t += tot;
    t += (v8 as DPINT) * (p8 as DPINT);
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 57;
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
    modcpy(w, &mut x);
    modcpy(&x, &mut t11);
    modsqr(&mut t11);
    modcpy(&x, &mut t14);
    modmul(&t11, &mut t14);
    modcpy(&t11, &mut t7);
    modmul(&t14, &mut t7);
    modcpy(&x, &mut t6);
    modmul(&t7, &mut t6);
    modcpy(&t11, &mut t0);
    modmul(&t6, &mut t0);
    modcpy(&x, &mut t10);
    modmul(&t0, &mut t10);
    modcpy(&t11, &mut t15);
    modmul(&t10, &mut t15);
    modcpy(&t6, &mut t12);
    modmul(&t10, &mut t12);
    modcpy(&t11, &mut t2);
    modmul(&t12, &mut t2);
    modcpy(&t0, &mut t4);
    modmul(&t2, &mut t4);
    modcpy(&t11, &mut t8);
    modmul(&t4, &mut t8);
    modcpy(&t11, &mut t9);
    modmul(&t8, &mut t9);
    modcpy(&t11, &mut t3);
    modmul(&t9, &mut t3);
    modcpy(&t11, &mut z);
    modmul(&t3, &mut z);
    modcpy(&t11, &mut t1);
    modmul(&z, &mut t1);
    modcpy(&t0, &mut t13);
    modmul(&t1, &mut t13);
    modcpy(&t11, &mut t0);
    modmul(&t13, &mut t0);
    modcpy(&t11, &mut t5);
    modmul(&t0, &mut t5);
    modmul(&t5, &mut t6);
    modmul(&t6, &mut t11);
    modcpy(&t6, &mut t16);
    modmul(&t11, &mut t16);
    modnsqr(&mut t16, 250);
    modmul(&t15, &mut t16);
    modnsqr(&mut t16, 7);
    modmul(&t2, &mut t16);
    modnsqr(&mut t16, 8);
    modmul(&t16, &mut t15);
    modnsqr(&mut t15, 8);
    modmul(&t8, &mut t15);
    modnsqr(&mut t15, 2);
    modmul(&x, &mut t15);
    modnsqr(&mut t15, 10);
    modmul(&t4, &mut t15);
    modnsqr(&mut t15, 3);
    modmul(&t15, &mut t14);
    modnsqr(&mut t14, 10);
    modmul(&t3, &mut t14);
    modnsqr(&mut t14, 9);
    modmul(&t0, &mut t14);
    modnsqr(&mut t14, 7);
    modmul(&t5, &mut t14);
    modnsqr(&mut t14, 8);
    modmul(&t14, &mut t13);
    modnsqr(&mut t13, 7);
    modmul(&t6, &mut t13);
    modnsqr(&mut t13, 5);
    modmul(&t13, &mut t12);
    modnsqr(&mut t12, 11);
    modmul(&t11, &mut t12);
    modnsqr(&mut t12, 6);
    modmul(&t12, &mut t11);
    modnsqr(&mut t11, 7);
    modmul(&z, &mut t11);
    modnsqr(&mut t11, 6);
    modmul(&t5, &mut t11);
    modnsqr(&mut t11, 6);
    modmul(&t1, &mut t11);
    modnsqr(&mut t11, 6);
    modmul(&t11, &mut t10);
    modnsqr(&mut t10, 4);
    modmul(&x, &mut t10);
    modnsqr(&mut t10, 12);
    modmul(&t2, &mut t10);
    modnsqr(&mut t10, 6);
    modmul(&t10, &mut t9);
    modnsqr(&mut t9, 7);
    modmul(&t9, &mut t8);
    modnsqr(&mut t8, 6);
    modmul(&t8, &mut t7);
    modnsqr(&mut t7, 9);
    modmul(&t2, &mut t7);
    modnsqr(&mut t7, 7);
    modmul(&t0, &mut t7);
    modnsqr(&mut t7, 7);
    modmul(&t7, &mut t6);
    modnsqr(&mut t6, 7);
    modmul(&t6, &mut t5);
    modnsqr(&mut t5, 5);
    modmul(&t5, &mut t4);
    modnsqr(&mut t4, 6);
    modmul(&t4, &mut t3);
    modnsqr(&mut t3, 11);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 7);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 10);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 10);
    modmul(&t0, &mut z);
    modsqr(&mut z);
    modmul(&x, &mut z);
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
        0x19ae907b78465e1,
        0x18ec7433e483c0c,
        0x12dc4972482df31,
        0xf5c9fbc9f4ffd9,
        0x13976ad1060f0f,
        0x2c90aebc17fd4d,
        0x175815a19b83755,
        0x19d8fd4070f22d4,
        0x367d448bd399,
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
    return (1 & ((d - 1) >> 57) & (((c0 ^ 1) - 1) >> 57)) != 0;
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
    return (1 & ((d - 1) >> 57)) != 0;
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
    return modis1(&r) || modis0(x);
}

//conditional move g to f if d=1
#[inline(never)]
fn modcmv(b: usize, g: &[SPINT], f: &mut [SPINT]) {
    static mut R: SPINT = 0;
    let w: SPINT;
    unsafe {
        R += 0x3cc3c33c5aa5a55a;
        w = R;
    }
    let bb = b as SPINT;
    let c0 = (!bb) & (w + 1);
    let c1 = bb + w;
    for i in 0..9 {
        let s = g[i];
        let t = f[i];
        unsafe {
            core::ptr::write_volatile(&mut f[i], c0 * t + c1 * s);
        }
        f[i] -= w * (t + s);
    }
    return;
}

//conditional swap g and f if d=1
#[inline(never)]
fn modcsw(b: usize, g: &mut [SPINT], f: &mut [SPINT]) {
    let bb = b as SPINT;
    static mut R: SPINT = 0;
    let w: SPINT;
    unsafe {
        R += 0x3cc3c33c5aa5a55a;
        w = R;
    }
    let c0 = (!bb) & (w + 1);
    let c1 = bb + w;
    for i in 0..9 {
        let s = g[i];
        let t = f[i];
        let v = w * (t + s);
        unsafe {
            core::ptr::write_volatile(&mut f[i], c0 * t + c1 * s);
        }
        f[i] -= v;
        unsafe {
            core::ptr::write_volatile(&mut g[i], c0 * s + c1 * t);
        }
        g[i] -= v;
    }
    return;
}

//Modular square root, provide progenitor h if available, NULL if not
fn modsqrt(x: &[SPINT], h: Option<&[SPINT]>, r: &mut [SPINT]) {
    let mut t: [SPINT; 9] = [0; 9];
    let mut b: [SPINT; 9] = [0; 9];
    let mut v: [SPINT; 9] = [0; 9];
    let mut z: [SPINT; 9] = [
        0x5b5d4e0c40dd0c,
        0x1bcb83ad3c1a4c,
        0x1492003cc5c19f8,
        0x1c4a36b6e3f592c,
        0x1acb9375829bced,
        0x1f78bf0c7460468,
        0x9c9f5c3d17208b,
        0x1b56c8b561affbb,
        0x66c1e2df0b89,
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
    a[9 - 1] = (a[9 - 1] << n) + (a[9 - 2] >> (57 - n));
    for i in (1..9 - 1).rev() {
        a[i] = ((a[i] << n) & 0x1ffffffffffffff) + (a[i - 1] >> (57 - n));
    }
    a[0] = (a[0] << n) & 0x1ffffffffffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..9 - 1 {
        a[i] = (a[i] >> n) + ((a[i + 1] << (57 - n)) & 0x1ffffffffffffff);
    }
    a[9 - 1] = a[9 - 1] >> n;
    return r as isize;
}

//divide by 2. Shift right 1 bit (or add p and shift right one bit)
fn modhaf(n: &mut [SPINT]) {
    let mut t: [SPINT; 9] = [0; 9];
    prop(n);
    modcpy(n, &mut t);
    let lsb = modshr(1, &mut t) as usize;
    n[0] += 0x1e5f0228c2d0871 as SPINT;
    n[1] += 0x8ba6c508ad6ab as SPINT;
    n[2] += 0xaf06fba1be3244 as SPINT;
    n[3] += 0x1b41960f8b5795b as SPINT;
    n[4] += 0x2c8858 as SPINT;
    n[8] += 0x6c0000000000 as SPINT;
    prop(n);
    modshr(1, n);
    modcmv(1 - lsb, &t, n);
}

//set a= 2^r
fn mod2r(r: usize, a: &mut [SPINT]) {
    let n = r / 57;
    let m = r % 57;
    modzer(a);
    if r >= 64 * 8 {
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
    for i in (0..64).rev() {
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
    for i in 0..64 {
        modshl(8, a);
        a[0] += b[i] as SPINT;
    }
    let res = modfsb(a);
    nres(a);
    return res == 1;
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
        eq &= (((c[i] ^ d[i]) - 1) >> 57) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 9;
const RADIX: usize = 57;
const NBITS: usize = 503;
const NBYTES: usize = 64;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;
/*** End of automatically generated code ***/

const LIMBS:usize= NLIMBS;
type GEL = [SPINT; LIMBS];
pub const BYTES:usize = NBYTES;

// reduce 1024 bit array h to integer r modulo group order q, in constant time
// Consider h as 2^496.(2^496x + y) + z, where x,y and z < q (z is bottom 496 bits, y is next 496 bits, x is top 32 bits)
// Important that x and y < q, q is group order
fn reduce(h:&[u8],r:&mut [SPINT]) {
    let mut buff:[u8;BYTES]=[0;BYTES];    
    let mut x:GEL=[0;LIMBS];
    let mut y:GEL=[0;LIMBS];
    let mut z:GEL=[0;LIMBS];
    let mut c:GEL=[0;LIMBS];

    mod2r(496,&mut c);

    for i in 0..62 {  // bottom 496 bits
        buff[i]=h[i];
    }
    buff[62]=0; buff[63]=0;
    buff.reverse();
    modimp(&buff,&mut z);

    for i in 0..62 { // middle 496 bits
        buff[i]=h[i+62];
    }
    buff[62]=0; buff[63]=0;
    buff.reverse();
    modimp(&buff,&mut y);

    for i in 0..4 {  // top 32 bits
        buff[i]=h[124+i];
    }
    for i in 4..64 {
        buff[i]=0;
    }
    buff.reverse();
    modimp(&buff,&mut x);

    modmul(&c,&mut x);
    modadd(&y,&mut x);
    modmul(&c,&mut x);
    modadd(&z,&mut x);
    modcpy(&x,r);
}

// general purpose SHAKE256 hash function
// Input ilen bytes, output olen bytes
fn H(ilen:usize,olen:usize,s:&[u8],digest: &mut [u8])
{
    let mut sha3=SHA3::new(SHAKE256);
    for i in 0..ilen { 
        sha3.process(s[i]);
    }
    sha3.shake(digest,olen); 
}


// API implementation

// Input private key
// Output public key

pub fn KEY_PAIR(prv: &[u8],public: &mut [u8]) {
    let mut P=ECP::new();
    ecngen(&mut P);   // get curve generator point
    let mut s:[u8;BYTES]=[0;BYTES]; 

    H(BYTES,BYTES,&prv,&mut s);
// clamp s
    s[0]&=0xFC;
    s[62]&=0x7F; s[62]|=0x40; s[63]=0;

    s.reverse(); // little endian to big endian
 
    ecnmul(&s,&mut P); 

    let sign=ecnget(&mut P,None,Some(&mut s));  // get y coordinate and sign
    s.reverse(); // big endian to little endian
    for i in 0..BYTES {
        public[i]=s[i];
    }
             
    public[BYTES-1]|=(sign<<7) as u8;

}

const dom4:[u8;10]=[b'S',b'i',b'g',b'E',b'd',b'5',b'0',b'0',0,0];


// Note that a per message "random" number usually generated by a Cryptographically Secure Random Number Generator (CSRNG) is often required, and if so is calculated externally and passed into the signature function
// Methods are
// 1. non-deterministic - a non-biased high entropy random number in the range from 0 - q (the group order) - recommended if CSRNG is good
// 2. deterministic - derived from a hash of the message to be signed and the secret key (must be done like this for EdDSA, optional for ECDSA)
// 3. hedged - a combination of methods 1 and 2 - recommended if CSRNG is poor
// Note that this functionality is out-of-scope for this exercise (except for EdDSA). For examples see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf and RFC6979

// To create such a per message random number, consider the method reduce() from Ed448.rs, which reduces a much larger number modulo q, as required to minimize bias

// input private key, per-message random number (or public key), message to be signed
// Output signature.
pub fn SIGN(prv: &[u8],public: Option<&[u8]>,m:&[u8],sig: &mut [u8]) {
    let mut R=ECP::new();
    let mut h:[u8;2*BYTES]=[0;2*BYTES];  
    let mut ipub:[u8;BYTES]=[0;BYTES];
    let mut sh:[u8;BYTES]=[0;BYTES];
    let mut r:GEL = [0;LIMBS]; // some group elements..
    let mut s:GEL = [0;LIMBS];
    let mut d:GEL=[0;LIMBS];
    
    let mut sha3=SHA3::new(SHAKE256);

    ecngen(&mut R);   // get curve generator point

    if let Some(pb) = public {
        for i in 0..BYTES {
            ipub[i]=pb[i];
        }
    } else {
        KEY_PAIR(prv,&mut ipub);
    }

    H(BYTES,2*BYTES,&prv,&mut h);

    for i in 0..BYTES {
        sh[i]=h[i];
    }

// derive and clamp s
    sh[0]&=0xFC;
    sh[62]&=0x7F; sh[62]|=0x40; sh[63]=0;
    sh.reverse();
    modimp(&sh,&mut s);

    for i in 0..10 {
        sha3.process(dom4[i]);
    }
    for i in BYTES..2*BYTES {
        sha3.process(h[i]);
    }
    for i in 0..m.len() {
        sha3.process(m[i]);
    }
    sha3.shake(&mut h,2*BYTES); 

    reduce(&h,&mut r);
    modexp(&r,&mut sh);  // convert to big endian array
    ecnmul(&sh,&mut R);

    let sign=ecnget(&mut R,None,Some(&mut sh));  // get y coordinate and sign
    sh.reverse();              // big endian to little endian

    for i in 0..BYTES {
        sig[i]=sh[i];
    }
    sig[BYTES-1]|=(sign<<7) as u8; // first part of signature

    sha3=SHA3::new(SHAKE256);
    for i in 0..10 {
        sha3.process(dom4[i]);
    }
    for i in 0..BYTES {
        sha3.process(sig[i]);  // R
    }
    for i in 0..BYTES {
        sha3.process(ipub[i]);  // Q
    }
    for i in 0..m.len() {
        sha3.process(m[i]);
    }
    sha3.shake(&mut h,2*BYTES);

    reduce(&h,&mut d);
    modmul(&s,&mut d);
    modadd(&r,&mut d);

    modexp(&d,&mut sh);
    sh.reverse();
    for i in 0..BYTES {
        sig[BYTES+i]=sh[i];
    }
}

// input public key, message and signature. 
// Return true for success, false for failure
pub fn VERIFY(public: &[u8],m:&[u8],sig:&[u8]) -> bool {
    let mut buff:[u8;BYTES]=[0;BYTES]; 
    let mut sh:[u8;BYTES]=[0;BYTES];
    let mut G=ECP::new();
    let mut R=ECP::new();
    let mut Q=ECP::new();
   
    let mut u:GEL=[0;LIMBS];
    let mut h:[u8;2*BYTES]=[0;2*BYTES];

    ecngen(&mut G);   // get curve generator point
// reconstruct point R
    for i in 0..BYTES {
        buff[i]=sig[i];
    }
    buff[BYTES-1]&=0x7f;
    buff.reverse();
    let mut sign=((sig[BYTES-1]>>7)&1) as usize;
    ecnset(sign,None,Some(&buff),&mut R);
    if ecnisinf(&R) {
        return false;
    }

// reconstruct point Q 
    for i in 0..BYTES {
        buff[i]=public[i];
    }
    buff[BYTES-1]&=0x7f;
    buff.reverse();
    sign=((public[BYTES-1]>>7)&1) as usize;
    ecnset(sign,None,Some(&buff),&mut Q);
    if ecnisinf(&Q) {
        return false;
    }
    for i in 0..BYTES {
        buff[i]=sig[i+BYTES];
    }
    buff.reverse();
    let mut sha3=SHA3::new(SHAKE256);

    for i in 0..10 {
        sha3.process(dom4[i]);
    }
    for i in 0..BYTES {
        sha3.process(sig[i]);  // R
    }
    for i in 0..BYTES {
        sha3.process(public[i]);  // Q
    }
    for i in 0..m.len() {
        sha3.process(m[i]);
    }
    sha3.shake(&mut h,2*BYTES);

    reduce(&h,&mut u); modneg(&mut u); modexp(&u,&mut sh);

    if !modimp(&buff,&mut u) {
        return false;  // out of range
    }
    ecncof(&mut G); ecncof(&mut R); ecncof(&mut Q);
    Q=ecnmul2(&buff,&G,&sh,&Q);

    if ecncmp(&R,&Q) {
        return true;
    }
    return false;
}

