#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// Elliptic curve based digital signature template
// See for example https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf


use crate::edwards376;
use crate::edwards376::*;
use crate::edwards376::ECP;

use crate::hash;
use crate::hash::*;
//use hash::SHA256;  // choose your hash function here

/*** Insert code automatically generated in group.rs here ***/
/* Note that much of this code is not needed and can be deleted */

//Automatically generated modular arithmetic Rust code
//Command line : python monty_rust.py 32 002501103908950821434373181256356772360674006887285404329509619144461898813929743809681342407547708774670617348304951
//Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

type SPINT = u32;
type SSPINT = i32;
type DPINT = u64;
type SDPINT = i64;
//propagate carries
#[allow(unused_variables)]
#[inline]
fn prop(n: &mut [SPINT]) -> SPINT {
    let mask = ((1 as SPINT) << 28) - 1;
    let mut carry = (n[0] as SSPINT) >> 28;
    n[0] &= mask;
    for i in 1..13 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 28;
    }
    n[13] += carry as SPINT;
    return ((n[13] as SSPINT) >> 31) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> SPINT {
    let q = (1 as SPINT) << 28;
    let carry = prop(n);
    n[0] += (0xc6a7037 as SPINT) & (carry as SPINT);
    n[1] += (0x855d4f3 as SPINT) & (carry as SPINT);
    n[2] += (0xd04f09f as SPINT) & (carry as SPINT);
    n[3] += (0x98f29f0 as SPINT) & (carry as SPINT);
    n[4] += (0x9cd109a as SPINT) & (carry as SPINT);
    n[5] += (0xb351487 as SPINT) & (carry as SPINT);
    n[6] += (0x303a69 as SPINT) & (carry as SPINT);
    n[13] += (0x10400 as SPINT) & (carry as SPINT);
    prop(n);
    return carry & 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> SPINT {
    let q = (1 as SPINT) << 28;
    n[0] -= 0xc6a7037 as SPINT;
    n[1] -= 0x855d4f3 as SPINT;
    n[2] -= 0xd04f09f as SPINT;
    n[3] -= 0x98f29f0 as SPINT;
    n[4] -= 0x9cd109a as SPINT;
    n[5] -= 0xb351487 as SPINT;
    n[6] -= 0x303a69 as SPINT;
    n[13] -= 0x10400 as SPINT;
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
    n[9] = n[9] + b[9];
    n[10] = n[10] + b[10];
    n[11] = n[11] + b[11];
    n[12] = n[12] + b[12];
    n[13] = n[13] + b[13];
    n[0] -= 0x18d4e06e as SPINT;
    n[1] -= 0x10aba9e6 as SPINT;
    n[2] -= 0x1a09e13e as SPINT;
    n[3] -= 0x131e53e0 as SPINT;
    n[4] -= 0x139a2134 as SPINT;
    n[5] -= 0x166a290e as SPINT;
    n[6] -= 0x6074d2 as SPINT;
    n[13] -= 0x20800 as SPINT;
    let carry = prop(n);
    n[0] += (0x18d4e06e as SPINT) & (carry as SPINT);
    n[1] += (0x10aba9e6 as SPINT) & (carry as SPINT);
    n[2] += (0x1a09e13e as SPINT) & (carry as SPINT);
    n[3] += (0x131e53e0 as SPINT) & (carry as SPINT);
    n[4] += (0x139a2134 as SPINT) & (carry as SPINT);
    n[5] += (0x166a290e as SPINT) & (carry as SPINT);
    n[6] += (0x6074d2 as SPINT) & (carry as SPINT);
    n[13] += (0x20800 as SPINT) & (carry as SPINT);
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
    n[9] = n[9] - b[9];
    n[10] = n[10] - b[10];
    n[11] = n[11] - b[11];
    n[12] = n[12] - b[12];
    n[13] = n[13] - b[13];
    let carry = prop(n);
    n[0] += (0x18d4e06e as SPINT) & (carry as SPINT);
    n[1] += (0x10aba9e6 as SPINT) & (carry as SPINT);
    n[2] += (0x1a09e13e as SPINT) & (carry as SPINT);
    n[3] += (0x131e53e0 as SPINT) & (carry as SPINT);
    n[4] += (0x139a2134 as SPINT) & (carry as SPINT);
    n[5] += (0x166a290e as SPINT) & (carry as SPINT);
    n[6] += (0x6074d2 as SPINT) & (carry as SPINT);
    n[13] += (0x20800 as SPINT) & (carry as SPINT);
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
    n[9] = (0 as SPINT) - n[9];
    n[10] = (0 as SPINT) - n[10];
    n[11] = (0 as SPINT) - n[11];
    n[12] = (0 as SPINT) - n[12];
    n[13] = (0 as SPINT) - n[13];
    let carry = prop(n);
    n[0] += (0x18d4e06e as SPINT) & (carry as SPINT);
    n[1] += (0x10aba9e6 as SPINT) & (carry as SPINT);
    n[2] += (0x1a09e13e as SPINT) & (carry as SPINT);
    n[3] += (0x131e53e0 as SPINT) & (carry as SPINT);
    n[4] += (0x139a2134 as SPINT) & (carry as SPINT);
    n[5] += (0x166a290e as SPINT) & (carry as SPINT);
    n[6] += (0x6074d2 as SPINT) & (carry as SPINT);
    n[13] += (0x20800 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
fn modmli(b: usize, c: &mut [SPINT]) {
    let p0 = 0xc6a7037 as SPINT;
    let p1 = 0x855d4f3 as SPINT;
    let p2 = 0xd04f09f as SPINT;
    let p3 = 0x98f29f0 as SPINT;
    let p4 = 0x9cd109a as SPINT;
    let p5 = 0xb351487 as SPINT;
    let p6 = 0x303a69 as SPINT;
    let p13 = 0x10400 as SPINT;
    let mask = ((1 as SPINT) << 28) - 1;
    let mut t = 0 as DPINT;
    let r = 0x1f81f81f as SPINT;
    t += (c[0] as DPINT) * (b as DPINT);
    c[0] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[1] as DPINT) * (b as DPINT);
    c[1] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[2] as DPINT) * (b as DPINT);
    c[2] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[3] as DPINT) * (b as DPINT);
    c[3] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[4] as DPINT) * (b as DPINT);
    c[4] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[5] as DPINT) * (b as DPINT);
    c[5] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[6] as DPINT) * (b as DPINT);
    c[6] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[7] as DPINT) * (b as DPINT);
    c[7] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[8] as DPINT) * (b as DPINT);
    c[8] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[9] as DPINT) * (b as DPINT);
    c[9] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[10] as DPINT) * (b as DPINT);
    c[10] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[11] as DPINT) * (b as DPINT);
    c[11] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[12] as DPINT) * (b as DPINT);
    c[12] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[13] as DPINT) * (b as DPINT);
    c[13] = t as SPINT;

    //Barrett-Dhem reduction
    let h = (t >> 13) as SPINT;
    let q = (((h as DPINT) * (r as DPINT)) >> 32) as SPINT;
    t = (q as DPINT) * (p0 as DPINT);
    c[0] -= (t as SPINT) & mask;
    c[1] -= (t >> 28) as SPINT;
    t = (q as DPINT) * (p1 as DPINT);
    c[1] -= (t as SPINT) & mask;
    c[2] -= (t >> 28) as SPINT;
    t = (q as DPINT) * (p2 as DPINT);
    c[2] -= (t as SPINT) & mask;
    c[3] -= (t >> 28) as SPINT;
    t = (q as DPINT) * (p3 as DPINT);
    c[3] -= (t as SPINT) & mask;
    c[4] -= (t >> 28) as SPINT;
    t = (q as DPINT) * (p4 as DPINT);
    c[4] -= (t as SPINT) & mask;
    c[5] -= (t >> 28) as SPINT;
    t = (q as DPINT) * (p5 as DPINT);
    c[5] -= (t as SPINT) & mask;
    c[6] -= (t >> 28) as SPINT;
    t = (q as DPINT) * (p6 as DPINT);
    c[6] -= (t as SPINT) & mask;
    c[7] -= (t >> 28) as SPINT;
    c[13] -= q * p13;
    prop(c);
    return;
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1299425116202482891
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0xc6a7037 as SPINT;
    let p1 = 0x855d4f3 as SPINT;
    let p2 = 0xd04f09f as SPINT;
    let p3 = 0x98f29f0 as SPINT;
    let p4 = 0x9cd109a as SPINT;
    let p5 = 0xb351487 as SPINT;
    let p6 = 0x303a69 as SPINT;
    let p13 = 0x10400 as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x69fba79 as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p1 as DPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p2 as DPINT);
    t += (v1 as DPINT) * (p1 as DPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v1 as DPINT) * (p2 as DPINT);
    t += (v2 as DPINT) * (p1 as DPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as DPINT) * (p0 as DPINT);
    t >>= 28;
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
    t >>= 28;
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p5 as DPINT);
    t += (v1 as DPINT) * (p4 as DPINT);
    t += (v2 as DPINT) * (p3 as DPINT);
    t += (v3 as DPINT) * (p2 as DPINT);
    t += (v4 as DPINT) * (p1 as DPINT);
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p6 as DPINT);
    t += (v1 as DPINT) * (p5 as DPINT);
    t += (v2 as DPINT) * (p4 as DPINT);
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[7] as DPINT);
    t += (c[1] as DPINT) * (b[6] as DPINT);
    t += (c[2] as DPINT) * (b[5] as DPINT);
    t += (c[3] as DPINT) * (b[4] as DPINT);
    t += (c[4] as DPINT) * (b[3] as DPINT);
    t += (c[5] as DPINT) * (b[2] as DPINT);
    t += (c[6] as DPINT) * (b[1] as DPINT);
    t += (c[7] as DPINT) * (b[0] as DPINT);
    t += (v1 as DPINT) * (p6 as DPINT);
    t += (v2 as DPINT) * (p5 as DPINT);
    t += (v3 as DPINT) * (p4 as DPINT);
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[8] as DPINT);
    t += (c[1] as DPINT) * (b[7] as DPINT);
    t += (c[2] as DPINT) * (b[6] as DPINT);
    t += (c[3] as DPINT) * (b[5] as DPINT);
    t += (c[4] as DPINT) * (b[4] as DPINT);
    t += (c[5] as DPINT) * (b[3] as DPINT);
    t += (c[6] as DPINT) * (b[2] as DPINT);
    t += (c[7] as DPINT) * (b[1] as DPINT);
    t += (c[8] as DPINT) * (b[0] as DPINT);
    t += (v2 as DPINT) * (p6 as DPINT);
    t += (v3 as DPINT) * (p5 as DPINT);
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += (v7 as DPINT) * (p1 as DPINT);
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[9] as DPINT);
    t += (c[1] as DPINT) * (b[8] as DPINT);
    t += (c[2] as DPINT) * (b[7] as DPINT);
    t += (c[3] as DPINT) * (b[6] as DPINT);
    t += (c[4] as DPINT) * (b[5] as DPINT);
    t += (c[5] as DPINT) * (b[4] as DPINT);
    t += (c[6] as DPINT) * (b[3] as DPINT);
    t += (c[7] as DPINT) * (b[2] as DPINT);
    t += (c[8] as DPINT) * (b[1] as DPINT);
    t += (c[9] as DPINT) * (b[0] as DPINT);
    t += (v3 as DPINT) * (p6 as DPINT);
    t += (v4 as DPINT) * (p5 as DPINT);
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    let v9 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v9 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[10] as DPINT);
    t += (c[1] as DPINT) * (b[9] as DPINT);
    t += (c[2] as DPINT) * (b[8] as DPINT);
    t += (c[3] as DPINT) * (b[7] as DPINT);
    t += (c[4] as DPINT) * (b[6] as DPINT);
    t += (c[5] as DPINT) * (b[5] as DPINT);
    t += (c[6] as DPINT) * (b[4] as DPINT);
    t += (c[7] as DPINT) * (b[3] as DPINT);
    t += (c[8] as DPINT) * (b[2] as DPINT);
    t += (c[9] as DPINT) * (b[1] as DPINT);
    t += (c[10] as DPINT) * (b[0] as DPINT);
    t += (v4 as DPINT) * (p6 as DPINT);
    t += (v5 as DPINT) * (p5 as DPINT);
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    t += (v9 as DPINT) * (p1 as DPINT);
    let v10 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v10 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[11] as DPINT);
    t += (c[1] as DPINT) * (b[10] as DPINT);
    t += (c[2] as DPINT) * (b[9] as DPINT);
    t += (c[3] as DPINT) * (b[8] as DPINT);
    t += (c[4] as DPINT) * (b[7] as DPINT);
    t += (c[5] as DPINT) * (b[6] as DPINT);
    t += (c[6] as DPINT) * (b[5] as DPINT);
    t += (c[7] as DPINT) * (b[4] as DPINT);
    t += (c[8] as DPINT) * (b[3] as DPINT);
    t += (c[9] as DPINT) * (b[2] as DPINT);
    t += (c[10] as DPINT) * (b[1] as DPINT);
    t += (c[11] as DPINT) * (b[0] as DPINT);
    t += (v5 as DPINT) * (p6 as DPINT);
    t += (v6 as DPINT) * (p5 as DPINT);
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    t += (v9 as DPINT) * (p2 as DPINT);
    t += (v10 as DPINT) * (p1 as DPINT);
    let v11 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v11 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[12] as DPINT);
    t += (c[1] as DPINT) * (b[11] as DPINT);
    t += (c[2] as DPINT) * (b[10] as DPINT);
    t += (c[3] as DPINT) * (b[9] as DPINT);
    t += (c[4] as DPINT) * (b[8] as DPINT);
    t += (c[5] as DPINT) * (b[7] as DPINT);
    t += (c[6] as DPINT) * (b[6] as DPINT);
    t += (c[7] as DPINT) * (b[5] as DPINT);
    t += (c[8] as DPINT) * (b[4] as DPINT);
    t += (c[9] as DPINT) * (b[3] as DPINT);
    t += (c[10] as DPINT) * (b[2] as DPINT);
    t += (c[11] as DPINT) * (b[1] as DPINT);
    t += (c[12] as DPINT) * (b[0] as DPINT);
    t += (v6 as DPINT) * (p6 as DPINT);
    t += (v7 as DPINT) * (p5 as DPINT);
    t += (v8 as DPINT) * (p4 as DPINT);
    t += (v9 as DPINT) * (p3 as DPINT);
    t += (v10 as DPINT) * (p2 as DPINT);
    t += (v11 as DPINT) * (p1 as DPINT);
    let v12 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v12 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[13] as DPINT);
    t += (c[1] as DPINT) * (b[12] as DPINT);
    t += (c[2] as DPINT) * (b[11] as DPINT);
    t += (c[3] as DPINT) * (b[10] as DPINT);
    t += (c[4] as DPINT) * (b[9] as DPINT);
    t += (c[5] as DPINT) * (b[8] as DPINT);
    t += (c[6] as DPINT) * (b[7] as DPINT);
    t += (c[7] as DPINT) * (b[6] as DPINT);
    t += (c[8] as DPINT) * (b[5] as DPINT);
    t += (c[9] as DPINT) * (b[4] as DPINT);
    t += (c[10] as DPINT) * (b[3] as DPINT);
    t += (c[11] as DPINT) * (b[2] as DPINT);
    t += (c[12] as DPINT) * (b[1] as DPINT);
    t += (c[13] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p13 as DPINT);
    t += (v7 as DPINT) * (p6 as DPINT);
    t += (v8 as DPINT) * (p5 as DPINT);
    t += (v9 as DPINT) * (p4 as DPINT);
    t += (v10 as DPINT) * (p3 as DPINT);
    t += (v11 as DPINT) * (p2 as DPINT);
    t += (v12 as DPINT) * (p1 as DPINT);
    let v13 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v13 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[1] as DPINT) * (b[13] as DPINT);
    t += (c[2] as DPINT) * (b[12] as DPINT);
    t += (c[3] as DPINT) * (b[11] as DPINT);
    t += (c[4] as DPINT) * (b[10] as DPINT);
    t += (c[5] as DPINT) * (b[9] as DPINT);
    t += (c[6] as DPINT) * (b[8] as DPINT);
    t += (c[7] as DPINT) * (b[7] as DPINT);
    t += (c[8] as DPINT) * (b[6] as DPINT);
    t += (c[9] as DPINT) * (b[5] as DPINT);
    t += (c[10] as DPINT) * (b[4] as DPINT);
    t += (c[11] as DPINT) * (b[3] as DPINT);
    t += (c[12] as DPINT) * (b[2] as DPINT);
    t += (c[13] as DPINT) * (b[1] as DPINT);
    t += (v1 as DPINT) * (p13 as DPINT);
    t += (v8 as DPINT) * (p6 as DPINT);
    t += (v9 as DPINT) * (p5 as DPINT);
    t += (v10 as DPINT) * (p4 as DPINT);
    t += (v11 as DPINT) * (p3 as DPINT);
    t += (v12 as DPINT) * (p2 as DPINT);
    t += (v13 as DPINT) * (p1 as DPINT);
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[2] as DPINT) * (b[13] as DPINT);
    t += (c[3] as DPINT) * (b[12] as DPINT);
    t += (c[4] as DPINT) * (b[11] as DPINT);
    t += (c[5] as DPINT) * (b[10] as DPINT);
    t += (c[6] as DPINT) * (b[9] as DPINT);
    t += (c[7] as DPINT) * (b[8] as DPINT);
    t += (c[8] as DPINT) * (b[7] as DPINT);
    t += (c[9] as DPINT) * (b[6] as DPINT);
    t += (c[10] as DPINT) * (b[5] as DPINT);
    t += (c[11] as DPINT) * (b[4] as DPINT);
    t += (c[12] as DPINT) * (b[3] as DPINT);
    t += (c[13] as DPINT) * (b[2] as DPINT);
    t += (v2 as DPINT) * (p13 as DPINT);
    t += (v9 as DPINT) * (p6 as DPINT);
    t += (v10 as DPINT) * (p5 as DPINT);
    t += (v11 as DPINT) * (p4 as DPINT);
    t += (v12 as DPINT) * (p3 as DPINT);
    t += (v13 as DPINT) * (p2 as DPINT);
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[3] as DPINT) * (b[13] as DPINT);
    t += (c[4] as DPINT) * (b[12] as DPINT);
    t += (c[5] as DPINT) * (b[11] as DPINT);
    t += (c[6] as DPINT) * (b[10] as DPINT);
    t += (c[7] as DPINT) * (b[9] as DPINT);
    t += (c[8] as DPINT) * (b[8] as DPINT);
    t += (c[9] as DPINT) * (b[7] as DPINT);
    t += (c[10] as DPINT) * (b[6] as DPINT);
    t += (c[11] as DPINT) * (b[5] as DPINT);
    t += (c[12] as DPINT) * (b[4] as DPINT);
    t += (c[13] as DPINT) * (b[3] as DPINT);
    t += (v3 as DPINT) * (p13 as DPINT);
    t += (v10 as DPINT) * (p6 as DPINT);
    t += (v11 as DPINT) * (p5 as DPINT);
    t += (v12 as DPINT) * (p4 as DPINT);
    t += (v13 as DPINT) * (p3 as DPINT);
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[4] as DPINT) * (b[13] as DPINT);
    t += (c[5] as DPINT) * (b[12] as DPINT);
    t += (c[6] as DPINT) * (b[11] as DPINT);
    t += (c[7] as DPINT) * (b[10] as DPINT);
    t += (c[8] as DPINT) * (b[9] as DPINT);
    t += (c[9] as DPINT) * (b[8] as DPINT);
    t += (c[10] as DPINT) * (b[7] as DPINT);
    t += (c[11] as DPINT) * (b[6] as DPINT);
    t += (c[12] as DPINT) * (b[5] as DPINT);
    t += (c[13] as DPINT) * (b[4] as DPINT);
    t += (v4 as DPINT) * (p13 as DPINT);
    t += (v11 as DPINT) * (p6 as DPINT);
    t += (v12 as DPINT) * (p5 as DPINT);
    t += (v13 as DPINT) * (p4 as DPINT);
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[5] as DPINT) * (b[13] as DPINT);
    t += (c[6] as DPINT) * (b[12] as DPINT);
    t += (c[7] as DPINT) * (b[11] as DPINT);
    t += (c[8] as DPINT) * (b[10] as DPINT);
    t += (c[9] as DPINT) * (b[9] as DPINT);
    t += (c[10] as DPINT) * (b[8] as DPINT);
    t += (c[11] as DPINT) * (b[7] as DPINT);
    t += (c[12] as DPINT) * (b[6] as DPINT);
    t += (c[13] as DPINT) * (b[5] as DPINT);
    t += (v5 as DPINT) * (p13 as DPINT);
    t += (v12 as DPINT) * (p6 as DPINT);
    t += (v13 as DPINT) * (p5 as DPINT);
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[6] as DPINT) * (b[13] as DPINT);
    t += (c[7] as DPINT) * (b[12] as DPINT);
    t += (c[8] as DPINT) * (b[11] as DPINT);
    t += (c[9] as DPINT) * (b[10] as DPINT);
    t += (c[10] as DPINT) * (b[9] as DPINT);
    t += (c[11] as DPINT) * (b[8] as DPINT);
    t += (c[12] as DPINT) * (b[7] as DPINT);
    t += (c[13] as DPINT) * (b[6] as DPINT);
    t += (v6 as DPINT) * (p13 as DPINT);
    t += (v13 as DPINT) * (p6 as DPINT);
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[7] as DPINT) * (b[13] as DPINT);
    t += (c[8] as DPINT) * (b[12] as DPINT);
    t += (c[9] as DPINT) * (b[11] as DPINT);
    t += (c[10] as DPINT) * (b[10] as DPINT);
    t += (c[11] as DPINT) * (b[9] as DPINT);
    t += (c[12] as DPINT) * (b[8] as DPINT);
    t += (c[13] as DPINT) * (b[7] as DPINT);
    t += (v7 as DPINT) * (p13 as DPINT);
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[8] as DPINT) * (b[13] as DPINT);
    t += (c[9] as DPINT) * (b[12] as DPINT);
    t += (c[10] as DPINT) * (b[11] as DPINT);
    t += (c[11] as DPINT) * (b[10] as DPINT);
    t += (c[12] as DPINT) * (b[9] as DPINT);
    t += (c[13] as DPINT) * (b[8] as DPINT);
    t += (v8 as DPINT) * (p13 as DPINT);
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[9] as DPINT) * (b[13] as DPINT);
    t += (c[10] as DPINT) * (b[12] as DPINT);
    t += (c[11] as DPINT) * (b[11] as DPINT);
    t += (c[12] as DPINT) * (b[10] as DPINT);
    t += (c[13] as DPINT) * (b[9] as DPINT);
    t += (v9 as DPINT) * (p13 as DPINT);
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[10] as DPINT) * (b[13] as DPINT);
    t += (c[11] as DPINT) * (b[12] as DPINT);
    t += (c[12] as DPINT) * (b[11] as DPINT);
    t += (c[13] as DPINT) * (b[10] as DPINT);
    t += (v10 as DPINT) * (p13 as DPINT);
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[11] as DPINT) * (b[13] as DPINT);
    t += (c[12] as DPINT) * (b[12] as DPINT);
    t += (c[13] as DPINT) * (b[11] as DPINT);
    t += (v11 as DPINT) * (p13 as DPINT);
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[12] as DPINT) * (b[13] as DPINT);
    t += (c[13] as DPINT) * (b[12] as DPINT);
    t += (v12 as DPINT) * (p13 as DPINT);
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[13] as DPINT) * (b[13] as DPINT);
    t += (v13 as DPINT) * (p13 as DPINT);
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    c[13] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t: DPINT;
    let mut tot: DPINT;
    let mut s: SPINT;
    let p0 = 0xc6a7037 as SPINT;
    let p1 = 0x855d4f3 as SPINT;
    let p2 = 0xd04f09f as SPINT;
    let p3 = 0x98f29f0 as SPINT;
    let p4 = 0x9cd109a as SPINT;
    let p5 = 0xb351487 as SPINT;
    let p6 = 0x303a69 as SPINT;
    let p13 = 0x10400 as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x69fba79 as SPINT;
    tot = (c[0] as DPINT) * (c[0] as DPINT);
    t = tot;
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[1] as DPINT);
    tot *= 2;
    t += tot;
    t += (v0 as DPINT) * (p1 as DPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[2] as DPINT);
    tot *= 2;
    tot += (c[1] as DPINT) * (c[1] as DPINT);
    t += tot;
    t += (v0 as DPINT) * (p2 as DPINT);
    t += (v1 as DPINT) * (p1 as DPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[3] as DPINT);
    tot += (c[1] as DPINT) * (c[2] as DPINT);
    tot *= 2;
    t += tot;
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v1 as DPINT) * (p2 as DPINT);
    t += (v2 as DPINT) * (p1 as DPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as DPINT) * (p0 as DPINT);
    t >>= 28;
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
    t >>= 28;
    tot = (c[0] as DPINT) * (c[5] as DPINT);
    tot += (c[1] as DPINT) * (c[4] as DPINT);
    tot += (c[2] as DPINT) * (c[3] as DPINT);
    tot *= 2;
    t += tot;
    t += (v0 as DPINT) * (p5 as DPINT);
    t += (v1 as DPINT) * (p4 as DPINT);
    t += (v2 as DPINT) * (p3 as DPINT);
    t += (v3 as DPINT) * (p2 as DPINT);
    t += (v4 as DPINT) * (p1 as DPINT);
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[6] as DPINT);
    tot += (c[1] as DPINT) * (c[5] as DPINT);
    tot += (c[2] as DPINT) * (c[4] as DPINT);
    tot *= 2;
    tot += (c[3] as DPINT) * (c[3] as DPINT);
    t += tot;
    t += (v0 as DPINT) * (p6 as DPINT);
    t += (v1 as DPINT) * (p5 as DPINT);
    t += (v2 as DPINT) * (p4 as DPINT);
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[7] as DPINT);
    tot += (c[1] as DPINT) * (c[6] as DPINT);
    tot += (c[2] as DPINT) * (c[5] as DPINT);
    tot += (c[3] as DPINT) * (c[4] as DPINT);
    tot *= 2;
    t += tot;
    t += (v1 as DPINT) * (p6 as DPINT);
    t += (v2 as DPINT) * (p5 as DPINT);
    t += (v3 as DPINT) * (p4 as DPINT);
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[8] as DPINT);
    tot += (c[1] as DPINT) * (c[7] as DPINT);
    tot += (c[2] as DPINT) * (c[6] as DPINT);
    tot += (c[3] as DPINT) * (c[5] as DPINT);
    tot *= 2;
    tot += (c[4] as DPINT) * (c[4] as DPINT);
    t += tot;
    t += (v2 as DPINT) * (p6 as DPINT);
    t += (v3 as DPINT) * (p5 as DPINT);
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += (v7 as DPINT) * (p1 as DPINT);
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[9] as DPINT);
    tot += (c[1] as DPINT) * (c[8] as DPINT);
    tot += (c[2] as DPINT) * (c[7] as DPINT);
    tot += (c[3] as DPINT) * (c[6] as DPINT);
    tot += (c[4] as DPINT) * (c[5] as DPINT);
    tot *= 2;
    t += tot;
    t += (v3 as DPINT) * (p6 as DPINT);
    t += (v4 as DPINT) * (p5 as DPINT);
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    let v9 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v9 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[10] as DPINT);
    tot += (c[1] as DPINT) * (c[9] as DPINT);
    tot += (c[2] as DPINT) * (c[8] as DPINT);
    tot += (c[3] as DPINT) * (c[7] as DPINT);
    tot += (c[4] as DPINT) * (c[6] as DPINT);
    tot *= 2;
    tot += (c[5] as DPINT) * (c[5] as DPINT);
    t += tot;
    t += (v4 as DPINT) * (p6 as DPINT);
    t += (v5 as DPINT) * (p5 as DPINT);
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    t += (v9 as DPINT) * (p1 as DPINT);
    let v10 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v10 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[11] as DPINT);
    tot += (c[1] as DPINT) * (c[10] as DPINT);
    tot += (c[2] as DPINT) * (c[9] as DPINT);
    tot += (c[3] as DPINT) * (c[8] as DPINT);
    tot += (c[4] as DPINT) * (c[7] as DPINT);
    tot += (c[5] as DPINT) * (c[6] as DPINT);
    tot *= 2;
    t += tot;
    t += (v5 as DPINT) * (p6 as DPINT);
    t += (v6 as DPINT) * (p5 as DPINT);
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    t += (v9 as DPINT) * (p2 as DPINT);
    t += (v10 as DPINT) * (p1 as DPINT);
    let v11 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v11 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[12] as DPINT);
    tot += (c[1] as DPINT) * (c[11] as DPINT);
    tot += (c[2] as DPINT) * (c[10] as DPINT);
    tot += (c[3] as DPINT) * (c[9] as DPINT);
    tot += (c[4] as DPINT) * (c[8] as DPINT);
    tot += (c[5] as DPINT) * (c[7] as DPINT);
    tot *= 2;
    tot += (c[6] as DPINT) * (c[6] as DPINT);
    t += tot;
    t += (v6 as DPINT) * (p6 as DPINT);
    t += (v7 as DPINT) * (p5 as DPINT);
    t += (v8 as DPINT) * (p4 as DPINT);
    t += (v9 as DPINT) * (p3 as DPINT);
    t += (v10 as DPINT) * (p2 as DPINT);
    t += (v11 as DPINT) * (p1 as DPINT);
    let v12 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v12 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[0] as DPINT) * (c[13] as DPINT);
    tot += (c[1] as DPINT) * (c[12] as DPINT);
    tot += (c[2] as DPINT) * (c[11] as DPINT);
    tot += (c[3] as DPINT) * (c[10] as DPINT);
    tot += (c[4] as DPINT) * (c[9] as DPINT);
    tot += (c[5] as DPINT) * (c[8] as DPINT);
    tot += (c[6] as DPINT) * (c[7] as DPINT);
    tot *= 2;
    t += tot;
    t += (v0 as DPINT) * (p13 as DPINT);
    t += (v7 as DPINT) * (p6 as DPINT);
    t += (v8 as DPINT) * (p5 as DPINT);
    t += (v9 as DPINT) * (p4 as DPINT);
    t += (v10 as DPINT) * (p3 as DPINT);
    t += (v11 as DPINT) * (p2 as DPINT);
    t += (v12 as DPINT) * (p1 as DPINT);
    let v13 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v13 as DPINT) * (p0 as DPINT);
    t >>= 28;
    tot = (c[1] as DPINT) * (c[13] as DPINT);
    tot += (c[2] as DPINT) * (c[12] as DPINT);
    tot += (c[3] as DPINT) * (c[11] as DPINT);
    tot += (c[4] as DPINT) * (c[10] as DPINT);
    tot += (c[5] as DPINT) * (c[9] as DPINT);
    tot += (c[6] as DPINT) * (c[8] as DPINT);
    tot *= 2;
    tot += (c[7] as DPINT) * (c[7] as DPINT);
    t += tot;
    t += (v1 as DPINT) * (p13 as DPINT);
    t += (v8 as DPINT) * (p6 as DPINT);
    t += (v9 as DPINT) * (p5 as DPINT);
    t += (v10 as DPINT) * (p4 as DPINT);
    t += (v11 as DPINT) * (p3 as DPINT);
    t += (v12 as DPINT) * (p2 as DPINT);
    t += (v13 as DPINT) * (p1 as DPINT);
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[2] as DPINT) * (c[13] as DPINT);
    tot += (c[3] as DPINT) * (c[12] as DPINT);
    tot += (c[4] as DPINT) * (c[11] as DPINT);
    tot += (c[5] as DPINT) * (c[10] as DPINT);
    tot += (c[6] as DPINT) * (c[9] as DPINT);
    tot += (c[7] as DPINT) * (c[8] as DPINT);
    tot *= 2;
    t += tot;
    t += (v2 as DPINT) * (p13 as DPINT);
    t += (v9 as DPINT) * (p6 as DPINT);
    t += (v10 as DPINT) * (p5 as DPINT);
    t += (v11 as DPINT) * (p4 as DPINT);
    t += (v12 as DPINT) * (p3 as DPINT);
    t += (v13 as DPINT) * (p2 as DPINT);
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[3] as DPINT) * (c[13] as DPINT);
    tot += (c[4] as DPINT) * (c[12] as DPINT);
    tot += (c[5] as DPINT) * (c[11] as DPINT);
    tot += (c[6] as DPINT) * (c[10] as DPINT);
    tot += (c[7] as DPINT) * (c[9] as DPINT);
    tot *= 2;
    tot += (c[8] as DPINT) * (c[8] as DPINT);
    t += tot;
    t += (v3 as DPINT) * (p13 as DPINT);
    t += (v10 as DPINT) * (p6 as DPINT);
    t += (v11 as DPINT) * (p5 as DPINT);
    t += (v12 as DPINT) * (p4 as DPINT);
    t += (v13 as DPINT) * (p3 as DPINT);
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[4] as DPINT) * (c[13] as DPINT);
    tot += (c[5] as DPINT) * (c[12] as DPINT);
    tot += (c[6] as DPINT) * (c[11] as DPINT);
    tot += (c[7] as DPINT) * (c[10] as DPINT);
    tot += (c[8] as DPINT) * (c[9] as DPINT);
    tot *= 2;
    t += tot;
    t += (v4 as DPINT) * (p13 as DPINT);
    t += (v11 as DPINT) * (p6 as DPINT);
    t += (v12 as DPINT) * (p5 as DPINT);
    t += (v13 as DPINT) * (p4 as DPINT);
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[5] as DPINT) * (c[13] as DPINT);
    tot += (c[6] as DPINT) * (c[12] as DPINT);
    tot += (c[7] as DPINT) * (c[11] as DPINT);
    tot += (c[8] as DPINT) * (c[10] as DPINT);
    tot *= 2;
    tot += (c[9] as DPINT) * (c[9] as DPINT);
    t += tot;
    t += (v5 as DPINT) * (p13 as DPINT);
    t += (v12 as DPINT) * (p6 as DPINT);
    t += (v13 as DPINT) * (p5 as DPINT);
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[6] as DPINT) * (c[13] as DPINT);
    tot += (c[7] as DPINT) * (c[12] as DPINT);
    tot += (c[8] as DPINT) * (c[11] as DPINT);
    tot += (c[9] as DPINT) * (c[10] as DPINT);
    tot *= 2;
    t += tot;
    t += (v6 as DPINT) * (p13 as DPINT);
    t += (v13 as DPINT) * (p6 as DPINT);
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[7] as DPINT) * (c[13] as DPINT);
    tot += (c[8] as DPINT) * (c[12] as DPINT);
    tot += (c[9] as DPINT) * (c[11] as DPINT);
    tot *= 2;
    tot += (c[10] as DPINT) * (c[10] as DPINT);
    t += tot;
    t += (v7 as DPINT) * (p13 as DPINT);
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[8] as DPINT) * (c[13] as DPINT);
    tot += (c[9] as DPINT) * (c[12] as DPINT);
    tot += (c[10] as DPINT) * (c[11] as DPINT);
    tot *= 2;
    t += tot;
    t += (v8 as DPINT) * (p13 as DPINT);
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[9] as DPINT) * (c[13] as DPINT);
    tot += (c[10] as DPINT) * (c[12] as DPINT);
    tot *= 2;
    tot += (c[11] as DPINT) * (c[11] as DPINT);
    t += tot;
    t += (v9 as DPINT) * (p13 as DPINT);
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[10] as DPINT) * (c[13] as DPINT);
    tot += (c[11] as DPINT) * (c[12] as DPINT);
    tot *= 2;
    t += tot;
    t += (v10 as DPINT) * (p13 as DPINT);
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[11] as DPINT) * (c[13] as DPINT);
    tot *= 2;
    tot += (c[12] as DPINT) * (c[12] as DPINT);
    t += tot;
    t += (v11 as DPINT) * (p13 as DPINT);
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[12] as DPINT) * (c[13] as DPINT);
    tot *= 2;
    t += tot;
    t += (v12 as DPINT) * (p13 as DPINT);
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[13] as DPINT) * (c[13] as DPINT);
    t += tot;
    t += (v13 as DPINT) * (p13 as DPINT);
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    c[13] = t as SPINT;
    return;
}

