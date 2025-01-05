#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// For example: cargo run --release --features ED448 --bin Ed448
// python curve_rust.py 64 ED448
// This completes edwards.rs for this curve
// EdDSA Implementation for curve ED448
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf

use crate::ed448;
use crate::ed448::*;
use crate::ed448::ECP;

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
    for i in 1..15 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 28;
    }
    n[15] += carry as SPINT;
    return ((n[15] as SSPINT) >> 31) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 28;
    let carry = prop(n);
    n[0] += (0xb5844f3 as SPINT) & (carry as SPINT);
    n[1] += (0x78c292a as SPINT) & (carry as SPINT);
    n[2] += (0x58f5523 as SPINT) & (carry as SPINT);
    n[3] += (0xc2728dc as SPINT) & (carry as SPINT);
    n[4] += (0x690216c as SPINT) & (carry as SPINT);
    n[5] += (0x49aed63 as SPINT) & (carry as SPINT);
    n[6] += (0x9c44edb as SPINT) & (carry as SPINT);
    n[7] += (0x7cca23e as SPINT) & (carry as SPINT);
    n[8] -= 1 & (carry as SPINT);
    n[15] += (0x4000000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 28;
    n[0] -= 0xb5844f3 as SPINT;
    n[1] -= 0x78c292a as SPINT;
    n[2] -= 0x58f5523 as SPINT;
    n[3] -= 0xc2728dc as SPINT;
    n[4] -= 0x690216c as SPINT;
    n[5] -= 0x49aed63 as SPINT;
    n[6] -= 0x9c44edb as SPINT;
    n[7] -= 0x7cca23e as SPINT;
    n[8] += 1;
    n[15] -= 0x4000000 as SPINT;
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
    n[0] -= 0x16b089e6 as SPINT;
    n[1] -= 0xf185254 as SPINT;
    n[2] -= 0xb1eaa46 as SPINT;
    n[3] -= 0x184e51b8 as SPINT;
    n[4] -= 0xd2042d8 as SPINT;
    n[5] -= 0x935dac6 as SPINT;
    n[6] -= 0x13889db6 as SPINT;
    n[7] -= 0xf99447c as SPINT;
    n[8] += 2;
    n[15] -= 0x8000000 as SPINT;
    let carry = prop(n);
    n[0] += (0x16b089e6 as SPINT) & (carry as SPINT);
    n[1] += (0xf185254 as SPINT) & (carry as SPINT);
    n[2] += (0xb1eaa46 as SPINT) & (carry as SPINT);
    n[3] += (0x184e51b8 as SPINT) & (carry as SPINT);
    n[4] += (0xd2042d8 as SPINT) & (carry as SPINT);
    n[5] += (0x935dac6 as SPINT) & (carry as SPINT);
    n[6] += (0x13889db6 as SPINT) & (carry as SPINT);
    n[7] += (0xf99447c as SPINT) & (carry as SPINT);
    n[8] -= 2 & (carry as SPINT);
    n[15] += (0x8000000 as SPINT) & (carry as SPINT);
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
    let carry = prop(n);
    n[0] += (0x16b089e6 as SPINT) & (carry as SPINT);
    n[1] += (0xf185254 as SPINT) & (carry as SPINT);
    n[2] += (0xb1eaa46 as SPINT) & (carry as SPINT);
    n[3] += (0x184e51b8 as SPINT) & (carry as SPINT);
    n[4] += (0xd2042d8 as SPINT) & (carry as SPINT);
    n[5] += (0x935dac6 as SPINT) & (carry as SPINT);
    n[6] += (0x13889db6 as SPINT) & (carry as SPINT);
    n[7] += (0xf99447c as SPINT) & (carry as SPINT);
    n[8] -= 2 & (carry as SPINT);
    n[15] += (0x8000000 as SPINT) & (carry as SPINT);
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
    let carry = prop(n);
    n[0] += (0x16b089e6 as SPINT) & (carry as SPINT);
    n[1] += (0xf185254 as SPINT) & (carry as SPINT);
    n[2] += (0xb1eaa46 as SPINT) & (carry as SPINT);
    n[3] += (0x184e51b8 as SPINT) & (carry as SPINT);
    n[4] += (0xd2042d8 as SPINT) & (carry as SPINT);
    n[5] += (0x935dac6 as SPINT) & (carry as SPINT);
    n[6] += (0x13889db6 as SPINT) & (carry as SPINT);
    n[7] += (0xf99447c as SPINT) & (carry as SPINT);
    n[8] -= 2 & (carry as SPINT);
    n[15] += (0x8000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
fn modmli(_b: usize, _c: &mut [SPINT]) {
    return;
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1465199085000135691
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0xb5844f3 as SPINT;
    let p1 = 0x78c292a as SPINT;
    let p2 = 0x58f5523 as SPINT;
    let p3 = 0xc2728dc as SPINT;
    let p4 = 0x690216c as SPINT;
    let p5 = 0x49aed63 as SPINT;
    let p6 = 0x9c44edb as SPINT;
    let p7 = 0x7cca23e as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0xe918bc5 as SPINT;
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
    t += (q - v0) as DPINT;
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
    s = mask as SPINT;
    s -= v1;
    t += (v2 as DPINT) * (p7 as DPINT);
    t += (v3 as DPINT) * (p6 as DPINT);
    t += (v4 as DPINT) * (p5 as DPINT);
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
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
    s = mask as SPINT;
    s -= v2;
    t += (v3 as DPINT) * (p7 as DPINT);
    t += (v4 as DPINT) * (p6 as DPINT);
    t += (v5 as DPINT) * (p5 as DPINT);
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    t += (v9 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
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
    s -= v3;
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
    s -= v4;
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
    s -= v5;
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
    s -= v6;
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
    t += (v0 as DPINT) << 26;
    s -= v7;
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
    s = mask as SPINT;
    t += (v1 as DPINT) << 26;
    s -= v8;
    t += (v9 as DPINT) * (p7 as DPINT);
    t += (v10 as DPINT) * (p6 as DPINT);
    t += (v11 as DPINT) * (p5 as DPINT);
    t += (v12 as DPINT) * (p4 as DPINT);
    t += (v13 as DPINT) * (p3 as DPINT);
    t += (v14 as DPINT) * (p2 as DPINT);
    t += (v15 as DPINT) * (p1 as DPINT);
    t += s as DPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    s = mask as SPINT;
    t += (v2 as DPINT) << 26;
    s -= v9;
    t += (v10 as DPINT) * (p7 as DPINT);
    t += (v11 as DPINT) * (p6 as DPINT);
    t += (v12 as DPINT) * (p5 as DPINT);
    t += (v13 as DPINT) * (p4 as DPINT);
    t += (v14 as DPINT) * (p3 as DPINT);
    t += (v15 as DPINT) * (p2 as DPINT);
    t += s as DPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    s = mask as SPINT;
    t += (v3 as DPINT) << 26;
    s -= v10;
    t += (v11 as DPINT) * (p7 as DPINT);
    t += (v12 as DPINT) * (p6 as DPINT);
    t += (v13 as DPINT) * (p5 as DPINT);
    t += (v14 as DPINT) * (p4 as DPINT);
    t += (v15 as DPINT) * (p3 as DPINT);
    t += s as DPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    s = mask as SPINT;
    t += (v4 as DPINT) << 26;
    s -= v11;
    t += (v12 as DPINT) * (p7 as DPINT);
    t += (v13 as DPINT) * (p6 as DPINT);
    t += (v14 as DPINT) * (p5 as DPINT);
    t += (v15 as DPINT) * (p4 as DPINT);
    t += s as DPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    s = mask as SPINT;
    t += (v5 as DPINT) << 26;
    s -= v12;
    t += (v13 as DPINT) * (p7 as DPINT);
    t += (v14 as DPINT) * (p6 as DPINT);
    t += (v15 as DPINT) * (p5 as DPINT);
    t += s as DPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    s = mask as SPINT;
    t += (v6 as DPINT) << 26;
    s -= v13;
    t += (v14 as DPINT) * (p7 as DPINT);
    t += (v15 as DPINT) * (p6 as DPINT);
    t += s as DPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[7] as DPINT) * (b[15] as DPINT);
    t += (c[8] as DPINT) * (b[14] as DPINT);
    t += (c[9] as DPINT) * (b[13] as DPINT);
    t += (c[10] as DPINT) * (b[12] as DPINT);
    t += (c[11] as DPINT) * (b[11] as DPINT);
    t += (c[12] as DPINT) * (b[10] as DPINT);
    t += (c[13] as DPINT) * (b[9] as DPINT);
    t += (c[14] as DPINT) * (b[8] as DPINT);
    t += (c[15] as DPINT) * (b[7] as DPINT);
    s = mask as SPINT;
    t += (v7 as DPINT) << 26;
    s -= v14;
    t += (v15 as DPINT) * (p7 as DPINT);
    t += s as DPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[8] as DPINT) * (b[15] as DPINT);
    t += (c[9] as DPINT) * (b[14] as DPINT);
    t += (c[10] as DPINT) * (b[13] as DPINT);
    t += (c[11] as DPINT) * (b[12] as DPINT);
    t += (c[12] as DPINT) * (b[11] as DPINT);
    t += (c[13] as DPINT) * (b[10] as DPINT);
    t += (c[14] as DPINT) * (b[9] as DPINT);
    t += (c[15] as DPINT) * (b[8] as DPINT);
    s = mask as SPINT;
    t += (v8 as DPINT) << 26;
    s -= v15;
    t += s as DPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[9] as DPINT) * (b[15] as DPINT);
    t += (c[10] as DPINT) * (b[14] as DPINT);
    t += (c[11] as DPINT) * (b[13] as DPINT);
    t += (c[12] as DPINT) * (b[12] as DPINT);
    t += (c[13] as DPINT) * (b[11] as DPINT);
    t += (c[14] as DPINT) * (b[10] as DPINT);
    t += (c[15] as DPINT) * (b[9] as DPINT);
    s = mask as SPINT;
    t += (v9 as DPINT) << 26;
    t += s as DPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[10] as DPINT) * (b[15] as DPINT);
    t += (c[11] as DPINT) * (b[14] as DPINT);
    t += (c[12] as DPINT) * (b[13] as DPINT);
    t += (c[13] as DPINT) * (b[12] as DPINT);
    t += (c[14] as DPINT) * (b[11] as DPINT);
    t += (c[15] as DPINT) * (b[10] as DPINT);
    s = mask as SPINT;
    t += (v10 as DPINT) << 26;
    t += s as DPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[11] as DPINT) * (b[15] as DPINT);
    t += (c[12] as DPINT) * (b[14] as DPINT);
    t += (c[13] as DPINT) * (b[13] as DPINT);
    t += (c[14] as DPINT) * (b[12] as DPINT);
    t += (c[15] as DPINT) * (b[11] as DPINT);
    s = mask as SPINT;
    t += (v11 as DPINT) << 26;
    t += s as DPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[12] as DPINT) * (b[15] as DPINT);
    t += (c[13] as DPINT) * (b[14] as DPINT);
    t += (c[14] as DPINT) * (b[13] as DPINT);
    t += (c[15] as DPINT) * (b[12] as DPINT);
    s = mask as SPINT;
    t += (v12 as DPINT) << 26;
    t += s as DPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[13] as DPINT) * (b[15] as DPINT);
    t += (c[14] as DPINT) * (b[14] as DPINT);
    t += (c[15] as DPINT) * (b[13] as DPINT);
    s = mask as SPINT;
    t += (v13 as DPINT) << 26;
    t += s as DPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[14] as DPINT) * (b[15] as DPINT);
    t += (c[15] as DPINT) * (b[14] as DPINT);
    s = mask as SPINT;
    t += (v14 as DPINT) << 26;
    t += s as DPINT;
    c[13] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[15] as DPINT) * (b[15] as DPINT);
    s = mask as SPINT;
    t += (v15 as DPINT) << 26;
    t += s as DPINT;
    c[14] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t -= 1;
    c[15] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let p0 = 0xb5844f3 as SPINT;
    let p1 = 0x78c292a as SPINT;
    let p2 = 0x58f5523 as SPINT;
    let p3 = 0xc2728dc as SPINT;
    let p4 = 0x690216c as SPINT;
    let p5 = 0x49aed63 as SPINT;
    let p6 = 0x9c44edb as SPINT;
    let p7 = 0x7cca23e as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0xe918bc5 as SPINT;
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
    t += (q - v0) as UDPINT;
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
    s = mask as SPINT;
    s -= v1;
    t += (v2 as UDPINT) * (p7 as UDPINT);
    t += (v3 as UDPINT) * (p6 as UDPINT);
    t += (v4 as UDPINT) * (p5 as UDPINT);
    t += (v5 as UDPINT) * (p4 as UDPINT);
    t += (v6 as UDPINT) * (p3 as UDPINT);
    t += (v7 as UDPINT) * (p2 as UDPINT);
    t += (v8 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
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
    s = mask as SPINT;
    s -= v2;
    t += (v3 as UDPINT) * (p7 as UDPINT);
    t += (v4 as UDPINT) * (p6 as UDPINT);
    t += (v5 as UDPINT) * (p5 as UDPINT);
    t += (v6 as UDPINT) * (p4 as UDPINT);
    t += (v7 as UDPINT) * (p3 as UDPINT);
    t += (v8 as UDPINT) * (p2 as UDPINT);
    t += (v9 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
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
    s -= v3;
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
    s -= v4;
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
    s -= v5;
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
    s -= v6;
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
    t += (v0 as UDPINT) << 26;
    s -= v7;
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
    tot = (c[1] as UDPINT) * (c[15] as UDPINT);
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
    t += (v1 as UDPINT) << 26;
    s -= v8;
    t += (v9 as UDPINT) * (p7 as UDPINT);
    t += (v10 as UDPINT) * (p6 as UDPINT);
    t += (v11 as UDPINT) * (p5 as UDPINT);
    t += (v12 as UDPINT) * (p4 as UDPINT);
    t += (v13 as UDPINT) * (p3 as UDPINT);
    t += (v14 as UDPINT) * (p2 as UDPINT);
    t += (v15 as UDPINT) * (p1 as UDPINT);
    t += s as UDPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[2] as UDPINT) * (c[15] as UDPINT);
    tot += (c[3] as UDPINT) * (c[14] as UDPINT);
    tot += (c[4] as UDPINT) * (c[13] as UDPINT);
    tot += (c[5] as UDPINT) * (c[12] as UDPINT);
    tot += (c[6] as UDPINT) * (c[11] as UDPINT);
    tot += (c[7] as UDPINT) * (c[10] as UDPINT);
    tot += (c[8] as UDPINT) * (c[9] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v2 as UDPINT) << 26;
    s -= v9;
    t += (v10 as UDPINT) * (p7 as UDPINT);
    t += (v11 as UDPINT) * (p6 as UDPINT);
    t += (v12 as UDPINT) * (p5 as UDPINT);
    t += (v13 as UDPINT) * (p4 as UDPINT);
    t += (v14 as UDPINT) * (p3 as UDPINT);
    t += (v15 as UDPINT) * (p2 as UDPINT);
    t += s as UDPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[3] as UDPINT) * (c[15] as UDPINT);
    tot += (c[4] as UDPINT) * (c[14] as UDPINT);
    tot += (c[5] as UDPINT) * (c[13] as UDPINT);
    tot += (c[6] as UDPINT) * (c[12] as UDPINT);
    tot += (c[7] as UDPINT) * (c[11] as UDPINT);
    tot += (c[8] as UDPINT) * (c[10] as UDPINT);
    tot *= 2;
    tot += (c[9] as UDPINT) * (c[9] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v3 as UDPINT) << 26;
    s -= v10;
    t += (v11 as UDPINT) * (p7 as UDPINT);
    t += (v12 as UDPINT) * (p6 as UDPINT);
    t += (v13 as UDPINT) * (p5 as UDPINT);
    t += (v14 as UDPINT) * (p4 as UDPINT);
    t += (v15 as UDPINT) * (p3 as UDPINT);
    t += s as UDPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[4] as UDPINT) * (c[15] as UDPINT);
    tot += (c[5] as UDPINT) * (c[14] as UDPINT);
    tot += (c[6] as UDPINT) * (c[13] as UDPINT);
    tot += (c[7] as UDPINT) * (c[12] as UDPINT);
    tot += (c[8] as UDPINT) * (c[11] as UDPINT);
    tot += (c[9] as UDPINT) * (c[10] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v4 as UDPINT) << 26;
    s -= v11;
    t += (v12 as UDPINT) * (p7 as UDPINT);
    t += (v13 as UDPINT) * (p6 as UDPINT);
    t += (v14 as UDPINT) * (p5 as UDPINT);
    t += (v15 as UDPINT) * (p4 as UDPINT);
    t += s as UDPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[5] as UDPINT) * (c[15] as UDPINT);
    tot += (c[6] as UDPINT) * (c[14] as UDPINT);
    tot += (c[7] as UDPINT) * (c[13] as UDPINT);
    tot += (c[8] as UDPINT) * (c[12] as UDPINT);
    tot += (c[9] as UDPINT) * (c[11] as UDPINT);
    tot *= 2;
    tot += (c[10] as UDPINT) * (c[10] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v5 as UDPINT) << 26;
    s -= v12;
    t += (v13 as UDPINT) * (p7 as UDPINT);
    t += (v14 as UDPINT) * (p6 as UDPINT);
    t += (v15 as UDPINT) * (p5 as UDPINT);
    t += s as UDPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[6] as UDPINT) * (c[15] as UDPINT);
    tot += (c[7] as UDPINT) * (c[14] as UDPINT);
    tot += (c[8] as UDPINT) * (c[13] as UDPINT);
    tot += (c[9] as UDPINT) * (c[12] as UDPINT);
    tot += (c[10] as UDPINT) * (c[11] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v6 as UDPINT) << 26;
    s -= v13;
    t += (v14 as UDPINT) * (p7 as UDPINT);
    t += (v15 as UDPINT) * (p6 as UDPINT);
    t += s as UDPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[7] as UDPINT) * (c[15] as UDPINT);
    tot += (c[8] as UDPINT) * (c[14] as UDPINT);
    tot += (c[9] as UDPINT) * (c[13] as UDPINT);
    tot += (c[10] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    tot += (c[11] as UDPINT) * (c[11] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v7 as UDPINT) << 26;
    s -= v14;
    t += (v15 as UDPINT) * (p7 as UDPINT);
    t += s as UDPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[8] as UDPINT) * (c[15] as UDPINT);
    tot += (c[9] as UDPINT) * (c[14] as UDPINT);
    tot += (c[10] as UDPINT) * (c[13] as UDPINT);
    tot += (c[11] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v8 as UDPINT) << 26;
    s -= v15;
    t += s as UDPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[9] as UDPINT) * (c[15] as UDPINT);
    tot += (c[10] as UDPINT) * (c[14] as UDPINT);
    tot += (c[11] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    tot += (c[12] as UDPINT) * (c[12] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v9 as UDPINT) << 26;
    t += s as UDPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[10] as UDPINT) * (c[15] as UDPINT);
    tot += (c[11] as UDPINT) * (c[14] as UDPINT);
    tot += (c[12] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v10 as UDPINT) << 26;
    t += s as UDPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[11] as UDPINT) * (c[15] as UDPINT);
    tot += (c[12] as UDPINT) * (c[14] as UDPINT);
    tot *= 2;
    tot += (c[13] as UDPINT) * (c[13] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v11 as UDPINT) << 26;
    t += s as UDPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[12] as UDPINT) * (c[15] as UDPINT);
    tot += (c[13] as UDPINT) * (c[14] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v12 as UDPINT) << 26;
    t += s as UDPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[13] as UDPINT) * (c[15] as UDPINT);
    tot *= 2;
    tot += (c[14] as UDPINT) * (c[14] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v13 as UDPINT) << 26;
    t += s as UDPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[14] as UDPINT) * (c[15] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v14 as UDPINT) << 26;
    t += s as UDPINT;
    c[13] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[15] as UDPINT) * (c[15] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v15 as UDPINT) << 26;
    t += s as UDPINT;
    c[14] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t -= 1;
    c[15] = t as SPINT;
    return;
}

