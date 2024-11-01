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
    let mask = ((1 as SPINT) << 29) - 1;
    let mut carry = (n[0] as SSPINT) >> 29;
    n[0] &= mask;
    for i in 1..17 {
        carry += n[i] as SSPINT;
        n[i] = (carry as SPINT) & mask;
        carry >>= 29;
    }
    n[17] += carry as SPINT;
    return ((n[17] as SSPINT) >> 31) as SPINT;
}

//propagate carries and add p if negative, propagate carries again
#[allow(unused_variables)]
#[inline]
fn flatten(n: &mut [SPINT]) -> bool {
    let carry = prop(n);
    n[0] -= (1) & (carry as SPINT);
    n[17] += (0x10000000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    n[0] += 1 as SPINT;
    n[17] -= 0x10000000 as SPINT;
    return flatten(n);
}

// Modular multiplication by an integer, c=c*b mod 2p
fn modmli(b: usize, c: &mut [SPINT]) {
    let mut t = 0 as UDPINT;
    let mask = ((1 as SPINT) << 29) - 1;
    t += (c[0] as UDPINT) * (b as UDPINT);
    let v0 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[1] as UDPINT) * (b as UDPINT);
    let v1 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[2] as UDPINT) * (b as UDPINT);
    let v2 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[3] as UDPINT) * (b as UDPINT);
    let v3 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[4] as UDPINT) * (b as UDPINT);
    let v4 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[5] as UDPINT) * (b as UDPINT);
    let v5 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[6] as UDPINT) * (b as UDPINT);
    let v6 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[7] as UDPINT) * (b as UDPINT);
    let v7 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[8] as UDPINT) * (b as UDPINT);
    let v8 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[9] as UDPINT) * (b as UDPINT);
    let v9 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[10] as UDPINT) * (b as UDPINT);
    let v10 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[11] as UDPINT) * (b as UDPINT);
    let v11 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[12] as UDPINT) * (b as UDPINT);
    let v12 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[13] as UDPINT) * (b as UDPINT);
    let v13 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[14] as UDPINT) * (b as UDPINT);
    let v14 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[15] as UDPINT) * (b as UDPINT);
    let v15 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[16] as UDPINT) * (b as UDPINT);
    let v16 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[17] as UDPINT) * (b as UDPINT);
    let v17 = (t as SPINT) & mask;
    t = t >> 29;
    // second reduction pass
    let mut nv = v17;
    let mut ut = t as UDPINT;
    ut = (ut << 1) + ((nv >> 28) as UDPINT);
    nv &= 0xfffffff;
    let s = v0 + ((ut as SPINT) & mask);
    c[0] = (s & mask) as SPINT;
    let carry = (s >> 29) + ((ut >> 29) as SPINT);
    c[1] = v1 + carry;
    c[2] = v2;
    c[3] = v3;
    c[4] = v4;
    c[5] = v5;
    c[6] = v6;
    c[7] = v7;
    c[8] = v8;
    c[9] = v9;
    c[10] = v10;
    c[11] = v11;
    c[12] = v12;
    c[13] = v13;
    c[14] = v14;
    c[15] = v15;
    c[16] = v16;
    c[17] = nv;
    return;
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
    n[0] += 2 as SPINT;
    n[17] -= 0x20000000 as SPINT;
    let carry = prop(n);
    n[0] -= (2) & (carry as SPINT);
    n[17] += (0x20000000 as SPINT) & (carry as SPINT);
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
    let carry = prop(n);
    n[0] -= (2) & (carry as SPINT);
    n[17] += (0x20000000 as SPINT) & (carry as SPINT);
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
    let carry = prop(n);
    n[0] -= (2) & (carry as SPINT);
    n[17] += (0x20000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mc1 = c[1] * 0x2;
    let mc2 = c[2] * 0x2;
    let mc3 = c[3] * 0x2;
    let mc4 = c[4] * 0x2;
    let mc5 = c[5] * 0x2;
    let mc6 = c[6] * 0x2;
    let mc7 = c[7] * 0x2;
    let mc8 = c[8] * 0x2;
    let mc9 = c[9] * 0x2;
    let mc10 = c[10] * 0x2;
    let mc11 = c[11] * 0x2;
    let mc12 = c[12] * 0x2;
    let mc13 = c[13] * 0x2;
    let mc14 = c[14] * 0x2;
    let mc15 = c[15] * 0x2;
    let mc16 = c[16] * 0x2;
    let mc17 = c[17] * 0x2;
    let mask = ((1 as SPINT) << 29) - 1;
    t += (mc1 as DPINT) * (b[17] as DPINT);
    t += (mc2 as DPINT) * (b[16] as DPINT);
    t += (mc3 as DPINT) * (b[15] as DPINT);
    t += (mc4 as DPINT) * (b[14] as DPINT);
    t += (mc5 as DPINT) * (b[13] as DPINT);
    t += (mc6 as DPINT) * (b[12] as DPINT);
    t += (mc7 as DPINT) * (b[11] as DPINT);
    t += (mc8 as DPINT) * (b[10] as DPINT);
    t += (mc9 as DPINT) * (b[9] as DPINT);
    t += (mc10 as DPINT) * (b[8] as DPINT);
    t += (mc11 as DPINT) * (b[7] as DPINT);
    t += (mc12 as DPINT) * (b[6] as DPINT);
    t += (mc13 as DPINT) * (b[5] as DPINT);
    t += (mc14 as DPINT) * (b[4] as DPINT);
    t += (mc15 as DPINT) * (b[3] as DPINT);
    t += (mc16 as DPINT) * (b[2] as DPINT);
    t += (mc17 as DPINT) * (b[1] as DPINT);
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc2 as DPINT) * (b[17] as DPINT);
    t += (mc3 as DPINT) * (b[16] as DPINT);
    t += (mc4 as DPINT) * (b[15] as DPINT);
    t += (mc5 as DPINT) * (b[14] as DPINT);
    t += (mc6 as DPINT) * (b[13] as DPINT);
    t += (mc7 as DPINT) * (b[12] as DPINT);
    t += (mc8 as DPINT) * (b[11] as DPINT);
    t += (mc9 as DPINT) * (b[10] as DPINT);
    t += (mc10 as DPINT) * (b[9] as DPINT);
    t += (mc11 as DPINT) * (b[8] as DPINT);
    t += (mc12 as DPINT) * (b[7] as DPINT);
    t += (mc13 as DPINT) * (b[6] as DPINT);
    t += (mc14 as DPINT) * (b[5] as DPINT);
    t += (mc15 as DPINT) * (b[4] as DPINT);
    t += (mc16 as DPINT) * (b[3] as DPINT);
    t += (mc17 as DPINT) * (b[2] as DPINT);
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    let v1 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc3 as DPINT) * (b[17] as DPINT);
    t += (mc4 as DPINT) * (b[16] as DPINT);
    t += (mc5 as DPINT) * (b[15] as DPINT);
    t += (mc6 as DPINT) * (b[14] as DPINT);
    t += (mc7 as DPINT) * (b[13] as DPINT);
    t += (mc8 as DPINT) * (b[12] as DPINT);
    t += (mc9 as DPINT) * (b[11] as DPINT);
    t += (mc10 as DPINT) * (b[10] as DPINT);
    t += (mc11 as DPINT) * (b[9] as DPINT);
    t += (mc12 as DPINT) * (b[8] as DPINT);
    t += (mc13 as DPINT) * (b[7] as DPINT);
    t += (mc14 as DPINT) * (b[6] as DPINT);
    t += (mc15 as DPINT) * (b[5] as DPINT);
    t += (mc16 as DPINT) * (b[4] as DPINT);
    t += (mc17 as DPINT) * (b[3] as DPINT);
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    let v2 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc4 as DPINT) * (b[17] as DPINT);
    t += (mc5 as DPINT) * (b[16] as DPINT);
    t += (mc6 as DPINT) * (b[15] as DPINT);
    t += (mc7 as DPINT) * (b[14] as DPINT);
    t += (mc8 as DPINT) * (b[13] as DPINT);
    t += (mc9 as DPINT) * (b[12] as DPINT);
    t += (mc10 as DPINT) * (b[11] as DPINT);
    t += (mc11 as DPINT) * (b[10] as DPINT);
    t += (mc12 as DPINT) * (b[9] as DPINT);
    t += (mc13 as DPINT) * (b[8] as DPINT);
    t += (mc14 as DPINT) * (b[7] as DPINT);
    t += (mc15 as DPINT) * (b[6] as DPINT);
    t += (mc16 as DPINT) * (b[5] as DPINT);
    t += (mc17 as DPINT) * (b[4] as DPINT);
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    let v3 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc5 as DPINT) * (b[17] as DPINT);
    t += (mc6 as DPINT) * (b[16] as DPINT);
    t += (mc7 as DPINT) * (b[15] as DPINT);
    t += (mc8 as DPINT) * (b[14] as DPINT);
    t += (mc9 as DPINT) * (b[13] as DPINT);
    t += (mc10 as DPINT) * (b[12] as DPINT);
    t += (mc11 as DPINT) * (b[11] as DPINT);
    t += (mc12 as DPINT) * (b[10] as DPINT);
    t += (mc13 as DPINT) * (b[9] as DPINT);
    t += (mc14 as DPINT) * (b[8] as DPINT);
    t += (mc15 as DPINT) * (b[7] as DPINT);
    t += (mc16 as DPINT) * (b[6] as DPINT);
    t += (mc17 as DPINT) * (b[5] as DPINT);
    t += (c[0] as DPINT) * (b[4] as DPINT);
    t += (c[1] as DPINT) * (b[3] as DPINT);
    t += (c[2] as DPINT) * (b[2] as DPINT);
    t += (c[3] as DPINT) * (b[1] as DPINT);
    t += (c[4] as DPINT) * (b[0] as DPINT);
    let v4 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc6 as DPINT) * (b[17] as DPINT);
    t += (mc7 as DPINT) * (b[16] as DPINT);
    t += (mc8 as DPINT) * (b[15] as DPINT);
    t += (mc9 as DPINT) * (b[14] as DPINT);
    t += (mc10 as DPINT) * (b[13] as DPINT);
    t += (mc11 as DPINT) * (b[12] as DPINT);
    t += (mc12 as DPINT) * (b[11] as DPINT);
    t += (mc13 as DPINT) * (b[10] as DPINT);
    t += (mc14 as DPINT) * (b[9] as DPINT);
    t += (mc15 as DPINT) * (b[8] as DPINT);
    t += (mc16 as DPINT) * (b[7] as DPINT);
    t += (mc17 as DPINT) * (b[6] as DPINT);
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    let v5 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc7 as DPINT) * (b[17] as DPINT);
    t += (mc8 as DPINT) * (b[16] as DPINT);
    t += (mc9 as DPINT) * (b[15] as DPINT);
    t += (mc10 as DPINT) * (b[14] as DPINT);
    t += (mc11 as DPINT) * (b[13] as DPINT);
    t += (mc12 as DPINT) * (b[12] as DPINT);
    t += (mc13 as DPINT) * (b[11] as DPINT);
    t += (mc14 as DPINT) * (b[10] as DPINT);
    t += (mc15 as DPINT) * (b[9] as DPINT);
    t += (mc16 as DPINT) * (b[8] as DPINT);
    t += (mc17 as DPINT) * (b[7] as DPINT);
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
    let v6 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc8 as DPINT) * (b[17] as DPINT);
    t += (mc9 as DPINT) * (b[16] as DPINT);
    t += (mc10 as DPINT) * (b[15] as DPINT);
    t += (mc11 as DPINT) * (b[14] as DPINT);
    t += (mc12 as DPINT) * (b[13] as DPINT);
    t += (mc13 as DPINT) * (b[12] as DPINT);
    t += (mc14 as DPINT) * (b[11] as DPINT);
    t += (mc15 as DPINT) * (b[10] as DPINT);
    t += (mc16 as DPINT) * (b[9] as DPINT);
    t += (mc17 as DPINT) * (b[8] as DPINT);
    t += (c[0] as DPINT) * (b[7] as DPINT);
    t += (c[1] as DPINT) * (b[6] as DPINT);
    t += (c[2] as DPINT) * (b[5] as DPINT);
    t += (c[3] as DPINT) * (b[4] as DPINT);
    t += (c[4] as DPINT) * (b[3] as DPINT);
    t += (c[5] as DPINT) * (b[2] as DPINT);
    t += (c[6] as DPINT) * (b[1] as DPINT);
    t += (c[7] as DPINT) * (b[0] as DPINT);
    let v7 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc9 as DPINT) * (b[17] as DPINT);
    t += (mc10 as DPINT) * (b[16] as DPINT);
    t += (mc11 as DPINT) * (b[15] as DPINT);
    t += (mc12 as DPINT) * (b[14] as DPINT);
    t += (mc13 as DPINT) * (b[13] as DPINT);
    t += (mc14 as DPINT) * (b[12] as DPINT);
    t += (mc15 as DPINT) * (b[11] as DPINT);
    t += (mc16 as DPINT) * (b[10] as DPINT);
    t += (mc17 as DPINT) * (b[9] as DPINT);
    t += (c[0] as DPINT) * (b[8] as DPINT);
    t += (c[1] as DPINT) * (b[7] as DPINT);
    t += (c[2] as DPINT) * (b[6] as DPINT);
    t += (c[3] as DPINT) * (b[5] as DPINT);
    t += (c[4] as DPINT) * (b[4] as DPINT);
    t += (c[5] as DPINT) * (b[3] as DPINT);
    t += (c[6] as DPINT) * (b[2] as DPINT);
    t += (c[7] as DPINT) * (b[1] as DPINT);
    t += (c[8] as DPINT) * (b[0] as DPINT);
    let v8 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc10 as DPINT) * (b[17] as DPINT);
    t += (mc11 as DPINT) * (b[16] as DPINT);
    t += (mc12 as DPINT) * (b[15] as DPINT);
    t += (mc13 as DPINT) * (b[14] as DPINT);
    t += (mc14 as DPINT) * (b[13] as DPINT);
    t += (mc15 as DPINT) * (b[12] as DPINT);
    t += (mc16 as DPINT) * (b[11] as DPINT);
    t += (mc17 as DPINT) * (b[10] as DPINT);
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
    let v9 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc11 as DPINT) * (b[17] as DPINT);
    t += (mc12 as DPINT) * (b[16] as DPINT);
    t += (mc13 as DPINT) * (b[15] as DPINT);
    t += (mc14 as DPINT) * (b[14] as DPINT);
    t += (mc15 as DPINT) * (b[13] as DPINT);
    t += (mc16 as DPINT) * (b[12] as DPINT);
    t += (mc17 as DPINT) * (b[11] as DPINT);
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
    let v10 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc12 as DPINT) * (b[17] as DPINT);
    t += (mc13 as DPINT) * (b[16] as DPINT);
    t += (mc14 as DPINT) * (b[15] as DPINT);
    t += (mc15 as DPINT) * (b[14] as DPINT);
    t += (mc16 as DPINT) * (b[13] as DPINT);
    t += (mc17 as DPINT) * (b[12] as DPINT);
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
    let v11 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc13 as DPINT) * (b[17] as DPINT);
    t += (mc14 as DPINT) * (b[16] as DPINT);
    t += (mc15 as DPINT) * (b[15] as DPINT);
    t += (mc16 as DPINT) * (b[14] as DPINT);
    t += (mc17 as DPINT) * (b[13] as DPINT);
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
    let v12 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc14 as DPINT) * (b[17] as DPINT);
    t += (mc15 as DPINT) * (b[16] as DPINT);
    t += (mc16 as DPINT) * (b[15] as DPINT);
    t += (mc17 as DPINT) * (b[14] as DPINT);
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
    let v13 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc15 as DPINT) * (b[17] as DPINT);
    t += (mc16 as DPINT) * (b[16] as DPINT);
    t += (mc17 as DPINT) * (b[15] as DPINT);
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
    let v14 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc16 as DPINT) * (b[17] as DPINT);
    t += (mc17 as DPINT) * (b[16] as DPINT);
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
    let v15 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc17 as DPINT) * (b[17] as DPINT);
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
    let v16 = (t as SPINT) & mask;
    t = t >> 29;
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
    let v17 = (t as SPINT) & mask;
    t = t >> 29;
    // second reduction pass
    let mut nv = v17;
    let mut ut = t as UDPINT;
    ut = (ut << 1) + ((nv >> 28) as UDPINT);
    nv &= 0xfffffff;
    let s = v0 + ((ut as SPINT) & mask);
    c[0] = (s & mask) as SPINT;
    let carry = (s >> 29) + ((ut >> 29) as SPINT);
    c[1] = v1 + carry;
    c[2] = v2;
    c[3] = v3;
    c[4] = v4;
    c[5] = v5;
    c[6] = v6;
    c[7] = v7;
    c[8] = v8;
    c[9] = v9;
    c[10] = v10;
    c[11] = v11;
    c[12] = v12;
    c[13] = v13;
    c[14] = v14;
    c[15] = v15;
    c[16] = v16;
    c[17] = nv;
    return;
}

