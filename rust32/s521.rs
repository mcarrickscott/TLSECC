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
type SPINT = u32;
type SSPINT = i32;
type UDPINT = u64;
type DPINT = u64;
//propagate carries
#[allow(unused_variables)]
#[inline]
fn prop(n: &mut [SPINT]) -> SPINT {
    let mask = ((1 as SPINT) << 28) - 1;
    let mut carry = (n[0] as SSPINT) >> 28;
    n[0] &= mask;
    for i in 1..18 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 28;
    }
    n[18] += carry as SPINT;
    return ((n[18] as SSPINT) >> 31) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 28;
    let carry = prop(n);
    n[0] += (0x1386409 as SPINT) & (carry as SPINT);
    n[1] += (0x6fb71e9 as SPINT) & (carry as SPINT);
    n[2] += (0xc47aebb as SPINT) & (carry as SPINT);
    n[3] += (0xc9b8899 as SPINT) & (carry as SPINT);
    n[4] += (0x5d03bb5 as SPINT) & (carry as SPINT);
    n[5] += (0x48f709a as SPINT) & (carry as SPINT);
    n[6] += (0xb7fcc01 as SPINT) & (carry as SPINT);
    n[7] += (0xbf2f966 as SPINT) & (carry as SPINT);
    n[8] += (0x1868783 as SPINT) & (carry as SPINT);
    n[9] += (0xfffffa5 as SPINT) & (carry as SPINT);
    n[10] -= 1 & (carry as SPINT);
    n[18] += (0x20000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 28;
    n[0] -= 0x1386409 as SPINT;
    n[1] -= 0x6fb71e9 as SPINT;
    n[2] -= 0xc47aebb as SPINT;
    n[3] -= 0xc9b8899 as SPINT;
    n[4] -= 0x5d03bb5 as SPINT;
    n[5] -= 0x48f709a as SPINT;
    n[6] -= 0xb7fcc01 as SPINT;
    n[7] -= 0xbf2f966 as SPINT;
    n[8] -= 0x1868783 as SPINT;
    n[9] -= 0xfffffa5 as SPINT;
    n[10] += 1;
    n[18] -= 0x20000 as SPINT;
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
    n[14] = n[14] + b[14];
    n[15] = n[15] + b[15];
    n[16] = n[16] + b[16];
    n[17] = n[17] + b[17];
    n[18] = n[18] + b[18];
    n[0] -= 0x270c812 as SPINT;
    n[1] -= 0xdf6e3d2 as SPINT;
    n[2] -= 0x188f5d76 as SPINT;
    n[3] -= 0x19371132 as SPINT;
    n[4] -= 0xba0776a as SPINT;
    n[5] -= 0x91ee134 as SPINT;
    n[6] -= 0x16ff9802 as SPINT;
    n[7] -= 0x17e5f2cc as SPINT;
    n[8] -= 0x30d0f06 as SPINT;
    n[9] -= 0x1fffff4a as SPINT;
    n[10] += 2;
    n[18] -= 0x40000 as SPINT;
    let carry = prop(n);
    n[0] += (0x270c812 as SPINT) & (carry as SPINT);
    n[1] += (0xdf6e3d2 as SPINT) & (carry as SPINT);
    n[2] += (0x188f5d76 as SPINT) & (carry as SPINT);
    n[3] += (0x19371132 as SPINT) & (carry as SPINT);
    n[4] += (0xba0776a as SPINT) & (carry as SPINT);
    n[5] += (0x91ee134 as SPINT) & (carry as SPINT);
    n[6] += (0x16ff9802 as SPINT) & (carry as SPINT);
    n[7] += (0x17e5f2cc as SPINT) & (carry as SPINT);
    n[8] += (0x30d0f06 as SPINT) & (carry as SPINT);
    n[9] += (0x1fffff4a as SPINT) & (carry as SPINT);
    n[10] -= 2 & (carry as SPINT);
    n[18] += (0x40000 as SPINT) & (carry as SPINT);
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
    n[14] = n[14] - b[14];
    n[15] = n[15] - b[15];
    n[16] = n[16] - b[16];
    n[17] = n[17] - b[17];
    n[18] = n[18] - b[18];
    let carry = prop(n);
    n[0] += (0x270c812 as SPINT) & (carry as SPINT);
    n[1] += (0xdf6e3d2 as SPINT) & (carry as SPINT);
    n[2] += (0x188f5d76 as SPINT) & (carry as SPINT);
    n[3] += (0x19371132 as SPINT) & (carry as SPINT);
    n[4] += (0xba0776a as SPINT) & (carry as SPINT);
    n[5] += (0x91ee134 as SPINT) & (carry as SPINT);
    n[6] += (0x16ff9802 as SPINT) & (carry as SPINT);
    n[7] += (0x17e5f2cc as SPINT) & (carry as SPINT);
    n[8] += (0x30d0f06 as SPINT) & (carry as SPINT);
    n[9] += (0x1fffff4a as SPINT) & (carry as SPINT);
    n[10] -= 2 & (carry as SPINT);
    n[18] += (0x40000 as SPINT) & (carry as SPINT);
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
    n[14] = (0 as SPINT) - n[14];
    n[15] = (0 as SPINT) - n[15];
    n[16] = (0 as SPINT) - n[16];
    n[17] = (0 as SPINT) - n[17];
    n[18] = (0 as SPINT) - n[18];
    let carry = prop(n);
    n[0] += (0x270c812 as SPINT) & (carry as SPINT);
    n[1] += (0xdf6e3d2 as SPINT) & (carry as SPINT);
    n[2] += (0x188f5d76 as SPINT) & (carry as SPINT);
    n[3] += (0x19371132 as SPINT) & (carry as SPINT);
    n[4] += (0xba0776a as SPINT) & (carry as SPINT);
    n[5] += (0x91ee134 as SPINT) & (carry as SPINT);
    n[6] += (0x16ff9802 as SPINT) & (carry as SPINT);
    n[7] += (0x17e5f2cc as SPINT) & (carry as SPINT);
    n[8] += (0x30d0f06 as SPINT) & (carry as SPINT);
    n[9] += (0x1fffff4a as SPINT) & (carry as SPINT);
    n[10] -= 2 & (carry as SPINT);
    n[18] += (0x40000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
fn modmli(_b: usize, _c: &mut [SPINT]) {
    return;
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1749402643979958766
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0x1386409 as SPINT;
    let p1 = 0x6fb71e9 as SPINT;
    let p2 = 0xc47aebb as SPINT;
    let p3 = 0xc9b8899 as SPINT;
    let p4 = 0x5d03bb5 as SPINT;
    let p5 = 0x48f709a as SPINT;
    let p6 = 0xb7fcc01 as SPINT;
    let p7 = 0xbf2f966 as SPINT;
    let p8 = 0x1868783 as SPINT;
    let p9 = 0xfffffa5 as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x9a995c7 as SPINT;
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
    t += (v0 as DPINT) * (p7 as DPINT);
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
    t += (v0 as DPINT) * (p8 as DPINT);
    t += (v1 as DPINT) * (p7 as DPINT);
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
    t += (v0 as DPINT) * (p9 as DPINT);
    t += (v1 as DPINT) * (p8 as DPINT);
    t += (v2 as DPINT) * (p7 as DPINT);
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
    t += (q - v0) as DPINT;
    t += (v1 as DPINT) * (p9 as DPINT);
    t += (v2 as DPINT) * (p8 as DPINT);
    t += (v3 as DPINT) * (p7 as DPINT);
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
    s = mask as SPINT;
    s -= v1;
    t += (v2 as DPINT) * (p9 as DPINT);
    t += (v3 as DPINT) * (p8 as DPINT);
    t += (v4 as DPINT) * (p7 as DPINT);
    t += (v5 as DPINT) * (p6 as DPINT);
    t += (v6 as DPINT) * (p5 as DPINT);
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    t += (v9 as DPINT) * (p2 as DPINT);
    t += (v10 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
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
    s = mask as SPINT;
    s -= v2;
    t += (v3 as DPINT) * (p9 as DPINT);
    t += (v4 as DPINT) * (p8 as DPINT);
    t += (v5 as DPINT) * (p7 as DPINT);
    t += (v6 as DPINT) * (p6 as DPINT);
    t += (v7 as DPINT) * (p5 as DPINT);
    t += (v8 as DPINT) * (p4 as DPINT);
    t += (v9 as DPINT) * (p3 as DPINT);
    t += (v10 as DPINT) * (p2 as DPINT);
    t += (v11 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
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
    s = mask as SPINT;
    s -= v3;
    t += (v4 as DPINT) * (p9 as DPINT);
    t += (v5 as DPINT) * (p8 as DPINT);
    t += (v6 as DPINT) * (p7 as DPINT);
    t += (v7 as DPINT) * (p6 as DPINT);
    t += (v8 as DPINT) * (p5 as DPINT);
    t += (v9 as DPINT) * (p4 as DPINT);
    t += (v10 as DPINT) * (p3 as DPINT);
    t += (v11 as DPINT) * (p2 as DPINT);
    t += (v12 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v13 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v13 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[14] as DPINT);
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
    t += (c[14] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    s -= v4;
    t += (v5 as DPINT) * (p9 as DPINT);
    t += (v6 as DPINT) * (p8 as DPINT);
    t += (v7 as DPINT) * (p7 as DPINT);
    t += (v8 as DPINT) * (p6 as DPINT);
    t += (v9 as DPINT) * (p5 as DPINT);
    t += (v10 as DPINT) * (p4 as DPINT);
    t += (v11 as DPINT) * (p3 as DPINT);
    t += (v12 as DPINT) * (p2 as DPINT);
    t += (v13 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v14 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v14 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[15] as DPINT);
    t += (c[1] as DPINT) * (b[14] as DPINT);
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
    t += (c[14] as DPINT) * (b[1] as DPINT);
    t += (c[15] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    s -= v5;
    t += (v6 as DPINT) * (p9 as DPINT);
    t += (v7 as DPINT) * (p8 as DPINT);
    t += (v8 as DPINT) * (p7 as DPINT);
    t += (v9 as DPINT) * (p6 as DPINT);
    t += (v10 as DPINT) * (p5 as DPINT);
    t += (v11 as DPINT) * (p4 as DPINT);
    t += (v12 as DPINT) * (p3 as DPINT);
    t += (v13 as DPINT) * (p2 as DPINT);
    t += (v14 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v15 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v15 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[16] as DPINT);
    t += (c[1] as DPINT) * (b[15] as DPINT);
    t += (c[2] as DPINT) * (b[14] as DPINT);
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
    t += (c[14] as DPINT) * (b[2] as DPINT);
    t += (c[15] as DPINT) * (b[1] as DPINT);
    t += (c[16] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    s -= v6;
    t += (v7 as DPINT) * (p9 as DPINT);
    t += (v8 as DPINT) * (p8 as DPINT);
    t += (v9 as DPINT) * (p7 as DPINT);
    t += (v10 as DPINT) * (p6 as DPINT);
    t += (v11 as DPINT) * (p5 as DPINT);
    t += (v12 as DPINT) * (p4 as DPINT);
    t += (v13 as DPINT) * (p3 as DPINT);
    t += (v14 as DPINT) * (p2 as DPINT);
    t += (v15 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v16 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v16 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[17] as DPINT);
    t += (c[1] as DPINT) * (b[16] as DPINT);
    t += (c[2] as DPINT) * (b[15] as DPINT);
    t += (c[3] as DPINT) * (b[14] as DPINT);
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
    t += (c[14] as DPINT) * (b[3] as DPINT);
    t += (c[15] as DPINT) * (b[2] as DPINT);
    t += (c[16] as DPINT) * (b[1] as DPINT);
    t += (c[17] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    s -= v7;
    t += (v8 as DPINT) * (p9 as DPINT);
    t += (v9 as DPINT) * (p8 as DPINT);
    t += (v10 as DPINT) * (p7 as DPINT);
    t += (v11 as DPINT) * (p6 as DPINT);
    t += (v12 as DPINT) * (p5 as DPINT);
    t += (v13 as DPINT) * (p4 as DPINT);
    t += (v14 as DPINT) * (p3 as DPINT);
    t += (v15 as DPINT) * (p2 as DPINT);
    t += (v16 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v17 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v17 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[0] as DPINT) * (b[18] as DPINT);
    t += (c[1] as DPINT) * (b[17] as DPINT);
    t += (c[2] as DPINT) * (b[16] as DPINT);
    t += (c[3] as DPINT) * (b[15] as DPINT);
    t += (c[4] as DPINT) * (b[14] as DPINT);
    t += (c[5] as DPINT) * (b[13] as DPINT);
    t += (c[6] as DPINT) * (b[12] as DPINT);
    t += (c[7] as DPINT) * (b[11] as DPINT);
    t += (c[8] as DPINT) * (b[10] as DPINT);
    t += (c[9] as DPINT) * (b[9] as DPINT);
    t += (c[10] as DPINT) * (b[8] as DPINT);
    t += (c[11] as DPINT) * (b[7] as DPINT);
    t += (c[12] as DPINT) * (b[6] as DPINT);
    t += (c[13] as DPINT) * (b[5] as DPINT);
    t += (c[14] as DPINT) * (b[4] as DPINT);
    t += (c[15] as DPINT) * (b[3] as DPINT);
    t += (c[16] as DPINT) * (b[2] as DPINT);
    t += (c[17] as DPINT) * (b[1] as DPINT);
    t += (c[18] as DPINT) * (b[0] as DPINT);
    s = mask as SPINT;
    t += (v0 as DPINT) << 17;
    s -= v8;
    t += (v9 as DPINT) * (p9 as DPINT);
    t += (v10 as DPINT) * (p8 as DPINT);
    t += (v11 as DPINT) * (p7 as DPINT);
    t += (v12 as DPINT) * (p6 as DPINT);
    t += (v13 as DPINT) * (p5 as DPINT);
    t += (v14 as DPINT) * (p4 as DPINT);
    t += (v15 as DPINT) * (p3 as DPINT);
    t += (v16 as DPINT) * (p2 as DPINT);
    t += (v17 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    let v18 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v18 as DPINT) * (p0 as DPINT);
    t >>= 28;
    t += (c[1] as DPINT) * (b[18] as DPINT);
    t += (c[2] as DPINT) * (b[17] as DPINT);
    t += (c[3] as DPINT) * (b[16] as DPINT);
    t += (c[4] as DPINT) * (b[15] as DPINT);
    t += (c[5] as DPINT) * (b[14] as DPINT);
    t += (c[6] as DPINT) * (b[13] as DPINT);
    t += (c[7] as DPINT) * (b[12] as DPINT);
    t += (c[8] as DPINT) * (b[11] as DPINT);
    t += (c[9] as DPINT) * (b[10] as DPINT);
    t += (c[10] as DPINT) * (b[9] as DPINT);
    t += (c[11] as DPINT) * (b[8] as DPINT);
    t += (c[12] as DPINT) * (b[7] as DPINT);
    t += (c[13] as DPINT) * (b[6] as DPINT);
    t += (c[14] as DPINT) * (b[5] as DPINT);
    t += (c[15] as DPINT) * (b[4] as DPINT);
    t += (c[16] as DPINT) * (b[3] as DPINT);
    t += (c[17] as DPINT) * (b[2] as DPINT);
    t += (c[18] as DPINT) * (b[1] as DPINT);
    s = mask as SPINT;
    t += (v1 as DPINT) << 17;
    s -= v9;
    t += (v10 as DPINT) * (p9 as DPINT);
    t += (v11 as DPINT) * (p8 as DPINT);
    t += (v12 as DPINT) * (p7 as DPINT);
    t += (v13 as DPINT) * (p6 as DPINT);
    t += (v14 as DPINT) * (p5 as DPINT);
    t += (v15 as DPINT) * (p4 as DPINT);
    t += (v16 as DPINT) * (p3 as DPINT);
    t += (v17 as DPINT) * (p2 as DPINT);
    t += (v18 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[2] as DPINT) * (b[18] as DPINT);
    t += (c[3] as DPINT) * (b[17] as DPINT);
    t += (c[4] as DPINT) * (b[16] as DPINT);
    t += (c[5] as DPINT) * (b[15] as DPINT);
    t += (c[6] as DPINT) * (b[14] as DPINT);
    t += (c[7] as DPINT) * (b[13] as DPINT);
    t += (c[8] as DPINT) * (b[12] as DPINT);
    t += (c[9] as DPINT) * (b[11] as DPINT);
    t += (c[10] as DPINT) * (b[10] as DPINT);
    t += (c[11] as DPINT) * (b[9] as DPINT);
    t += (c[12] as DPINT) * (b[8] as DPINT);
    t += (c[13] as DPINT) * (b[7] as DPINT);
    t += (c[14] as DPINT) * (b[6] as DPINT);
    t += (c[15] as DPINT) * (b[5] as DPINT);
    t += (c[16] as DPINT) * (b[4] as DPINT);
    t += (c[17] as DPINT) * (b[3] as DPINT);
    t += (c[18] as DPINT) * (b[2] as DPINT);
    s = mask as SPINT;
    t += (v2 as DPINT) << 17;
    s -= v10;
    t += (v11 as DPINT) * (p9 as DPINT);
    t += (v12 as DPINT) * (p8 as DPINT);
    t += (v13 as DPINT) * (p7 as DPINT);
    t += (v14 as DPINT) * (p6 as DPINT);
    t += (v15 as DPINT) * (p5 as DPINT);
    t += (v16 as DPINT) * (p4 as DPINT);
    t += (v17 as DPINT) * (p3 as DPINT);
    t += (v18 as DPINT) * (p2 as DPINT);
    t += s as DPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[3] as DPINT) * (b[18] as DPINT);
    t += (c[4] as DPINT) * (b[17] as DPINT);
    t += (c[5] as DPINT) * (b[16] as DPINT);
    t += (c[6] as DPINT) * (b[15] as DPINT);
    t += (c[7] as DPINT) * (b[14] as DPINT);
    t += (c[8] as DPINT) * (b[13] as DPINT);
    t += (c[9] as DPINT) * (b[12] as DPINT);
    t += (c[10] as DPINT) * (b[11] as DPINT);
    t += (c[11] as DPINT) * (b[10] as DPINT);
    t += (c[12] as DPINT) * (b[9] as DPINT);
    t += (c[13] as DPINT) * (b[8] as DPINT);
    t += (c[14] as DPINT) * (b[7] as DPINT);
    t += (c[15] as DPINT) * (b[6] as DPINT);
    t += (c[16] as DPINT) * (b[5] as DPINT);
    t += (c[17] as DPINT) * (b[4] as DPINT);
    t += (c[18] as DPINT) * (b[3] as DPINT);
    s = mask as SPINT;
    t += (v3 as DPINT) << 17;
    s -= v11;
    t += (v12 as DPINT) * (p9 as DPINT);
    t += (v13 as DPINT) * (p8 as DPINT);
    t += (v14 as DPINT) * (p7 as DPINT);
    t += (v15 as DPINT) * (p6 as DPINT);
    t += (v16 as DPINT) * (p5 as DPINT);
    t += (v17 as DPINT) * (p4 as DPINT);
    t += (v18 as DPINT) * (p3 as DPINT);
    t += s as DPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[4] as DPINT) * (b[18] as DPINT);
    t += (c[5] as DPINT) * (b[17] as DPINT);
    t += (c[6] as DPINT) * (b[16] as DPINT);
    t += (c[7] as DPINT) * (b[15] as DPINT);
    t += (c[8] as DPINT) * (b[14] as DPINT);
    t += (c[9] as DPINT) * (b[13] as DPINT);
    t += (c[10] as DPINT) * (b[12] as DPINT);
    t += (c[11] as DPINT) * (b[11] as DPINT);
    t += (c[12] as DPINT) * (b[10] as DPINT);
    t += (c[13] as DPINT) * (b[9] as DPINT);
    t += (c[14] as DPINT) * (b[8] as DPINT);
    t += (c[15] as DPINT) * (b[7] as DPINT);
    t += (c[16] as DPINT) * (b[6] as DPINT);
    t += (c[17] as DPINT) * (b[5] as DPINT);
    t += (c[18] as DPINT) * (b[4] as DPINT);
    s = mask as SPINT;
    t += (v4 as DPINT) << 17;
    s -= v12;
    t += (v13 as DPINT) * (p9 as DPINT);
    t += (v14 as DPINT) * (p8 as DPINT);
    t += (v15 as DPINT) * (p7 as DPINT);
    t += (v16 as DPINT) * (p6 as DPINT);
    t += (v17 as DPINT) * (p5 as DPINT);
    t += (v18 as DPINT) * (p4 as DPINT);
    t += s as DPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[5] as DPINT) * (b[18] as DPINT);
    t += (c[6] as DPINT) * (b[17] as DPINT);
    t += (c[7] as DPINT) * (b[16] as DPINT);
    t += (c[8] as DPINT) * (b[15] as DPINT);
    t += (c[9] as DPINT) * (b[14] as DPINT);
    t += (c[10] as DPINT) * (b[13] as DPINT);
    t += (c[11] as DPINT) * (b[12] as DPINT);
    t += (c[12] as DPINT) * (b[11] as DPINT);
    t += (c[13] as DPINT) * (b[10] as DPINT);
    t += (c[14] as DPINT) * (b[9] as DPINT);
    t += (c[15] as DPINT) * (b[8] as DPINT);
    t += (c[16] as DPINT) * (b[7] as DPINT);
    t += (c[17] as DPINT) * (b[6] as DPINT);
    t += (c[18] as DPINT) * (b[5] as DPINT);
    s = mask as SPINT;
    t += (v5 as DPINT) << 17;
    s -= v13;
    t += (v14 as DPINT) * (p9 as DPINT);
    t += (v15 as DPINT) * (p8 as DPINT);
    t += (v16 as DPINT) * (p7 as DPINT);
    t += (v17 as DPINT) * (p6 as DPINT);
    t += (v18 as DPINT) * (p5 as DPINT);
    t += s as DPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[6] as DPINT) * (b[18] as DPINT);
    t += (c[7] as DPINT) * (b[17] as DPINT);
    t += (c[8] as DPINT) * (b[16] as DPINT);
    t += (c[9] as DPINT) * (b[15] as DPINT);
    t += (c[10] as DPINT) * (b[14] as DPINT);
    t += (c[11] as DPINT) * (b[13] as DPINT);
    t += (c[12] as DPINT) * (b[12] as DPINT);
    t += (c[13] as DPINT) * (b[11] as DPINT);
    t += (c[14] as DPINT) * (b[10] as DPINT);
    t += (c[15] as DPINT) * (b[9] as DPINT);
    t += (c[16] as DPINT) * (b[8] as DPINT);
    t += (c[17] as DPINT) * (b[7] as DPINT);
    t += (c[18] as DPINT) * (b[6] as DPINT);
    s = mask as SPINT;
    t += (v6 as DPINT) << 17;
    s -= v14;
    t += (v15 as DPINT) * (p9 as DPINT);
    t += (v16 as DPINT) * (p8 as DPINT);
    t += (v17 as DPINT) * (p7 as DPINT);
    t += (v18 as DPINT) * (p6 as DPINT);
    t += s as DPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[7] as DPINT) * (b[18] as DPINT);
    t += (c[8] as DPINT) * (b[17] as DPINT);
    t += (c[9] as DPINT) * (b[16] as DPINT);
    t += (c[10] as DPINT) * (b[15] as DPINT);
    t += (c[11] as DPINT) * (b[14] as DPINT);
    t += (c[12] as DPINT) * (b[13] as DPINT);
    t += (c[13] as DPINT) * (b[12] as DPINT);
    t += (c[14] as DPINT) * (b[11] as DPINT);
    t += (c[15] as DPINT) * (b[10] as DPINT);
    t += (c[16] as DPINT) * (b[9] as DPINT);
    t += (c[17] as DPINT) * (b[8] as DPINT);
    t += (c[18] as DPINT) * (b[7] as DPINT);
    s = mask as SPINT;
    t += (v7 as DPINT) << 17;
    s -= v15;
    t += (v16 as DPINT) * (p9 as DPINT);
    t += (v17 as DPINT) * (p8 as DPINT);
    t += (v18 as DPINT) * (p7 as DPINT);
    t += s as DPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[8] as DPINT) * (b[18] as DPINT);
    t += (c[9] as DPINT) * (b[17] as DPINT);
    t += (c[10] as DPINT) * (b[16] as DPINT);
    t += (c[11] as DPINT) * (b[15] as DPINT);
    t += (c[12] as DPINT) * (b[14] as DPINT);
    t += (c[13] as DPINT) * (b[13] as DPINT);
    t += (c[14] as DPINT) * (b[12] as DPINT);
    t += (c[15] as DPINT) * (b[11] as DPINT);
    t += (c[16] as DPINT) * (b[10] as DPINT);
    t += (c[17] as DPINT) * (b[9] as DPINT);
    t += (c[18] as DPINT) * (b[8] as DPINT);
    s = mask as SPINT;
    t += (v8 as DPINT) << 17;
    s -= v16;
    t += (v17 as DPINT) * (p9 as DPINT);
    t += (v18 as DPINT) * (p8 as DPINT);
    t += s as DPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[9] as DPINT) * (b[18] as DPINT);
    t += (c[10] as DPINT) * (b[17] as DPINT);
    t += (c[11] as DPINT) * (b[16] as DPINT);
    t += (c[12] as DPINT) * (b[15] as DPINT);
    t += (c[13] as DPINT) * (b[14] as DPINT);
    t += (c[14] as DPINT) * (b[13] as DPINT);
    t += (c[15] as DPINT) * (b[12] as DPINT);
    t += (c[16] as DPINT) * (b[11] as DPINT);
    t += (c[17] as DPINT) * (b[10] as DPINT);
    t += (c[18] as DPINT) * (b[9] as DPINT);
    s = mask as SPINT;
    t += (v9 as DPINT) << 17;
    s -= v17;
    t += (v18 as DPINT) * (p9 as DPINT);
    t += s as DPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[10] as DPINT) * (b[18] as DPINT);
    t += (c[11] as DPINT) * (b[17] as DPINT);
    t += (c[12] as DPINT) * (b[16] as DPINT);
    t += (c[13] as DPINT) * (b[15] as DPINT);
    t += (c[14] as DPINT) * (b[14] as DPINT);
    t += (c[15] as DPINT) * (b[13] as DPINT);
    t += (c[16] as DPINT) * (b[12] as DPINT);
    t += (c[17] as DPINT) * (b[11] as DPINT);
    t += (c[18] as DPINT) * (b[10] as DPINT);
    s = mask as SPINT;
    t += (v10 as DPINT) << 17;
    s -= v18;
    t += s as DPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[11] as DPINT) * (b[18] as DPINT);
    t += (c[12] as DPINT) * (b[17] as DPINT);
    t += (c[13] as DPINT) * (b[16] as DPINT);
    t += (c[14] as DPINT) * (b[15] as DPINT);
    t += (c[15] as DPINT) * (b[14] as DPINT);
    t += (c[16] as DPINT) * (b[13] as DPINT);
    t += (c[17] as DPINT) * (b[12] as DPINT);
    t += (c[18] as DPINT) * (b[11] as DPINT);
    s = mask as SPINT;
    t += (v11 as DPINT) << 17;
    t += s as DPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[12] as DPINT) * (b[18] as DPINT);
    t += (c[13] as DPINT) * (b[17] as DPINT);
    t += (c[14] as DPINT) * (b[16] as DPINT);
    t += (c[15] as DPINT) * (b[15] as DPINT);
    t += (c[16] as DPINT) * (b[14] as DPINT);
    t += (c[17] as DPINT) * (b[13] as DPINT);
    t += (c[18] as DPINT) * (b[12] as DPINT);
    s = mask as SPINT;
    t += (v12 as DPINT) << 17;
    t += s as DPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[13] as DPINT) * (b[18] as DPINT);
    t += (c[14] as DPINT) * (b[17] as DPINT);
    t += (c[15] as DPINT) * (b[16] as DPINT);
    t += (c[16] as DPINT) * (b[15] as DPINT);
    t += (c[17] as DPINT) * (b[14] as DPINT);
    t += (c[18] as DPINT) * (b[13] as DPINT);
    s = mask as SPINT;
    t += (v13 as DPINT) << 17;
    t += s as DPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[14] as DPINT) * (b[18] as DPINT);
    t += (c[15] as DPINT) * (b[17] as DPINT);
    t += (c[16] as DPINT) * (b[16] as DPINT);
    t += (c[17] as DPINT) * (b[15] as DPINT);
    t += (c[18] as DPINT) * (b[14] as DPINT);
    s = mask as SPINT;
    t += (v14 as DPINT) << 17;
    t += s as DPINT;
    c[13] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[15] as DPINT) * (b[18] as DPINT);
    t += (c[16] as DPINT) * (b[17] as DPINT);
    t += (c[17] as DPINT) * (b[16] as DPINT);
    t += (c[18] as DPINT) * (b[15] as DPINT);
    s = mask as SPINT;
    t += (v15 as DPINT) << 17;
    t += s as DPINT;
    c[14] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[16] as DPINT) * (b[18] as DPINT);
    t += (c[17] as DPINT) * (b[17] as DPINT);
    t += (c[18] as DPINT) * (b[16] as DPINT);
    s = mask as SPINT;
    t += (v16 as DPINT) << 17;
    t += s as DPINT;
    c[15] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[17] as DPINT) * (b[18] as DPINT);
    t += (c[18] as DPINT) * (b[17] as DPINT);
    s = mask as SPINT;
    t += (v17 as DPINT) << 17;
    t += s as DPINT;
    c[16] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[18] as DPINT) * (b[18] as DPINT);
    s = mask as SPINT;
    t += (v18 as DPINT) << 17;
    t += s as DPINT;
    c[17] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t -= 1;
    c[18] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let p0 = 0x1386409 as SPINT;
    let p1 = 0x6fb71e9 as SPINT;
    let p2 = 0xc47aebb as SPINT;
    let p3 = 0xc9b8899 as SPINT;
    let p4 = 0x5d03bb5 as SPINT;
    let p5 = 0x48f709a as SPINT;
    let p6 = 0xb7fcc01 as SPINT;
    let p7 = 0xbf2f966 as SPINT;
    let p8 = 0x1868783 as SPINT;
    let p9 = 0xfffffa5 as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x9a995c7 as SPINT;
    tot = (c[0] as UDPINT) * (c[0] as UDPINT);
    t = tot;
    let v0 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v0 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[1] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p1 as UDPINT);
    let v1 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v1 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    tot += (c[1] as UDPINT) * (c[1] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p2 as UDPINT);
    t += (v1 as UDPINT) * (p1 as UDPINT);
    let v2 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v2 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[3] as UDPINT);
    tot += (c[1] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p3 as UDPINT);
    t += (v1 as UDPINT) * (p2 as UDPINT);
    t += (v2 as UDPINT) * (p1 as UDPINT);
    let v3 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v3 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
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
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[5] as UDPINT);
    tot += (c[1] as UDPINT) * (c[4] as UDPINT);
    tot += (c[2] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p5 as UDPINT);
    t += (v1 as UDPINT) * (p4 as UDPINT);
    t += (v2 as UDPINT) * (p3 as UDPINT);
    t += (v3 as UDPINT) * (p2 as UDPINT);
    t += (v4 as UDPINT) * (p1 as UDPINT);
    let v5 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v5 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[6] as UDPINT);
    tot += (c[1] as UDPINT) * (c[5] as UDPINT);
    tot += (c[2] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    tot += (c[3] as UDPINT) * (c[3] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p6 as UDPINT);
    t += (v1 as UDPINT) * (p5 as UDPINT);
    t += (v2 as UDPINT) * (p4 as UDPINT);
    t += (v3 as UDPINT) * (p3 as UDPINT);
    t += (v4 as UDPINT) * (p2 as UDPINT);
    t += (v5 as UDPINT) * (p1 as UDPINT);
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[7] as UDPINT);
    tot += (c[1] as UDPINT) * (c[6] as UDPINT);
    tot += (c[2] as UDPINT) * (c[5] as UDPINT);
    tot += (c[3] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p7 as UDPINT);
    t += (v1 as UDPINT) * (p6 as UDPINT);
    t += (v2 as UDPINT) * (p5 as UDPINT);
    t += (v3 as UDPINT) * (p4 as UDPINT);
    t += (v4 as UDPINT) * (p3 as UDPINT);
    t += (v5 as UDPINT) * (p2 as UDPINT);
    t += (v6 as UDPINT) * (p1 as UDPINT);
    let v7 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v7 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[8] as UDPINT);
    tot += (c[1] as UDPINT) * (c[7] as UDPINT);
    tot += (c[2] as UDPINT) * (c[6] as UDPINT);
    tot += (c[3] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    tot += (c[4] as UDPINT) * (c[4] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p8 as UDPINT);
    t += (v1 as UDPINT) * (p7 as UDPINT);
    t += (v2 as UDPINT) * (p6 as UDPINT);
    t += (v3 as UDPINT) * (p5 as UDPINT);
    t += (v4 as UDPINT) * (p4 as UDPINT);
    t += (v5 as UDPINT) * (p3 as UDPINT);
    t += (v6 as UDPINT) * (p2 as UDPINT);
    t += (v7 as UDPINT) * (p1 as UDPINT);
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[9] as UDPINT);
    tot += (c[1] as UDPINT) * (c[8] as UDPINT);
    tot += (c[2] as UDPINT) * (c[7] as UDPINT);
    tot += (c[3] as UDPINT) * (c[6] as UDPINT);
    tot += (c[4] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p9 as UDPINT);
    t += (v1 as UDPINT) * (p8 as UDPINT);
    t += (v2 as UDPINT) * (p7 as UDPINT);
    t += (v3 as UDPINT) * (p6 as UDPINT);
    t += (v4 as UDPINT) * (p5 as UDPINT);
    t += (v5 as UDPINT) * (p4 as UDPINT);
    t += (v6 as UDPINT) * (p3 as UDPINT);
    t += (v7 as UDPINT) * (p2 as UDPINT);
    t += (v8 as UDPINT) * (p1 as UDPINT);
    let v9 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v9 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[10] as UDPINT);
    tot += (c[1] as UDPINT) * (c[9] as UDPINT);
    tot += (c[2] as UDPINT) * (c[8] as UDPINT);
    tot += (c[3] as UDPINT) * (c[7] as UDPINT);
    tot += (c[4] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    tot += (c[5] as UDPINT) * (c[5] as UDPINT);
    t += tot;
    t += (q - v0) as UDPINT;
    t += (v1 as UDPINT) * (p9 as UDPINT);
    t += (v2 as UDPINT) * (p8 as UDPINT);
    t += (v3 as UDPINT) * (p7 as UDPINT);
    t += (v4 as UDPINT) * (p6 as UDPINT);
    t += (v5 as UDPINT) * (p5 as UDPINT);
    t += (v6 as UDPINT) * (p4 as UDPINT);
    t += (v7 as UDPINT) * (p3 as UDPINT);
    t += (v8 as UDPINT) * (p2 as UDPINT);
    t += (v9 as UDPINT) * (p1 as UDPINT);
    let v10 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v10 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[11] as UDPINT);
    tot += (c[1] as UDPINT) * (c[10] as UDPINT);
    tot += (c[2] as UDPINT) * (c[9] as UDPINT);
    tot += (c[3] as UDPINT) * (c[8] as UDPINT);
    tot += (c[4] as UDPINT) * (c[7] as UDPINT);
    tot += (c[5] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s -= v1;
    t += (v2 as UDPINT) * (p9 as UDPINT);
    t += (v3 as UDPINT) * (p8 as UDPINT);
    t += (v4 as UDPINT) * (p7 as UDPINT);
    t += (v5 as UDPINT) * (p6 as UDPINT);
    t += (v6 as UDPINT) * (p5 as UDPINT);
    t += (v7 as UDPINT) * (p4 as UDPINT);
    t += (v8 as UDPINT) * (p3 as UDPINT);
    t += (v9 as UDPINT) * (p2 as UDPINT);
    t += (v10 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v11 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v11 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[12] as UDPINT);
    tot += (c[1] as UDPINT) * (c[11] as UDPINT);
    tot += (c[2] as UDPINT) * (c[10] as UDPINT);
    tot += (c[3] as UDPINT) * (c[9] as UDPINT);
    tot += (c[4] as UDPINT) * (c[8] as UDPINT);
    tot += (c[5] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    tot += (c[6] as UDPINT) * (c[6] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s -= v2;
    t += (v3 as UDPINT) * (p9 as UDPINT);
    t += (v4 as UDPINT) * (p8 as UDPINT);
    t += (v5 as UDPINT) * (p7 as UDPINT);
    t += (v6 as UDPINT) * (p6 as UDPINT);
    t += (v7 as UDPINT) * (p5 as UDPINT);
    t += (v8 as UDPINT) * (p4 as UDPINT);
    t += (v9 as UDPINT) * (p3 as UDPINT);
    t += (v10 as UDPINT) * (p2 as UDPINT);
    t += (v11 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v12 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v12 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[13] as UDPINT);
    tot += (c[1] as UDPINT) * (c[12] as UDPINT);
    tot += (c[2] as UDPINT) * (c[11] as UDPINT);
    tot += (c[3] as UDPINT) * (c[10] as UDPINT);
    tot += (c[4] as UDPINT) * (c[9] as UDPINT);
    tot += (c[5] as UDPINT) * (c[8] as UDPINT);
    tot += (c[6] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s -= v3;
    t += (v4 as UDPINT) * (p9 as UDPINT);
    t += (v5 as UDPINT) * (p8 as UDPINT);
    t += (v6 as UDPINT) * (p7 as UDPINT);
    t += (v7 as UDPINT) * (p6 as UDPINT);
    t += (v8 as UDPINT) * (p5 as UDPINT);
    t += (v9 as UDPINT) * (p4 as UDPINT);
    t += (v10 as UDPINT) * (p3 as UDPINT);
    t += (v11 as UDPINT) * (p2 as UDPINT);
    t += (v12 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v13 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v13 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[14] as UDPINT);
    tot += (c[1] as UDPINT) * (c[13] as UDPINT);
    tot += (c[2] as UDPINT) * (c[12] as UDPINT);
    tot += (c[3] as UDPINT) * (c[11] as UDPINT);
    tot += (c[4] as UDPINT) * (c[10] as UDPINT);
    tot += (c[5] as UDPINT) * (c[9] as UDPINT);
    tot += (c[6] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    tot += (c[7] as UDPINT) * (c[7] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s -= v4;
    t += (v5 as UDPINT) * (p9 as UDPINT);
    t += (v6 as UDPINT) * (p8 as UDPINT);
    t += (v7 as UDPINT) * (p7 as UDPINT);
    t += (v8 as UDPINT) * (p6 as UDPINT);
    t += (v9 as UDPINT) * (p5 as UDPINT);
    t += (v10 as UDPINT) * (p4 as UDPINT);
    t += (v11 as UDPINT) * (p3 as UDPINT);
    t += (v12 as UDPINT) * (p2 as UDPINT);
    t += (v13 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v14 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v14 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[15] as UDPINT);
    tot += (c[1] as UDPINT) * (c[14] as UDPINT);
    tot += (c[2] as UDPINT) * (c[13] as UDPINT);
    tot += (c[3] as UDPINT) * (c[12] as UDPINT);
    tot += (c[4] as UDPINT) * (c[11] as UDPINT);
    tot += (c[5] as UDPINT) * (c[10] as UDPINT);
    tot += (c[6] as UDPINT) * (c[9] as UDPINT);
    tot += (c[7] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s -= v5;
    t += (v6 as UDPINT) * (p9 as UDPINT);
    t += (v7 as UDPINT) * (p8 as UDPINT);
    t += (v8 as UDPINT) * (p7 as UDPINT);
    t += (v9 as UDPINT) * (p6 as UDPINT);
    t += (v10 as UDPINT) * (p5 as UDPINT);
    t += (v11 as UDPINT) * (p4 as UDPINT);
    t += (v12 as UDPINT) * (p3 as UDPINT);
    t += (v13 as UDPINT) * (p2 as UDPINT);
    t += (v14 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v15 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v15 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[16] as UDPINT);
    tot += (c[1] as UDPINT) * (c[15] as UDPINT);
    tot += (c[2] as UDPINT) * (c[14] as UDPINT);
    tot += (c[3] as UDPINT) * (c[13] as UDPINT);
    tot += (c[4] as UDPINT) * (c[12] as UDPINT);
    tot += (c[5] as UDPINT) * (c[11] as UDPINT);
    tot += (c[6] as UDPINT) * (c[10] as UDPINT);
    tot += (c[7] as UDPINT) * (c[9] as UDPINT);
    tot *= 2;
    tot += (c[8] as UDPINT) * (c[8] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s -= v6;
    t += (v7 as UDPINT) * (p9 as UDPINT);
    t += (v8 as UDPINT) * (p8 as UDPINT);
    t += (v9 as UDPINT) * (p7 as UDPINT);
    t += (v10 as UDPINT) * (p6 as UDPINT);
    t += (v11 as UDPINT) * (p5 as UDPINT);
    t += (v12 as UDPINT) * (p4 as UDPINT);
    t += (v13 as UDPINT) * (p3 as UDPINT);
    t += (v14 as UDPINT) * (p2 as UDPINT);
    t += (v15 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v16 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v16 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[17] as UDPINT);
    tot += (c[1] as UDPINT) * (c[16] as UDPINT);
    tot += (c[2] as UDPINT) * (c[15] as UDPINT);
    tot += (c[3] as UDPINT) * (c[14] as UDPINT);
    tot += (c[4] as UDPINT) * (c[13] as UDPINT);
    tot += (c[5] as UDPINT) * (c[12] as UDPINT);
    tot += (c[6] as UDPINT) * (c[11] as UDPINT);
    tot += (c[7] as UDPINT) * (c[10] as UDPINT);
    tot += (c[8] as UDPINT) * (c[9] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s -= v7;
    t += (v8 as UDPINT) * (p9 as UDPINT);
    t += (v9 as UDPINT) * (p8 as UDPINT);
    t += (v10 as UDPINT) * (p7 as UDPINT);
    t += (v11 as UDPINT) * (p6 as UDPINT);
    t += (v12 as UDPINT) * (p5 as UDPINT);
    t += (v13 as UDPINT) * (p4 as UDPINT);
    t += (v14 as UDPINT) * (p3 as UDPINT);
    t += (v15 as UDPINT) * (p2 as UDPINT);
    t += (v16 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v17 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v17 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[18] as UDPINT);
    tot += (c[1] as UDPINT) * (c[17] as UDPINT);
    tot += (c[2] as UDPINT) * (c[16] as UDPINT);
    tot += (c[3] as UDPINT) * (c[15] as UDPINT);
    tot += (c[4] as UDPINT) * (c[14] as UDPINT);
    tot += (c[5] as UDPINT) * (c[13] as UDPINT);
    tot += (c[6] as UDPINT) * (c[12] as UDPINT);
    tot += (c[7] as UDPINT) * (c[11] as UDPINT);
    tot += (c[8] as UDPINT) * (c[10] as UDPINT);
    tot *= 2;
    tot += (c[9] as UDPINT) * (c[9] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v0 as UDPINT) << 17;
    s -= v8;
    t += (v9 as UDPINT) * (p9 as UDPINT);
    t += (v10 as UDPINT) * (p8 as UDPINT);
    t += (v11 as UDPINT) * (p7 as UDPINT);
    t += (v12 as UDPINT) * (p6 as UDPINT);
    t += (v13 as UDPINT) * (p5 as UDPINT);
    t += (v14 as UDPINT) * (p4 as UDPINT);
    t += (v15 as UDPINT) * (p3 as UDPINT);
    t += (v16 as UDPINT) * (p2 as UDPINT);
    t += (v17 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    let v18 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v18 as UDPINT) * (p0 as UDPINT);
    t >>= 28;
    tot = (c[1] as UDPINT) * (c[18] as UDPINT);
    tot += (c[2] as UDPINT) * (c[17] as UDPINT);
    tot += (c[3] as UDPINT) * (c[16] as UDPINT);
    tot += (c[4] as UDPINT) * (c[15] as UDPINT);
    tot += (c[5] as UDPINT) * (c[14] as UDPINT);
    tot += (c[6] as UDPINT) * (c[13] as UDPINT);
    tot += (c[7] as UDPINT) * (c[12] as UDPINT);
    tot += (c[8] as UDPINT) * (c[11] as UDPINT);
    tot += (c[9] as UDPINT) * (c[10] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v1 as UDPINT) << 17;
    s -= v9;
    t += (v10 as UDPINT) * (p9 as UDPINT);
    t += (v11 as UDPINT) * (p8 as UDPINT);
    t += (v12 as UDPINT) * (p7 as UDPINT);
    t += (v13 as UDPINT) * (p6 as UDPINT);
    t += (v14 as UDPINT) * (p5 as UDPINT);
    t += (v15 as UDPINT) * (p4 as UDPINT);
    t += (v16 as UDPINT) * (p3 as UDPINT);
    t += (v17 as UDPINT) * (p2 as UDPINT);
    t += (v18 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[2] as UDPINT) * (c[18] as UDPINT);
    tot += (c[3] as UDPINT) * (c[17] as UDPINT);
    tot += (c[4] as UDPINT) * (c[16] as UDPINT);
    tot += (c[5] as UDPINT) * (c[15] as UDPINT);
    tot += (c[6] as UDPINT) * (c[14] as UDPINT);
    tot += (c[7] as UDPINT) * (c[13] as UDPINT);
    tot += (c[8] as UDPINT) * (c[12] as UDPINT);
    tot += (c[9] as UDPINT) * (c[11] as UDPINT);
    tot *= 2;
    tot += (c[10] as UDPINT) * (c[10] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v2 as UDPINT) << 17;
    s -= v10;
    t += (v11 as UDPINT) * (p9 as UDPINT);
    t += (v12 as UDPINT) * (p8 as UDPINT);
    t += (v13 as UDPINT) * (p7 as UDPINT);
    t += (v14 as UDPINT) * (p6 as UDPINT);
    t += (v15 as UDPINT) * (p5 as UDPINT);
    t += (v16 as UDPINT) * (p4 as UDPINT);
    t += (v17 as UDPINT) * (p3 as UDPINT);
    t += (v18 as UDPINT) * (p2 as UDPINT);
    t += s as UDPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[3] as UDPINT) * (c[18] as UDPINT);
    tot += (c[4] as UDPINT) * (c[17] as UDPINT);
    tot += (c[5] as UDPINT) * (c[16] as UDPINT);
    tot += (c[6] as UDPINT) * (c[15] as UDPINT);
    tot += (c[7] as UDPINT) * (c[14] as UDPINT);
    tot += (c[8] as UDPINT) * (c[13] as UDPINT);
    tot += (c[9] as UDPINT) * (c[12] as UDPINT);
    tot += (c[10] as UDPINT) * (c[11] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v3 as UDPINT) << 17;
    s -= v11;
    t += (v12 as UDPINT) * (p9 as UDPINT);
    t += (v13 as UDPINT) * (p8 as UDPINT);
    t += (v14 as UDPINT) * (p7 as UDPINT);
    t += (v15 as UDPINT) * (p6 as UDPINT);
    t += (v16 as UDPINT) * (p5 as UDPINT);
    t += (v17 as UDPINT) * (p4 as UDPINT);
    t += (v18 as UDPINT) * (p3 as UDPINT);
    t += s as UDPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[4] as UDPINT) * (c[18] as UDPINT);
    tot += (c[5] as UDPINT) * (c[17] as UDPINT);
    tot += (c[6] as UDPINT) * (c[16] as UDPINT);
    tot += (c[7] as UDPINT) * (c[15] as UDPINT);
    tot += (c[8] as UDPINT) * (c[14] as UDPINT);
    tot += (c[9] as UDPINT) * (c[13] as UDPINT);
    tot += (c[10] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    tot += (c[11] as UDPINT) * (c[11] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v4 as UDPINT) << 17;
    s -= v12;
    t += (v13 as UDPINT) * (p9 as UDPINT);
    t += (v14 as UDPINT) * (p8 as UDPINT);
    t += (v15 as UDPINT) * (p7 as UDPINT);
    t += (v16 as UDPINT) * (p6 as UDPINT);
    t += (v17 as UDPINT) * (p5 as UDPINT);
    t += (v18 as UDPINT) * (p4 as UDPINT);
    t += s as UDPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[5] as UDPINT) * (c[18] as UDPINT);
    tot += (c[6] as UDPINT) * (c[17] as UDPINT);
    tot += (c[7] as UDPINT) * (c[16] as UDPINT);
    tot += (c[8] as UDPINT) * (c[15] as UDPINT);
    tot += (c[9] as UDPINT) * (c[14] as UDPINT);
    tot += (c[10] as UDPINT) * (c[13] as UDPINT);
    tot += (c[11] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v5 as UDPINT) << 17;
    s -= v13;
    t += (v14 as UDPINT) * (p9 as UDPINT);
    t += (v15 as UDPINT) * (p8 as UDPINT);
    t += (v16 as UDPINT) * (p7 as UDPINT);
    t += (v17 as UDPINT) * (p6 as UDPINT);
    t += (v18 as UDPINT) * (p5 as UDPINT);
    t += s as UDPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[6] as UDPINT) * (c[18] as UDPINT);
    tot += (c[7] as UDPINT) * (c[17] as UDPINT);
    tot += (c[8] as UDPINT) * (c[16] as UDPINT);
    tot += (c[9] as UDPINT) * (c[15] as UDPINT);
    tot += (c[10] as UDPINT) * (c[14] as UDPINT);
    tot += (c[11] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    tot += (c[12] as UDPINT) * (c[12] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v6 as UDPINT) << 17;
    s -= v14;
    t += (v15 as UDPINT) * (p9 as UDPINT);
    t += (v16 as UDPINT) * (p8 as UDPINT);
    t += (v17 as UDPINT) * (p7 as UDPINT);
    t += (v18 as UDPINT) * (p6 as UDPINT);
    t += s as UDPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[7] as UDPINT) * (c[18] as UDPINT);
    tot += (c[8] as UDPINT) * (c[17] as UDPINT);
    tot += (c[9] as UDPINT) * (c[16] as UDPINT);
    tot += (c[10] as UDPINT) * (c[15] as UDPINT);
    tot += (c[11] as UDPINT) * (c[14] as UDPINT);
    tot += (c[12] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v7 as UDPINT) << 17;
    s -= v15;
    t += (v16 as UDPINT) * (p9 as UDPINT);
    t += (v17 as UDPINT) * (p8 as UDPINT);
    t += (v18 as UDPINT) * (p7 as UDPINT);
    t += s as UDPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[8] as UDPINT) * (c[18] as UDPINT);
    tot += (c[9] as UDPINT) * (c[17] as UDPINT);
    tot += (c[10] as UDPINT) * (c[16] as UDPINT);
    tot += (c[11] as UDPINT) * (c[15] as UDPINT);
    tot += (c[12] as UDPINT) * (c[14] as UDPINT);
    tot *= 2;
    tot += (c[13] as UDPINT) * (c[13] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v8 as UDPINT) << 17;
    s -= v16;
    t += (v17 as UDPINT) * (p9 as UDPINT);
    t += (v18 as UDPINT) * (p8 as UDPINT);
    t += s as UDPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[9] as UDPINT) * (c[18] as UDPINT);
    tot += (c[10] as UDPINT) * (c[17] as UDPINT);
    tot += (c[11] as UDPINT) * (c[16] as UDPINT);
    tot += (c[12] as UDPINT) * (c[15] as UDPINT);
    tot += (c[13] as UDPINT) * (c[14] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v9 as UDPINT) << 17;
    s -= v17;
    t += (v18 as UDPINT) * (p9 as UDPINT);
    t += s as UDPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[10] as UDPINT) * (c[18] as UDPINT);
    tot += (c[11] as UDPINT) * (c[17] as UDPINT);
    tot += (c[12] as UDPINT) * (c[16] as UDPINT);
    tot += (c[13] as UDPINT) * (c[15] as UDPINT);
    tot *= 2;
    tot += (c[14] as UDPINT) * (c[14] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v10 as UDPINT) << 17;
    s -= v18;
    t += s as UDPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[11] as UDPINT) * (c[18] as UDPINT);
    tot += (c[12] as UDPINT) * (c[17] as UDPINT);
    tot += (c[13] as UDPINT) * (c[16] as UDPINT);
    tot += (c[14] as UDPINT) * (c[15] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v11 as UDPINT) << 17;
    t += s as UDPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[12] as UDPINT) * (c[18] as UDPINT);
    tot += (c[13] as UDPINT) * (c[17] as UDPINT);
    tot += (c[14] as UDPINT) * (c[16] as UDPINT);
    tot *= 2;
    tot += (c[15] as UDPINT) * (c[15] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v12 as UDPINT) << 17;
    t += s as UDPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[13] as UDPINT) * (c[18] as UDPINT);
    tot += (c[14] as UDPINT) * (c[17] as UDPINT);
    tot += (c[15] as UDPINT) * (c[16] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v13 as UDPINT) << 17;
    t += s as UDPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[14] as UDPINT) * (c[18] as UDPINT);
    tot += (c[15] as UDPINT) * (c[17] as UDPINT);
    tot *= 2;
    tot += (c[16] as UDPINT) * (c[16] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v14 as UDPINT) << 17;
    t += s as UDPINT;
    c[13] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[15] as UDPINT) * (c[18] as UDPINT);
    tot += (c[16] as UDPINT) * (c[17] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v15 as UDPINT) << 17;
    t += s as UDPINT;
    c[14] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[16] as UDPINT) * (c[18] as UDPINT);
    tot *= 2;
    tot += (c[17] as UDPINT) * (c[17] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v16 as UDPINT) << 17;
    t += s as UDPINT;
    c[15] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[17] as UDPINT) * (c[18] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v17 as UDPINT) << 17;
    t += s as UDPINT;
    c[16] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[18] as UDPINT) * (c[18] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v18 as UDPINT) << 17;
    t += s as UDPINT;
    c[17] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t -= 1;
    c[18] = t as SPINT;
    return;
}

