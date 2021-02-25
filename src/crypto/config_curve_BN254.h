#ifndef CONFIG_CURVE_BN254_H
#define CONFIG_CURVE_BN254_H

#include"amcl.h"
#include"config_field_BN254.h"

// ECP stuff

#define CURVETYPE_BN254 WEIERSTRASS
#define PAIRING_FRIENDLY_BN254 BN
#define CURVE_SECURITY_BN254 128


#if PAIRING_FRIENDLY_BN254 != NOT
#define USE_GLV_BN254      /**< Note this method is patented (GLV), so maybe you want to comment this out */
#define USE_GS_G2_BN254 /**< Well we didn't patent it :) But may be covered by GLV patent :( */
#define USE_GS_GT_BN254 /**< Not patented, so probably safe to always use this */

#define POSITIVEX 0
#define NEGATIVEX 1

#define SEXTIC_TWIST_BN254 D_TYPE
#define SIGN_OF_X_BN254 NEGATIVEX

#define ATE_BITS_BN254 66

#endif

#if CURVE_SECURITY_BN254 == 128
#define AESKEY_BN254 16 /**< Symmetric Key size - 128 bits */
#define HASH_TYPE_BN254 SHA256  /**< Hash type */
#endif

#endif