// Modular squaring, c=c*c mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t = 0 as UDPINT;
    let tc1 = c[1] * 2;
    let tc2 = c[2] * 2;
    let tc3 = c[3] * 2;
    let tc4 = c[4] * 2;
    let tc5 = c[5] * 2;
    let tc6 = c[6] * 2;
    let tc7 = c[7] * 2;
    let tc8 = c[8] * 2;
    let tc9 = c[9] * 2;
    let tc10 = c[10] * 2;
    let tc11 = c[11] * 2;
    let tc12 = c[12] * 2;
    let tc13 = c[13] * 2;
    let tc14 = c[14] * 2;
    let tc15 = c[15] * 2;
    let tc16 = c[16] * 2;
    let tc17 = c[17] * 2;
    let mc1 = c[1] * 0x2;
    let mc2 = c[2] * 0x2;
    let mc3 = c[3] * 0x2;
    let mc4 = c[4] * 0x2;
    let mc5 = c[5] * 0x2;
    let mc6 = c[6] * 0x2;
    let mc7 = c[7] * 0x2;
    let mc8 = c[8] * 0x2;
    let mc9 = c[9] * 0x2;
    let mc10 = c[10] * 0x2;
    let mc11 = c[11] * 0x2;
    let mc12 = c[12] * 0x2;
    let mc13 = c[13] * 0x2;
    let mc14 = c[14] * 0x2;
    let mc15 = c[15] * 0x2;
    let mc16 = c[16] * 0x2;
    let mc17 = c[17] * 0x2;
    let mask = ((1 as SPINT) << 29) - 1;
    t += (mc1 as UDPINT) * (tc17 as UDPINT);
    t += (mc2 as UDPINT) * (tc16 as UDPINT);
    t += (mc3 as UDPINT) * (tc15 as UDPINT);
    t += (mc4 as UDPINT) * (tc14 as UDPINT);
    t += (mc5 as UDPINT) * (tc13 as UDPINT);
    t += (mc6 as UDPINT) * (tc12 as UDPINT);
    t += (mc7 as UDPINT) * (tc11 as UDPINT);
    t += (mc8 as UDPINT) * (tc10 as UDPINT);
    t += (mc9 as UDPINT) * (c[9] as UDPINT);
    t += (c[0] as UDPINT) * (c[0] as UDPINT);
    let v0 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc2 as UDPINT) * (tc17 as UDPINT);
    t += (mc3 as UDPINT) * (tc16 as UDPINT);
    t += (mc4 as UDPINT) * (tc15 as UDPINT);
    t += (mc5 as UDPINT) * (tc14 as UDPINT);
    t += (mc6 as UDPINT) * (tc13 as UDPINT);
    t += (mc7 as UDPINT) * (tc12 as UDPINT);
    t += (mc8 as UDPINT) * (tc11 as UDPINT);
    t += (mc9 as UDPINT) * (tc10 as UDPINT);
    t += (c[0] as UDPINT) * (tc1 as UDPINT);
    let v1 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc3 as UDPINT) * (tc17 as UDPINT);
    t += (mc4 as UDPINT) * (tc16 as UDPINT);
    t += (mc5 as UDPINT) * (tc15 as UDPINT);
    t += (mc6 as UDPINT) * (tc14 as UDPINT);
    t += (mc7 as UDPINT) * (tc13 as UDPINT);
    t += (mc8 as UDPINT) * (tc12 as UDPINT);
    t += (mc9 as UDPINT) * (tc11 as UDPINT);
    t += (mc10 as UDPINT) * (c[10] as UDPINT);
    t += (c[0] as UDPINT) * (tc2 as UDPINT);
    t += (c[1] as UDPINT) * (c[1] as UDPINT);
    let v2 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc4 as UDPINT) * (tc17 as UDPINT);
    t += (mc5 as UDPINT) * (tc16 as UDPINT);
    t += (mc6 as UDPINT) * (tc15 as UDPINT);
    t += (mc7 as UDPINT) * (tc14 as UDPINT);
    t += (mc8 as UDPINT) * (tc13 as UDPINT);
    t += (mc9 as UDPINT) * (tc12 as UDPINT);
    t += (mc10 as UDPINT) * (tc11 as UDPINT);
    t += (c[0] as UDPINT) * (tc3 as UDPINT);
    t += (c[1] as UDPINT) * (tc2 as UDPINT);
    let v3 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc5 as UDPINT) * (tc17 as UDPINT);
    t += (mc6 as UDPINT) * (tc16 as UDPINT);
    t += (mc7 as UDPINT) * (tc15 as UDPINT);
    t += (mc8 as UDPINT) * (tc14 as UDPINT);
    t += (mc9 as UDPINT) * (tc13 as UDPINT);
    t += (mc10 as UDPINT) * (tc12 as UDPINT);
    t += (mc11 as UDPINT) * (c[11] as UDPINT);
    t += (c[0] as UDPINT) * (tc4 as UDPINT);
    t += (c[1] as UDPINT) * (tc3 as UDPINT);
    t += (c[2] as UDPINT) * (c[2] as UDPINT);
    let v4 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc6 as UDPINT) * (tc17 as UDPINT);
    t += (mc7 as UDPINT) * (tc16 as UDPINT);
    t += (mc8 as UDPINT) * (tc15 as UDPINT);
    t += (mc9 as UDPINT) * (tc14 as UDPINT);
    t += (mc10 as UDPINT) * (tc13 as UDPINT);
    t += (mc11 as UDPINT) * (tc12 as UDPINT);
    t += (c[0] as UDPINT) * (tc5 as UDPINT);
    t += (c[1] as UDPINT) * (tc4 as UDPINT);
    t += (c[2] as UDPINT) * (tc3 as UDPINT);
    let v5 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc7 as UDPINT) * (tc17 as UDPINT);
    t += (mc8 as UDPINT) * (tc16 as UDPINT);
    t += (mc9 as UDPINT) * (tc15 as UDPINT);
    t += (mc10 as UDPINT) * (tc14 as UDPINT);
    t += (mc11 as UDPINT) * (tc13 as UDPINT);
    t += (mc12 as UDPINT) * (c[12] as UDPINT);
    t += (c[0] as UDPINT) * (tc6 as UDPINT);
    t += (c[1] as UDPINT) * (tc5 as UDPINT);
    t += (c[2] as UDPINT) * (tc4 as UDPINT);
    t += (c[3] as UDPINT) * (c[3] as UDPINT);
    let v6 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc8 as UDPINT) * (tc17 as UDPINT);
    t += (mc9 as UDPINT) * (tc16 as UDPINT);
    t += (mc10 as UDPINT) * (tc15 as UDPINT);
    t += (mc11 as UDPINT) * (tc14 as UDPINT);
    t += (mc12 as UDPINT) * (tc13 as UDPINT);
    t += (c[0] as UDPINT) * (tc7 as UDPINT);
    t += (c[1] as UDPINT) * (tc6 as UDPINT);
    t += (c[2] as UDPINT) * (tc5 as UDPINT);
    t += (c[3] as UDPINT) * (tc4 as UDPINT);
    let v7 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc9 as UDPINT) * (tc17 as UDPINT);
    t += (mc10 as UDPINT) * (tc16 as UDPINT);
    t += (mc11 as UDPINT) * (tc15 as UDPINT);
    t += (mc12 as UDPINT) * (tc14 as UDPINT);
    t += (mc13 as UDPINT) * (c[13] as UDPINT);
    t += (c[0] as UDPINT) * (tc8 as UDPINT);
    t += (c[1] as UDPINT) * (tc7 as UDPINT);
    t += (c[2] as UDPINT) * (tc6 as UDPINT);
    t += (c[3] as UDPINT) * (tc5 as UDPINT);
    t += (c[4] as UDPINT) * (c[4] as UDPINT);
    let v8 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc10 as UDPINT) * (tc17 as UDPINT);
    t += (mc11 as UDPINT) * (tc16 as UDPINT);
    t += (mc12 as UDPINT) * (tc15 as UDPINT);
    t += (mc13 as UDPINT) * (tc14 as UDPINT);
    t += (c[0] as UDPINT) * (tc9 as UDPINT);
    t += (c[1] as UDPINT) * (tc8 as UDPINT);
    t += (c[2] as UDPINT) * (tc7 as UDPINT);
    t += (c[3] as UDPINT) * (tc6 as UDPINT);
    t += (c[4] as UDPINT) * (tc5 as UDPINT);
    let v9 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc11 as UDPINT) * (tc17 as UDPINT);
    t += (mc12 as UDPINT) * (tc16 as UDPINT);
    t += (mc13 as UDPINT) * (tc15 as UDPINT);
    t += (mc14 as UDPINT) * (c[14] as UDPINT);
    t += (c[0] as UDPINT) * (tc10 as UDPINT);
    t += (c[1] as UDPINT) * (tc9 as UDPINT);
    t += (c[2] as UDPINT) * (tc8 as UDPINT);
    t += (c[3] as UDPINT) * (tc7 as UDPINT);
    t += (c[4] as UDPINT) * (tc6 as UDPINT);
    t += (c[5] as UDPINT) * (c[5] as UDPINT);
    let v10 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc12 as UDPINT) * (tc17 as UDPINT);
    t += (mc13 as UDPINT) * (tc16 as UDPINT);
    t += (mc14 as UDPINT) * (tc15 as UDPINT);
    t += (c[0] as UDPINT) * (tc11 as UDPINT);
    t += (c[1] as UDPINT) * (tc10 as UDPINT);
    t += (c[2] as UDPINT) * (tc9 as UDPINT);
    t += (c[3] as UDPINT) * (tc8 as UDPINT);
    t += (c[4] as UDPINT) * (tc7 as UDPINT);
    t += (c[5] as UDPINT) * (tc6 as UDPINT);
    let v11 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc13 as UDPINT) * (tc17 as UDPINT);
    t += (mc14 as UDPINT) * (tc16 as UDPINT);
    t += (mc15 as UDPINT) * (c[15] as UDPINT);
    t += (c[0] as UDPINT) * (tc12 as UDPINT);
    t += (c[1] as UDPINT) * (tc11 as UDPINT);
    t += (c[2] as UDPINT) * (tc10 as UDPINT);
    t += (c[3] as UDPINT) * (tc9 as UDPINT);
    t += (c[4] as UDPINT) * (tc8 as UDPINT);
    t += (c[5] as UDPINT) * (tc7 as UDPINT);
    t += (c[6] as UDPINT) * (c[6] as UDPINT);
    let v12 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc14 as UDPINT) * (tc17 as UDPINT);
    t += (mc15 as UDPINT) * (tc16 as UDPINT);
    t += (c[0] as UDPINT) * (tc13 as UDPINT);
    t += (c[1] as UDPINT) * (tc12 as UDPINT);
    t += (c[2] as UDPINT) * (tc11 as UDPINT);
    t += (c[3] as UDPINT) * (tc10 as UDPINT);
    t += (c[4] as UDPINT) * (tc9 as UDPINT);
    t += (c[5] as UDPINT) * (tc8 as UDPINT);
    t += (c[6] as UDPINT) * (tc7 as UDPINT);
    let v13 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc15 as UDPINT) * (tc17 as UDPINT);
    t += (mc16 as UDPINT) * (c[16] as UDPINT);
    t += (c[0] as UDPINT) * (tc14 as UDPINT);
    t += (c[1] as UDPINT) * (tc13 as UDPINT);
    t += (c[2] as UDPINT) * (tc12 as UDPINT);
    t += (c[3] as UDPINT) * (tc11 as UDPINT);
    t += (c[4] as UDPINT) * (tc10 as UDPINT);
    t += (c[5] as UDPINT) * (tc9 as UDPINT);
    t += (c[6] as UDPINT) * (tc8 as UDPINT);
    t += (c[7] as UDPINT) * (c[7] as UDPINT);
    let v14 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc16 as UDPINT) * (tc17 as UDPINT);
    t += (c[0] as UDPINT) * (tc15 as UDPINT);
    t += (c[1] as UDPINT) * (tc14 as UDPINT);
    t += (c[2] as UDPINT) * (tc13 as UDPINT);
    t += (c[3] as UDPINT) * (tc12 as UDPINT);
    t += (c[4] as UDPINT) * (tc11 as UDPINT);
    t += (c[5] as UDPINT) * (tc10 as UDPINT);
    t += (c[6] as UDPINT) * (tc9 as UDPINT);
    t += (c[7] as UDPINT) * (tc8 as UDPINT);
    let v15 = (t as SPINT) & mask;
    t = t >> 29;
    t += (mc17 as UDPINT) * (c[17] as UDPINT);
    t += (c[0] as UDPINT) * (tc16 as UDPINT);
    t += (c[1] as UDPINT) * (tc15 as UDPINT);
    t += (c[2] as UDPINT) * (tc14 as UDPINT);
    t += (c[3] as UDPINT) * (tc13 as UDPINT);
    t += (c[4] as UDPINT) * (tc12 as UDPINT);
    t += (c[5] as UDPINT) * (tc11 as UDPINT);
    t += (c[6] as UDPINT) * (tc10 as UDPINT);
    t += (c[7] as UDPINT) * (tc9 as UDPINT);
    t += (c[8] as UDPINT) * (c[8] as UDPINT);
    let v16 = (t as SPINT) & mask;
    t = t >> 29;
    t += (c[0] as UDPINT) * (tc17 as UDPINT);
    t += (c[1] as UDPINT) * (tc16 as UDPINT);
    t += (c[2] as UDPINT) * (tc15 as UDPINT);
    t += (c[3] as UDPINT) * (tc14 as UDPINT);
    t += (c[4] as UDPINT) * (tc13 as UDPINT);
    t += (c[5] as UDPINT) * (tc12 as UDPINT);
    t += (c[6] as UDPINT) * (tc11 as UDPINT);
    t += (c[7] as UDPINT) * (tc10 as UDPINT);
    t += (c[8] as UDPINT) * (tc9 as UDPINT);
    let v17 = (t as SPINT) & mask;
    t = t >> 29;
    // second reduction pass
    let mut nv = v17;
    let mut ut = t as UDPINT;
    ut = (ut << 1) + ((nv >> 28) as UDPINT);
    nv &= 0xfffffff;
    let s = v0 + ((ut as SPINT) & mask);
    c[0] = (s & mask) as SPINT;
    let carry = (s >> 29) + ((ut >> 29) as SPINT);
    c[1] = v1 + carry;
    c[2] = v2;
    c[3] = v3;
    c[4] = v4;
    c[5] = v5;
    c[6] = v6;
    c[7] = v7;
    c[8] = v8;
    c[9] = v9;
    c[10] = v10;
    c[11] = v11;
    c[12] = v12;
    c[13] = v13;
    c[14] = v14;
    c[15] = v15;
    c[16] = v16;
    c[17] = nv;
    return;
}

