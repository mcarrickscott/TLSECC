#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// For example: cargo run --release --features ED448 --bin Ed448
// python curve_rust.py 64 ED448
// This completes edwards.rs for this curve
// EdDSA Implementation for curve ED448
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf

use crate::ed25519;
use crate::ed25519::*;
use crate::ed25519::ECP;

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
    n[0] += (0x1cf5d3ed as SPINT) & (carry as SPINT);
    n[1] += (0x9318d2 as SPINT) & (carry as SPINT);
    n[2] += (0x1de73596 as SPINT) & (carry as SPINT);
    n[3] += (0x1df3bd45 as SPINT) & (carry as SPINT);
    n[4] += (0x14d as SPINT) & (carry as SPINT);
    n[8] += (0x100000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 29;
    n[0] -= 0x1cf5d3ed as SPINT;
    n[1] -= 0x9318d2 as SPINT;
    n[2] -= 0x1de73596 as SPINT;
    n[3] -= 0x1df3bd45 as SPINT;
    n[4] -= 0x14d as SPINT;
    n[8] -= 0x100000 as SPINT;
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
    n[0] -= 0x39eba7da as SPINT;
    n[1] -= 0x12631a4 as SPINT;
    n[2] -= 0x3bce6b2c as SPINT;
    n[3] -= 0x3be77a8a as SPINT;
    n[4] -= 0x29a as SPINT;
    n[8] -= 0x200000 as SPINT;
    let carry = prop(n);
    n[0] += (0x39eba7da as SPINT) & (carry as SPINT);
    n[1] += (0x12631a4 as SPINT) & (carry as SPINT);
    n[2] += (0x3bce6b2c as SPINT) & (carry as SPINT);
    n[3] += (0x3be77a8a as SPINT) & (carry as SPINT);
    n[4] += (0x29a as SPINT) & (carry as SPINT);
    n[8] += (0x200000 as SPINT) & (carry as SPINT);
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
    n[0] += (0x39eba7da as SPINT) & (carry as SPINT);
    n[1] += (0x12631a4 as SPINT) & (carry as SPINT);
    n[2] += (0x3bce6b2c as SPINT) & (carry as SPINT);
    n[3] += (0x3be77a8a as SPINT) & (carry as SPINT);
    n[4] += (0x29a as SPINT) & (carry as SPINT);
    n[8] += (0x200000 as SPINT) & (carry as SPINT);
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
    n[0] += (0x39eba7da as SPINT) & (carry as SPINT);
    n[1] += (0x12631a4 as SPINT) & (carry as SPINT);
    n[2] += (0x3bce6b2c as SPINT) & (carry as SPINT);
    n[3] += (0x3be77a8a as SPINT) & (carry as SPINT);
    n[4] += (0x29a as SPINT) & (carry as SPINT);
    n[8] += (0x200000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
fn modmli(_b: usize, _c: &mut [SPINT]) {
    return;
}

// Overflow limit   = 18446744073709551616
// maximum possible = 3399791255365623586
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p0 = 0x1cf5d3ed as SPINT;
    let p1 = 0x9318d2 as SPINT;
    let p2 = 0x1de73596 as SPINT;
    let p3 = 0x1df3bd45 as SPINT;
    let p4 = 0x14d as SPINT;
    let q = (1 as SPINT) << 29; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x12547e1b as SPINT;
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
    t += (v2 as DPINT) * (p4 as DPINT);
    t += (v3 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) * (p2 as DPINT);
    t += (v5 as DPINT) * (p1 as DPINT);
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
    t += (v3 as DPINT) * (p4 as DPINT);
    t += (v4 as DPINT) * (p3 as DPINT);
    t += (v5 as DPINT) * (p2 as DPINT);
    t += (v6 as DPINT) * (p1 as DPINT);
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
    t += (v0 as DPINT) << 20;
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v6 as DPINT) * (p2 as DPINT);
    t += (v7 as DPINT) * (p1 as DPINT);
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
    t += (v1 as DPINT) << 20;
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) * (p2 as DPINT);
    t += (v8 as DPINT) * (p1 as DPINT);
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[2] as DPINT) * (b[8] as DPINT);
    t += (c[3] as DPINT) * (b[7] as DPINT);
    t += (c[4] as DPINT) * (b[6] as DPINT);
    t += (c[5] as DPINT) * (b[5] as DPINT);
    t += (c[6] as DPINT) * (b[4] as DPINT);
    t += (c[7] as DPINT) * (b[3] as DPINT);
    t += (c[8] as DPINT) * (b[2] as DPINT);
    t += (v2 as DPINT) << 20;
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) * (p2 as DPINT);
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[3] as DPINT) * (b[8] as DPINT);
    t += (c[4] as DPINT) * (b[7] as DPINT);
    t += (c[5] as DPINT) * (b[6] as DPINT);
    t += (c[6] as DPINT) * (b[5] as DPINT);
    t += (c[7] as DPINT) * (b[4] as DPINT);
    t += (c[8] as DPINT) * (b[3] as DPINT);
    t += (v3 as DPINT) << 20;
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[4] as DPINT) * (b[8] as DPINT);
    t += (c[5] as DPINT) * (b[7] as DPINT);
    t += (c[6] as DPINT) * (b[6] as DPINT);
    t += (c[7] as DPINT) * (b[5] as DPINT);
    t += (c[8] as DPINT) * (b[4] as DPINT);
    t += (v4 as DPINT) << 20;
    t += (v8 as DPINT) * (p4 as DPINT);
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[5] as DPINT) * (b[8] as DPINT);
    t += (c[6] as DPINT) * (b[7] as DPINT);
    t += (c[7] as DPINT) * (b[6] as DPINT);
    t += (c[8] as DPINT) * (b[5] as DPINT);
    t += (v5 as DPINT) << 20;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[6] as DPINT) * (b[8] as DPINT);
    t += (c[7] as DPINT) * (b[7] as DPINT);
    t += (c[8] as DPINT) * (b[6] as DPINT);
    t += (v6 as DPINT) << 20;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[7] as DPINT) * (b[8] as DPINT);
    t += (c[8] as DPINT) * (b[7] as DPINT);
    t += (v7 as DPINT) << 20;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    t += (c[8] as DPINT) * (b[8] as DPINT);
    t += (v8 as DPINT) << 20;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    c[8] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let p0 = 0x1cf5d3ed as SPINT;
    let p1 = 0x9318d2 as SPINT;
    let p2 = 0x1de73596 as SPINT;
    let p3 = 0x1df3bd45 as SPINT;
    let p4 = 0x14d as SPINT;
    let q = (1 as SPINT) << 29; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    let ndash = 0x12547e1b as SPINT;
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
    t += (v2 as UDPINT) * (p4 as UDPINT);
    t += (v3 as UDPINT) * (p3 as UDPINT);
    t += (v4 as UDPINT) * (p2 as UDPINT);
    t += (v5 as UDPINT) * (p1 as UDPINT);
    let v6 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v6 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[0] as UDPINT) * (c[7] as UDPINT);
    tot += (c[1] as UDPINT) * (c[6] as UDPINT);
    tot += (c[2] as UDPINT) * (c[5] as UDPINT);
    tot += (c[3] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v3 as UDPINT) * (p4 as UDPINT);
    t += (v4 as UDPINT) * (p3 as UDPINT);
    t += (v5 as UDPINT) * (p2 as UDPINT);
    t += (v6 as UDPINT) * (p1 as UDPINT);
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
    t += (v0 as UDPINT) << 20;
    t += (v4 as UDPINT) * (p4 as UDPINT);
    t += (v5 as UDPINT) * (p3 as UDPINT);
    t += (v6 as UDPINT) * (p2 as UDPINT);
    t += (v7 as UDPINT) * (p1 as UDPINT);
    let v8 = (((t as SPINT) * ndash) & mask) as SPINT;
    t += (v8 as UDPINT) * (p0 as UDPINT);
    t >>= 29;
    tot = (c[1] as UDPINT) * (c[8] as UDPINT);
    tot += (c[2] as UDPINT) * (c[7] as UDPINT);
    tot += (c[3] as UDPINT) * (c[6] as UDPINT);
    tot += (c[4] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v1 as UDPINT) << 20;
    t += (v5 as UDPINT) * (p4 as UDPINT);
    t += (v6 as UDPINT) * (p3 as UDPINT);
    t += (v7 as UDPINT) * (p2 as UDPINT);
    t += (v8 as UDPINT) * (p1 as UDPINT);
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[2] as UDPINT) * (c[8] as UDPINT);
    tot += (c[3] as UDPINT) * (c[7] as UDPINT);
    tot += (c[4] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    tot += (c[5] as UDPINT) * (c[5] as UDPINT);
    t += tot;
    t += (v2 as UDPINT) << 20;
    t += (v6 as UDPINT) * (p4 as UDPINT);
    t += (v7 as UDPINT) * (p3 as UDPINT);
    t += (v8 as UDPINT) * (p2 as UDPINT);
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[3] as UDPINT) * (c[8] as UDPINT);
    tot += (c[4] as UDPINT) * (c[7] as UDPINT);
    tot += (c[5] as UDPINT) * (c[6] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v3 as UDPINT) << 20;
    t += (v7 as UDPINT) * (p4 as UDPINT);
    t += (v8 as UDPINT) * (p3 as UDPINT);
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[4] as UDPINT) * (c[8] as UDPINT);
    tot += (c[5] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    tot += (c[6] as UDPINT) * (c[6] as UDPINT);
    t += tot;
    t += (v4 as UDPINT) << 20;
    t += (v8 as UDPINT) * (p4 as UDPINT);
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[5] as UDPINT) * (c[8] as UDPINT);
    tot += (c[6] as UDPINT) * (c[7] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v5 as UDPINT) << 20;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[6] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    tot += (c[7] as UDPINT) * (c[7] as UDPINT);
    t += tot;
    t += (v6 as UDPINT) << 20;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[7] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v7 as UDPINT) << 20;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
    tot = (c[8] as UDPINT) * (c[8] as UDPINT);
    t += tot;
    t += (v8 as UDPINT) << 20;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 29;
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
    let mut t9: [SPINT; 9] = [0; 9];
    let mut t10: [SPINT; 9] = [0; 9];
    let mut t11: [SPINT; 9] = [0; 9];
    modcpy(w, &mut x);
    modcpy(&x, &mut t1);
    modsqr(&mut t1);
    modcpy(&x, &mut z);
    modmul(&t1, &mut z);
    modcpy(&z, &mut t4);
    modsqr(&mut t4);
    modcpy(&t1, &mut t8);
    modmul(&t4, &mut t8);
    modcpy(&z, &mut t6);
    modmul(&t8, &mut t6);
    modcpy(&x, &mut t3);
    modmul(&t6, &mut t3);
    modcpy(&t8, &mut t2);
    modmul(&t3, &mut t2);
    modcpy(&t4, &mut t0);
    modmul(&t2, &mut t0);
    modmul(&t0, &mut z);
    modmul(&z, &mut t4);
    modcpy(&t3, &mut t10);
    modmul(&t4, &mut t10);
    modcpy(&t0, &mut t5);
    modmul(&t10, &mut t5);
    modcpy(&t8, &mut t9);
    modmul(&t5, &mut t9);
    modcpy(&t1, &mut t0);
    modmul(&t9, &mut t0);
    modcpy(&t2, &mut t7);
    modmul(&t0, &mut t7);
    modcpy(&t1, &mut t3);
    modmul(&t7, &mut t3);
    modmul(&t3, &mut t1);
    modcpy(&t8, &mut t2);
    modmul(&t1, &mut t2);
    modmul(&t2, &mut t8);
    modcpy(&t10, &mut t11);
    modmul(&t9, &mut t11);
    modnsqr(&mut t11, 127);
    modmul(&t0, &mut t11);
    modnsqr(&mut t11, 8);
    modmul(&t8, &mut t11);
    modnsqr(&mut t11, 7);
    modmul(&t2, &mut t11);
    modnsqr(&mut t11, 6);
    modmul(&t11, &mut t10);
    modnsqr(&mut t10, 8);
    modmul(&t10, &mut t9);
    modnsqr(&mut t9, 8);
    modmul(&t9, &mut t8);
    modnsqr(&mut t8, 7);
    modmul(&t8, &mut t7);
    modnsqr(&mut t7, 9);
    modmul(&t1, &mut t7);
    modnsqr(&mut t7, 6);
    modmul(&t7, &mut t6);
    modnsqr(&mut t6, 13);
    modmul(&t6, &mut t5);
    modnsqr(&mut t5, 6);
    modmul(&t5, &mut t4);
    modnsqr(&mut t4, 10);
    modmul(&t4, &mut t3);
    modnsqr(&mut t3, 8);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 7);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 7);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 5);
    modmul(&t0, &mut z);
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
    for _i in 0..2 - 1 {
        modsqr(&mut s);
        modmul(&z, &mut s);
    }
    modnsqr(&mut t, 3);
    modcpy(&t, z);
    modmul(&s, z);
    return;
}

