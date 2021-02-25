#ifndef PAIR_BN254_H
#define PAIR_BN254_H

#include "fp12_BN254.h"
#include "ecp2_BN254.h"
#include "ecp_BN254.h"

/* Pairing constants */

extern const BIG_256_56 CURVE_Bnx_BN254; /**< BN curve x parameter */
extern const BIG_256_56 CURVE_Cru_BN254; /**< BN curve Cube Root of Unity */

extern const BIG_256_56 CURVE_W_BN254[2];     /**< BN curve constant for GLV decomposition */
extern const BIG_256_56 CURVE_SB_BN254[2][2]; /**< BN curve constant for GLV decomposition */
extern const BIG_256_56 CURVE_WB_BN254[4];     /**< BN curve constant for GS decomposition */
extern const BIG_256_56 CURVE_BB_BN254[4][4]; /**< BN curve constant for GS decomposition */

/* Pairing function prototypes */

/**	@brief Precompute line functions for n-pairing
 *
	@param r array of precomputed FP12 products of line functions
	@param PV ECP2 instance, an element of G2
	@param QV ECP instance, an element of G1

 */
extern void PAIR_BN254_another(FP12_BN254 r[], ECP2_BN254 *PV, ECP_BN254 *QV);

/**	@brief Calculate Miller loop for Optimal ATE pairing e(P,Q)
 *
	@param r FP12 result of the pairing calculation e(P,Q)
	@param P ECP2 instance, an element of G2
	@param Q ECP instance, an element of G1

 */
extern void PAIR_BN254_ate(FP12_BN254 *r, ECP2_BN254 *P, ECP_BN254 *Q);

/**	@brief Calculate Miller loop for Optimal ATE double-pairing e(P,Q).e(R,S)
 *
	Faster than calculating two separate pairings
	@param r FP12 result of the pairing calculation e(P,Q).e(R,S), an element of GT
	@param P ECP2 instance, an element of G2
	@param Q ECP instance, an element of G1
	@param R ECP2 instance, an element of G2
	@param S ECP instance, an element of G1
 */
extern void PAIR_BN254_double_ate(FP12_BN254 *r, ECP2_BN254 *P, ECP_BN254 *Q, ECP2_BN254 *R, ECP_BN254 *S);

/**	@brief Final exponentiation of pairing, converts output of Miller loop to element in GT
 *
	Here p is the internal modulus, and r is the group order
	@param x FP12, on exit = x^((p^12-1)/r)
 */
extern void PAIR_BN254_fexp(FP12_BN254 *x);

/**	@brief Fast point multiplication of a member of the group G1 by a BIG number
 *
	May exploit endomorphism for speed.
	@param Q ECP member of G1.
	@param b BIG multiplier

 */
extern void PAIR_BN254_G1mul(ECP_BN254 *Q, BIG_256_56 b);

/**	@brief Fast point multiplication of a member of the group G2 by a BIG number
 *
	May exploit endomorphism for speed.
	@param P ECP2 member of G1.
	@param b BIG multiplier

 */
extern void PAIR_BN254_G2mul(ECP2_BN254 *P, BIG_256_56 b);

/**	@brief Fast raising of a member of GT to a BIG power
 *
	May exploit endomorphism for speed.
	@param x FP12 member of GT.
	@param b BIG exponent

 */
extern void PAIR_BN254_GTpow(FP12_BN254 *x, BIG_256_56 b);

/**	@brief Tests FP12 for membership of GT
 *
	@param x FP12 instance
	@return 1 if x is in GT, else return 0

 */
extern int PAIR_BN254_GTmember(FP12_BN254 *x);

/**	@brief Prepare Ate parameter
 *
	@param n BIG parameter
	@param n3 BIG paramter = 3*n
	@return number of nits in n3

 */
extern int PAIR_BN254_nbits(BIG_256_56 n3, BIG_256_56 n);

/**	@brief Initialise structure for multi-pairing
 *
	@param r FP12 array, to be initialised to 1

 */
extern void PAIR_BN254_initmp(FP12_BN254 r[]);


/**	@brief Miller loop
 *
 	@param res FP12 result
	@param r FP12 precomputed array of accumulated line functions

 */
extern void PAIR_BN254_miller(FP12_BN254 *res, FP12_BN254 r[]);

#endif