//copy
fn modcpy(a: &[SPINT], c: &mut [SPINT]) {
    for i in 0..16 {
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
    let mut x: [SPINT; 16] = [0; 16];
    let mut z: [SPINT; 16] = [0; 16];
    let mut t0: [SPINT; 16] = [0; 16];
    let mut t1: [SPINT; 16] = [0; 16];
    let mut t2: [SPINT; 16] = [0; 16];
    let mut t3: [SPINT; 16] = [0; 16];
    let mut t4: [SPINT; 16] = [0; 16];
    let mut t5: [SPINT; 16] = [0; 16];
    let mut t6: [SPINT; 16] = [0; 16];
    let mut t7: [SPINT; 16] = [0; 16];
    let mut t8: [SPINT; 16] = [0; 16];
    let mut t9: [SPINT; 16] = [0; 16];
    let mut t10: [SPINT; 16] = [0; 16];
    let mut t11: [SPINT; 16] = [0; 16];
    let mut t12: [SPINT; 16] = [0; 16];
    let mut t13: [SPINT; 16] = [0; 16];
    let mut t14: [SPINT; 16] = [0; 16];
    let mut t15: [SPINT; 16] = [0; 16];
    modcpy(w, &mut x);
    modcpy(&x, &mut t12);
    modsqr(&mut t12);
    modcpy(&x, &mut t1);
    modmul(&t12, &mut t1);
    modcpy(&x, &mut z);
    modmul(&t1, &mut z);
    modcpy(&x, &mut t5);
    modmul(&z, &mut t5);
    modcpy(&z, &mut t4);
    modmul(&t5, &mut t4);
    modcpy(&t12, &mut t7);
    modmul(&t4, &mut t7);
    modcpy(&t12, &mut t2);
    modmul(&t7, &mut t2);
    modcpy(&t12, &mut z);
    modmul(&t2, &mut z);
    modcpy(&t12, &mut t0);
    modmul(&z, &mut t0);
    modcpy(&t12, &mut t9);
    modmul(&t0, &mut t9);
    modcpy(&t12, &mut t3);
    modmul(&t9, &mut t3);
    modcpy(&t12, &mut t8);
    modmul(&t3, &mut t8);
    modcpy(&t12, &mut t10);
    modmul(&t8, &mut t10);
    modcpy(&t12, &mut t6);
    modmul(&t10, &mut t6);
    modcpy(&t12, &mut t11);
    modmul(&t6, &mut t11);
    modmul(&t11, &mut t12);
    modcpy(&t12, &mut t14);
    modsqr(&mut t14);
    modcpy(&t14, &mut t13);
    modsqr(&mut t13);
    modcpy(&t13, &mut t15);
    modnsqr(&mut t15, 5);
    modmul(&t15, &mut t13);
    modcpy(&t13, &mut t15);
    modnsqr(&mut t15, 4);
    modmul(&t14, &mut t15);
    modnsqr(&mut t15, 11);
    modmul(&t15, &mut t13);
    modcpy(&t13, &mut t15);
    modnsqr(&mut t15, 4);
    modmul(&t15, &mut t14);
    modnsqr(&mut t14, 26);
    modmul(&t14, &mut t13);
    modcpy(&t13, &mut t14);
    modnsqr(&mut t14, 55);
    modmul(&t14, &mut t13);
    modcpy(&t13, &mut t14);
    modnsqr(&mut t14, 110);
    modmul(&t14, &mut t13);
    modmul(&t1, &mut t13);
    modnsqr(&mut t13, 6);
    modmul(&t12, &mut t13);
    modnsqr(&mut t13, 7);
    modmul(&t10, &mut t13);
    modnsqr(&mut t13, 6);
    modmul(&t0, &mut t13);
    modnsqr(&mut t13, 8);
    modmul(&t13, &mut t12);
    modnsqr(&mut t12, 6);
    modmul(&t9, &mut t12);
    modnsqr(&mut t12, 5);
    modmul(&t0, &mut t12);
    modnsqr(&mut t12, 8);
    modmul(&t9, &mut t12);
    modnsqr(&mut t12, 4);
    modmul(&t7, &mut t12);
    modnsqr(&mut t12, 6);
    modmul(&t6, &mut t12);
    modnsqr(&mut t12, 5);
    modmul(&t4, &mut t12);
    modnsqr(&mut t12, 6);
    modmul(&t2, &mut t12);
    modnsqr(&mut t12, 6);
    modmul(&t12, &mut t11);
    modnsqr(&mut t11, 5);
    modmul(&t3, &mut t11);
    modnsqr(&mut t11, 5);
    modmul(&t0, &mut t11);
    modnsqr(&mut t11, 4);
    modmul(&t7, &mut t11);
    modnsqr(&mut t11, 5);
    modmul(&t4, &mut t11);
    modnsqr(&mut t11, 7);
    modmul(&x, &mut t11);
    modnsqr(&mut t11, 8);
    modmul(&t7, &mut t11);
    modnsqr(&mut t11, 6);
    modmul(&t11, &mut t10);
    modsqr(&mut t10);
    modmul(&x, &mut t10);
    modnsqr(&mut t10, 9);
    modmul(&t10, &mut t9);
    modnsqr(&mut t9, 4);
    modmul(&t4, &mut t9);
    modnsqr(&mut t9, 6);
    modmul(&t0, &mut t9);
    modnsqr(&mut t9, 5);
    modmul(&t9, &mut t8);
    modnsqr(&mut t8, 7);
    modmul(&t8, &mut t7);
    modnsqr(&mut t7, 7);
    modmul(&z, &mut t7);
    modnsqr(&mut t7, 6);
    modmul(&t3, &mut t7);
    modnsqr(&mut t7, 5);
    modmul(&t4, &mut t7);
    modnsqr(&mut t7, 8);
    modmul(&t7, &mut t6);
    modnsqr(&mut t6, 2);
    modmul(&t1, &mut t6);
    modnsqr(&mut t6, 5);
    modmul(&t1, &mut t6);
    modnsqr(&mut t6, 7);
    modmul(&t6, &mut t5);
    modnsqr(&mut t5, 6);
    modmul(&t5, &mut t4);
    modnsqr(&mut t4, 6);
    modmul(&t4, &mut t3);
    modnsqr(&mut t3, 5);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 3);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 9);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 6);
    modmul(&t0, &mut z);
    modnsqr(&mut z, 2);
    modcpy(&z, r);
    return;
}

