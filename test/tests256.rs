
extern crate tlsecc;

use std::str;

use tlsecc::s256::BYTES;
use tlsecc::s256::NIST256_KEY_PAIR;
use tlsecc::s256::NIST256_SIGN;
use tlsecc::s256::NIST256_VERIFY;


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



// test vector for FIPS 186-3 ECDSA Signature Generation
fn main() {
    const SK:&str="519b423d715f8b581f4fa8ee59f4771a5b44c8130b4e3eacca54a56dda72b464";
    const RAN:&str="94a1bbb14b906a61a280f245f9e93c7f3b4a6247824f5d33b9670787642a68deb9670787642a68de";
    const MSG:&str="44acf6b7e36c1342c2c5897204fe09504e1e2efb1a900377dbc4e7a6a133ec56";
    let mut k:[u8;BYTES+8]=[0;BYTES+8];
    let mut m:[u8;BYTES]=[0;BYTES];
    let mut prv:[u8;BYTES]=[0;BYTES];
    let mut public:[u8;2*BYTES+1]=[0;2*BYTES+1];
    let mut sig:[u8;2*BYTES]=[0;2*BYTES];
    let compress:bool=true;
    println!("Run test vector");
    from_hex(BYTES,&SK,&mut prv);
    print!("private key= "); printhex(BYTES,&prv);
    from_hex(BYTES+8,&RAN,&mut k);
    from_hex(BYTES,&MSG,&mut m);
    NIST256_KEY_PAIR(compress,&prv,&mut public);  
    print!("Public key= ");
    if compress {
        printhex(BYTES+1,&public);
    } else {
        printhex(2*BYTES+1,&public);
    }
    NIST256_SIGN(&prv,&k,&m[0..32],&mut sig);
    print!("signature= "); printhex(2*BYTES,&sig);

    let res=NIST256_VERIFY(&public,&m[0..32],&sig);
    if res {
        println!("Signature is valid");
    } else {
        println!("Signature is NOT valid");
    }
}