//copy
fn modcpy(a: &[SPINT], c: &mut [SPINT]) {
    for i in 0..18 {
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

//Calculate progenitor - use optimal addition chain
fn modpro(w: &[SPINT], r: &mut [SPINT]) {
    let mut x: [SPINT; 18] = [0; 18];
    let mut z: [SPINT; 18] = [0; 18];
    let mut t0: [SPINT; 18] = [0; 18];
    modcpy(w, &mut x);
    modcpy(&x, &mut z);
    modsqr(&mut z);
    modmul(&x, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 2);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 4);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 8);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 16);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 32);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modsqr(&mut t0);
    modmul(&x, &mut t0);
    modnsqr(&mut t0, 64);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modsqr(&mut t0);
    modmul(&x, &mut t0);
    modnsqr(&mut t0, 129);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modsqr(&mut t0);
    modmul(&x, &mut t0);
    modnsqr(&mut t0, 259);
    modmul(&t0, &mut z);
    modcpy(&z, r);
    return;
}

//calculate inverse, provide progenitor h if available
fn modinv(h: Option<&[SPINT]>, z: &mut [SPINT]) {
    let mut s: [SPINT; 18] = [0; 18];
    let mut t: [SPINT; 18] = [0; 18];
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
fn nres(_n: &mut [SPINT]) {
    return;
}

//Convert m back to normal form, m=redc(n)
fn redc(m: &mut [SPINT]) {
    modfsb(m);
    return;
}

//is unity?
fn modis1(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 18] = [0; 18];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 1..18 {
        d |= c[i] as SSPINT;
    }
    let c0 = c[0] as SSPINT;
    return (1 & ((d - 1) >> 29) & (((c0 ^ 1) - 1) >> 29)) != 0;
}

