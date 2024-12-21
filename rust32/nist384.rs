#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
// Weierstrass curve support 
// Use python scripts to generate code for NIST256, or your own curve
//
// Mike Scott 4th September 2024
// TII
//
// code for 32/64-bit processor for NIST256 curve can be generated  by 
//
// python curve_rust.py 32/64 NIST256
//

// make sure decoration and generic are both set to False in monty_rust.py or pseudo_rust.py

/*** Insert automatically generated code for modulus field.rs here ***/

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
fn flatten(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 28;
    let carry = prop(n);
    n[0] -= 1 & (carry as SPINT);
    n[1] += (0x10 as SPINT) & (carry as SPINT);
    n[3] += (0xffff000 as SPINT) & (carry as SPINT);
    n[4] += (0xffeffff as SPINT) & (carry as SPINT);
    n[5] -= 1 & (carry as SPINT);
    n[13] += (0x100000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 28;
    n[0] += 1;
    n[1] -= 0x10 as SPINT;
    n[3] -= 0xffff000 as SPINT;
    n[4] -= 0xffeffff as SPINT;
    n[5] += 1;
    n[13] -= 0x100000 as SPINT;
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
    n[0] += 2;
    n[1] -= 0x20 as SPINT;
    n[3] -= 0x1fffe000 as SPINT;
    n[4] -= 0x1ffdfffe as SPINT;
    n[5] += 2;
    n[13] -= 0x200000 as SPINT;
    let carry = prop(n);
    n[0] -= 2 & (carry as SPINT);
    n[1] += (0x20 as SPINT) & (carry as SPINT);
    n[3] += (0x1fffe000 as SPINT) & (carry as SPINT);
    n[4] += (0x1ffdfffe as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[13] += (0x200000 as SPINT) & (carry as SPINT);
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
    n[0] -= 2 & (carry as SPINT);
    n[1] += (0x20 as SPINT) & (carry as SPINT);
    n[3] += (0x1fffe000 as SPINT) & (carry as SPINT);
    n[4] += (0x1ffdfffe as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[13] += (0x200000 as SPINT) & (carry as SPINT);
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
    n[0] -= 2 & (carry as SPINT);
    n[1] += (0x20 as SPINT) & (carry as SPINT);
    n[3] += (0x1fffe000 as SPINT) & (carry as SPINT);
    n[4] += (0x1ffdfffe as SPINT) & (carry as SPINT);
    n[5] -= 2 & (carry as SPINT);
    n[13] += (0x200000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
fn modmli(_b: usize, _c: &mut [SPINT]) {
    return;
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1153184352846286846
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let p3 = 0xffff000 as SPINT;
    let p4 = 0xffeffff as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) << 4;
    let v1 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    t += (v1 as DPINT) << 4;
    let v2 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p3 as DPINT);
    t += (v2 as DPINT) << 4;
    let v3 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[4] as DPINT);
    t += (c[1] as DPINT) * (b[3] as DPINT);
    t += (c[2] as DPINT) * (b[2] as DPINT);
    t += (c[3] as DPINT) * (b[1] as DPINT);
    t += (c[4] as DPINT) * (b[0] as DPINT);
    t += (v0 as DPINT) * (p4 as DPINT);
    t += (v1 as DPINT) * (p3 as DPINT);
    t += (v3 as DPINT) << 4;
    let v4 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    t += (q - v0) as DPINT;
    t += (v1 as DPINT) * (p4 as DPINT);
    t += (v2 as DPINT) * (p3 as DPINT);
    t += (v4 as DPINT) << 4;
    let v5 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    t += (v5 as DPINT) << 4;
    t += s as DPINT;
    let v6 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    t += (v6 as DPINT) << 4;
    t += s as DPINT;
    let v7 = ((t as SPINT) & mask) as SPINT;
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
    s = mask as SPINT;
    s -= v3;
    t += (v4 as DPINT) * (p4 as DPINT);
    t += (v5 as DPINT) * (p3 as DPINT);
    t += (v7 as DPINT) << 4;
    t += s as DPINT;
    let v8 = ((t as SPINT) & mask) as SPINT;
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
    s -= v4;
    t += (v5 as DPINT) * (p4 as DPINT);
    t += (v6 as DPINT) * (p3 as DPINT);
    t += (v8 as DPINT) << 4;
    t += s as DPINT;
    let v9 = ((t as SPINT) & mask) as SPINT;
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
    s -= v5;
    t += (v6 as DPINT) * (p4 as DPINT);
    t += (v7 as DPINT) * (p3 as DPINT);
    t += (v9 as DPINT) << 4;
    t += s as DPINT;
    let v10 = ((t as SPINT) & mask) as SPINT;
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
    s -= v6;
    t += (v7 as DPINT) * (p4 as DPINT);
    t += (v8 as DPINT) * (p3 as DPINT);
    t += (v10 as DPINT) << 4;
    t += s as DPINT;
    let v11 = ((t as SPINT) & mask) as SPINT;
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
    s -= v7;
    t += (v8 as DPINT) * (p4 as DPINT);
    t += (v9 as DPINT) * (p3 as DPINT);
    t += (v11 as DPINT) << 4;
    t += s as DPINT;
    let v12 = ((t as SPINT) & mask) as SPINT;
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
    t += (v0 as DPINT) << 20;
    s -= v8;
    t += (v9 as DPINT) * (p4 as DPINT);
    t += (v10 as DPINT) * (p3 as DPINT);
    t += (v12 as DPINT) << 4;
    t += s as DPINT;
    let v13 = ((t as SPINT) & mask) as SPINT;
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
    s = mask as SPINT;
    t += (v1 as DPINT) << 20;
    s -= v9;
    t += (v10 as DPINT) * (p4 as DPINT);
    t += (v11 as DPINT) * (p3 as DPINT);
    t += (v13 as DPINT) << 4;
    t += s as DPINT;
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
    s = mask as SPINT;
    t += (v2 as DPINT) << 20;
    s -= v10;
    t += (v11 as DPINT) * (p4 as DPINT);
    t += (v12 as DPINT) * (p3 as DPINT);
    t += s as DPINT;
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
    s = mask as SPINT;
    t += (v3 as DPINT) << 20;
    s -= v11;
    t += (v12 as DPINT) * (p4 as DPINT);
    t += (v13 as DPINT) * (p3 as DPINT);
    t += s as DPINT;
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
    s = mask as SPINT;
    t += (v4 as DPINT) << 20;
    s -= v12;
    t += (v13 as DPINT) * (p4 as DPINT);
    t += s as DPINT;
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
    s = mask as SPINT;
    t += (v5 as DPINT) << 20;
    s -= v13;
    t += s as DPINT;
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
    s = mask as SPINT;
    t += (v6 as DPINT) << 20;
    t += s as DPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[7] as DPINT) * (b[13] as DPINT);
    t += (c[8] as DPINT) * (b[12] as DPINT);
    t += (c[9] as DPINT) * (b[11] as DPINT);
    t += (c[10] as DPINT) * (b[10] as DPINT);
    t += (c[11] as DPINT) * (b[9] as DPINT);
    t += (c[12] as DPINT) * (b[8] as DPINT);
    t += (c[13] as DPINT) * (b[7] as DPINT);
    s = mask as SPINT;
    t += (v7 as DPINT) << 20;
    t += s as DPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[8] as DPINT) * (b[13] as DPINT);
    t += (c[9] as DPINT) * (b[12] as DPINT);
    t += (c[10] as DPINT) * (b[11] as DPINT);
    t += (c[11] as DPINT) * (b[10] as DPINT);
    t += (c[12] as DPINT) * (b[9] as DPINT);
    t += (c[13] as DPINT) * (b[8] as DPINT);
    s = mask as SPINT;
    t += (v8 as DPINT) << 20;
    t += s as DPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[9] as DPINT) * (b[13] as DPINT);
    t += (c[10] as DPINT) * (b[12] as DPINT);
    t += (c[11] as DPINT) * (b[11] as DPINT);
    t += (c[12] as DPINT) * (b[10] as DPINT);
    t += (c[13] as DPINT) * (b[9] as DPINT);
    s = mask as SPINT;
    t += (v9 as DPINT) << 20;
    t += s as DPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[10] as DPINT) * (b[13] as DPINT);
    t += (c[11] as DPINT) * (b[12] as DPINT);
    t += (c[12] as DPINT) * (b[11] as DPINT);
    t += (c[13] as DPINT) * (b[10] as DPINT);
    s = mask as SPINT;
    t += (v10 as DPINT) << 20;
    t += s as DPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[11] as DPINT) * (b[13] as DPINT);
    t += (c[12] as DPINT) * (b[12] as DPINT);
    t += (c[13] as DPINT) * (b[11] as DPINT);
    s = mask as SPINT;
    t += (v11 as DPINT) << 20;
    t += s as DPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[12] as DPINT) * (b[13] as DPINT);
    t += (c[13] as DPINT) * (b[12] as DPINT);
    s = mask as SPINT;
    t += (v12 as DPINT) << 20;
    t += s as DPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[13] as DPINT) * (b[13] as DPINT);
    s = mask as SPINT;
    t += (v13 as DPINT) << 20;
    t += s as DPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t -= 1;
    c[13] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let p3 = 0xffff000 as SPINT;
    let p4 = 0xffeffff as SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    tot = (c[0] as UDPINT) * (c[0] as UDPINT);
    t = tot;
    let v0 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[1] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) << 4;
    let v1 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    tot += (c[1] as UDPINT) * (c[1] as UDPINT);
    t += tot;
    t += (v1 as UDPINT) << 4;
    let v2 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[3] as UDPINT);
    tot += (c[1] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    t += tot;
    t += (v0 as UDPINT) * (p3 as UDPINT);
    t += (v2 as UDPINT) << 4;
    let v3 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[4] as UDPINT);
    tot += (c[1] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    tot += (c[2] as UDPINT) * (c[2] as UDPINT);
    t += tot;
    t += (v0 as UDPINT) * (p4 as UDPINT);
    t += (v1 as UDPINT) * (p3 as UDPINT);
    t += (v3 as UDPINT) << 4;
    let v4 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[5] as UDPINT);
    tot += (c[1] as UDPINT) * (c[4] as UDPINT);
    tot += (c[2] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    t += tot;
    t += (q - v0) as UDPINT;
    t += (v1 as UDPINT) * (p4 as UDPINT);
    t += (v2 as UDPINT) * (p3 as UDPINT);
    t += (v4 as UDPINT) << 4;
    let v5 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    t += (v5 as UDPINT) << 4;
    t += s as UDPINT;
    let v6 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
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
    t += (v6 as UDPINT) << 4;
    t += s as UDPINT;
    let v7 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[8] as UDPINT);
    tot += (c[1] as UDPINT) * (c[7] as UDPINT);
    tot += (c[2] as UDPINT) * (c[6] as UDPINT);
    tot += (c[3] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    tot += (c[4] as UDPINT) * (c[4] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s -= v3;
    t += (v4 as UDPINT) * (p4 as UDPINT);
    t += (v5 as UDPINT) * (p3 as UDPINT);
    t += (v7 as UDPINT) << 4;
    t += s as UDPINT;
    let v8 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[9] as UDPINT);
    tot += (c[1] as UDPINT) * (c[8] as UDPINT);
    tot += (c[2] as UDPINT) * (c[7] as UDPINT);
    tot += (c[3] as UDPINT) * (c[6] as UDPINT);
    tot += (c[4] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s -= v4;
    t += (v5 as UDPINT) * (p4 as UDPINT);
    t += (v6 as UDPINT) * (p3 as UDPINT);
    t += (v8 as UDPINT) << 4;
    t += s as UDPINT;
    let v9 = ((t as SPINT) & mask) as SPINT;
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
    s -= v5;
    t += (v6 as UDPINT) * (p4 as UDPINT);
    t += (v7 as UDPINT) * (p3 as UDPINT);
    t += (v9 as UDPINT) << 4;
    t += s as UDPINT;
    let v10 = ((t as SPINT) & mask) as SPINT;
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
    s -= v6;
    t += (v7 as UDPINT) * (p4 as UDPINT);
    t += (v8 as UDPINT) * (p3 as UDPINT);
    t += (v10 as UDPINT) << 4;
    t += s as UDPINT;
    let v11 = ((t as SPINT) & mask) as SPINT;
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
    s -= v7;
    t += (v8 as UDPINT) * (p4 as UDPINT);
    t += (v9 as UDPINT) * (p3 as UDPINT);
    t += (v11 as UDPINT) << 4;
    t += s as UDPINT;
    let v12 = ((t as SPINT) & mask) as SPINT;
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
    t += (v0 as UDPINT) << 20;
    s -= v8;
    t += (v9 as UDPINT) * (p4 as UDPINT);
    t += (v10 as UDPINT) * (p3 as UDPINT);
    t += (v12 as UDPINT) << 4;
    t += s as UDPINT;
    let v13 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[1] as UDPINT) * (c[13] as UDPINT);
    tot += (c[2] as UDPINT) * (c[12] as UDPINT);
    tot += (c[3] as UDPINT) * (c[11] as UDPINT);
    tot += (c[4] as UDPINT) * (c[10] as UDPINT);
    tot += (c[5] as UDPINT) * (c[9] as UDPINT);
    tot += (c[6] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    tot += (c[7] as UDPINT) * (c[7] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v1 as UDPINT) << 20;
    s -= v9;
    t += (v10 as UDPINT) * (p4 as UDPINT);
    t += (v11 as UDPINT) * (p3 as UDPINT);
    t += (v13 as UDPINT) << 4;
    t += s as UDPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[2] as UDPINT) * (c[13] as UDPINT);
    tot += (c[3] as UDPINT) * (c[12] as UDPINT);
    tot += (c[4] as UDPINT) * (c[11] as UDPINT);
    tot += (c[5] as UDPINT) * (c[10] as UDPINT);
    tot += (c[6] as UDPINT) * (c[9] as UDPINT);
    tot += (c[7] as UDPINT) * (c[8] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v2 as UDPINT) << 20;
    s -= v10;
    t += (v11 as UDPINT) * (p4 as UDPINT);
    t += (v12 as UDPINT) * (p3 as UDPINT);
    t += s as UDPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[3] as UDPINT) * (c[13] as UDPINT);
    tot += (c[4] as UDPINT) * (c[12] as UDPINT);
    tot += (c[5] as UDPINT) * (c[11] as UDPINT);
    tot += (c[6] as UDPINT) * (c[10] as UDPINT);
    tot += (c[7] as UDPINT) * (c[9] as UDPINT);
    tot *= 2;
    tot += (c[8] as UDPINT) * (c[8] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v3 as UDPINT) << 20;
    s -= v11;
    t += (v12 as UDPINT) * (p4 as UDPINT);
    t += (v13 as UDPINT) * (p3 as UDPINT);
    t += s as UDPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[4] as UDPINT) * (c[13] as UDPINT);
    tot += (c[5] as UDPINT) * (c[12] as UDPINT);
    tot += (c[6] as UDPINT) * (c[11] as UDPINT);
    tot += (c[7] as UDPINT) * (c[10] as UDPINT);
    tot += (c[8] as UDPINT) * (c[9] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v4 as UDPINT) << 20;
    s -= v12;
    t += (v13 as UDPINT) * (p4 as UDPINT);
    t += s as UDPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[5] as UDPINT) * (c[13] as UDPINT);
    tot += (c[6] as UDPINT) * (c[12] as UDPINT);
    tot += (c[7] as UDPINT) * (c[11] as UDPINT);
    tot += (c[8] as UDPINT) * (c[10] as UDPINT);
    tot *= 2;
    tot += (c[9] as UDPINT) * (c[9] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v5 as UDPINT) << 20;
    s -= v13;
    t += s as UDPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[6] as UDPINT) * (c[13] as UDPINT);
    tot += (c[7] as UDPINT) * (c[12] as UDPINT);
    tot += (c[8] as UDPINT) * (c[11] as UDPINT);
    tot += (c[9] as UDPINT) * (c[10] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v6 as UDPINT) << 20;
    t += s as UDPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[7] as UDPINT) * (c[13] as UDPINT);
    tot += (c[8] as UDPINT) * (c[12] as UDPINT);
    tot += (c[9] as UDPINT) * (c[11] as UDPINT);
    tot *= 2;
    tot += (c[10] as UDPINT) * (c[10] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v7 as UDPINT) << 20;
    t += s as UDPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[8] as UDPINT) * (c[13] as UDPINT);
    tot += (c[9] as UDPINT) * (c[12] as UDPINT);
    tot += (c[10] as UDPINT) * (c[11] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v8 as UDPINT) << 20;
    t += s as UDPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[9] as UDPINT) * (c[13] as UDPINT);
    tot += (c[10] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    tot += (c[11] as UDPINT) * (c[11] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v9 as UDPINT) << 20;
    t += s as UDPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[10] as UDPINT) * (c[13] as UDPINT);
    tot += (c[11] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v10 as UDPINT) << 20;
    t += s as UDPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[11] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    tot += (c[12] as UDPINT) * (c[12] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v11 as UDPINT) << 20;
    t += s as UDPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[12] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    t += (v12 as UDPINT) << 20;
    t += s as UDPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[13] as UDPINT) * (c[13] as UDPINT);
    t += tot;
    s = mask as SPINT;
    t += (v13 as UDPINT) << 20;
    t += s as UDPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t -= 1;
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
    modcpy(w, &mut x);
    modcpy(&x, &mut z);
    modsqr(&mut z);
    modmul(&x, &mut z);
    modsqr(&mut z);
    modcpy(&x, &mut t1);
    modmul(&z, &mut t1);
    modcpy(&t1, &mut z);
    modnsqr(&mut z, 3);
    modmul(&t1, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 6);
    modmul(&z, &mut t0);
    modcpy(&t0, &mut t2);
    modnsqr(&mut t2, 12);
    modmul(&t2, &mut t0);
    modnsqr(&mut t0, 6);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modsqr(&mut t0);
    modcpy(&x, &mut t2);
    modmul(&t0, &mut t2);
    modcpy(&t2, &mut t0);
    modsqr(&mut t0);
    modmul(&x, &mut t0);
    modcpy(&t0, &mut t3);
    modnsqr(&mut t3, 31);
    modmul(&t3, &mut t2);
    modcpy(&t2, &mut t3);
    modnsqr(&mut t3, 63);
    modmul(&t3, &mut t2);
    modcpy(&t2, &mut t3);
    modnsqr(&mut t3, 126);
    modmul(&t3, &mut t2);
    modnsqr(&mut t2, 3);
    modmul(&t2, &mut t1);
    modnsqr(&mut t1, 33);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 94);
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
        0x10000, 0xfe00000, 0xffffff, 0x0, 0x2, 0x0, 0xffffe00, 0xfff, 0x20000, 0x100000, 0x0, 0x0,
        0x0, 0x0,
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
    return modis1(&r);
}

