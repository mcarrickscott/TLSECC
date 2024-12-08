#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
// Edwards curve support 
// Use python scripts to generate code for ED25519 or ED448, or your own curve
//
// Mike Scott 4th September 2024
// TII
//
// code for 32/64-bit processor for ED25519 curve can be generated  by 
//
// python curve_rust.py 32/64 ED25519
//
// code for 32/64-bit processor for ED448 curve can be generated  by
//
// python curve_rust.py 32/64 ED448

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
    n[0] -= 1 & (carry as SPINT);
    n[8] -= 1 & (carry as SPINT);
    n[15] += (1 * q) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    let q = (1 as SPINT) << 28;
    n[0] += 1;
    n[8] += 1;
    n[15] -= 1 * q;
    return flatten(n);
}

//Modular addition - reduce less than 2p
#[allow(unused_variables)]
fn modadd(b: &[SPINT], n: &mut [SPINT]) {
    let q = (1 as SPINT) << 28;
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
    n[0] += 2;
    n[8] += 2;
    n[15] -= 2 * q;
    let carry = prop(n);
    n[0] -= 2 & (carry as SPINT);
    n[8] -= 2 & (carry as SPINT);
    n[15] += (2 * q) & (carry as SPINT);
    prop(n);
    return;
}

//Modular subtraction - reduce less than 2p
#[allow(unused_variables)]
fn modsub(b: &[SPINT], n: &mut [SPINT]) {
    let q = (1 as SPINT) << 28;
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
    n[0] -= 2 & (carry as SPINT);
    n[8] -= 2 & (carry as SPINT);
    n[15] += (2 * q) & (carry as SPINT);
    prop(n);
    return;
}

