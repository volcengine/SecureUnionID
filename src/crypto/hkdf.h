#ifndef HDKF_H
#define HDKF_H

#include "amcl.h"
#include "string.h"
#define blockSize 72
#define outputSize 64

void HMAC_SHA3_512(unsigned char *key, int klen, unsigned char *message, int mlen, unsigned char *hmessage);

void HKDF(unsigned char *masterkey, int klen, unsigned char *message, int mlen, unsigned char *childkey);

#endif