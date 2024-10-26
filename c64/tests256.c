// test for FIPS 186-5 ECDSA Signature Generation

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
    const char *sk= (const char *)"519b423d715f8b581f4fa8ee59f4771a5b44c8130b4e3eacca54a56dda72b464";
    const char *ran=(const char *)"94a1bbb14b906a61a280f245f9e93c7f3b4a6247824f5d33b9670787642a68deb9670787642a68de";
    const char *msg=(const char *)"44acf6b7e36c1342c2c5897204fe09504e1e2efb1a900377dbc4e7a6a133ec56";
    char prv[BYTES],pub[2*BYTES+1];
    char buff[256],m[BYTES],k[BYTES+8],sig[2*BYTES];
    int res,compress=1;
    printf("Run test vector\n");
    printf("private key= "); puts(sk); 
    fromHex(BYTES,sk,prv);
    fromHex(BYTES+8,ran,k);
    fromHex(BYTES,msg,m);
    NIST256_KEY_PAIR(compress,prv,pub);
    if (compress)
        toHex(BYTES+1,pub,buff);
    else
        toHex(2*BYTES+1,pub,buff);

    printf("public key= "); puts(buff);
    NIST256_SIGN(prv,k,32,m,sig);
    toHex(2*BYTES,sig,buff);
    printf("signature=  "); puts(buff);

    res=NIST256_VERIFY(pub,32,m,sig);
    if (res)
        printf("Signature is valid\n");
    else
        printf("Signature is NOT valid\n");
}