//Modular negation
#[allow(unused_variables)]
fn modneg(n: &mut [SPINT]) {
    let q = (1 as SPINT) << 28;
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
    n[0] -= 2 & (carry as SPINT);
    n[8] -= 2 & (carry as SPINT);
    n[15] += (2 * q) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication by an integer, c=c*b mod 2p
#[allow(dead_code)]
fn modmli(b: usize, c: &mut [SPINT]) {
    let mut t = 0 as UDPINT;
    let mask = ((1 as SPINT) << 28) - 1;
    t += (c[0] as UDPINT) * (b as UDPINT);
    c[0] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[1] as UDPINT) * (b as UDPINT);
    c[1] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[2] as UDPINT) * (b as UDPINT);
    c[2] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[3] as UDPINT) * (b as UDPINT);
    c[3] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[4] as UDPINT) * (b as UDPINT);
    c[4] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[5] as UDPINT) * (b as UDPINT);
    c[5] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[6] as UDPINT) * (b as UDPINT);
    c[6] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[7] as UDPINT) * (b as UDPINT);
    c[7] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[8] as UDPINT) * (b as UDPINT);
    c[8] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[9] as UDPINT) * (b as UDPINT);
    c[9] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[10] as UDPINT) * (b as UDPINT);
    c[10] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[11] as UDPINT) * (b as UDPINT);
    c[11] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[12] as UDPINT) * (b as UDPINT);
    c[12] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[13] as UDPINT) * (b as UDPINT);
    c[13] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[14] as UDPINT) * (b as UDPINT);
    c[14] = (t as SPINT) & mask;
    t = t >> 28;
    t += (c[15] as UDPINT) * (b as UDPINT);
    c[15] = (t as SPINT) & mask;
    t = t >> 28;
    // reduction pass

    let s = t as SPINT;
    c[0] += s;
    c[8] += s;
    return;
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1152921565004824591
// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut s: SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    let v1 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    let v2 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    let v3 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[4] as DPINT);
    t += (c[1] as DPINT) * (b[3] as DPINT);
    t += (c[2] as DPINT) * (b[2] as DPINT);
    t += (c[3] as DPINT) * (b[1] as DPINT);
    t += (c[4] as DPINT) * (b[0] as DPINT);
    let v4 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    let v5 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
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
    t += (q - v0) as DPINT;
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
    s -= v1;
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
    s -= v2;
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
    s -= v3;
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
    s -= v4;
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
    s -= v5;
    t += s as DPINT;
    let v13 = ((t as SPINT) & mask) as SPINT;
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
    t += s as DPINT;
    let v14 = ((t as SPINT) & mask) as SPINT;
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
    s -= v7;
    t += s as DPINT;
    let v15 = ((t as SPINT) & mask) as SPINT;
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
    s += v0;
    s -= v8;
    t += s as DPINT;
    let v16 = ((t as SPINT) & mask) as SPINT;
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
    s += v1;
    s -= v9;
    t += s as DPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
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
    s += v2;
    s -= v10;
    t += s as DPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
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
    s += v3;
    s -= v11;
    t += s as DPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
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
    s += v4;
    s -= v12;
    t += s as DPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
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
    s += v5;
    s -= v13;
    t += s as DPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
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
    s += v6;
    s -= v14;
    t += s as DPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
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
    s += v7;
    s -= v15;
    t += s as DPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[9] as DPINT) * (b[15] as DPINT);
    t += (c[10] as DPINT) * (b[14] as DPINT);
    t += (c[11] as DPINT) * (b[13] as DPINT);
    t += (c[12] as DPINT) * (b[12] as DPINT);
    t += (c[13] as DPINT) * (b[11] as DPINT);
    t += (c[14] as DPINT) * (b[10] as DPINT);
    t += (c[15] as DPINT) * (b[9] as DPINT);
    s = mask as SPINT;
    s += v8;
    s -= v16;
    t += s as DPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[10] as DPINT) * (b[15] as DPINT);
    t += (c[11] as DPINT) * (b[14] as DPINT);
    t += (c[12] as DPINT) * (b[13] as DPINT);
    t += (c[13] as DPINT) * (b[12] as DPINT);
    t += (c[14] as DPINT) * (b[11] as DPINT);
    t += (c[15] as DPINT) * (b[10] as DPINT);
    s = mask as SPINT;
    s += v9;
    t += s as DPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[11] as DPINT) * (b[15] as DPINT);
    t += (c[12] as DPINT) * (b[14] as DPINT);
    t += (c[13] as DPINT) * (b[13] as DPINT);
    t += (c[14] as DPINT) * (b[12] as DPINT);
    t += (c[15] as DPINT) * (b[11] as DPINT);
    s = mask as SPINT;
    s += v10;
    t += s as DPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[12] as DPINT) * (b[15] as DPINT);
    t += (c[13] as DPINT) * (b[14] as DPINT);
    t += (c[14] as DPINT) * (b[13] as DPINT);
    t += (c[15] as DPINT) * (b[12] as DPINT);
    s = mask as SPINT;
    s += v11;
    t += s as DPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[13] as DPINT) * (b[15] as DPINT);
    t += (c[14] as DPINT) * (b[14] as DPINT);
    t += (c[15] as DPINT) * (b[13] as DPINT);
    s = mask as SPINT;
    s += v12;
    t += s as DPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[14] as DPINT) * (b[15] as DPINT);
    t += (c[15] as DPINT) * (b[14] as DPINT);
    s = mask as SPINT;
    s += v13;
    t += s as DPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (c[15] as DPINT) * (b[15] as DPINT);
    s = mask as SPINT;
    s += v14;
    t += s as DPINT;
    c[13] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    s = mask as SPINT;
    s += v15;
    t += s as DPINT;
    c[14] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (v16 - 1) as DPINT;
    c[15] = t as SPINT;
    return;
}

