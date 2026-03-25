
// Test program for ed376 signature
// gcc -O2 tests376.c ed376.c edwards376.c hash.c -o tests376

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
    char pad[256];
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

#define BYTES 48

// Test vector
int main()
{
    const char *sk=(const char *)"c4eab05d357007c632f3dbb48489924d552b08fe0c353a0d4a1f00acda2c463afbea67c5e8d2877c5e3bc397a659949e";
    char prv[BYTES],pub[BYTES],sig[2*BYTES];
    char buff[256],m[2];
    int res;
    printf("private key= "); puts(sk); 
    fromHex(BYTES,sk,prv);
    ED376_KEY_PAIR(prv,pub);
    toHex(BYTES,pub,buff);
    printf("public key=  "); puts(buff);

    m[0]=0x03; // message to be signed
    ED376_SIGN(prv,pub,1,m,sig);
    toHex(2*BYTES,sig,buff);
    printf("signature=  "); puts(buff); 

    res=ED376_VERIFY(pub,1,m,sig);
    if (res)
        printf("Signature is valid\n");
    else
        printf("Signature is NOT valid\n");
}