//copy
fn modcpy(a: &[SPINT], c: &mut [SPINT]) {
    for i in 0..14 {
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
    let mut x: [SPINT; 14] = [0; 14];
    let mut z: [SPINT; 14] = [0; 14];
    let mut t0: [SPINT; 14] = [0; 14];
    let mut t1: [SPINT; 14] = [0; 14];
    let mut t2: [SPINT; 14] = [0; 14];
    let mut t3: [SPINT; 14] = [0; 14];
    let mut t4: [SPINT; 14] = [0; 14];
    let mut t5: [SPINT; 14] = [0; 14];
    let mut t6: [SPINT; 14] = [0; 14];
    let mut t7: [SPINT; 14] = [0; 14];
    let mut t8: [SPINT; 14] = [0; 14];
    let mut t9: [SPINT; 14] = [0; 14];
    let mut t10: [SPINT; 14] = [0; 14];
    let mut t11: [SPINT; 14] = [0; 14];
    let mut t12: [SPINT; 14] = [0; 14];
    let mut t13: [SPINT; 14] = [0; 14];
    let mut t14: [SPINT; 14] = [0; 14];
    modcpy(w, &mut x);
    modcpy(&x, &mut t1);
    modsqr(&mut t1);
    modcpy(&x, &mut t0);
    modmul(&t1, &mut t0);
    modcpy(&t1, &mut t11);
    modmul(&t0, &mut t11);
    modcpy(&x, &mut t6);
    modmul(&t11, &mut t6);
    modcpy(&t1, &mut t14);
    modmul(&t6, &mut t14);
    modcpy(&t11, &mut z);
    modmul(&t14, &mut z);
    modcpy(&t1, &mut t12);
    modmul(&z, &mut t12);
    modcpy(&t6, &mut t5);
    modmul(&t12, &mut t5);
    modcpy(&t6, &mut t13);
    modmul(&t5, &mut t13);
    modcpy(&t1, &mut t4);
    modmul(&t13, &mut t4);
    modcpy(&t0, &mut t7);
    modmul(&t4, &mut t7);
    modcpy(&x, &mut t8);
    modmul(&t7, &mut t8);
    modcpy(&t1, &mut t2);
    modmul(&t8, &mut t2);
    modcpy(&t6, &mut t3);
    modmul(&t8, &mut t3);
    modmul(&t3, &mut t1);
    modcpy(&t14, &mut t9);
    modmul(&t1, &mut t9);
    modcpy(&t5, &mut t10);
    modmul(&t7, &mut t10);
    modcpy(&t14, &mut t7);
    modmul(&t9, &mut t7);
    modmul(&t7, &mut t14);
    modnsqr(&mut t14, 186);
    modmul(&t0, &mut t14);
    modnsqr(&mut t14, 11);
    modmul(&t4, &mut t14);
    modnsqr(&mut t14, 6);
    modmul(&z, &mut t14);
    modnsqr(&mut t14, 7);
    modmul(&t14, &mut t13);
    modnsqr(&mut t13, 8);
    modmul(&t10, &mut t13);
    modnsqr(&mut t13, 9);
    modmul(&t1, &mut t13);
    modnsqr(&mut t13, 8);
    modmul(&t13, &mut t12);
    modnsqr(&mut t12, 8);
    modmul(&t7, &mut t12);
    modnsqr(&mut t12, 3);
    modmul(&t12, &mut t11);
    modnsqr(&mut t11, 9);
    modmul(&t8, &mut t11);
    modnsqr(&mut t11, 8);
    modmul(&t11, &mut t10);
    modnsqr(&mut t10, 8);
    modmul(&t10, &mut t9);
    modnsqr(&mut t9, 6);
    modmul(&t7, &mut t9);
    modnsqr(&mut t9, 7);
    modmul(&t3, &mut t9);
    modnsqr(&mut t9, 2);
    modmul(&t0, &mut t9);
    modnsqr(&mut t9, 8);
    modmul(&z, &mut t9);
    modnsqr(&mut t9, 11);
    modmul(&t3, &mut t9);
    modnsqr(&mut t9, 6);
    modmul(&t9, &mut t8);
    modnsqr(&mut t8, 8);
    modmul(&t8, &mut t7);
    modmul(&t7, &mut t6);
    modnsqr(&mut t6, 9);
    modmul(&t6, &mut t5);
    modnsqr(&mut t5, 6);
    modmul(&t5, &mut t4);
    modnsqr(&mut t4, 7);
    modmul(&t3, &mut t4);
    modnsqr(&mut t4, 6);
    modmul(&t4, &mut t3);
    modnsqr(&mut t3, 6);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 7);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 2);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 10);
    modmul(&t0, &mut z);
    modcpy(&z, r);
    return;
}