// Modular squaring, c=a*a  mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t: UDPINT;
    let mut tot: UDPINT;
    let mut s: SPINT;
    let q = (1 as SPINT) << 28; // q is unsaturated radix
    let mask = (q - 1) as SPINT;
    tot = (c[0] as UDPINT) * (c[0] as UDPINT);
    t = tot;
    let v0 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[1] as UDPINT);
    tot *= 2;
    t += tot;
    let v1 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    tot += (c[1] as UDPINT) * (c[1] as UDPINT);
    t += tot;
    let v2 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[3] as UDPINT);
    tot += (c[1] as UDPINT) * (c[2] as UDPINT);
    tot *= 2;
    t += tot;
    let v3 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[4] as UDPINT);
    tot += (c[1] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    tot += (c[2] as UDPINT) * (c[2] as UDPINT);
    t += tot;
    let v4 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[5] as UDPINT);
    tot += (c[1] as UDPINT) * (c[4] as UDPINT);
    tot += (c[2] as UDPINT) * (c[3] as UDPINT);
    tot *= 2;
    t += tot;
    let v5 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[6] as UDPINT);
    tot += (c[1] as UDPINT) * (c[5] as UDPINT);
    tot += (c[2] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    tot += (c[3] as UDPINT) * (c[3] as UDPINT);
    t += tot;
    let v6 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[7] as UDPINT);
    tot += (c[1] as UDPINT) * (c[6] as UDPINT);
    tot += (c[2] as UDPINT) * (c[5] as UDPINT);
    tot += (c[3] as UDPINT) * (c[4] as UDPINT);
    tot *= 2;
    t += tot;
    let v7 = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[0] as UDPINT) * (c[8] as UDPINT);
    tot += (c[1] as UDPINT) * (c[7] as UDPINT);
    tot += (c[2] as UDPINT) * (c[6] as UDPINT);
    tot += (c[3] as UDPINT) * (c[5] as UDPINT);
    tot *= 2;
    tot += (c[4] as UDPINT) * (c[4] as UDPINT);
    t += tot;
    t += (q - v0) as UDPINT;
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
    s -= v1;
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
    s -= v2;
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
    s -= v3;
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
    s -= v4;
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
    s -= v5;
    t += s as UDPINT;
    let v13 = ((t as SPINT) & mask) as SPINT;
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
    t += s as UDPINT;
    let v14 = ((t as SPINT) & mask) as SPINT;
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
    s -= v7;
    t += s as UDPINT;
    let v15 = ((t as SPINT) & mask) as SPINT;
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
    s += v0;
    s -= v8;
    t += s as UDPINT;
    let v16 = ((t as SPINT) & mask) as SPINT;
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
    s += v1;
    s -= v9;
    t += s as UDPINT;
    c[0] = ((t as SPINT) & mask) as SPINT;
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
    s += v2;
    s -= v10;
    t += s as UDPINT;
    c[1] = ((t as SPINT) & mask) as SPINT;
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
    s += v3;
    s -= v11;
    t += s as UDPINT;
    c[2] = ((t as SPINT) & mask) as SPINT;
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
    s += v4;
    s -= v12;
    t += s as UDPINT;
    c[3] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[6] as UDPINT) * (c[15] as UDPINT);
    tot += (c[7] as UDPINT) * (c[14] as UDPINT);
    tot += (c[8] as UDPINT) * (c[13] as UDPINT);
    tot += (c[9] as UDPINT) * (c[12] as UDPINT);
    tot += (c[10] as UDPINT) * (c[11] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s += v5;
    s -= v13;
    t += s as UDPINT;
    c[4] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[7] as UDPINT) * (c[15] as UDPINT);
    tot += (c[8] as UDPINT) * (c[14] as UDPINT);
    tot += (c[9] as UDPINT) * (c[13] as UDPINT);
    tot += (c[10] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    tot += (c[11] as UDPINT) * (c[11] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s += v6;
    s -= v14;
    t += s as UDPINT;
    c[5] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[8] as UDPINT) * (c[15] as UDPINT);
    tot += (c[9] as UDPINT) * (c[14] as UDPINT);
    tot += (c[10] as UDPINT) * (c[13] as UDPINT);
    tot += (c[11] as UDPINT) * (c[12] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s += v7;
    s -= v15;
    t += s as UDPINT;
    c[6] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[9] as UDPINT) * (c[15] as UDPINT);
    tot += (c[10] as UDPINT) * (c[14] as UDPINT);
    tot += (c[11] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    tot += (c[12] as UDPINT) * (c[12] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s += v8;
    s -= v16;
    t += s as UDPINT;
    c[7] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[10] as UDPINT) * (c[15] as UDPINT);
    tot += (c[11] as UDPINT) * (c[14] as UDPINT);
    tot += (c[12] as UDPINT) * (c[13] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s += v9;
    t += s as UDPINT;
    c[8] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[11] as UDPINT) * (c[15] as UDPINT);
    tot += (c[12] as UDPINT) * (c[14] as UDPINT);
    tot *= 2;
    tot += (c[13] as UDPINT) * (c[13] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s += v10;
    t += s as UDPINT;
    c[9] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[12] as UDPINT) * (c[15] as UDPINT);
    tot += (c[13] as UDPINT) * (c[14] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s += v11;
    t += s as UDPINT;
    c[10] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[13] as UDPINT) * (c[15] as UDPINT);
    tot *= 2;
    tot += (c[14] as UDPINT) * (c[14] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s += v12;
    t += s as UDPINT;
    c[11] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[14] as UDPINT) * (c[15] as UDPINT);
    tot *= 2;
    t += tot;
    s = mask as SPINT;
    s += v13;
    t += s as UDPINT;
    c[12] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    tot = (c[15] as UDPINT) * (c[15] as UDPINT);
    t += tot;
    s = mask as SPINT;
    s += v14;
    t += s as UDPINT;
    c[13] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    s = mask as SPINT;
    s += v15;
    t += s as UDPINT;
    c[14] = ((t as SPINT) & mask) as SPINT;
    t >>= 28;
    t += (v16 - 1) as UDPINT;
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
    modcpy(w, &mut x);
    modcpy(&x, &mut z);
    modsqr(&mut z);
    modmul(&x, &mut z);
    modsqr(&mut z);
    modmul(&x, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 3);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 6);
    modmul(&z, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 12);
    modmul(&t1, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 6);
    modmul(&t1, &mut z);
    modnsqr(&mut t1, 18);
    modmul(&t1, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 48);
    modmul(&t1, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 96);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 30);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t0);
    modsqr(&mut t0);
    modmul(&x, &mut t0);
    modnsqr(&mut t0, 223);
    modmul(&t0, &mut z);
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
        0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0,
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
    let r0 = f[0] ^ g[1];
    let r1 = f[1] ^ g[0];
    let dd = d as SPINT;
    for i in 0..16 {
        f[i] = f[i] * (1 - (dd - r0)) + g[i] * (dd + r1) - r0 * f[i] - r1 * g[i];
    }
    return;
}

