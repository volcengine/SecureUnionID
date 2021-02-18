/* Architecture definition header file */
/* NOTE: There is only one user configurable section in this header - see below */

#ifndef ARCH_H
#define ARCH_H


#define CHUNK 64        /**< size of chunk in bits = wordlength of computer = 16, 32 or 64. Note not all curve options are supported on 16-bit processors - see rom.c */

#define byte unsigned char        /**< 8-bit unsigned integer */
#define sign32 int            /**< 32-bit signed integer */
#define sign8 signed char        /**< 8-bit signed integer */
#define sign16 short int        /**< 16-bit signed integer */
#define sign64 long long        /**< 64-bit signed integer */
#define unsign32 unsigned int /**< 32-bit unsigned integer */
#define unsign64 unsigned long long  /**< 64-bit unsigned integer */

#define uchar unsigned char  /**<  Unsigned char */

#define chunk long long        /**< C type corresponding to word length */

#define dchunk __int128                /**< Always define double length chunk type if available (supported by GCC & Clang on 64-bit architectures) */

#define COMBA      /**< Use COMBA method for faster muls, sqrs and reductions */


#endif