//calculate inverse, provide progenitor h if available
fn modinv(h: Option<&[SPINT]>, z: &mut [SPINT]) {
    let mut s: [SPINT; 14] = [0; 14];
    let mut t: [SPINT; 14] = [0; 14];
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
    let c: [SPINT; 14] = [
        0xdbcd4ba, 0x776d5c8, 0x6c906a8, 0x15d6ae9, 0xea69e0e, 0xfa0c01a, 0x704847d, 0x4255610,
        0x2eeefd8, 0xee6c03c, 0x7e6d239, 0x10979e6, 0x8db637e, 0x1476,
    ];
    modmul(&c, n);
    return;
}

//Convert m back to normal form, m=redc(n)
fn redc(m: &mut [SPINT]) {
    let mut c: [SPINT; 14] = [0; 14];
    c[0] = 1;
    modmul(&c, m);
    modfsb(m);
    return;
}

//is unity?
fn modis1(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 14] = [0; 14];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 1..14 {
        d |= c[i] as SSPINT;
    }
    let c0 = c[0] as SSPINT;
    return (1 & ((d - 1) >> 28) & (((c0 ^ 1) - 1) >> 28)) != 0;
}

//is zero?
fn modis0(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 14] = [0; 14];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 0..14 {
        d |= c[i] as SSPINT;
    }
    return (1 & ((d - 1) >> 28)) != 0;
}