//conditional swap g and f if d=1
fn modcsw(d: usize, g: &mut [SPINT], f: &mut [SPINT]) {
    let r0 = f[0] ^ g[1];
    let r1 = f[1] ^ g[0];
    let dd = d as SPINT;
    for i in 0..16 {
        let t = f[i];
        f[i] = f[i] * (1 - (dd - r0)) + g[i] * (dd + r1) - r0 * f[i] - r1 * g[i];
        g[i] = g[i] * (1 - (dd - r0)) + t * (dd + r1) - r0 * g[i] - r1 * t;
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
const NBITS: usize = 448;
const NBYTES: usize = 56;

const MERSENNE: bool = false;
const MONTGOMERY: bool = true;

const MULBYINT: bool = true;


/*** Insert automatically generated definition for curve curve.rs here ***/


const COF:usize = 2;
const CONSTANT_A: isize = 1;
const CONSTANT_B: isize = -39081;
const constant_b: [SPINT;16]=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
const CONSTANT_X: usize = 0;
const constant_x: [SPINT;16]=[0x420685f,0x17aa383,0x35bf93b,0x4f8fe6d,0xb7bc291,0xab765fa,0xe44cd37,0xa7e9b28,0x34f39b1,0xbe886a8,0x44d6fb9,0x95e6eb4,0xee96c72,0x5d88b9,0xd16ef09,0xea8836];
const constant_y: [SPINT;16]=[0xd81f4fb,0x9a99632,0xac119c7,0x23c2104,0xda8e9ac,0x9fc5486,0x416ef25,0x8a98abb,0x46ff58f,0x1223251,0x4fa9dd0,0xcaf9471,0xa1f0e6a,0xa33ab50,0x65f7687,0xa184177];



/*** End of automatically generated code ***/


/*** Insert automatically generated point definition point.rs here ***/

// elliptic curve point in projective coordinates
const WORDLENGTH: usize = 32;
#[derive(Clone)]
pub struct ECP {
	x: [u32;16],
	y: [u32;16],
	z: [u32;16],
}

#[allow(non_snake_case)]
impl ECP {
	pub fn new() -> ECP {
		ECP {
			x: [0;16],
			y: [0;16],
			z: [0;16],
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
    modneg(&mut P.x);
}

// add Q to P
// standard projective method from EFD - https://www.hyperelliptic.org/EFD/
pub fn ecnadd(Q: &ECP,P: &mut ECP) {
    let mut a:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut b:[SPINT;NLIMBS]=[0;NLIMBS];     
    let mut c:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut d:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut e:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut f:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut g:[SPINT;NLIMBS]=[0;NLIMBS]; 

    modcpy(&Q.z,&mut a); modmul(&P.z,&mut a);
    modcpy(&a,&mut b); modsqr(&mut b);
    modcpy(&Q.x,&mut c); modmul(&P.x,&mut c);
    modcpy(&Q.y,&mut d); modmul(&P.y,&mut d);
    modcpy(&c,&mut e); modmul(&d,&mut e);
    if CONSTANT_B>0 {
        modmli(CONSTANT_B as usize,&mut e);
        modcpy(&b, &mut f); modsub(&e,&mut f);
        modcpy(&b, &mut g); modadd(&e,&mut g);
    }
    if CONSTANT_B<0 {
        modmli(-CONSTANT_B as usize,&mut e);
        modcpy(&b, &mut f); modadd(&e,&mut f);
        modcpy(&b, &mut g); modsub(&e,&mut g);
    }
    if CONSTANT_B==0 {
        modmul(&constant_b,&mut e);
        modcpy(&b, &mut f); modsub(&e,&mut f);
        modcpy(&b, &mut g); modadd(&e,&mut g);
    }
    modcpy(&P.x,&mut b); modadd(&P.y,&mut b);
    modcpy(&Q.x,&mut e); modadd(&Q.y,&mut e);
    modcpy(&b,&mut P.x); modmul(&e,&mut P.x);
    modsub(&c,&mut P.x);
    modsub(&d,&mut P.x);
    modmul(&f,&mut P.x);
    modmul(&a,&mut P.x);
    if CONSTANT_A == -1 {
        modcpy(&d,&mut P.y); modadd(&c,&mut P.y);
    } else {
        modcpy(&d,&mut P.y); modsub(&c,&mut P.y);
    }
    modmul(&a,&mut P.y);
    modmul(&g,&mut P.y);
    modcpy(&f,&mut P.z); modmul(&g,&mut P.z);
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
    let mut c:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut d:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut e:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut f:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut h:[SPINT;NLIMBS]=[0;NLIMBS];  
    let mut j:[SPINT;NLIMBS]=[0;NLIMBS];    

    modcpy(&P.x,&mut b); modadd(&P.y,&mut b);
    modsqr(&mut b);
    modcpy(&P.x,&mut c); modsqr(&mut c);
    modcpy(&P.y,&mut d); modsqr(&mut d);
    modcpy(&P.z,&mut h); modsqr(&mut h);
    modcpy(&h,&mut j); modadd(&j,&mut h);
    modcpy(&c,&mut e);

    if CONSTANT_A == -1 {
        modneg(&mut e);
    } 
    modcpy(&e,&mut f); modadd(&d,&mut f);
    modcpy(&f,&mut j); modsub(&h,&mut j);
    modcpy(&b,&mut P.x); modsub(&c,&mut P.x);
    modsub(&d,&mut P.x);
    modmul(&j,&mut P.x);
    modcpy(&e,&mut P.y); modsub(&d,&mut P.y);
    modmul(&f,&mut P.y);
    modcpy(&f,&mut P.z); modmul(&j,&mut P.z);
}

// set to infinity
pub fn ecninf(P: &mut ECP) {
    modzer(&mut P.x);
    modone(&mut P.y);
    modone(&mut P.z);
}

// test for infinity
pub fn ecnisinf(P: &ECP) -> bool {
    return modis0(&P.x) && modcmp(&P.y,&P.z);
}

// set to affine
pub fn ecnaffine(P: &mut ECP) {
    let mut i:[SPINT;NLIMBS]=[0;NLIMBS]; 
    if modis0(&P.z) {
        ecninf(P);
        return;
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

// return 1 if equal, else 0
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

// extract (x,y) from point, if y is NULL compress and just return x and sign of y, if x is NULL compress and just return y and sign of x
pub fn ecnget(P: &mut ECP,x: Option<&mut [u8]>,y: Option<&mut [u8]>) -> usize {
    let mut sx:[SPINT;NLIMBS]=[0;NLIMBS];  
    let mut sy:[SPINT;NLIMBS]=[0;NLIMBS]; 
    let mut ynull:bool=false;
    let mut xnull:bool=false;
    ecnaffine(P);
    if let Some(rx) = x {
        modcpy(&P.x,&mut sx);
        modexp(&sx,rx);
    } else {
        xnull=true;
    }
    if let Some(ry) = y {
        modcpy(&P.y,&mut sy);
        modexp(&sy,ry);
    } else {
        ynull=true;
    }
    if ynull {
        return modsign(&P.y);
    }
    if xnull {
        return modsign(&P.x);
    }
    return 0;
}

// general purpose set point function
// sets P=O if point not on curve
// if x and y are not NULL tries to set (x,y)
// if y==NULL tries to set from x and sign s of y (decompression)
// if x==NULL tries to set from y and sign s of x

fn setxy(s: usize,x: Option<&[SPINT]>,y: Option<&[SPINT]>,P: &mut ECP) {
    let mut sx:[SPINT;NLIMBS]=[0;NLIMBS];     
    let mut sy:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut o:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut u:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut v:[SPINT;NLIMBS]=[0;NLIMBS];   
    let mut h:[SPINT;NLIMBS]=[0;NLIMBS];  
    modone(&mut o);
    if let Some(ry)=y {
        if let Some(rx)=x {
            modcpy(&rx,&mut sx); modsqr(&mut sx);
            modcpy(&ry,&mut sy); modsqr(&mut sy);
            if CONSTANT_A == -1 {
                modcpy(&sy,&mut u); modsub(&sx,&mut u);
            } else {
                modcpy(&sy,&mut u); modadd(&sx,&mut u);
            }
            modcpy(&sx,&mut v); modmul(&sy,&mut v);
            if CONSTANT_B>0 {
                modmli(CONSTANT_B as usize,&mut v);
                modadd(&o,&mut v);
            }
            if CONSTANT_B<0 {
                modmli(-CONSTANT_B as usize,&mut v);
                modsub(&o,&mut v); modneg(&mut v);
            }
            if CONSTANT_B==0 {
                modmul(&constant_b,&mut v);
                modadd(&o,&mut v);
            }
            modmul(&o,&mut u); modmul(&o,&mut v); 
            if modcmp(&u,&v) {
                modcpy(&rx,&mut P.x);
                modcpy(&ry,&mut P.y);
                modone(&mut P.z);
            } else {
                ecninf(P);
            }
            return;
        }
    }  

    if let Some(rx)=x {
        modcpy(&rx,&mut sx); modsqr(&mut sx);
        if CONSTANT_A == -1 {
            modcpy(&o,&mut u); modadd(&sx, &mut u);
        } else {
            modcpy(&o,&mut u); modsub(&sx, &mut u); 
        }
        modcpy(&sx,&mut v);
    }
    if let Some(ry)=y {        
        modcpy(&ry,&mut sy); modsqr(&mut sy);
        modcpy(&o,&mut u); modsub(&sy,&mut u);
        if CONSTANT_A == -1 {
            modneg(&mut o);
        } 
        modcpy(&sy,&mut v);
    }
    if CONSTANT_B>0 {
        modmli(CONSTANT_B as usize,&mut v);
        modsub(&o,&mut v); modneg(&mut v);
    }
    if CONSTANT_B<0 {
        modmli(-CONSTANT_B as usize,&mut v);
        modadd(&o,&mut v);
    }
    if CONSTANT_B==0 {
        modmul(&constant_b,&mut v);
        modsub(&o,&mut v); modneg(&mut v);
    }
    modcpy(&u,&mut o); modsqr(&mut o);
    modmul(&o,&mut u);
    modmul(&v,&mut u);
    modpro(&u,&mut h);
    if !modqr(Some(&h),&u) {
        ecninf(P);
        return;
    }
    modsqrt(&u,Some(&h),&mut v);
    modinv(Some(&h),&mut u);
    modmul(&v, &mut u);
    modmul(&o, &mut u);
    let d=(modsign(&u)-s)&1;
    modcpy(&u, &mut v); modneg(&mut v);
    modcmv(d,&v, &mut u);

    if let Some(rx)=x {
        modcpy(&u, &mut P.y);
        modcpy(&rx,&mut P.x);
    }
    if let Some(ry)=y {
        modcpy(&u, &mut P.x);
        modcpy(&ry,&mut P.y);
    }
    modone(&mut P.z);
}

// multiply point by small curve cofactor (here assumed to be 4 or 8)
pub fn ecncof(P: &mut ECP) {
    for _ in 0..COF {
        ecndbl(P);
    }
}

// Is (x,y) of the right order? Must be checked by calling program!
// api visible version, x and y are big endian byte arrays
pub fn ecnset(s: usize,x: Option<&[u8]>,y: Option<&[u8]>,P: &mut ECP) {
    let mut sx:[SPINT;NLIMBS]=[0;NLIMBS];     
    let mut sy:[SPINT;NLIMBS]=[0;NLIMBS];  
    if let Some(ry)=y {
        if let Some(rx)=x {
            modimp(&rx,&mut sx);
            modimp(&ry,&mut sy);
            setxy(s,Some(&sx),Some(&sy),P);
            return;
        }
    }
    if let Some(rx)=x {
        modimp(rx,&mut sx);
        setxy(s,Some(&sx),None,P);
    }
    if let Some(ry)=y {
        modimp(ry,&mut sy);
        setxy(s,None,Some(&sy),P);
    }
}

// set generator
pub fn ecngen(P: &mut ECP) {
    if CONSTANT_X!=0 {
        let mut sx:[SPINT;NLIMBS]=[0;NLIMBS];  
        modint(CONSTANT_X,&mut sx);
        setxy(0,Some(&sx),None,P);
    } 
    if CONSTANT_X==0 {
        setxy(0,Some(&constant_x),Some(&constant_y),P);
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

