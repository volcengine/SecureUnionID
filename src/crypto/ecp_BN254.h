#ifndef ECP_BN254_H
#define ECP_BN254_H

#include "fp_BN254.h"
#include "config_curve_BN254.h"

/* Curve Params - see rom_zzz.c */
extern const int CURVE_A_BN254;         /**< Elliptic curve A parameter */
extern const int CURVE_Cof_I_BN254;     /**< Elliptic curve cofactor */
extern const int CURVE_B_I_BN254;       /**< Elliptic curve B_i parameter */
extern const BIG_256_56 CURVE_B_BN254;     /**< Elliptic curve B parameter */
extern const BIG_256_56 CURVE_Order_BN254; /**< Elliptic curve group order */
extern const BIG_256_56 CURVE_Cof_BN254;   /**< Elliptic curve cofactor */

/* Generator point on G1 */
extern const BIG_256_56 CURVE_Gx_BN254; /**< x-coordinate of generator point in group G1  */
extern const BIG_256_56 CURVE_Gy_BN254; /**< y-coordinate of generator point in group G1  */


/* For Pairings only */

/* Generator point on G2 */
extern const BIG_256_56 CURVE_Pxa_BN254; /**< real part of x-coordinate of generator point in group G2 */
extern const BIG_256_56 CURVE_Pxb_BN254; /**< imaginary part of x-coordinate of generator point in group G2 */
extern const BIG_256_56 CURVE_Pya_BN254; /**< real part of y-coordinate of generator point in group G2 */
extern const BIG_256_56 CURVE_Pyb_BN254; /**< imaginary part of y-coordinate of generator point in group G2 */

extern const BIG_256_56 CURVE_Bnx_BN254; /**< BN curve x parameter */

extern const BIG_256_56 CURVE_Cru_BN254; /**< BN curve Cube Root of Unity */

extern const BIG_256_56 Fra_BN254; /**< real part of BN curve Frobenius Constant */
extern const BIG_256_56 Frb_BN254; /**< imaginary part of BN curve Frobenius Constant */


extern const BIG_256_56 CURVE_W_BN254[2];     /**< BN curve constant for GLV decomposition */
extern const BIG_256_56 CURVE_SB_BN254[2][2]; /**< BN curve constant for GLV decomposition */
extern const BIG_256_56 CURVE_WB_BN254[4];     /**< BN curve constant for GS decomposition */
extern const BIG_256_56 CURVE_BB_BN254[4][4]; /**< BN curve constant for GS decomposition */


/**
	@brief ECP structure - Elliptic Curve Point over base field
*/

typedef struct {
//    int inf; /**< Infinity Flag - not needed for Edwards representation */
    FP_BN254 x; /**< x-coordinate of point */
    FP_BN254 y; /**< y-coordinate of point. Not needed for Montgomery representation */
    FP_BN254 z;/**< z-coordinate of point */
} ECP_BN254;


/* ECP E(Fp) prototypes */
/**	@brief Tests for ECP point equal to infinity
 *
	@param P ECP point to be tested
	@return 1 if infinity, else returns 0
 */
extern int ECP_BN254_isinf(ECP_BN254 *P);

/**	@brief Tests for equality of two ECPs
 *
	@param P ECP instance to be compared
	@param Q ECP instance to be compared
	@return 1 if P=Q, else returns 0
 */
extern int ECP_BN254_equals(ECP_BN254 *P, ECP_BN254 *Q);

/**	@brief Copy ECP point to another ECP point
 *
	@param P ECP instance, on exit = Q
	@param Q ECP instance to be copied
 */
extern void ECP_BN254_copy(ECP_BN254 *P, ECP_BN254 *Q);

/**	@brief Negation of an ECP point
 *
	@param P ECP instance, on exit = -P
 */
extern void ECP_BN254_neg(ECP_BN254 *P);

/**	@brief Set ECP to point-at-infinity
 *
	@param P ECP instance to be set to infinity
 */
extern void ECP_BN254_inf(ECP_BN254 *P);

/**	@brief Calculate Right Hand Side of curve equation y^2=f(x)
 *
	Function f(x) depends on form of elliptic curve, Weierstrass, Edwards or Montgomery.
	Used internally.
	@param r BIG n-residue value of f(x)
	@param x BIG n-residue x
 */
extern void ECP_BN254_rhs(FP_BN254 *r, FP_BN254 *x);

/**	@brief Set ECP to point(x,y) given x and y
 *
	Point P set to infinity if no such point on the curve.
	@param P ECP instance to be set (x,y)
	@param x BIG x coordinate of point
	@param y BIG y coordinate of point
	@return 1 if point exists, else 0
 */
extern int ECP_BN254_set(ECP_BN254 *P, BIG_256_56 x, BIG_256_56 y);

/**	@brief Extract x and y coordinates of an ECP point P
 *
	If x=y, returns only x
	@param x BIG on exit = x coordinate of point
	@param y BIG on exit = y coordinate of point (unless x=y)
	@param P ECP instance (x,y)
	@return sign of y, or -1 if P is point-at-infinity
 */
