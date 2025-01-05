#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(non_upper_case_globals)]
#![allow(unused_imports)]
// Program to implement RFC7748 - https://datatracker.ietf.org/doc/html/rfc7748
// Montgomery curve key exchange code, as used by TLS
// Use associated python scripts to generate code for X25519 or X448, but easily modified for other Montgomery curves
//
// A good Montgomery curve can be found by running the sagemath script bowe.sage
//
// Mike Scott 23rd November 2023
// TII
// code for 16/32/64-bit processor for X25519 curve can be generated  by 
//
// python pseudo_rust.py 16/32/64 X25519
//


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
    let carry = prop(n);
    n[0] -= (19) & (carry as SPINT);
    n[8] += (0x800000 as SPINT) & (carry as SPINT);
    prop(n);
    return (carry & 1) == 1;
}

//Montgomery final subtract
#[allow(unused_variables)]
fn modfsb(n: &mut [SPINT]) -> bool {
    n[0] += 19 as SPINT;
    n[8] -= 0x800000 as SPINT;
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
    // second reduction pass
    let mut nv = v8;
    let mut ut = t as UDPINT;
    ut = (ut << 6) + ((nv >> 23) as UDPINT);
    nv &= 0x7fffff;
    ut *= 19;
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
    c[8] = nv;
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
    n[0] += 38 as SPINT;
    n[8] -= 0x1000000 as SPINT;
    let carry = prop(n);
    n[0] -= (38) & (carry as SPINT);
    n[8] += (0x1000000 as SPINT) & (carry as SPINT);
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
    n[0] -= (38) & (carry as SPINT);
    n[8] += (0x1000000 as SPINT) & (carry as SPINT);
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
    n[0] -= (38) & (carry as SPINT);
    n[8] += (0x1000000 as SPINT) & (carry as SPINT);
    prop(n);
    return;
}

