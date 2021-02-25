#ifndef ECP2_BN254_H
#define ECP2_BN254_H

#include "fp2_BN254.h"
#include "config_curve_BN254.h"

/**
	@brief ECP2 Structure - Elliptic Curve Point over quadratic extension field
*/

typedef struct {
//    int inf; /**< Infinity Flag */
    FP2_BN254 x;   /**< x-coordinate of point */
    FP2_BN254 y;   /**< y-coordinate of point */
    FP2_BN254 z;   /**< z-coordinate of point */
} ECP2_BN254;


/* Curve Params - see rom_zzz.c */
extern const int CURVE_A_BN254;        /**< Elliptic curve A parameter */
extern const int CURVE_B_I_BN254;        /**< Elliptic curve B parameter */
extern const BIG_256_56 CURVE_B_BN254;     /**< Elliptic curve B parameter */
extern const BIG_256_56 CURVE_Order_BN254; /**< Elliptic curve group order */
extern const BIG_256_56 CURVE_Cof_BN254;   /**< Elliptic curve cofactor */
extern const BIG_256_56 CURVE_Bnx_BN254;   /**< Elliptic curve parameter */

extern const BIG_256_56 Fra_BN254; /**< real part of BN curve Frobenius Constant */
extern const BIG_256_56 Frb_BN254; /**< imaginary part of BN curve Frobenius Constant */


/* Generator point on G1 */
extern const BIG_256_56 CURVE_Gx_BN254; /**< x-coordinate of generator point in group G1  */
extern const BIG_256_56 CURVE_Gy_BN254; /**< y-coordinate of generator point in group G1  */

/* For Pairings only */

/* Generator point on G2 */
extern const BIG_256_56 CURVE_Pxa_BN254; /**< real part of x-coordinate of generator point in group G2 */
extern const BIG_256_56 CURVE_Pxb_BN254; /**< imaginary part of x-coordinate of generator point in group G2 */
extern const BIG_256_56 CURVE_Pya_BN254; /**< real part of y-coordinate of generator point in group G2 */
extern const BIG_256_56 CURVE_Pyb_BN254; /**< imaginary part of y-coordinate of generator point in group G2 */

/* ECP2 E(Fp2) prototypes */
/**	@brief Tests for ECP2 point equal to infinity
 *
	@param P ECP2 point to be tested
	@return 1 if infinity, else returns 0
 */
extern int ECP2_BN254_isinf(ECP2_BN254 *P);

/**	@brief Copy ECP2 point to another ECP2 point
 *
	@param P ECP2 instance, on exit = Q
	@param Q ECP2 instance to be copied
 */
extern void ECP2_BN254_copy(ECP2_BN254 *P, ECP2_BN254 *Q);

/**	@brief Set ECP2 to point-at-infinity
 *
	@param P ECP2 instance to be set to infinity
 */
extern void ECP2_BN254_inf(ECP2_BN254 *P);

/**	@brief Tests for equality of two ECP2s
 *
	@param P ECP2 instance to be compared
	@param Q ECP2 instance to be compared
	@return 1 if P=Q, else returns 0
 */
extern int ECP2_BN254_equals(ECP2_BN254 *P, ECP2_BN254 *Q);

/**	@brief Converts an ECP2 point from Projective (x,y,z) coordinates to affine (x,y) coordinates
 *
	@param P ECP2 instance to be converted to affine form
 */
extern void ECP2_BN254_affine(ECP2_BN254 *P);

/**	@brief Extract x and y coordinates of an ECP2 point P
 *
	If x=y, returns only x
	@param x FP2 on exit = x coordinate of point
	@param y FP2 on exit = y coordinate of point (unless x=y)
	@param P ECP2 instance (x,y)
	@return -1 if P is point-at-infinity, else 0
 */
extern int ECP2_BN254_get(FP2_BN254 *x, FP2_BN254 *y, ECP2_BN254 *P);

/**	@brief Formats and outputs an ECP2 point to the console, converted to affine coordinates
 *
	@param P ECP2 instance to be printed
 */
extern void ECP2_BN254_output(ECP2_BN254 *P);

/**	@brief Formats and outputs an ECP2 point to the console, in projective coordinates
 *
	@param P ECP2 instance to be printed
 */
extern void ECP2_BN254_outputxyz(ECP2_BN254 *P);

