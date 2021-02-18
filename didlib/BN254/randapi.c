#include "randapi.h"
#include "string.h"
//#include <random>

/* Initialise a Cryptographically Strong Random Number Generator from
   an octet of raw random data */

void CREATE_CSPRNG(csprng *RNG, octet *RAW) {
    RAND_seed(RNG, RAW->len, RAW->val);
}

void KILL_CSPRNG(csprng *RNG) {
    RAND_clean(RNG);
}

/*void random_init(unsigned char *nonce, unsigned char *seed) {
    int i;

    srand((unsigned int) time(NULL));

    if (seed == NULL) {
        std::random_device rand_div;
        for (i = 0; i < 32; i++) {
            nonce[i] = rand_div() ^ (rand() & 0xff);
        }
    } else {
        for (i = 0; i < 32; i++) {
            nonce[i] = seed[i];
        }
    }
    for (i = 32; i < 64; i++) {
        nonce[i] = 0;
    }
    nonce[31] = 0x1;

    return;
}

void random_data(unsigned char *data, int nbytes, unsigned char *nonce) {
    int i, j;
    unsigned char out[32];

    unsigned char seed[32], counter[32];
    hash256 sh;
    HASH256_init(&sh);
    for (i = 0; i < nbytes / 32; i++) {
        for (j = 0; j < 32; j++) {
            HASH256_process(&sh, seed[j]);
        }

        for (j = 0; j < 32; j++) {
            HASH256_process(&sh, counter[j]);
        }

        HASH256_hash(&sh, (char *) data + 32 * i);

        for (j = 31; j >= 0; j--) {
            counter[j] += 1;
            if (counter[j] != 0) {
                break;
            }
        }
    }

    if (nbytes % 32 != 0) {
        for (j = 0; j < 32; j++) {
            HASH256_process(&sh, seed[j]);
        }
        for (j = 0; j < 32; j++) {
            HASH256_process(&sh, counter[j]);
        }

        HASH256_hash(&sh, (char *) out);
        memcpy((nbytes / 32 * 32) + (char *) data, out, nbytes % 32);

        for (j = 31; j >= 0; j--) {
            counter[j] += 1;
            if (counter[j] != 0) {
                break;
            }
        }
    }

    for (i = 0; i < 32; i++) {
        nonce[i] = seed[i];
        nonce[i + 32] = counter[i];
    }
    return;
}

void KDF(unsigned char *data, int dataLen, unsigned char *seed, int seedLen, unsigned char *nonce) {
    int i, j;
    unsigned char out[32];
    unsigned char counter[32] = {0};
    hash256 sh;
    HASH256_init(&sh);
    if(nonce != NULL){
        for (i = 0; i < 32; i++) {
            counter[i] = nonce[i];
        }
    }
    for (i = 0; i < dataLen / 32; i++) {
        for (j = 0; j < seedLen; j++) {
            HASH256_process(&sh, seed[j]);
        }

        for (j = 0; j < 32; j++) {
            HASH256_process(&sh, counter[j]);
        }

        HASH256_hash(&sh, (char *) (data + 32 * i));

        for (j = 31; j >= 0; j--) {
            counter[j] += 1;
            if (counter[j] != 0) {
                break;
            }
        }
    }

    if (dataLen % 32 != 0) {
        for (j = 0; j < 32; j++) {
            HASH256_process(&sh, seed[j]);
        }
        for (j = 0; j < 32; j++) {
            HASH256_process(&sh, counter[j]);
        }

        HASH256_hash(&sh, (char *) out);
        memcpy((dataLen / 32 * 32) + (char *) data, out, dataLen % 32);

        for (j = 31; j >= 0; j--) {
            counter[j] += 1;
            if (counter[j] != 0) {
                break;
            }
        }
    }

    if (nonce != NULL) {
        for (i = 0; i < 32; i++) {
            nonce[i] = counter[i];
        }
    }
    return;
}*/