// Modular multiplication, c=c*b mod 2p
#[allow(unused_variables)]
fn modmul(b: &[SPINT], c: &mut [SPINT]) {
    let mut t = 0 as DPINT;
    let mut tt: DPINT;
    let mut lo: SPINT;
    let mut hi: SPINT;
    let mask = ((1 as SPINT) << 29) - 1;
    tt = (c[1] as DPINT) * (b[8] as DPINT);
    tt += (c[2] as DPINT) * (b[7] as DPINT);
    tt += (c[3] as DPINT) * (b[6] as DPINT);
    tt += (c[4] as DPINT) * (b[5] as DPINT);
    tt += (c[5] as DPINT) * (b[4] as DPINT);
    tt += (c[6] as DPINT) * (b[3] as DPINT);
    tt += (c[7] as DPINT) * (b[2] as DPINT);
    tt += (c[8] as DPINT) * (b[1] as DPINT);
    lo = (tt as SPINT) & mask;
    t += (lo as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += (c[0] as DPINT) * (b[0] as DPINT);
    let v0 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[2] as DPINT) * (b[8] as DPINT);
    tt += (c[3] as DPINT) * (b[7] as DPINT);
    tt += (c[4] as DPINT) * (b[6] as DPINT);
    tt += (c[5] as DPINT) * (b[5] as DPINT);
    tt += (c[6] as DPINT) * (b[4] as DPINT);
    tt += (c[7] as DPINT) * (b[3] as DPINT);
    tt += (c[8] as DPINT) * (b[2] as DPINT);
    lo = (tt as SPINT) & mask;
    t += ((lo + hi) as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += (c[0] as DPINT) * (b[1] as DPINT);
    t += (c[1] as DPINT) * (b[0] as DPINT);
    let v1 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[3] as DPINT) * (b[8] as DPINT);
    tt += (c[4] as DPINT) * (b[7] as DPINT);
    tt += (c[5] as DPINT) * (b[6] as DPINT);
    tt += (c[6] as DPINT) * (b[5] as DPINT);
    tt += (c[7] as DPINT) * (b[4] as DPINT);
    tt += (c[8] as DPINT) * (b[3] as DPINT);
    lo = (tt as SPINT) & mask;
    t += ((lo + hi) as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += (c[0] as DPINT) * (b[2] as DPINT);
    t += (c[1] as DPINT) * (b[1] as DPINT);
    t += (c[2] as DPINT) * (b[0] as DPINT);
    let v2 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[4] as DPINT) * (b[8] as DPINT);
    tt += (c[5] as DPINT) * (b[7] as DPINT);
    tt += (c[6] as DPINT) * (b[6] as DPINT);
    tt += (c[7] as DPINT) * (b[5] as DPINT);
    tt += (c[8] as DPINT) * (b[4] as DPINT);
    lo = (tt as SPINT) & mask;
    t += ((lo + hi) as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += (c[0] as DPINT) * (b[3] as DPINT);
    t += (c[1] as DPINT) * (b[2] as DPINT);
    t += (c[2] as DPINT) * (b[1] as DPINT);
    t += (c[3] as DPINT) * (b[0] as DPINT);
    let v3 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[5] as DPINT) * (b[8] as DPINT);
    tt += (c[6] as DPINT) * (b[7] as DPINT);
    tt += (c[7] as DPINT) * (b[6] as DPINT);
    tt += (c[8] as DPINT) * (b[5] as DPINT);
    lo = (tt as SPINT) & mask;
    t += ((lo + hi) as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += (c[0] as DPINT) * (b[4] as DPINT);
    t += (c[1] as DPINT) * (b[3] as DPINT);
    t += (c[2] as DPINT) * (b[2] as DPINT);
    t += (c[3] as DPINT) * (b[1] as DPINT);
    t += (c[4] as DPINT) * (b[0] as DPINT);
    let v4 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[6] as DPINT) * (b[8] as DPINT);
    tt += (c[7] as DPINT) * (b[7] as DPINT);
    tt += (c[8] as DPINT) * (b[6] as DPINT);
    lo = (tt as SPINT) & mask;
    t += ((lo + hi) as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += (c[0] as DPINT) * (b[5] as DPINT);
    t += (c[1] as DPINT) * (b[4] as DPINT);
    t += (c[2] as DPINT) * (b[3] as DPINT);
    t += (c[3] as DPINT) * (b[2] as DPINT);
    t += (c[4] as DPINT) * (b[1] as DPINT);
    t += (c[5] as DPINT) * (b[0] as DPINT);
    let v5 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[7] as DPINT) * (b[8] as DPINT);
    tt += (c[8] as DPINT) * (b[7] as DPINT);
    lo = (tt as SPINT) & mask;
    t += ((lo + hi) as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += (c[0] as DPINT) * (b[6] as DPINT);
    t += (c[1] as DPINT) * (b[5] as DPINT);
    t += (c[2] as DPINT) * (b[4] as DPINT);
    t += (c[3] as DPINT) * (b[3] as DPINT);
    t += (c[4] as DPINT) * (b[2] as DPINT);
    t += (c[5] as DPINT) * (b[1] as DPINT);
    t += (c[6] as DPINT) * (b[0] as DPINT);
    let v6 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[8] as DPINT) * (b[8] as DPINT);
    lo = (tt as SPINT) & mask;
    t += ((lo + hi) as DPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
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
    t += (c[0] as DPINT) * (b[8] as DPINT);
    t += (c[1] as DPINT) * (b[7] as DPINT);
    t += (c[2] as DPINT) * (b[6] as DPINT);
    t += (c[3] as DPINT) * (b[5] as DPINT);
    t += (c[4] as DPINT) * (b[4] as DPINT);
    t += (c[5] as DPINT) * (b[3] as DPINT);
    t += (c[6] as DPINT) * (b[2] as DPINT);
    t += (c[7] as DPINT) * (b[1] as DPINT);
    t += (c[8] as DPINT) * (b[0] as DPINT);
    t += (hi as DPINT) * 0x4c0;
    let v8 = (t as SPINT) & mask;
    t = t >> 29;
    // second reduction pass
    let mut nv = v8;
    let mut ut = t as UDPINT;
    ut = (ut << 6) + ((nv >> 23) as UDPINT);
    nv &= 0x7fffff;
    ut *= 19;
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
    c[8] = nv;
    return;
}

// Modular squaring, c=c*c mod 2p
#[allow(unused_variables)]
fn modsqr(c: &mut [SPINT]) {
    let mut t = 0 as UDPINT;
    let mut tt: UDPINT;
    let mut t2: UDPINT;
    let mask = ((1 as SPINT) << 29) - 1;
    let mut lo: SPINT;
    let mut hi: SPINT;
    tt = (c[1] as UDPINT) * (c[8] as UDPINT);
    tt += (c[2] as UDPINT) * (c[7] as UDPINT);
    tt += (c[3] as UDPINT) * (c[6] as UDPINT);
    tt += (c[4] as UDPINT) * (c[5] as UDPINT);
    tt *= 2;
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[0] as UDPINT);
    t2 += (lo as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v0 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[2] as UDPINT) * (c[8] as UDPINT);
    tt += (c[3] as UDPINT) * (c[7] as UDPINT);
    tt += (c[4] as UDPINT) * (c[6] as UDPINT);
    tt *= 2;
    tt += (c[5] as UDPINT) * (c[5] as UDPINT);
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[1] as UDPINT);
    t2 *= 2;
    t2 += ((lo + hi) as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v1 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[3] as UDPINT) * (c[8] as UDPINT);
    tt += (c[4] as UDPINT) * (c[7] as UDPINT);
    tt += (c[5] as UDPINT) * (c[6] as UDPINT);
    tt *= 2;
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[2] as UDPINT);
    t2 *= 2;
    t2 += (c[1] as UDPINT) * (c[1] as UDPINT);
    t2 += ((lo + hi) as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v2 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[4] as UDPINT) * (c[8] as UDPINT);
    tt += (c[5] as UDPINT) * (c[7] as UDPINT);
    tt *= 2;
    tt += (c[6] as UDPINT) * (c[6] as UDPINT);
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[3] as UDPINT);
    t2 += (c[1] as UDPINT) * (c[2] as UDPINT);
    t2 *= 2;
    t2 += ((lo + hi) as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v3 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[5] as UDPINT) * (c[8] as UDPINT);
    tt += (c[6] as UDPINT) * (c[7] as UDPINT);
    tt *= 2;
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[4] as UDPINT);
    t2 += (c[1] as UDPINT) * (c[3] as UDPINT);
    t2 *= 2;
    t2 += (c[2] as UDPINT) * (c[2] as UDPINT);
    t2 += ((lo + hi) as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v4 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[6] as UDPINT) * (c[8] as UDPINT);
    tt *= 2;
    tt += (c[7] as UDPINT) * (c[7] as UDPINT);
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[5] as UDPINT);
    t2 += (c[1] as UDPINT) * (c[4] as UDPINT);
    t2 += (c[2] as UDPINT) * (c[3] as UDPINT);
    t2 *= 2;
    t2 += ((lo + hi) as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v5 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[7] as UDPINT) * (c[8] as UDPINT);
    tt *= 2;
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[6] as UDPINT);
    t2 += (c[1] as UDPINT) * (c[5] as UDPINT);
    t2 += (c[2] as UDPINT) * (c[4] as UDPINT);
    t2 *= 2;
    t2 += (c[3] as UDPINT) * (c[3] as UDPINT);
    t2 += ((lo + hi) as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v6 = (t as SPINT) & mask;
    t = t >> 29;
    tt = (c[8] as UDPINT) * (c[8] as UDPINT);
    lo = (tt as SPINT) & mask;
    t2 = (c[0] as UDPINT) * (c[7] as UDPINT);
    t2 += (c[1] as UDPINT) * (c[6] as UDPINT);
    t2 += (c[2] as UDPINT) * (c[5] as UDPINT);
    t2 += (c[3] as UDPINT) * (c[4] as UDPINT);
    t2 *= 2;
    t2 += ((lo + hi) as UDPINT) * 0x4c0;
    hi = (tt >> 29) as SPINT;
    t += t2;
    let v7 = (t as SPINT) & mask;
    t = t >> 29;
    t2 = (c[0] as UDPINT) * (c[8] as UDPINT);
    t2 += (c[1] as UDPINT) * (c[7] as UDPINT);
    t2 += (c[2] as UDPINT) * (c[6] as UDPINT);
    t2 += (c[3] as UDPINT) * (c[5] as UDPINT);
    t2 *= 2;
    t2 += (c[4] as UDPINT) * (c[4] as UDPINT);
    t += (hi as UDPINT) * 0x4c0;
    t += t2;
    let v8 = (t as SPINT) & mask;
    t = t >> 29;
    // second reduction pass
    let mut nv = v8;
    let mut ut = t as UDPINT;
    ut = (ut << 6) + ((nv >> 23) as UDPINT);
    nv &= 0x7fffff;
    ut *= 19;
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
    c[8] = nv;
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

//Calculate progenitor - use optimal addition chain
fn modpro(w: &[SPINT], r: &mut [SPINT]) {
    let mut x: [SPINT; 9] = [0; 9];
    let mut z: [SPINT; 9] = [0; 9];
    let mut t0: [SPINT; 9] = [0; 9];
    let mut t1: [SPINT; 9] = [0; 9];
    modcpy(w, &mut x);
    modcpy(&x, &mut z);
    modsqr(&mut z);
    modmul(&x, &mut z);
    modcpy(&z, &mut t0);
    modnsqr(&mut t0, 2);
    modmul(&t0, &mut z);
    modsqr(&mut z);
    modcpy(&x, &mut t0);
    modmul(&z, &mut t0);
    modcpy(&t0, &mut z);
    modnsqr(&mut z, 5);
    modmul(&t0, &mut z);
    modcpy(&z, &mut t1);
    modnsqr(&mut t1, 5);
    modmul(&t1, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 15);
    modmul(&t1, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 30);
    modmul(&t1, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 60);
    modmul(&t1, &mut t0);
    modcpy(&t0, &mut t1);
    modnsqr(&mut t1, 120);
    modmul(&t1, &mut t0);
    modnsqr(&mut t0, 10);
    modmul(&t0, &mut z);
    modnsqr(&mut z, 2);
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
    let dd=d as SPINT;
    for i in 0..9 {
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
    for i in 0..9 {
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
    let mut t: [SPINT; 9] = [0; 9];
    let mut b: [SPINT; 9] = [0; 9];
    let mut v: [SPINT; 9] = [0; 9];
    let mut z: [SPINT; 9] = [
        0xa0ea0b0, 0x770d93a, 0xbf91e31, 0x6300d5a, 0x1d7a72f4, 0x4c9efd, 0x1c2cad34, 0x1009f83b,
        0x2b8324,
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
const NBITS: usize = 255;
pub const NBYTES: usize = 32;

const MERSENNE: bool = true;
const MONTGOMERY: bool = false;

const MULBYINT: bool = true;

/*** End of automatically generated code ***/

fn char2int(inp: u8) -> u8 {
    if inp>='0' as u8 && inp <='9' as u8 {
        return inp-'0' as u8;
    }
    if inp>='A' as u8 && inp <='F' as u8 {
        return inp-('A' as u8) +10;
    }
    if inp>='a' as u8 && inp <='f' as u8 {
        return inp-('a' as u8) +10;
    }
    return 0;
}

// string s better have even number of characters!
fn from_hex(s: &str,x: &mut[u8]) {
    let mut pad:[u8;2*NBYTES]=[0;2*NBYTES];
    let c=s.as_bytes();
    let len=c.len();
    let mut lz=2*NBYTES-len;
    if 2*NBYTES<len {lz=0;}
    for i in 0..lz {
        pad[i]='0' as u8;
    }
    for i in lz..2*NBYTES {
        pad[i]=c[i-lz];
    }

    for i in 0..NBYTES {
        x[i]=char2int(pad[2*i])*16+char2int(pad[2*i+1]);
    }
}

fn printhex(array: &[u8]) {
    for i in 0..array.len() {
        print!("{:02X}", array[i])
    }
    println!("")
}

// output a modulo number in hex
#[allow(dead_code)]
fn output(x: &[SPINT]) {
    let mut b:[u8;NBYTES]=[0;NBYTES];
    let mut c:[SPINT;NLIMBS]=[0;NLIMBS];
    modcpy(x,&mut c);
    modexp(&c,&mut b);
    printhex(&b);
}

// Describe Montgomery Curve parameters
// ******
//X25519
const A24: usize = 121665;
const COF: usize = 3;
const GENERATOR: u8=9;
const TWIST_SECURE: bool = true;


// clamp input - see RFC7748
fn clamp(bk: &mut [u8]) {
    let s=(8-(NBITS%8))%8;
    bk[0]&=(-(1<<COF)) as u8;
    let mask=(0xff as u8)>>s;
    bk[NBYTES-1]&=mask;
    bk[NBYTES-1]|=(0x80 as u8)>>s;
}

// extract a bit from a byte array
fn bit(n: usize,a: &[u8]) -> usize {
    return ((a[n / 8] & (1 << (n % 8))) >> (n%8)) as usize;
}

// RFC7748 - Montgomery curve
// bv=bk*bu, bu,bv are x coordinates on elliptic curve
fn rfc7748(bk: &[u8],bu: &[u8],bv: &mut [u8]) {
    let mut swap = 0;
    let mut ck:[u8;NBYTES]=[0;NBYTES];
    let mut cu:[u8;NBYTES]=[0;NBYTES];
    let mut u:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut x1:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut x2:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut x3:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut z2:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut z3:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut a:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut b:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut aa:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut bb:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut c:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut d:[SPINT;NLIMBS]=[0;NLIMBS];
    let mut e:[SPINT;NLIMBS]=[0;NLIMBS];

    for i in 0..NBYTES {
        ck[i]=bk[i];
        cu[i]=bu[i];
    }

    cu.reverse(); // convert from little to big endian
//X25519 only!
    cu[0]&=0x7f;   // implementations of X25519 (but not X448) MUST mask the most significant bit in the final byte

// clamp input
    clamp(&mut ck);

// import into internal representation
    modimp(&cu,&mut u);

    modcpy(&u,&mut x1);  // x_1=u
    modone(&mut x2);     // x_2=1
    modzer(&mut z2);     // z_2=0
    modcpy(&u,&mut x3);  // x_3=u
    modone(&mut z3);     // z_3=1

    for i in (0..NBITS).rev() {
        let kt=bit(i,&ck);
        swap^=kt;
        modcsw(swap,&mut x2,&mut x3);
        modcsw(swap,&mut z2,&mut z3);
        swap=kt;
            
        modcpy(&x2,&mut a); modadd(&z2,&mut a);   // A = x_2 + z_2
        modcpy(&a,&mut aa); modsqr(&mut aa);      // AA = A^2
        modcpy(&x2,&mut b); modsub(&z2,&mut b);   // B = x_2 - z_2
        modcpy(&b,&mut bb); modsqr(&mut bb);      // BB = B^2

        modcpy(&aa,&mut e); modsub(&bb,&mut e);   // E = AA - BB
        modcpy(&x3,&mut c); modadd(&z3,&mut c);   // C = x_3 + z_3

        modcpy(&x3,&mut d); modsub(&z3,&mut d);   // D = x_3 - z_3
        modmul(&a,&mut d);  // DA = D * A
        modmul(&b,&mut c);  // CB = C * B
 
        modcpy(&d,&mut x3); modadd(&c,&mut x3); modsqr(&mut x3); // x_3 = (DA + CB)^2

        modcpy(&d,&mut z3); modsub(&c,&mut z3); modsqr(&mut z3); modmul(&x1,&mut z3); // z_3 = x_1 * (DA - CB)^2
        modcpy(&aa,&mut x2); modmul(&bb,&mut x2); // x_2 = AA * BB
       
        modcpy(&e,&mut z2);
        modmli(A24,&mut z2); 
              
        modadd(&aa,&mut z2); modmul(&e,&mut z2);   // z_2 = E * (AA + a24 * E)
    }
    modcsw(swap,&mut x2,&mut x3);
    modcsw(swap,&mut z2,&mut z3);

    if TWIST_SECURE {
        modpro(&z2,&mut a);       
        modinv(Some(&a),&mut z2);    // sufficient for twist secure curves like X25519 and X448 
    } else { // Do cheap point validation here - see https://eprint.iacr.org/2020/1497
        modcpy(&u,&mut b); modmul(&z2,&mut b);
        modcpy(&b,&mut a); modmul(&z2,&mut a);
        modpro(&a,&mut e);
        modcpy(&a,&mut c);
        modcpy(&e,&mut d); modmul(&z2,&mut d);
        modsqr(&mut d);
        modmul(&u,&mut d);

        for _ in 0..COF-2 {  // COF is 2 or 3
            modsqr(&mut c);
            modmul(&a,&mut c);
        }
        for _ in 0..COF {
            modsqr(&mut e);
        }
        modmul(&e,&mut c);
        modcpy(&c,&mut z2); modmul(&b,&mut z2);

        for _ in 0..COF-2 {
            modsqr(&mut d);
        }
        modone(&mut a); modadd(&a,&mut d); modfsb(&mut d); modshr(1,&mut d); // 1 for QR, else 0
        modmul(&d,&mut x2); // set to zero for bad input point
    }
    modmul(&z2,&mut x2);   

    modexp(&x2,bv);
    bv.reverse(); // convert to little endian
}


pub fn KEY_PAIR(sk: &[u8],pk: &mut [u8])
{
    let mut bu:[u8;NBYTES]=[0;NBYTES];
    bu[0]=GENERATOR;
    rfc7748(sk,&bu,pk);
}

pub fn SHARED_SECRET(sk: &[u8],pk: &[u8],ss: &mut [u8]) -> bool
{
    let mut ors=0 as u8;
    rfc7748(sk,pk,ss);

// all zeros is suspect...
    for i in 0..NBYTES {
        ors|=ss[i];
    }
    if ors==0 {
        return false;
    }
    return true;
}