//is zero?
fn modis0(a: &[SPINT]) -> bool {
    let mut c: [SPINT; 18] = [0; 18];
    let mut d = 0 as SSPINT;
    modcpy(a, &mut c);
    redc(&mut c);
    for i in 0..18 {
        d |= c[i] as SSPINT;
    }
    return (1 & ((d - 1) >> 29)) != 0;
}

//set to zero
fn modzer(a: &mut [SPINT]) {
    for i in 0..18 {
        a[i] = 0;
    }
    return;
}

//set to one
fn modone(a: &mut [SPINT]) {
    a[0] = 1;
    for i in 1..18 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//set to integer
fn modint(x: usize, a: &mut [SPINT]) {
    a[0] = x as SPINT;
    for i in 1..18 {
        a[i] = 0;
    }
    nres(a);
    return;
}

//Test for quadratic residue
fn modqr(h: Option<&[SPINT]>, x: &[SPINT]) -> bool {
    let mut r: [SPINT; 18] = [0; 18];
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
fn modcmv(d: usize, g: &[SPINT], f: &mut [SPINT]) -> SPINT {
    let c = -(d as isize) as SPINT;
    let mut w = 0 as SPINT;
    let r = f[0] ^ g[1];
    let mut ra = r.wrapping_add(r);
    ra >>= 1;
    for i in 0..18 {
        let mut t = (f[i] ^ g[i]) & c;
        t ^= r;
        let e = f[i] ^ t;
        w ^= e;
        f[i] = e ^ ra;
    }
    return w;
}

//conditional swap g and f if d=1
fn modcsw(d: usize, g: &mut [SPINT], f: &mut [SPINT]) -> SPINT {
    let c = -(d as isize) as SPINT;
    let mut w = 0 as SPINT;
    let r = f[0] ^ g[1];
    let mut ra = r.wrapping_add(r);
    ra >>= 1;
    for i in 0..18 {
        let mut t = (f[i] ^ g[i]) & c;
        t ^= r;
        let mut e = f[i] ^ t;
        w ^= e;
        f[i] = e ^ ra;
        e = g[i] ^ t;
        w ^= e;
        g[i] = e ^ ra;
    }
    return w;
}

//Modular square root, provide progenitor h if available, NULL if not
fn modsqrt(x: &[SPINT], h: Option<&[SPINT]>, r: &mut [SPINT]) {
    let mut y: [SPINT; 18] = [0; 18];
    let mut s: [SPINT; 18] = [0; 18];
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
    a[18 - 1] = (a[18 - 1] << n) | (a[18 - 2] >> (29 - n));
    for i in (1..18 - 1).rev() {
        a[i] = ((a[i] << n) & 0x1fffffff) | (a[i - 1] >> (29 - n));
    }
    a[0] = (a[0] << n) & 0x1fffffff;
    return;
}

//shift right by less than a word. Return shifted out part
fn modshr(n: isize, a: &mut [SPINT]) -> isize {
    let r = a[0] & ((1 << n) - 1);
    for i in 0..18 - 1 {
        a[i] = (a[i] >> n) | ((a[i + 1] << (29 - n)) & 0x1fffffff);
    }
    a[18 - 1] = a[18 - 1] >> n;
    return r as isize;
}

//export to byte array
fn modexp(a: &[SPINT], b: &mut [u8]) {
    let mut c: [SPINT; 18] = [0; 18];
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
    for i in 0..18 {
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
    let mut c: [SPINT; 18] = [0; 18];
    modcpy(a, &mut c);
    redc(&mut c);
    return (c[0] % 2) as usize;
}

//return true if equal
fn modcmp(a: &[SPINT], b: &[SPINT]) -> bool {
    let mut c: [SPINT; 18] = [0; 18];
    let mut d: [SPINT; 18] = [0; 18];
    let mut eq = 1;
    modcpy(a, &mut c);
    redc(&mut c);
    modcpy(b, &mut d);
    redc(&mut d);
    for i in 0..18 {
        eq &= (((c[i] ^ d[i]) - 1) >> 29) & 1;
    }
    return eq == 1;
}

const NLIMBS: usize = 18;
const RADIX: usize = 29;
const NBITS: usize = 521;
const NBYTES: usize = 66;

const MERSENNE: bool = true;
const MONTGOMERY: bool = false;

const MULBYINT: bool = true;


/*** Insert automatically generated definition for curve curve.rs here ***/


const COF:usize = 1;
const CONSTANT_A: isize = -3;
const CONSTANT_B: isize = 0;
const constant_b: [SPINT;18]=[0xb503f00,0x1a28fea3,0xb0d3c7b,0x7bf107a,0x1bf07357,0x5e9dd8,0xdec594b,0xa3d8fd2,0x1561939,0xc77884f,0xe2d2264,0x13662be7,0xda725b9,0x2a07751,0x88682da,0x1343f253,0x19618e1c,0x28ca9f5];
const constant_b3: [SPINT;18]=[0x1f0bd00,0xe7afbea,0x127b573,0x173d316f,0x13d15a05,0x11bd98a,0x9c50be1,0x1eb8af77,0x4024bab,0x56698ed,0xa87672d,0x1a3283b6,0x8f5712c,0x7e165f4,0x1993888e,0x19cbd6f9,0xc24aa55,0x7a5fde1];
const CONSTANT_X: usize = 0;
const constant_x: [SPINT;18]=[0x2e5bd66,0xbf3f18e,0x1a90a6fe,0x1167830a,0x1a8de334,0x93d17f,0x4a3f877,0xefdfceb,0x1aa14b5e,0x35a69ed,0x1e0a2bd8,0xa7f6a43,0x6481390,0xada214e,0x1b2d988e,0x1d39b3c7,0x6b70404,0x6342c70];
const constant_y: [SPINT;18]=[0x1fd16650,0x5f4a3b4,0x1cb09022,0x18e10d44,0x10761353,0x1c809fd6,0x19031542,0x132bde84,0xc97ee72,0x1939f331,0x5ebef45,0xf3688d0,0xf544495,0x1e8deccc,0x97ed0b1,0x18008b91,0xa789a3b,0x8c1c94b];


/*** End of automatically generated code ***/


/*** Insert automatically generated point definition point.rs here ***/

// elliptic curve point in projective coordinates
const WORDLENGTH: usize = 32;
#[derive(Clone)]
pub struct ECP {
	x: [u32;18],
	y: [u32;18],
	z: [u32;18],
}

#[allow(non_snake_case)]
impl ECP {
	pub fn new() -> ECP {
		ECP {
			x: [0;18],
			y: [0;18],
			z: [0;18],
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


