// Test vectors from Wycheproof
//
// Create a working directory. Copy into it this file, the python script parse.py, and all files from https://github.com/mcarrickscott/TLSECC/c64 and https://github.com/mcarrickscott/TLSECC/include64
//
// Next download Wycheproof testvectors from https://github.com/C2SP/wycheproof
// Then copy https://github.com/C2SP/wycheproof/blob/master/testvectors/ed448_test.json into the work directory
// Move to the work directory and process the test vectors into a form digestible by simple C code
//
// python3 parse.py ed448_test.json > tvs448.txt
//
// Finally compile and run
//
// gcc -O2 tests448.c s448.c ed448.c hash.c -o tests448
//
// run the program and search its output for the word DISAGREE

#include <stdio.h>
#include "tlsecc.h"
#include <string.h>

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
    char pad[4096];
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

int process(char *line)
{
    int len=0;
    while (line[len]!='\n' && line[len]!='\r')
    {
        len++;
    }
    line[len]=0;
    return len;
}

#define BYTES 56

int main()
{
    int res,len,lenm,lens,maxlen=0;
    char *ptr;
    FILE *fp;

    char buff[4096],pub[BYTES+1];
    char m[4096],sig[2*BYTES+2];

    fp=fopen("tvs448.txt","rt");

    while(fgets(buff, 4096, fp)) {
        len=process(buff); if (len>maxlen) maxlen=len;
        //printf("pub= %s\n", buff);
        fromHex(BYTES+1,buff,pub);

        ptr=fgets(buff, 4096, fp);
        len=process(buff); if (len>maxlen) maxlen=len;
        if (len>0)
            printf("%s ", buff);
        else 
            printf("no comment ");    

        ptr=fgets(buff,4096,fp);
        len=process(buff); if (len>maxlen) maxlen=len;
        //printf("mess= %s\n", buff);
        fromHex(len/2,buff,m); lenm=len/2;

        ptr=fgets(buff, 4096, fp);
        len=process(buff); if (len>maxlen) maxlen=len; lens=len;
        //printf("sig= %s\n", buff);
        fromHex(2*BYTES+2,buff,sig);

        ptr=fgets(buff, 4096, fp);
        len=process(buff); if (len>maxlen) maxlen=len;
        printf(" - Wycheproof result is %s - ", buff);

	    res=ED448_VERIFY(pub,lenm,m,sig);
	
    	if (res && lens/2==2*BYTES+2) {
        	printf("our signature is valid\n");
        	if (strcmp(buff,"valid")!=0) printf("DISAGREE\n");
    	} else {
        	printf("our signature is invalid\n");
            if (strcmp(buff,"acceptable")!=0) {
        	    if (strcmp(buff,"valid")==0) printf("DISAGREE\n");
            }
        }
//break;
    }
    printf("maxlen= %d\n",maxlen);
    fclose(fp);
    return 0; 
}
