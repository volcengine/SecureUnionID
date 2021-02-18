//
// Created by bytedance on 2020/12/8.
//

#include "arch.h"
#include "fp_BN254.h"
#include "fn_BN254.h"
#include "ecp_BN254.h"
#include "ecp2_BN254.h"

/* Curve BN254 - Pairing friendly BN curve */

/* Nogami's fast curve */
// Base Bits= 56
const chunk NConst_BN254= 0x3B13B13B13B13BL;
const BIG_256_56 R2modn_BN254= {0x94148B52A2B905L,0xD8CD08A605C920L,0x0CB9B1BD5A99E6L,0x3C7CB8229EE79FL,0x2174036FL};