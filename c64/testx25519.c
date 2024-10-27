// a test vector for x25519 from RFC7748
// gcc -O2 testx25519.c x25519.c -o testx25519

#include <stdio.h>
#include "tlsecc.h"

static int char2int(char input)
{
    if ((input >= '0') && (input <= '9'))
        return input - '0';
    if ((input >= 'A') && (input <= 'F'))
        return input - 'A' + 10;
    if ((input >= 'a') && (input <= 'f'))
        return input - 'a' + 10;
    return 0;
}

// Convert from a hex string to byte array 
static void fromHex(int ilen, const char *src, char *dst)
{
    int i,lz,len=0;
    char pad[300];
    while (src[len]!=0) len++;
    lz=2*ilen-len;
    if (lz<0) lz=0;
    for (i=0;i<lz;i++) pad[i]='0';  // pad with leading zeros
    for (i=lz;i<2*ilen;i++) pad[i]=src[i-lz];

    for (i=0;i<ilen;i++)
    {
        dst[i] = (char2int(pad[2*i]) * 16) + char2int(pad[2*i + 1]);
    }
}

static void byte2hex(char *ptr,unsigned char ch)
{
    int t=ch/16;
    int b=ch%16;
    if (t<10)
    	ptr[0]='0'+t;
    else
    	ptr[0]='a'+(t-10);
    if (b<10)
    	ptr[1]='0'+b;
    else
    	ptr[1]='a'+(b-10);    	
}

// Convert a byte array to a hex string 
static void toHex(int len, const char *src, char *dst)
{
    int i;
    for (i = 0; i < len; i++)
    {
        unsigned char ch = src[i];
        byte2hex(&dst[i * 2],ch);
    }
    dst[2*len]='\0';
}

#define BYTES 32

int main()
{
    const char *sk=(const char *)"77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a";

    char sv[(BYTES*2)+1];
    char bk[BYTES],bv[BYTES];

// convert to byte array
    fromHex(BYTES,sk,bk);
    X25519_KEY_PAIR(bk,bv);
   
// convert to Hex
    toHex(BYTES,bv,sv);

    puts(sk); 
    puts(sv); 
}