/**	@brief Formats and outputs an ECP2 point to an octet string
 *
	The octet string is created in the form x|y.
	Convert the real and imaginary parts of the x and y coordinates to big-endian base 256 form.
	@param S output octet string
	@param P ECP2 instance to be converted to an octet string
 */
extern void ECP2_BN254_toOctet(octet *S, ECP2_BN254 *P);

/**	@brief Creates an ECP2 point from an octet string
 *
	The octet string is in the form x|y
	The real and imaginary parts of the x and y coordinates are in big-endian base 256 form.
	@param P ECP2 instance to be created from the octet string
	@param S input octet string
	return 1 if octet string corresponds to a point on the curve, else 0
 */
extern int ECP2_BN254_fromOctet(ECP2_BN254 *P, octet *S);

/**	@brief Calculate Right Hand Side of curve equation y^2=f(x)
 *
	Function f(x)=x^3+Ax+B
	Used internally.
	@param r FP2 value of f(x)
	@param x FP2 instance
 */
extern void ECP2_BN254_rhs(FP2_BN254 *r, FP2_BN254 *x);

/**	@brief Set ECP2 to point(x,y) given x and y
 *
	Point P set to infinity if no such point on the curve.
	@param P ECP2 instance to be set (x,y)
	@param x FP2 x coordinate of point
	@param y FP2 y coordinate of point
	@return 1 if point exists, else 0
 */
extern int ECP2_BN254_set(ECP2_BN254 *P, FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Set ECP to point(x,[y]) given x
 *
	Point P set to infinity if no such point on the curve. Otherwise y coordinate is calculated from x.
	@param P ECP instance to be set (x,[y])
	@param x BIG x coordinate of point
	@return 1 if point exists, else 0
 */
extern int ECP2_BN254_setx(ECP2_BN254 *P, FP2_BN254 *x);

/**	@brief Negation of an ECP2 point
 *
	@param P ECP2 instance, on exit = -P
 */
extern void ECP2_BN254_neg(ECP2_BN254 *P);

/**	@brief Doubles an ECP2 instance P
 *
	@param P ECP2 instance, on exit =2*P
 */
extern int ECP2_BN254_dbl(ECP2_BN254 *P);

/**	@brief Adds ECP2 instance Q to ECP2 instance P
 *
	@param P ECP2 instance, on exit =P+Q
	@param Q ECP2 instance to be added to P
 */
extern int ECP2_BN254_add(ECP2_BN254 *P, ECP2_BN254 *Q);

/**	@brief Subtracts ECP instance Q from ECP2 instance P
 *
	@param P ECP2 instance, on exit =P-Q
	@param Q ECP2 instance to be subtracted from P
 */
extern void ECP2_BN254_sub(ECP2_BN254 *P, ECP2_BN254 *Q);

/**	@brief Multiplies an ECP2 instance P by a BIG, side-channel resistant
 *
	Uses fixed sized windows.
	@param P ECP2 instance, on exit =b*P
	@param b BIG number multiplier

 */
extern void ECP2_BN254_mul(ECP2_BN254 *P, BIG_256_56 b);

/**	@brief Multiplies an ECP2 instance P by the internal modulus p, using precalculated Frobenius constant f
 *
	Fast point multiplication using Frobenius
	@param P ECP2 instance, on exit = p*P
	@param f FP2 precalculated Frobenius constant

 */
extern void ECP2_BN254_frob(ECP2_BN254 *P, FP2_BN254 *f);

/**	@brief Calculates P=b[0]*Q[0]+b[1]*Q[1]+b[2]*Q[2]+b[3]*Q[3]
 *
	@param P ECP2 instance, on exit = b[0]*Q[0]+b[1]*Q[1]+b[2]*Q[2]+b[3]*Q[3]
	@param Q ECP2 array of 4 points
	@param b BIG array of 4 multipliers
 */
extern void ECP2_BN254_mul4(ECP2_BN254 *P, ECP2_BN254 *Q, BIG_256_56 *b);

/**	@brief Maps random BIG to curve point of correct order
 *
	@param P ECP2 instance of correct order
	@param w OCTET byte array to be mapped
 */
extern void ECP2_BN254_mapit(ECP2_BN254 *P, octet *w);

/**	@brief Get Group Generator from ROM
 *
	@param G ECP2 instance
 */
extern void ECP2_BN254_generator(ECP2_BN254 *G);

#endif