//copy
fn modcpy(a: &[SPINT], c: &mut [SPINT]) {
    for i in 0..19 {
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
    let mut x: [SPINT; 19] = [0; 19];
    let mut z: [SPINT; 19] = [0; 19];
    let mut t0: [SPINT; 19] = [0; 19];
    let mut t1: [SPINT; 19] = [0; 19];
    let mut t2: [SPINT; 19] = [0; 19];
    let mut t3: [SPINT; 19] = [0; 19];
    let mut t4: [SPINT; 19] = [0; 19];
    let mut t5: [SPINT; 19] = [0; 19];
    let mut t6: [SPINT; 19] = [0; 19];
    let mut t7: [SPINT; 19] = [0; 19];
    let mut t8: [SPINT; 19] = [0; 19];
    let mut t9: [SPINT; 19] = [0; 19];
    let mut t10: [SPINT; 19] = [0; 19];
    let mut t11: [SPINT; 19] = [0; 19];
    let mut t12: [SPINT; 19] = [0; 19];
    let mut t13: [SPINT; 19] = [0; 19];
    let mut t14: [SPINT; 19] = [0; 19];
    let mut t15: [SPINT; 19] = [0; 19];
    let mut t16: [SPINT; 19] = [0; 19];
    let mut t17: [SPINT; 19] = [0; 19];
    let mut t18: [SPINT; 19] = [0; 19];
    let mut t19: [SPINT; 19] = [0; 19];
    let mut t20: [SPINT; 19] = [0; 19];
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
    let mut s: [SPINT; 19] = [0; 19];
    let mut t: [SPINT; 19] = [0; 19];
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
    let c: [SPINT; 19] = [
        0x603fb63, 0x69c815c, 0xf1cfbba, 0x38a6efa, 0xde5cd23, 0x1524db6, 0xd09706c, 0xeaaa8ea,
        0xb4ace7f, 0xd142b76, 0x7bccff3, 0x61a8e56, 0x55bcc6d, 0x492d0d4, 0xd8e03d1, 0x44383d2,
        0xa3afe8c, 0xa39a5b5, 0x391,
    ];
    modmul(&c, n);
    return;
}

//Convert m back to normal form, m=redc(n)
fn redc(m: &mut [SPINT]) {
    let mut c: [SPINT; 19] = [0; 19];
    c[0] = 1;
    modmul(&c, m);
    modfsb(m);
    return;
}

//is unity?
fn modis1(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 19] = [0; 19];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 1..19 {
        d |= c[i] as SSPINT;
    }
    let c0 = c[0] as SSPINT;
    return (1 & ((d - 1) >> 28) & (((c0 ^ 1) - 1) >> 28)) != 0;
}