//calculate inverse, provide progenitor h if available
fn modinv(h: Option<&[SPINT]>, z: &mut [SPINT]) {
    let mut s: [SPINT; 16] = [0; 16];
    let mut t: [SPINT; 16] = [0; 16];
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
    let c: [SPINT; 16] = [
        0x49b9b60, 0x5392570, 0x195d9e3, 0x2c4bc1b, 0x8597af3, 0x2388ea1, 0x80d66de, 0x5ee4d83,
        0xe17cf72, 0xc47c44a, 0xcc14ba3, 0x70af1a9, 0xcb7e4d0, 0x292052b, 0x9f823b7, 0x3402a93,
    ];
    modmul(&c, n);
    return;
}

//Convert m back to normal form, m=redc(n)
fn redc(m: &mut [SPINT]) {
    let mut c: [SPINT; 16] = [0; 16];
    c[0] = 1;
    modmul(&c, m);
    modfsb(m);
    return;
}

//is unity?
fn modis1(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 16] = [0; 16];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 1..16 {
        d |= c[i] as SSPINT;
    }
    let c0 = c[0] as SSPINT;
    return (1 & ((d - 1) >> 28) & (((c0 ^ 1) - 1) >> 28)) != 0;
}

//is zero?
fn modis0(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 16] = [0; 16];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 0..16 {
        d |= c[i] as SSPINT;
    }
    return (1 & ((d - 1) >> 28)) != 0;
}