extern int ECP_BN254_get(BIG_256_56 x, BIG_256_56 y, ECP_BN254 *P);

/**	@brief Adds ECP instance Q to ECP instance P
 *
	@param P ECP instance, on exit =P+Q
	@param Q ECP instance to be added to P
 */
extern void ECP_BN254_add(ECP_BN254 *P, ECP_BN254 *Q);

/**	@brief Subtracts ECP instance Q from ECP instance P
 *
	@param P ECP instance, on exit =P-Q
	@param Q ECP instance to be subtracted from P
 */
extern void ECP_BN254_sub(ECP_BN254 *P, ECP_BN254 *Q);

/**	@brief Set ECP to point(x,y) given just x and sign of y
 *
	Point P set to infinity if no such point on the curve. If x is on the curve then y is calculated from the curve equation.
	The correct y value (plus or minus) is selected given its sign s.
	@param P ECP instance to be set (x,[y])
	@param x BIG x coordinate of point
	@param s an integer representing the "sign" of y, in fact its least significant bit.
 */
extern int ECP_BN254_setx(ECP_BN254 *P, BIG_256_56 x, int s);

/**	@brief Multiplies Point by curve co-factor
 *
	@param Q ECP instance
 */
extern void ECP_BN254_cfp(ECP_BN254 *Q);

/**	@brief Maps random BIG to curve point of correct order
 *
	@param Q ECP instance of correct order
	@param w OCTET byte array to be mapped
 */
extern void ECP_BN254_mapit(ECP_BN254 *Q, octet *w);

/**	@brief Converts an ECP point from Projective (x,y,z) coordinates to affine (x,y) coordinates
 *
	@param P ECP instance to be converted to affine form
 */
extern void ECP_BN254_affine(ECP_BN254 *P);

/**	@brief Formats and outputs an ECP point to the console, in projective coordinates
 *
	@param P ECP instance to be printed
 */
extern void ECP_BN254_outputxyz(ECP_BN254 *P);

/**	@brief Formats and outputs an ECP point to the console, converted to affine coordinates
 *
	@param P ECP instance to be printed
 */
extern void ECP_BN254_output(ECP_BN254 *P);

/**	@brief Formats and outputs an ECP point to the console
 *
	@param P ECP instance to be printed
 */
extern void ECP_BN254_rawoutput(ECP_BN254 *P);

/**	@brief Formats and outputs an ECP point to an octet string
	The octet string is normally in the standard form 0x04|x|y
	Here x (and y) are the x and y coordinates in left justified big-endian base 256 form.
	For Montgomery curve it is 0x06|x
	If c is true, only the x coordinate is provided as in 0x2|x if y is even, or 0x3|x if y is odd
	@param c compression required, true or false
	@param S output octet string
	@param P ECP instance to be converted to an octet string
 */
extern void ECP_BN254_toOctet(octet *S, ECP_BN254 *P, bool c);

/**	@brief Creates an ECP point from an octet string
 *
	The octet string is normally in the standard form 0x04|x|y
	Here x (and y) are the x and y coordinates in left justified big-endian base 256 form.
	For Montgomery curve it is 0x06|x
	If in compressed form only the x coordinate is provided as in 0x2|x if y is even, or 0x3|x if y is odd
	@param P ECP instance to be created from the octet string
	@param S input octet string
	return 1 if octet string corresponds to a point on the curve, else 0
 */
extern int ECP_BN254_fromOctet(ECP_BN254 *P, octet *S);

/**	@brief Doubles an ECP instance P
 *
	@param P ECP instance, on exit =2*P
 */
extern void ECP_BN254_dbl(ECP_BN254 *P);

/**	@brief Multiplies an ECP instance P by a small integer, side-channel resistant
 *
	@param P ECP instance, on exit =i*P
	@param i small integer multiplier
	@param b maximum number of bits in multiplier
 */
extern void ECP_BN254_pinmul(ECP_BN254 *P, int i, int b);

/**	@brief Multiplies an ECP instance P by a BIG, side-channel resistant
 *
	Uses Montgomery ladder for Montgomery curves, otherwise fixed sized windows.
	@param P ECP instance, on exit =b*P
	@param b BIG number multiplier

 */
extern void ECP_BN254_mul(ECP_BN254 *P, BIG_256_56 b);

/**	@brief Calculates double multiplication P=e*P+f*Q, side-channel resistant
 *
	@param P ECP instance, on exit =e*P+f*Q
	@param Q ECP instance
	@param e BIG number multiplier
	@param f BIG number multiplier
 */
extern void ECP_BN254_mul2(ECP_BN254 *P, ECP_BN254 *Q, BIG_256_56 e, BIG_256_56 f);

/**	@brief Get Group Generator from ROM
 *
	@param G ECP instance
 */
extern void ECP_BN254_generator(ECP_BN254 *G);


#endif
