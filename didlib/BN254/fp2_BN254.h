#ifndef FP2_BN254_H
#define FP2_BN254_H

#include "fp_BN254.h"

/**
	@brief FP2 Structure - quadratic extension field
*/

typedef struct {
    FP_BN254 a; /**< real part of FP2 */
    FP_BN254 b; /**< imaginary part of FP2 */
} FP2_BN254;

/* FP2 prototypes */

/**	@brief Tests for FP2 equal to zero
 *
	@param x FP2 number to be tested
	@return 1 if zero, else returns 0
 */
extern int FP2_BN254_iszilch(FP2_BN254 *x);

/**	@brief Conditional copy of FP2 number
 *
	Conditionally copies second parameter to the first (without branching)
	@param x FP2 instance, set to y if s!=0
	@param y another FP2 instance
	@param s copy only takes place if not equal to 0
 */
extern void FP2_BN254_cmove(FP2_BN254 *x, FP2_BN254 *y, int s);

/**	@brief Tests for FP2 equal to one
 *
	@param x FP2 instance to be tested
	@return 1 if x=1, else returns 0
 */
extern int FP2_BN254_isunity(FP2_BN254 *x);

/**	@brief Tests for equality of two FP2s
 *
	@param x FP2 instance to be compared
	@param y FP2 instance to be compared
	@return 1 if x=y, else returns 0
 */
extern int FP2_BN254_equals(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Initialise FP2 from two FP numbers
 *
	@param x FP2 instance to be initialised
	@param a FP to form real part of FP2
	@param b FP to form imaginary part of FP2
 */
extern void FP2_BN254_from_FPs(FP2_BN254 *x, FP_BN254 *a, FP_BN254 *b);

/**	@brief Initialise FP2 from two BIG integers
 *
	@param x FP2 instance to be initialised
	@param a BIG to form real part of FP2
	@param b BIG to form imaginary part of FP2
 */
extern void FP2_BN254_from_BIGs(FP2_BN254 *x, BIG_256_56 a, BIG_256_56 b);

/**	@brief Initialise FP2 from single FP
 *
	Imaginary part is set to zero
	@param x FP2 instance to be initialised
	@param a FP to form real part of FP2
 */
extern void FP2_BN254_from_FP(FP2_BN254 *x, FP_BN254 *a);

/**	@brief Initialise FP2 from single BIG
 *
	Imaginary part is set to zero
	@param x FP2 instance to be initialised
	@param a BIG to form real part of FP2
 */
extern void FP2_BN254_from_BIG(FP2_BN254 *x, BIG_256_56 a);

/**	@brief Copy FP2 to another FP2
 *
	@param x FP2 instance, on exit = y
	@param y FP2 instance to be copied
 */
extern void FP2_BN254_copy(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Set FP2 to zero
 *
	@param x FP2 instance to be set to zero
 */
extern void FP2_BN254_zero(FP2_BN254 *x);

/**	@brief Set FP2 to unity
 *
	@param x FP2 instance to be set to one
 */
extern void FP2_BN254_one(FP2_BN254 *x);

/**	@brief Negation of FP2
 *
	@param x FP2 instance, on exit = -y
	@param y FP2 instance
 */
extern void FP2_BN254_neg(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Conjugation of FP2
 *
	If y=(a,b) on exit x=(a,-b)
	@param x FP2 instance, on exit = conj(y)
	@param y FP2 instance
 */
extern void FP2_BN254_conj(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief addition of two FP2s
 *
	@param x FP2 instance, on exit = y+z
	@param y FP2 instance
	@param z FP2 instance
 */
extern void FP2_BN254_add(FP2_BN254 *x, FP2_BN254 *y, FP2_BN254 *z);

/**	@brief subtraction of two FP2s
 *
	@param x FP2 instance, on exit = y-z
	@param y FP2 instance
	@param z FP2 instance
 */
extern void FP2_BN254_sub(FP2_BN254 *x, FP2_BN254 *y, FP2_BN254 *z);

/**	@brief Multiplication of an FP2 by an FP
 *
	@param x FP2 instance, on exit = y*b
	@param y FP2 instance
	@param b FP residue
 */
extern void FP2_BN254_pmul(FP2_BN254 *x, FP2_BN254 *y, FP_BN254 *b);

/**	@brief Multiplication of an FP2 by a small integer
 *
	@param x FP2 instance, on exit = y*i
	@param y FP2 instance
	@param i an integer
 */
extern void FP2_BN254_imul(FP2_BN254 *x, FP2_BN254 *y, int i);

/**	@brief Squaring an FP2
 *
	@param x FP2 instance, on exit = y^2
	@param y FP2 instance
 */
extern void FP2_BN254_sqr(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Multiplication of two FP2s
 *
	@param x FP2 instance, on exit = y*z
	@param y FP2 instance
	@param z FP2 instance
 */
extern void FP2_BN254_mul(FP2_BN254 *x, FP2_BN254 *y, FP2_BN254 *z);

/**	@brief Formats and outputs an FP2 to the console
 *
	@param x FP2 instance
 */
extern void FP2_BN254_output(FP2_BN254 *x);

/**	@brief Formats and outputs an FP2 to the console in raw form (for debugging)
 *
	@param x FP2 instance
 */
extern void FP2_BN254_rawoutput(FP2_BN254 *x);

/**	@brief Inverting an FP2
 *
	@param x FP2 instance, on exit = 1/y
	@param y FP2 instance
 */
extern void FP2_BN254_inv(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Divide an FP2 by 2
 *
	@param x FP2 instance, on exit = y/2
	@param y FP2 instance
 */
extern void FP2_BN254_div2(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Multiply an FP2 by (1+sqrt(-1))
 *
	Note that (1+sqrt(-1)) is irreducible for FP4
	@param x FP2 instance, on exit = x*(1+sqrt(-1))
 */
extern void FP2_BN254_mul_ip(FP2_BN254 *x);

/**	@brief Divide an FP2 by (1+sqrt(-1))/2 -
 *
	Note that (1+sqrt(-1)) is irreducible for FP4
	@param x FP2 instance, on exit = 2x/(1+sqrt(-1))
 */
extern void FP2_BN254_div_ip2(FP2_BN254 *x);

/**	@brief Divide an FP2 by (1+sqrt(-1))
 *
	Note that (1+sqrt(-1)) is irreducible for FP4
	@param x FP2 instance, on exit = x/(1+sqrt(-1))
 */
extern void FP2_BN254_div_ip(FP2_BN254 *x);

/**	@brief Normalises the components of an FP2
 *
	@param x FP2 instance to be normalised
 */
extern void FP2_BN254_norm(FP2_BN254 *x);

/**	@brief Reduces all components of possibly unreduced FP2 mod Modulus
 *
	@param x FP2 instance, on exit reduced mod Modulus
 */
extern void FP2_BN254_reduce(FP2_BN254 *x);

/**	@brief Raises an FP2 to the power of a BIG
 *
	@param x FP2 instance, on exit = y^b
	@param y FP2 instance
	@param b BIG number
 */
extern void FP2_BN254_pow(FP2_BN254 *x, FP2_BN254 *y, BIG_256_56 b);

/**	@brief Square root of an FP2
 *
	@param x FP2 instance, on exit = sqrt(y)
	@param y FP2 instance
 */
extern int FP2_BN254_sqrt(FP2_BN254 *x, FP2_BN254 *y);

/**	@brief Multiply an FP2 by sqrt(-1)
 *
	Note that -1 is QNR
	@param x FP2 instance, on exit = x*sqrt(-1)
 */
extern void FP2_BN254_times_i(FP2_BN254 *x);

#endif