//Convert n to n-residue form, n=nres(m)
fn nres(n: &mut [SPINT]) {
    let c: [SPINT; 9] = [
        0xb5f9d12, 0x1e141b17, 0x158d7f3d, 0x143f3757, 0x1972d781, 0x42feb7c, 0x1ceec73d,
        0x1e184d1e, 0x5046d,
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
    modnsqr(&mut r, 1);
    return modis1(&r);
}

//conditional move g to f if d=1
fn modcmv(d: usize, g: &[SPINT], f: &mut [SPINT]) {
    let r0 = f[0] ^ g[1];
    let r1 = f[1] ^ g[0];
    let dd = d as SPINT;
    let c0=1-(dd-((r0<<1)>>1));
    let c1=dd+((r1<<1)>>1);
    for i in 0..9 {
        f[i] = f[i]*c0 + g[i]*c1 - r0*((f[i]<<1)>>1) - r1*((g[i]<<1)>>1);
    }
    return;
}

//conditional swap g and f if d=1
fn modcsw(d: usize, g: &mut [SPINT], f: &mut [SPINT]) {
    let r0 = f[0] ^ g[1];
    let r1 = f[1] ^ g[0];
    let dd = d as SPINT;
    let c0=1-(dd-((r0<<1)>>1));
    let c1=dd+((r1<<1)>>1);
    for i in 0..9 {
        let t = f[i];
        f[i] = t*c0 + g[i]*c1 - r0*((t<<1)>>1) - r1*((g[i]<<1)>>1);
        g[i] = g[i]*c0 + t*c1 - r0*((g[i]<<1)>>1) - r1*((t<<1)>>1);
    }
    return;
}

//Modular square root, provide progenitor h if available, NULL if not
fn modsqrt(x: &[SPINT], h: Option<&[SPINT]>, r: &mut [SPINT]) {
    let mut t: [SPINT; 9] = [0; 9];
    let mut b: [SPINT; 9] = [0; 9];
    let mut v: [SPINT; 9] = [0; 9];
    let mut z: [SPINT; 9] = [
        0xbbe07d4, 0x143baeff, 0x13a0ffaf, 0xaca685, 0x1c27a0ef, 0x16b055e0, 0x79df4f5, 0x21c0f30,
        0x94a73,
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
    for k in (2..=2).rev() {
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
const NBITS: usize = 253;
const NBYTES: usize = 32;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;

/*** End of automatically generated code ***/

// number of limbs and bytes in representation
pub const BYTES:usize = NBYTES;
const LIMBS:usize= NLIMBS;
type GEL = [SPINT; LIMBS];


// reduce 64 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^248.(2^248.x + y)  + z, where x,y and z < q (z is bottom 31 bytes, y is next 31 bytes, x is top 2 bytes)
// Important that x,y and z < q, 31 bytes = 248 bits, q is 256 bits
fn reduce(h:&[u8],r:&mut [SPINT]) {
    let mut buff:[u8;BYTES]=[0;BYTES];    
    let mut x:GEL=[0;LIMBS];
    let mut y:GEL=[0;LIMBS];
    let mut z:GEL=[0;LIMBS];
    let mut c:GEL=[0;LIMBS];

    mod2r(8*(BYTES-1),&mut c);

    for i in 0..31 {  // bottom 31 bytes
        buff[i]=h[i];
    }
    buff[31]=0;
    buff.reverse();
    modimp(&buff,&mut z);

    for i in 0..31 { // middle 31 bytes
        buff[i]=h[i+31];
    }
    buff[31]=0;
    buff.reverse();
    modimp(&buff,&mut y);


    for i in 0..2 {
        buff[i]=h[62+i];
    }
    for i in 2..32 {
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

// general purpose HASH512 hash function
// Input ilen bytes, output 64 bytes
fn H(ilen:usize,s:&[u8]) -> [u8; 64] {
    let mut sha512=SHA512::new();
    for i in 0..ilen { 
        sha512.process(s[i]);
    }
    return sha512.hash();
}

// Input private key - 32 random bytes
// Output public key - 32 bytes
pub fn KEY_PAIR(prv: &[u8],public: &mut [u8]) {
    let mut G=ECP::new();
    ecngen(&mut G);
    let mut s:[u8;BYTES]=[0;BYTES]; 

    let d=H(BYTES,&prv);
    for i in 0..BYTES { // get first half
        s[i]=d[i];
    }

// clamp s
    s[0]&=0xF8;
    s[BYTES-1]&=0x7f; s[BYTES-1]|=0x40;
    s.reverse(); // little endian to big endian
    ecnmul(&s,&mut G); 

    let sign=ecnget(&mut G,None,Some(&mut s));  // get y coordinate and sign
    s.reverse(); // big endian to little endian
    for i in 0..32 {
        public[i]=s[i];
    }     
    public[BYTES-1]|=(sign<<7) as u8;
}

// input private key, public key, message to be signed. Output signature
pub fn SIGN(prv:&[u8],public: Option<&[u8]>,m:&[u8],sig:&mut [u8]) {
    let mut sh:[u8;BYTES]=[0;BYTES];
    let mut ipub:[u8;BYTES]=[0;BYTES];
    let mut s:GEL=[0;LIMBS];
    let mut r:GEL=[0;LIMBS];
    let mut d:GEL=[0;LIMBS];

    let mut sha512=SHA512::new();

    let mut G=ECP::new();
    ecngen(&mut G);

    if let Some(pb) = public {
        for i in 0..BYTES {
            ipub[i]=pb[i];
        }
    } else {
        KEY_PAIR(prv,&mut ipub);
    }

    let mut h=H(BYTES,&prv);

    for i in 0..BYTES {
        sh[i]=h[i];
    }

// derive and clamp s
    sh[0]&=0xF8;
    sh[BYTES-1]&=0x7f; sh[BYTES-1]|=0x40;
    sh.reverse();
    modimp(&sh,&mut s);
    
    for i in BYTES..2*BYTES {
        sha512.process(h[i]);
    }
    for i in 0..m.len() {
        sha512.process(m[i]);
    }
    h=sha512.hash(); 

    reduce(&h,&mut r);
    modexp(&r,&mut sh);  // convert to big endian array
    ecnmul(&sh,&mut G);

    let sign=ecnget(&mut G,None,Some(&mut sh));  // get y coordinate and sign
    sh.reverse();              // big endian to little endian

    for i in 0..BYTES {
        sig[i]=sh[i];
    }
    sig[BYTES-1]|=(sign<<7) as u8; // first part of signature

    sha512=SHA512::new();

    for i in 0..BYTES {
        sha512.process(sig[i]);  // R
    }
    for i in 0..BYTES {
        sha512.process(ipub[i]);  // Q
    }
    for i in 0..m.len() {
        sha512.process(m[i]);
    }
    h=sha512.hash();

    reduce(&h,&mut d);
    modmul(&s,&mut d);
    modadd(&r,&mut d);

    modexp(&d,&mut sh);
    sh.reverse();
    for i in 0..BYTES {
        sig[BYTES+i]=sh[i];
    }
}

pub fn VERIFY(public: &[u8],m:&[u8],sig:&[u8]) -> bool {
    let mut buff:[u8;BYTES]=[0;BYTES]; 
    let mut sh:[u8;BYTES]=[0;BYTES];
    let mut G=ECP::new();
    let mut R=ECP::new();
    let mut Q=ECP::new();
   
    let mut u:GEL=[0;LIMBS];

    ecngen(&mut G);

// reconstruct point R
    let mut sign=((sig[BYTES-1]>>7)&1) as usize;
    for i in 0..BYTES {
        buff[i]=sig[i];
    }
    buff[BYTES-1]&=0x7f;
    buff.reverse();
    ecnset(sign,None,Some(&buff),&mut R);
    if ecnisinf(&R) {
        return false;
    }

// reconstruct point Q 
    sign=((public[BYTES-1]>>7)&1) as usize;
    for i in 0..BYTES {
        buff[i]=public[i];
    }
    buff[BYTES-1]&=0x7f;
    buff.reverse();
    ecnset(sign,None,Some(&buff),&mut Q);
    if ecnisinf(&Q) {
        return false;
    }

    for i in 0..BYTES {
        buff[i]=sig[i+BYTES];
    }
    buff.reverse();

    let mut sha512=SHA512::new();
    for i in 0..BYTES {
        sha512.process(sig[i]);  // R
    }
    for i in 0..BYTES {
        sha512.process(public[i]);  // Q
    }
    for i in 0..m.len() {
        sha512.process(m[i]);
    }
    let h=sha512.hash();
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