//is zero?
fn modis0(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 19] = [0; 19];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 0..19 {
        d |= c[i] as SSPINT;
    }
    return (1 & ((d - 1) >> 28)) != 0;
}

//set to zero
fn modzer(a: &mut [SPINT]) {
    for i in 0..19 {
        a[i] = 0;
    }
    return;
}

//set to one
fn modone(a: &mut [SPINT]) {
    a[0] = 1;
    for i in 1..19 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//set to integer
fn modint(x: usize, a: &mut [SPINT]) {
    a[0] = x as SPINT;
    for i in 1..19 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//Test for quadratic residue
fn modqr(h: Option<&[SPINT]>, x: &[SPINT]) -> bool {
    let mut r: [SPINT; 19] = [0; 19];
    if let Some(hint) = h {
        modcpy(&hint, &mut r);
    } else {
        modpro(&x, &mut r);
    }
    modsqr(&mut r);
    modmul(&x, &mut r);
    modnsqr(&mut r, 2);
    return modis1(&r);
}

//conditional move g to f if d=1
fn modcmv(d: usize, g: &[SPINT], f: &mut [SPINT]) {
    let dd=d as SPINT;
    for i in 0..19 {
        let s = g[i];
        let t = f[i];
        let mut r=s^t;
        let c0=1-dd+r;
        let c1=dd+r;
        r*=t+s;
        unsafe{core::ptr::write_volatile(&mut f[i],c0*t+c1*s)}  
        f[i]-=r;
    }
    return;
}

//conditional swap g and f if d=1
fn modcsw(d: usize, g: &mut [SPINT], f: &mut [SPINT]) {
    let dd=d as SPINT;
    for i in 0..19 {
        let s = g[i];
        let t = f[i];
        let mut r=s^t;
        let c0=1-dd+r;
        let c1=dd+r;
        r*=t+s;
        unsafe{core::ptr::write_volatile(&mut f[i],c0*t+c1*s)}  
        f[i]-=r;
        unsafe{core::ptr::write_volatile(&mut g[i],c0*s+c1*t)}  
        g[i]-=r;
    }
    return;
}

//Modular square root, provide progenitor h if available, NULL if not
fn modsqrt(x: &[SPINT], h: Option<&[SPINT]>, r: &mut [SPINT]) {
    let mut t: [SPINT; 19] = [0; 19];
    let mut b: [SPINT; 19] = [0; 19];
    let mut v: [SPINT; 19] = [0; 19];
    let mut z: [SPINT; 19] = [
        0x63e2205, 0xf9c7b7b, 0xac810a7, 0xb3da861, 0x3a72472, 0x836647d, 0xcd998b7, 0x391dcdb,
        0x35f79ff, 0x55276d5, 0x5673a30, 0xfa92fcc, 0xd7c12da, 0xc7e6000, 0xfa8c4fb, 0xd708ad2,
        0x28c17f3, 0x650d44b, 0x9a0a,
    ];
    let mut y: [SPINT; 19] = [0; 19];
    let mut s: [SPINT; 19] = [0; 19];
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
    a[19 - 1] = (a[19 - 1] << n) | (a[19 - 2] >> (28 - n));
    for i in (1..19 - 1).rev() {
        a[i] = ((a[i] << n) & 0xfffffff) | (a[i - 1] >> (28 - n));
    }
    a[0] = (a[0] << n) & 0xfffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..19 - 1 {
        a[i] = (a[i] >> n) | ((a[i + 1] << (28 - n)) & 0xfffffff);
    }
    a[19 - 1] = a[19 - 1] >> n;
    return r as isize;
}

//set a= 2^r
fn mod2r(r: usize, a: &mut [SPINT]) {
    let n = r / 28;
    let m = r % 28;
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
    let mut c: [SPINT; 19] = [0; 19];
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
    for i in 0..19 {
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
    let mut c: [SPINT; 19] = [0; 19];
    modcpy(a, &mut c);
    redc(&mut c);
    return (c[0] % 2) as usize;
}

//return true if equal
fn modcmp(a: &[SPINT], b: &[SPINT]) -> bool {
    let mut c: [SPINT; 19] = [0; 19];
    let mut d: [SPINT; 19] = [0; 19];
    let mut eq = 1;
    modcpy(a, &mut c);
    redc(&mut c);
    modcpy(b, &mut d);
    redc(&mut d);
    for i in 0..19 {
        eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 19;
const RADIX: usize = 28;
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
