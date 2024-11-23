// Test vectors from Wycheproof
//
// Create a working directory. Copy into it this file, the python script parse.py, and all files from https://github.com/mcarrickscott/TLSECC/c64 and https://github.com/mcarrickscott/TLSECC/include64
//
// Next download Wycheproof testvectors from https://github.com/C2SP/wycheproof
// Then copy https://github.com/C2SP/wycheproof/blob/master/testvectors/ecdh_secp256r1_ecpoint_test.json into the work directory
// Move to the work directory and process the test vectors into a form digestible by simple C code
//
// python3 parsex.py ecdh_secp256r1_ecpoint_test.json > tvx256.txt
//
// Finally compile and run
//
// gcc -O2 testx256.c s256.c nist256.c hash.c -o testx256
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

#define BYTES 32

int main()
{
    int i,len,same,res;
    char pub[2*BYTES+1],priv[BYTES],sh[BYTES],mysh[BYTES];
    char buff[256];
    FILE *fp=fopen("tvx256.txt","rt");

    while(fgets(buff, 256, fp)!=NULL) {
	    len=process(buff);
	    if (len>0)
	        printf("%s ", buff);
        else 
            printf("no comment ");   

        if (fgets(buff,256,fp)==NULL) break;
        len=process(buff);
        //printf("public= %d %s\n",len, buff);	
        if (len/2==2*BYTES+1)
            fromHex(2*BYTES+1,buff,pub);
        else
            fromHex(BYTES+1,buff,pub); // its compressed

        if (fgets(buff,256,fp)==NULL) break;
        len=process(buff);
        //printf("private= %d %s\n", len, buff);
        if (len>=2*BYTES)
            fromHex(BYTES,&buff[len-2*BYTES],priv);  // skip leading zeros
        else
            fromHex(BYTES,buff,priv);

        if (fgets(buff,256,fp)==NULL) break;
        len=process(buff);
       	//printf("shared= %s\n", buff);         
        fromHex(BYTES,buff,sh);            
                 
    	res=NIST256_SHARED_SECRET(priv,pub,mysh);
    	
    	if (fgets(buff,256,fp)==NULL) break;  // get wycheproof result
    	len=process(buff);
    	
    	printf(" - Wycheproof says its %s", buff);
    	 	
    	same=1;
    	for (i=0;i<BYTES;i++)
            if (mysh[i]!=sh[i]) same=0;

        if (same==0) res=0; 

        if (res) printf(", we say its valid ");
        else printf(", we say its invalid ");    
                        
        if (strcmp(buff,"invalid")==0 && res) printf(" - we DISAGREE"); 
        if (strcmp(buff,"valid")==0 && !res) printf(" - we DISAGREE");
        printf("\n");  	 
   }
   fclose(fp); 
   return 0;         
}

