extern crate tlsecc;

use std::str;

use tlsecc::ed376::BYTES;
use tlsecc::ed376::KEY_PAIR;
use tlsecc::ed376::SIGN;
use tlsecc::ed376::VERIFY;


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

fn main() {
    const SK:&str="c4eab05d357007c632f3dbb48489924d552b08fe0c353a0d4a1f00acda2c463afbea67c5e8d2877c5e3bc397a659949e";

    let mut prv:[u8;BYTES]=[0;BYTES];
    let mut public:[u8;BYTES]=[0;BYTES];
    let mut sig:[u8;2*BYTES]=[0;2*BYTES];
    let mut m:[u8;2]=[0;2];

    from_hex(BYTES,&SK,&mut prv); 
    print!("private key= "); printhex(BYTES,&prv);
    KEY_PAIR(&prv,&mut public);
    print!("Public key= "); printhex(BYTES,&public);

    m[0]=0x03; // message to be signed
    SIGN(&prv,Some(&public),&m[0..1],&mut sig);
    print!("signature=  "); printhex(2*BYTES,&sig); 

    let res=VERIFY(&public,&m[0..1],&sig);
    if res {
        println!("Signature is valid");
    } else {
        println!("Signature is NOT valid");
    }
}