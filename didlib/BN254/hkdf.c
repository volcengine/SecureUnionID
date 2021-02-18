#include "hkdf.h"

void HMAC_SHA3_512(unsigned char *key, int klen, unsigned char *message, int mlen, unsigned char *hmessage){
    sha3 hs;
    int i;
    unsigned char hashkey[blockSize]={0};
    if(klen>blockSize){
        SHA3_init(&hs, SHA3_HASH512);
        for (i = 0; i<klen; i++) SHA3_process(&hs, key[i]);
        SHA3_hash(&hs, (char*)hashkey);
    }
    else if(klen<blockSize){
        for (i = 0; i<klen; i++)
            hashkey[i]=key[i];
    }

    unsigned char o_key_pad[blockSize], i_key_pad[blockSize];
    for(i=0;i<blockSize;i++){
        o_key_pad[i]=hashkey[i]^0x5c;
        i_key_pad[i]=hashkey[i]^0x36;
    }

    unsigned char temp[mlen+blockSize];
    memcpy(temp,i_key_pad,blockSize);
    memcpy(temp+blockSize,message,mlen);
    SHA3_init(&hs, SHA3_HASH512);
    for (i = 0; i<mlen+blockSize; i++) SHA3_process(&hs, temp[i]);
    SHA3_hash(&hs, (char*)hmessage);

    memcpy(temp,o_key_pad,blockSize);
    memcpy(temp+blockSize,hmessage,mlen);
    SHA3_init(&hs, SHA3_HASH512);
    for (i = 0; i<mlen+blockSize; i++) SHA3_process(&hs, temp[i]);
    SHA3_hash(&hs, (char*)hmessage);
}

void HKDF(unsigned char *masterkey, int klen, unsigned char *message, int mlen, unsigned char *childkey){
    sha3 hs;
    int i;
    unsigned char hashkey[outputSize];
    SHA3_init(&hs, SHA3_HASH512);
    for (i = 0; i<klen; i++) SHA3_process(&hs, masterkey[i]);
    SHA3_hash(&hs, (char*)hashkey);

    unsigned char data[outputSize/2+mlen];
    memcpy(data,masterkey+outputSize/2,outputSize/2);
    memcpy(data+outputSize/2,message,mlen);
    unsigned char temp[outputSize];
    HMAC_SHA3_512(hashkey,outputSize/2,data,outputSize/2+mlen,temp);

    memcpy(childkey,temp,32);
}