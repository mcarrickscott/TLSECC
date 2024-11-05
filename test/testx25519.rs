#![allow(unused_imports)]
extern crate tlsecc;

use std::str;

use tlsecc::x25519::NBYTES;
use tlsecc::x25519::KEY_PAIR;
use tlsecc::x25519::SHARED_SECRET;

// Some utility functions for I/O and debugging

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
fn from_hex(ilen:usize,s: &str,x: &mut[u8]) {
    let mut pad:[u8;128]=[0;128];
    let c=s.as_bytes();
    let len=c.len();
    let mut lz=2*ilen-len;
    if 2*ilen<len {lz=0;}
    for i in 0..lz {
        pad[i]='0' as u8;
    }
    for i in lz..2*ilen {
        pad[i]=c[i-lz];
    }

    for i in 0..ilen {
        x[i]=char2int(pad[2*i])*16+char2int(pad[2*i+1]);
    }
}

fn printhex(len:usize,array: &[u8]) {
    for i in 0..len {
        print!("{:02X}", array[i])
    }
    println!("")
}

// a test vector for x25519 from RFC7748
fn main() {
    const SK:&str="77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a";
    let mut bk:[u8;NBYTES]=[0;NBYTES];
    let mut bv:[u8;NBYTES]=[0;NBYTES];

// convert to byte array
    from_hex(NBYTES,&SK,&mut bk);

    KEY_PAIR(&bk,&mut bv);

    println!("{}",&SK);
    printhex(NBYTES,&bv);
}