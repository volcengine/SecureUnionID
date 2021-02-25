#ifndef RANDOMAPI_H
#define RANDOMAPI_H

#include "amcl.h"

/**	@brief Initialise a random number generator
 *
	@param R is a pointer to a cryptographically secure random number generator
	@param S is an input truly random seed value
 */
extern void CREATE_CSPRNG(csprng *RNG, octet *RAW);

/**	@brief Kill a random number generator
 *
	Deletes all internal state
	@param R is a pointer to a cryptographically secure random number generator
 */
extern void KILL_CSPRNG(csprng *RNG);

/**
input:
 seed, 32bytes, seed=NULL(when init)
output:
 nonce value, 64bytes
*/
void random_init(unsigned char *nonce, unsigned char *seed);

/**
input:
 nonce, nonce value, 64bytes
 nbytes, len of data
output:
 data
*/
void random_data(unsigned char *data, int nbytes, unsigned char *nonce);

/**
input:
 nonce,  nonce value, 32bytes
 seed,
 seedLen, len of seed
output:
 data, nbytes(len)
*/
void KDF(unsigned char *data, int dataLen, unsigned char *seed, int seedLen, unsigned char *nonce);


#endif