//set to zero
fn modzer(a: &mut [SPINT]) {
    for i in 0..14 {
        a[i] = 0;
    }
    return;
}

//set to one
fn modone(a: &mut [SPINT]) {
    a[0] = 1;
    for i in 1..14 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//set to integer
fn modint(x: usize, a: &mut [SPINT]) {
    a[0] = x as SPINT;
    for i in 1..14 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//Test for quadratic residue
fn modqr(h: Option<&[SPINT]>, x: &[SPINT]) -> bool {
    let mut r: [SPINT; 14] = [0; 14];
    if let Some(hint) = h {
        modcpy(&hint, &mut r);
    } else {
        modpro(&x, &mut r);
    }
    modsqr(&mut r);
    modmul(&x, &mut r);
    return modis1(&r) || modis0(x);
}

//conditional move g to f if d=1
#[inline(never)]
fn modcmv(b: usize, g: &[SPINT], f: &mut [SPINT]) {
    static mut R: SPINT = 0;
    let w: SPINT;
    unsafe {
        R += 0x5aa5a55a;
        w = R;
    }
    let bb = b as SPINT;
    let c0 = (!bb) & (w + 1);
    let c1 = bb + w;
    for i in 0..14 {
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
        R += 0x5aa5a55a;
        w = R;
    }
    let c0 = (!bb) & (w + 1);
    let c1 = bb + w;
    for i in 0..14 {
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
    let mut y: [SPINT; 14] = [0; 14];
    let mut s: [SPINT; 14] = [0; 14];
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
    a[14 - 1] = (a[14 - 1] << n) + (a[14 - 2] >> (28 - n));
    for i in (1..14 - 1).rev() {
        a[i] = ((a[i] << n) & 0xfffffff) + (a[i - 1] >> (28 - n));
    }
    a[0] = (a[0] << n) & 0xfffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..14 - 1 {
        a[i] = (a[i] >> n) + ((a[i + 1] << (28 - n)) & 0xfffffff);
    }
    a[14 - 1] = a[14 - 1] >> n;
    return r as isize;
}

//divide by 2. Shift right 1 bit (or add p and shift right one bit)
fn modhaf(n: &mut [SPINT]) {
    let mut t: [SPINT; 14] = [0; 14];
    prop(n);
    modcpy(n, &mut t);
    let lsb = modshr(1, &mut t) as usize;
    n[0] += 0xc6a7037 as SPINT;
    n[1] += 0x855d4f3 as SPINT;
    n[2] += 0xd04f09f as SPINT;
    n[3] += 0x98f29f0 as SPINT;
    n[4] += 0x9cd109a as SPINT;
    n[5] += 0xb351487 as SPINT;
    n[6] += 0x303a69 as SPINT;
    n[13] += 0x10400 as SPINT;
    prop(n);
    modshr(1, n);
    modcmv(1 - lsb, &t, n);
}

//set a= 2^r
fn mod2r(r: usize, a: &mut [SPINT]) {
    let n = r / 28;
    let m = r % 28;
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
    let mut c: [SPINT; 14] = [0; 14];
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
    for i in 0..14 {
        a[i] = 0;
    }
    for i in 0..48 {
        modshl(8, a);
        a[0] += b[i] as SPINT;
    }
    let res = modfsb(a);
    nres(a);
    return res == 1;
}

//determine sign
fn modsign(a: &[SPINT]) -> usize {
    let mut c: [SPINT; 14] = [0; 14];
    modcpy(a, &mut c);
    redc(&mut c);
    return (c[0] % 2) as usize;
}

//return true if equal
fn modcmp(a: &[SPINT], b: &[SPINT]) -> bool {
    let mut c: [SPINT; 14] = [0; 14];
    let mut d: [SPINT; 14] = [0; 14];
    let mut eq = 1;
    modcpy(a, &mut c);
    redc(&mut c);
    modcpy(b, &mut d);
    redc(&mut d);
    for i in 0..14 {
        eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 14;
const RADIX: usize = 28;
const NBITS: usize = 381;
const NBYTES: usize = 48;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;


/*** End of automatically generated code ***/

const LIMBS:usize= NLIMBS;
type GEL = [SPINT; LIMBS];
pub const BYTES:usize = NBYTES;

// reduce 768 bit array h to integer r modulo group order q, in constant time
// Consider h as 2^376.(2^376x + y) + z, where x,y and z < q (z is bottom 376 bits, y is next 376 bits, x is top 16 bits)
// Important that x and y < q, q is group order
fn reduce(h:&[u8],r:&mut [SPINT]) {
    let mut buff:[u8;BYTES]=[0;BYTES];    
    let mut x:GEL=[0;LIMBS];
    let mut y:GEL=[0;LIMBS];
    let mut z:GEL=[0;LIMBS];
    let mut c:GEL=[0;LIMBS];

    mod2r(8*(BYTES-1),&mut c);

    for i in 0..47 {  // bottom 376 bits
        buff[i]=h[i];
    }
    buff[47]=0;
    buff.reverse();
    modimp(&buff,&mut z);

    for i in 0..47 { // middle 376 bits
        buff[i]=h[i+47];
    }
    buff[47]=0;
    buff.reverse();
    modimp(&buff,&mut y);

    for i in 0..2 {  // top 16 bits
        buff[i]=h[94+i];
    }
    for i in 2..48 {
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
    s[47]&=0x7F; s[47]|=0x40;

    s.reverse(); // little endian to big endian
 
    ecnmul(&s,&mut P); 

    let sign=ecnget(&mut P,None,Some(&mut s));  // get y coordinate and sign
    s.reverse(); // big endian to little endian
    for i in 0..48 {
        public[i]=s[i];
    }
             
    public[47]|=(sign<<7) as u8;

}

const dom4:[u8;10]=[b'S',b'i',b'g',b'E',b'd',b'3',b'8',b'3',0,0];


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
    sh[47]&=0x7F; sh[47]|=0x40;
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
    sign|=((public[BYTES-1]>>7)&1) as usize;
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