//conditional move g to f if d=1
fn modcmv(d: usize, g: &[SPINT], f: &mut [SPINT]) {
    let r0 = f[0] ^ g[1];
    let r1 = f[1] ^ g[0];
    let dd = d as SPINT;
    let c0=1-(dd-r0);
    let c1=dd+r1;
    for i in 0..14 {
        let t = f[i];
        f[i] = c0 * t + c1 * g[i];
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
    for i in 0..14 {
        let t = f[i];
        let s = g[i];
        f[i] = c0 * t + c1 * s;
        g[i] = c0 * s + c1 * t;
        f[i] -= r0 * t + r1 * s;
        g[i] -= r0 * s + r1 * t;
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
    a[14 - 1] = (a[14 - 1] << n) | (a[14 - 2] >> (28 - n));
    for i in (1..14 - 1).rev() {
        a[i] = ((a[i] << n) & 0xfffffff) | (a[i - 1] >> (28 - n));
    }
    a[0] = (a[0] << n) & 0xfffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..14 - 1 {
        a[i] = (a[i] >> n) | ((a[i + 1] << (28 - n)) & 0xfffffff);
    }
    a[14 - 1] = a[14 - 1] >> n;
    return r as isize;
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
    return res;
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
const NBITS: usize = 384;
const NBYTES: usize = 48;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = false;

/*** Insert automatically generated definition for curve curve.rs here ***/

const COF:usize = 1;
const CONSTANT_A: isize = -3;
const CONSTANT_B: isize = 0;
const constant_b: [SPINT;14]=[0x12dcccd,0x8870d04,0x2ec0811,0xd9474c3,0xfc429ad,0x1920022,0x7f2209b,0x938ae27,0x74bee94,0x2094e33,0x1f41f02,0xf9b62b2,0xb604fbf,0x8114];
const constant_b3: [SPINT;14]=[0x3896667,0x995270c,0x8c41834,0x8bd5e49,0xf4c7d09,0x4b60068,0x7d661d1,0xbaa0a76,0x5e3cbbd,0x61bea9a,0x5dc5d06,0xed22816,0x220ef3f,0x1833e];
const CONSTANT_X: usize = 0;
const constant_x: [SPINT;14]=[0xb5284d,0x7565fcc,0xe383dd0,0xe2edd6c,0xe6de378,0x541b4d6,0x79c3afc,0xa30eff8,0x4868459,0xde2b645,0x723614e,0x13812ff,0x2299e15,0x3aadc];
const constant_y: [SPINT;14]=[0x3a4fe2b,0x3dad200,0x9ac2304,0xbfa6b4a,0xccbfa8,0x2e83b05,0xbade756,0xf4ffd98,0x3521968,0xa840c6c,0x2263969,0xe9dd800,0x25a15c5,0x78abc];


/*** End of automatically generated code ***/


/*** Insert automatically generated point definition point.rs here ***/

// elliptic curve point in projective coordinates
const WORDLENGTH: usize = 32;
#[derive(Clone)]
pub struct ECP {
	x: [u32;14],
	y: [u32;14],
	z: [u32;14],
}

#[allow(non_snake_case)]
impl ECP {
	pub fn new() -> ECP {
		ECP {
			x: [0;14],
			y: [0;14],
			z: [0;14],
		}
	}
}


/*** End of automatically generated code ***/

fn teq(b: usize,c: usize) -> usize {
    let mut x=b^c;
    x-=1;
    return (x>>(usize::BITS-1))&1;
}

pub fn ecncpy(Q: &ECP,P: &mut ECP) {
    modcpy(&Q.x,&mut P.x);
    modcpy(&Q.y,&mut P.y);
    modcpy(&Q.z,&mut P.z);
}

pub fn ecnneg(P: &mut ECP) {
    modneg(&mut P.y);
}

// add Q to P
// standard projective method from EFD - https://www.hyperelliptic.org/EFD/
pub fn ecnadd(Q: &ECP,P: &mut ECP) {
    let mut b:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t0:[SPINT;NLIMBS]=[0;NLIMBS];     
    let mut t1:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t2:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t3:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t4:[SPINT;NLIMBS]=[0;NLIMBS];   

    modcpy(&P.x,&mut t0); modmul(&Q.x,&mut t0);
    modcpy(&P.y,&mut t1); modmul(&Q.y,&mut t1);
    modcpy(&P.z,&mut t2); modmul(&Q.z,&mut t2);
    
    modcpy(&P.x,&mut t3); modadd(&P.y,&mut t3);
    modcpy(&Q.x,&mut t4); modadd(&Q.y,&mut t4);
    modmul(&t4,&mut t3);

    modcpy(&t0,&mut t4); modadd(&t1,&mut t4);
    modsub(&mut t4,&mut t3);
    modcpy(&P.y,&mut t4); modadd(&P.z,&mut t4);

    modcpy(&Q.y,&mut b); modadd(&Q.z,&mut b);
    modmul(&b,&mut t4);
    modcpy(&t1,&mut b); modadd(&t2,&mut b);
    
    modsub(&b, &mut t4);
    modadd(&P.z,&mut P.x);
    modcpy(&Q.z,&mut P.y); modadd(&Q.x,&mut P.y);

    modmul(&P.y,&mut P.x);
    modcpy(&t0,&mut P.y); modadd(&t2,&mut P.y);
    modsub(&P.x,&mut P.y); modneg(&mut P.y);// P.y is negative!

    if CONSTANT_A==0 {
        modcpy(&t0,&mut P.x); modadd(&t0,&mut P.x);
        modadd(&P.x,&mut t0);
        if CONSTANT_B>0 {
            modmli(3*CONSTANT_B as usize,&mut t2);
            modmli(3*CONSTANT_B as usize,&mut P.y);
        }
        if CONSTANT_B<0 {
            modmli((-3*CONSTANT_B) as usize,&mut t2); modneg(&mut t2);
            modmli((-3*CONSTANT_B) as usize,&mut P.y); modneg(&mut P.y);
        }
        if CONSTANT_B==0 {
            modcpy(&constant_b3,&mut b);
            modmul(&b, &mut t2);
            modmul(&b,&mut P.y); 
        }
        modcpy(&t1,&mut P.z); modadd(&t2,&mut P.z);
        modsub(&t2,&mut t1);

        modcpy(&P.y,&mut P.x); modmul(&t4,&mut P.x);
        modcpy(&t3,&mut t2); modmul(&t1,&mut t2);
        modsub(&t2,&mut P.x); modneg(&mut P.x);
        modmul(&t0,&mut P.y);
        modmul(&P.z,&mut t1);
        modadd(&t1,&mut P.y);
        modmul(&t3,&mut t0);
        modmul(&t4,&mut P.z);
        modadd(&t0,&mut P.z);
    } else {
        if CONSTANT_B>0 {
            modcpy(&t2,&mut P.z); modmli(CONSTANT_B as usize,&mut P.z);
            modcpy(&P.y,&mut P.x); modsub(&P.z,&mut P.x);
            modmli(CONSTANT_B as usize,&mut P.y);
        }
        if CONSTANT_B<0 {
            modcpy(&t2,&mut P.z); modmli((-CONSTANT_B) as usize,&mut P.z);
            modcpy(&P.y,&mut P.x); modadd(&P.z,&mut P.x);
            modmli((-CONSTANT_B) as usize,&mut P.y); modneg(&mut P.y);
        }
        if CONSTANT_B==0 {
            modcpy(&constant_b,&mut b);
            modcpy(&b,&mut P.z); modmul(&t2,&mut P.z);
            modcpy(&P.y,&mut P.x); modsub(&P.z,&mut P.x);
            modmul(&b,&mut P.y);
        }
        modcpy(&P.x,&mut P.z); modadd(&P.x,&mut P.z);

        modadd(&P.z,&mut P.x);
        modcpy(&t1,&mut P.z); modsub(&P.x,&mut P.z);
        modadd(&t1,&mut P.x);

        modcpy(&t2,&mut t1);  modadd(&t2,&mut t1);
        modadd(&t1,&mut t2);

        modsub(&t2,&mut P.y);
        modsub(&t0,&mut P.y);
        modcpy(&P.y,&mut t1); modadd(&P.y,&mut t1); 

        modadd(&t1,&mut P.y);
        modcpy(&t0,&mut t1); modadd(&t0,&mut t1);
        modadd(&t1,&mut t0);

        modsub(&t2,&mut t0);
        modcpy(&t4,&mut t1); modmul(&P.y,&mut t1);
        modcpy(&t0,&mut t2); modmul(&P.y,&mut t2);

        modcpy(&P.x,&mut P.y); modmul(&P.z,&mut P.y);
        modadd(&t2,&mut P.y);
        modmul(&t3,&mut P.x);

        modsub(&t1,&mut P.x);
        modmul(&t4,&mut P.z);
        modcpy(&t3,&mut t1); modmul(&t0,&mut t1);

        modadd(&t1,&mut P.z);
    }
}

pub fn ecnsub(Q: &ECP,P: &mut ECP) {
    let mut W = ECP::new();  
    ecncpy(Q,&mut W); ecnneg(&mut W);
    ecnadd(&W,P);
}

// double P
// standard projective method from EFD - https://www.hyperelliptic.org/EFD/
pub fn ecndbl(P: &mut ECP) {
    let mut b:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t0:[SPINT;NLIMBS]=[0;NLIMBS];     
    let mut t1:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t2:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t3:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut t4:[SPINT;NLIMBS]=[0;NLIMBS]; 

    if CONSTANT_A==0 {
        modcpy(&P.y,&mut t0); modsqr(&mut t0);
        modcpy(&t0,&mut t3); modadd(&t0,&mut t3);
        modcpy(&t3,&mut t1); modadd(&t3,&mut t1);
        modcpy(&t1,&mut t3); modadd(&t1,&mut t3);
        modcpy(&P.x,&mut t4); modmul(&P.y,&mut t4);
        modcpy(&P.y,&mut t1); modmul(&P.z,&mut t1);
        modcpy(&P.z,&mut t2); modsqr(&mut t2);
        if CONSTANT_B>0 {
            modmli(3*CONSTANT_B as usize,&mut t2);
        }
        if CONSTANT_B<0 {
            modmli((-3*CONSTANT_B) as usize,&mut t2); modneg(&mut t2);
        }
        if CONSTANT_B==0 {
            modcpy(&constant_b3,&mut b);
            modmul(&b,&mut t2);
        }
        modcpy(&t2,&mut P.x); modmul(&t3,&mut P.x);
        modcpy(&t0,&mut P.y); modadd(&t2,&mut P.y);
        modcpy(&t3,&mut P.z); modmul(&t1,&mut P.z);

        modcpy(&t2,&mut t1); modadd(&t2,&mut t1);
        modadd(&t1,&mut t2);
        modsub(&t2,&mut t0);
        modmul(&t0,&mut P.y);
        modadd(&P.x,&mut P.y);
        modcpy(&t0,&mut P.x); modmul(&t4,&mut P.x);
        modcpy(&P.x,&mut t0); modadd(&t0,&mut P.x);
    } else {
        modcpy(&P.x,&mut t0); modsqr(&mut t0);
        modcpy(&P.y,&mut t1); modsqr(&mut t1);
        modcpy(&P.z,&mut t2); modsqr(&mut t2);

        modcpy(&P.x,&mut t3); modmul(&P.y,&mut t3);
        modcpy(&P.y,&mut t4); modmul(&P.z,&mut t4);
        modcpy(&t3,&mut b); modadd(&b,&mut t3);
        modmul(&P.x,&mut P.z);
        modcpy(&P.z,&mut b); modadd(&b,&mut P.z);
        if CONSTANT_B>0 {
            modcpy(&t2,&mut P.y); modmli(CONSTANT_B as usize,&mut P.y);
            modsub(&P.z,&mut P.y);
            modmli(CONSTANT_B as usize,&mut P.z);
        }
        if CONSTANT_B<0 {
            modcpy(&t2,&mut P.y); modmli((-CONSTANT_B) as usize,&mut P.y);  modneg(&mut P.y);
            modsub(&P.z,&mut P.y);
            modmli((-CONSTANT_B) as usize,&mut P.z); modneg(&mut P.z);
        }
        if CONSTANT_B==0 {
            modcpy(&constant_b,&mut b);
            modcpy(&t2,&mut P.y); modmul(&b,&mut P.y);
            modsub(&P.z,&mut P.y);
            modmul(&b,&mut P.z);
        }
        modcpy(&P.y,&mut P.x); modadd(&P.y,&mut P.x);
        modadd(&P.x,&mut P.y);
        modcpy(&t1,&mut P.x); modsub(&P.y,&mut P.x);

        modadd(&t1,&mut P.y);
        modmul(&P.x,&mut P.y);
        modmul(&t3,&mut P.x);

        modcpy(&t2,&mut t3); modadd(&t2,&mut t3);
        modadd(&t3,&mut t2);

        modsub(&t2,&mut P.z);
        modsub(&t0,&mut P.z);
        modcpy(&P.z,&mut t3); modadd(&P.z,&mut t3);

        modadd(&t3,&mut P.z);
        modcpy(&t0,&mut t3); modadd(&t0,&mut t3);
        modadd(&t3,&mut t0);

        modsub(&t2,&mut t0);
        modmul(&P.z,&mut t0);
        modadd(&t0,&mut P.y);

        modcpy(&t4,&mut b); modadd(&b,&mut t4);
        modmul(&t4,&mut P.z);

        modsub(&P.z,&mut P.x);
        modcpy(&t4,&mut P.z); modmul(&t1,&mut P.z);
        modcpy(&P.z,&mut b); modadd(&b,&mut P.z);
        modcpy(&P.z,&mut b); modadd(&b,&mut P.z);
    }
}

// set to infinity
pub fn ecninf(P: &mut ECP) {
    modzer(&mut P.x);
    modone(&mut P.y);
    modzer(&mut P.z);
}

// test for infinity
pub fn ecnisinf(P: &ECP) -> bool {
    return modis0(&P.x) && modis0(&P.z);
}

// set to affine
pub fn ecnaffine(P: &mut ECP) {
    let mut i:[SPINT;NLIMBS]=[0;NLIMBS]; 
    if modis0(&P.z) {
        modzer(&mut P.x);
        modone(&mut P.y);        
    }
    modcpy(&P.z,&mut i); modinv(None,&mut i);
    modone(&mut P.z);
    modmul(&i,&mut P.x);
    modmul(&i,&mut P.y);
}

// move Q to P if d=1
fn ecncmv(d: usize,Q: &ECP,P: &mut ECP) {
    modcmv(d,&Q.x,&mut P.x);
    modcmv(d,&Q.y,&mut P.y);
    modcmv(d,&Q.z,&mut P.z);
}

// return true if equal, else false
pub fn ecncmp(Q: &ECP,P: &ECP) -> bool {
    let mut a:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut b:[SPINT;NLIMBS]=[0;NLIMBS];  
    modcpy(&P.x,&mut a); modmul(&Q.z,&mut a);
    modcpy(&Q.x,&mut b); modmul(&P.z,&mut b);
    if !modcmp(&a,&b) {
        return false;
    }
    modcpy(&P.y,&mut a); modmul(&Q.z,&mut a);
    modcpy(&Q.y,&mut b); modmul(&P.z,&mut b);
    if !modcmp(&a,&b) {
        return false;
    }
    return true;
}

// extract (x,y) from point, if y is NULL compress and just return x and sign of y
pub fn ecnget(P: &mut ECP,x: &mut [u8],y: Option<&mut [u8]>) -> usize {
    let mut sx:[SPINT;NLIMBS]=[0;NLIMBS];  
    let mut sy:[SPINT;NLIMBS]=[0;NLIMBS]; 
    ecnaffine(P);
    modcpy(&P.x,&mut sx);
    modexp(&sx,x);
    if let Some(ry) = y {
        modcpy(&P.y,&mut sy);
        modexp(&sy,ry);
        return 0;
    } else {
        return modsign(&P.y); 
    }
}

// weierstrass set point function
// sets P=O if point not on curve
// if y!=NULL tries to set (x,y)
// if y==NULL calculates y (decompresses x) and selects sign from s=0/1

fn setxy(s: usize,x: &[SPINT],y: Option<&[SPINT]>,P: &mut ECP) {
    let mut t:[SPINT;NLIMBS]=[0;NLIMBS];     
    let mut v:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut h:[SPINT;NLIMBS]=[0;NLIMBS];    
    modcpy(x,&mut P.x);
    modcpy(x,&mut v); modsqr(&mut v);
    modmul(x,&mut v); // x^3
    if CONSTANT_A==-3 {
        modsub(x,&mut v);
        modsub(x,&mut v);
        modsub(x,&mut v); // x^3-3x
    }  
    if CONSTANT_B>0 {
        modint(CONSTANT_B as usize,&mut t);
        modadd(&t,&mut v); // V=1+dx^2
    }
    if CONSTANT_B<0 {
        modint((-CONSTANT_B) as usize,&mut t);
        modsub(&t,&mut v);
    }
    if CONSTANT_B==0 {
        modadd(&constant_b,&mut v);
    }
    if let Some(ry)=y {
        modcpy(&ry,&mut t); modsqr(&mut t);
        if modcmp(&t,&v) {
            modcpy(&ry,&mut P.y);
            modone(&mut P.z);
        } else {
            ecninf(P);
        }
        return;
    }
    modpro(&v,&mut h);
    if !modqr(Some(&h),&v)
    { // point not on curve
        ecninf(P);
        return;
    }
    modsqrt(&v,Some(&h),&mut P.y);
    let d=(modsign(&P.y)-s)&1;
    modcpy(&P.y,&mut t); modneg(&mut t);
    modcmv(d,&t,&mut P.y);
    modone(&mut P.z);
}

// multiply point by small curve cofactor (here assumed to be 4 or 8)
pub fn ecncof(_P: &mut ECP) {
}

// Is (x,y) of the right order? Must be checked by calling program!
// api visible version, x and y are big endian byte arrays
pub fn ecnset(s: usize,x: &[u8],y: Option<&[u8]>,P: &mut ECP) {
    let mut sx:[SPINT;NLIMBS]=[0;NLIMBS];     
    let mut sy:[SPINT;NLIMBS]=[0;NLIMBS];   
    modimp(&x,&mut sx);
    if let Some(ry)=y {
        modimp(&ry,&mut sy);
        setxy(s,&sx,Some(&sy),P);
        return;
    }        
    setxy(s,&sx,None,P);    
}

// set generator
pub fn ecngen(P: &mut ECP) {
    if CONSTANT_X!=0 {
        let mut sx:[SPINT;NLIMBS]=[0;NLIMBS];  
        modint(CONSTANT_X,&mut sx);
        setxy(0,&sx,None,P);
    } 
    if CONSTANT_X==0 {
        setxy(0,&constant_x,Some(&constant_y),P);
    }
}

// select point from precomputed array in constant time
fn select(b:isize,W: &[ECP],P: &mut ECP) {
    let mut MP=ECP::new();
    let m=b>>(isize::BITS-1);
    let babs=((b^m)-m) as usize;

    ecncmv(teq(babs,0),&W[0],P);
    ecncmv(teq(babs,1),&W[1],P);
    ecncmv(teq(babs,2),&W[2],P);
    ecncmv(teq(babs,3),&W[3],P);
    ecncmv(teq(babs,4),&W[4],P);
    ecncmv(teq(babs,5),&W[5],P);
    ecncmv(teq(babs,6),&W[6],P);
    ecncmv(teq(babs,7),&W[7],P);
    ecncmv(teq(babs,8),&W[8],P);
    
    ecncpy(P,&mut MP);
    ecnneg(&mut MP);
    ecncmv((m&1) as usize,&MP,P);
}

// convert to double naf form
fn dnaf(e: &[u8],f: &[u8],w: &mut [i8]) {
    let mut ce:u8=0;
    let mut cf:u8=0;
    for i in 0..NBYTES {
        let mut m=e[NBYTES-i-1];
        let mut n=m;
        let mut t=3*(n as usize)+(ce as usize);
        ce=(t>>8) as u8;
        n=(t&0xff) as u8;
        let mut p=f[NBYTES-i-1];
        let mut q=p;
        t=3*(q as usize)+(cf as usize);
        cf=(t>>8) as u8;
        q=(t&0xff) as u8;
        for j in 0..8 {
            w[8*i+j]=((n&1)-(m&1)+3*((q&1)-(p&1))) as i8;
            n>>=1; m>>=1; p>>=1; q>>=1;
        }
    }
    for j in 0..8 {
        w[8*NBYTES+j]=((ce&1)+3*(cf&1)) as i8;
        ce>>=1; cf>>=1;
    }
}

// point multiplication - constant time
pub fn ecnmul(e: &[u8],P: &mut ECP) {
    let mut Q=ECP::new();
    let mut W: [ECP; 9] = [
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
    ];  
    let mut w: [i8; 2*NBYTES+1] = [0; 2*NBYTES+1];

    ecninf(&mut W[0]);                      // O
    ecncpy(P,&mut W[1]);                    // P
    ecncpy(P,&mut W[2]); ecndbl(&mut W[2]);         // 2P
    ecncpy(&W[2],&mut Q); ecncpy(&Q,&mut W[3]); ecnadd(P,&mut W[3]);  // 3P
    ecncpy(&Q,&mut W[4]); ecndbl(&mut W[4]); // 4P
    ecncpy(&W[4],&mut Q); ecncpy(&Q,&mut W[5]); ecnadd(P,&mut W[5]); // 5P
    ecncpy(&Q,&mut W[8]); ecndbl(&mut W[8]); // 8P
    ecncpy(&W[3],&mut Q); ecndbl(&mut Q); ecncpy(&Q,&mut W[6]); // 6P
    ecncpy(&Q,&mut W[7]); ecnadd(P,&mut W[7]); // 7P

    let mut j=0;
    for i in 0..NBYTES {
        let c=e[NBYTES-i-1] as i8;
        w[j]=c&0x0f;
        w[j+1]=(c>>4)&0xf;
        j+=2;
    }
    w[2*NBYTES]=0;
    for j in 0..2*NBYTES {
        let t=7-w[j];
        let m=(t>>4)&1;
        w[j]-=m<<4;
        w[j+1]+=m;
    }
    select(w[2*NBYTES] as isize,&W,P);
    for i in (0..2*NBYTES).rev() {
        select(w[i] as isize,&W,&mut Q);
        ecndbl(P);
        ecndbl(P);
        ecndbl(P);
        ecndbl(P);
        ecnadd(&Q,P);
    }
}

// double point multiplication R=eP+fQ
// not constant time
pub fn ecnmul2(e: &[u8],P: &ECP,f: &[u8],Q: &ECP) -> ECP {
    let mut R=ECP::new();
    let mut W: [ECP; 5] = [
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
        ECP::new(),
    ]; 
    let mut w: [i8; 8*NBYTES+8] = [0; 8*NBYTES+8]; 
    ecninf(&mut W[0]);      // O
    ecncpy(P,&mut W[1]);    // P
    ecncpy(Q,&mut W[3]);    // Q
    ecncpy(Q,&mut W[2]); ecnsub(P,&mut W[2]);    // Q-P
    ecncpy(Q,&mut W[4]); ecnadd(P,&mut W[4]);    // Q+P

    dnaf(&e,&f,&mut w);
    let mut i=8*NBYTES+7;
    while w[i]==0 {  // ignore leading zeros
        i-=1;
    }
    ecninf(&mut R);
    while i>=1 {
        ecndbl(&mut R);
        let j=w[i];
        if j>0 {
            ecnadd(&W[j as usize],&mut R);
        }
        if j<0 {
            ecnsub(&W[(-j) as usize],&mut R);
        }
        i-=1;
    }
    return R;
}