//set to zero
fn modzer(a: &mut [SPINT]) {
    for i in 0..16 {
        a[i] = 0;
    }
    return;
}

//set to one
fn modone(a: &mut [SPINT]) {
    a[0] = 1;
    for i in 1..16 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//set to integer
fn modint(x: usize, a: &mut [SPINT]) {
    a[0] = x as SPINT;
    for i in 1..16 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//Test for quadratic residue
fn modqr(h: Option<&[SPINT]>, x: &[SPINT]) -> bool {
    let mut r: [SPINT; 16] = [0; 16];
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
    let dd=d as SPINT;
    for i in 0..16 {
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
    for i in 0..16 {
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
    let mut y: [SPINT; 16] = [0; 16];
    let mut s: [SPINT; 16] = [0; 16];
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
    a[16 - 1] = (a[16 - 1] << n) | (a[16 - 2] >> (28 - n));
    for i in (1..16 - 1).rev() {
        a[i] = ((a[i] << n) & 0xfffffff) | (a[i - 1] >> (28 - n));
    }
    a[0] = (a[0] << n) & 0xfffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..16 - 1 {
        a[i] = (a[i] >> n) | ((a[i + 1] << (28 - n)) & 0xfffffff);
    }
    a[16 - 1] = a[16 - 1] >> n;
    return r as isize;
}

//set a= 2^r
fn mod2r(r: usize, a: &mut [SPINT]) {
    let n = r / 28;
    let m = r % 28;
    modzer(a);
    if r >= 56 * 8 {
        return;
    }
    a[n] = 1;
    a[n] <<= m;
    nres(a);
}

//export to byte array
fn modexp(a: &[SPINT], b: &mut [u8]) {
    let mut c: [SPINT; 16] = [0; 16];
    modcpy(a, &mut c);
    redc(&mut c);
    for i in (0..56).rev() {
        b[i] = (c[0] & 0xff) as u8;
        modshr(8, &mut c);
    }
    return;
}

//import from byte array
fn modimp(b: &[u8], a: &mut [SPINT]) -> bool {
    for i in 0..16 {
        a[i] = 0;
    }
    for i in 0..56 {
        modshl(8, a);
        a[0] += b[i] as SPINT;
    }
    let res = modfsb(a);
    nres(a);
    return res;
}

//determine sign
fn modsign(a: &[SPINT]) -> usize {
    let mut c: [SPINT; 16] = [0; 16];
    modcpy(a, &mut c);
    redc(&mut c);
    return (c[0] % 2) as usize;
}

//return true if equal
fn modcmp(a: &[SPINT], b: &[SPINT]) -> bool {
    let mut c: [SPINT; 16] = [0; 16];
    let mut d: [SPINT; 16] = [0; 16];
    let mut eq = 1;
    modcpy(a, &mut c);
    redc(&mut c);
    modcpy(b, &mut d);
    redc(&mut d);
    for i in 0..16 {
        eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 16;
const RADIX: usize = 28;
const NBITS: usize = 446;
const NBYTES: usize = 56;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;

/*** End of automatically generated code ***/

// number of limbs and bytes in representation
pub const BYTES:usize = NBYTES;
const LIMBS:usize= NLIMBS;
type GEL = [SPINT; LIMBS];



// reduce 114 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^440.(2^440 + y) + z, where x,y and z < q (z is bottom 55 bytes, y is next 55 bytes, x is top 4 bytes)
// Important that x,y and z < q, 55 bytes = 440 bits, q is 446 bits
fn reduce(h:&[u8],r:&mut [SPINT]) {
    let mut buff:[u8;BYTES]=[0;BYTES];    
    let mut x:GEL=[0;LIMBS];
    let mut y:GEL=[0;LIMBS];
    let mut z:GEL=[0;LIMBS];
    let mut c:GEL=[0;LIMBS];

    mod2r(8*(BYTES-1),&mut c);

    for i in 0..55 {  // bottom 55 bytes
        buff[i]=h[i];
    }
    buff[55]=0;
    buff.reverse();
    modimp(&buff,&mut z);

    for i in 0..55 { // middle 55 bytes
        buff[i]=h[i+55];
    }
    buff[55]=0;
    buff.reverse();
    modimp(&buff,&mut y);


    for i in 0..4 {
        buff[i]=h[110+i];
    }
    for i in 4..56 {
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

// Input private key - 57 random bytes
// Output public key - 57 bytes
pub fn KEY_PAIR(prv: &[u8],public: &mut [u8]) {
    let mut P=ECP::new();
    ecngen(&mut P);
    let mut s:[u8;BYTES]=[0;BYTES]; 

    H(BYTES+1,BYTES,&prv,&mut s);
// clamp s
    s[0]&=0xFC;
    s[55]|=0x80;

    s.reverse(); // little endian to big endian
 
    ecnmul(&s,&mut P); 

    let sign=ecnget(&mut P,None,Some(&mut s));  // get y coordinate and sign
    s.reverse(); // big endian to little endian
    for i in 0..56 {
        public[i]=s[i];
    }
             
    public[56]=(sign<<7) as u8;
}

const dom4:[u8;10]=[b'S',b'i',b'g',b'E',b'd',b'4',b'4',b'8',0,0];

// input private key, public key, message to be signed. Output signature
pub fn SIGN(prv:&[u8],public: Option<&[u8]>,m:&[u8],sig:&mut [u8]) {
    let mut h:[u8;2*BYTES+2]=[0;2*BYTES+2];  
    let mut ipub:[u8;BYTES+1]=[0;BYTES+1];
    let mut sh:[u8;BYTES]=[0;BYTES];
    let mut s:GEL=[0;LIMBS];
    let mut r:GEL=[0;LIMBS];
    let mut d:GEL=[0;LIMBS];

    let mut sha3=SHA3::new(SHAKE256);

    let mut R=ECP::new();
    ecngen(&mut R);

    if let Some(pb) = public {
        for i in 0..BYTES+1 {
            ipub[i]=pb[i];
        }
    } else {
        KEY_PAIR(prv,&mut ipub);
    }

    H(BYTES+1,2*BYTES+2,&prv,&mut h);

    for i in 0..BYTES {
        sh[i]=h[i];
    }

// derive and clamp s
    sh[0]&=0xFC;
    sh[55]|=0x80;
    sh.reverse();
    modimp(&sh,&mut s);
    
    for i in 0..10 {
        sha3.process(dom4[i]);
    }
    for i in BYTES+1..2*BYTES+2 {
        sha3.process(h[i]);
    }

    for i in 0..m.len() {
        sha3.process(m[i]);
    }
    sha3.shake(&mut h,2*BYTES+2); 

    reduce(&h,&mut r);
    modexp(&r,&mut sh);  // convert to big endian array
    ecnmul(&sh,&mut R);

    let sign=ecnget(&mut R,None,Some(&mut sh));  // get y coordinate and sign
    sh.reverse();              // big endian to little endian

    for i in 0..BYTES {
        sig[i]=sh[i];
    }
    sig[BYTES]=(sign<<7) as u8; // first part of signature

    sha3=SHA3::new(SHAKE256);

    for i in 0..10 {
        sha3.process(dom4[i]);
    }
    for i in 0..BYTES+1 {
        sha3.process(sig[i]);  // R
    }
    for i in 0..BYTES+1 {
        sha3.process(ipub[i]);  // Q
    }
    for i in 0..m.len() {
        sha3.process(m[i]);
    }
    sha3.shake(&mut h,2*BYTES+2);

    reduce(&h,&mut d);
    modmul(&s,&mut d);
    modadd(&r,&mut d);

    modexp(&d,&mut sh);
    sh.reverse();
    for i in 0..BYTES {
        sig[BYTES+i+1]=sh[i];
    }
    sig[2*BYTES+1]=0;           // second part of signature
}

pub fn VERIFY(public: &[u8],m:&[u8],sig:&[u8]) -> bool {
    let mut buff:[u8;BYTES]=[0;BYTES]; 
    let mut sh:[u8;BYTES]=[0;BYTES];
    let mut G=ECP::new();
    let mut R=ECP::new();
    let mut Q=ECP::new();
   
    let mut u:GEL=[0;LIMBS];
    let mut h:[u8;2*BYTES+2]=[0;2*BYTES+2];

    ecngen(&mut G);

// reconstruct point R
    for i in 0..BYTES {
        buff[i]=sig[i];
    }
    buff.reverse();
    let mut sign=(sig[BYTES]>>7) as usize;
    ecnset(sign,None,Some(&buff),&mut R);
    if ecnisinf(&R) {
        return false;
    }

// reconstruct point Q 
    for i in 0..BYTES {
        buff[i]=public[i];
    }
    buff.reverse();
    sign=((public[BYTES]>>7)&1) as usize;
    ecnset(sign,None,Some(&buff),&mut Q);
    if ecnisinf(&Q) {
        return false;
    }


    for i in 0..BYTES {
        buff[i]=sig[i+BYTES+1];
    }
    buff.reverse();

    let mut sha3=SHA3::new(SHAKE256);

    for i in 0..10 {
        sha3.process(dom4[i]);
    }
    for i in 0..BYTES+1 {
        sha3.process(sig[i]);  // R
    }
    for i in 0..BYTES+1 {
        sha3.process(public[i]);  // Q
    }
    for i in 0..m.len() {
        sha3.process(m[i]);
    }
    sha3.shake(&mut h,2*BYTES+2);

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

/*

} */
