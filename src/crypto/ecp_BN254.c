/* AMCL Elliptic Curve Functions */
/* SU=m, SU is Stack Usage (Weierstrass Curves) */

//#define HAS_MAIN

#include "ecp_BN254.h"

/* test for P=O point-at-infinity */
int ECP_BN254_isinf(ECP_BN254 *P) {
    return (FP_BN254_iszilch(&(P->x)) && FP_BN254_iszilch(&(P->z)));
}

/* Conditional swap of P and Q dependant on d */
static void ECP_BN254_cswap(ECP_BN254 *P, ECP_BN254 *Q, int d) {
    FP_BN254_cswap(&(P->x), &(Q->x), d);
    FP_BN254_cswap(&(P->y), &(Q->y), d);
    FP_BN254_cswap(&(P->z), &(Q->z), d);
}

/* Conditional move Q to P dependant on d */
static void ECP_BN254_cmove(ECP_BN254 *P, ECP_BN254 *Q, int d) {
    FP_BN254_cmove(&(P->x), &(Q->x), d);
    FP_BN254_cmove(&(P->y), &(Q->y), d);
    FP_BN254_cmove(&(P->z), &(Q->z), d);
}

/* return 1 if b==c, no branching */
static int ECP_teq(sign32 b, sign32 c) {
    sign32 x = b ^c;
    x -= 1;  // if x=0, x now -1
    return (int) ((x >> 31) & 1);
}

/* Constant time select from pre-computed table */
static void ECP_BN254_select(ECP_BN254 *P, ECP_BN254 W[], sign32 b) {
    ECP_BN254 MP;
    sign32 m = b >> 31;
    sign32 babs = (b ^ m) - m;

    babs = (babs - 1) / 2;

    ECP_BN254_cmove(P, &W[0], ECP_teq(babs, 0));  // conditional move
    ECP_BN254_cmove(P, &W[1], ECP_teq(babs, 1));
    ECP_BN254_cmove(P, &W[2], ECP_teq(babs, 2));
    ECP_BN254_cmove(P, &W[3], ECP_teq(babs, 3));
    ECP_BN254_cmove(P, &W[4], ECP_teq(babs, 4));
    ECP_BN254_cmove(P, &W[5], ECP_teq(babs, 5));
    ECP_BN254_cmove(P, &W[6], ECP_teq(babs, 6));
    ECP_BN254_cmove(P, &W[7], ECP_teq(babs, 7));

    ECP_BN254_copy(&MP, P);
    ECP_BN254_neg(&MP);  // minus P
    ECP_BN254_cmove(P, &MP, (int) (m & 1));
}

/* Test P == Q */
/* SU=168 */
int ECP_BN254_equals(ECP_BN254 *P, ECP_BN254 *Q) {
    FP_BN254 a, b;

    FP_BN254_mul(&a, &(P->x), &(Q->z));
    FP_BN254_mul(&b, &(Q->x), &(P->z));
    if (!FP_BN254_equals(&a, &b)) return 0;

    FP_BN254_mul(&a, &(P->y), &(Q->z));
    FP_BN254_mul(&b, &(Q->y), &(P->z));
    if (!FP_BN254_equals(&a, &b)) return 0;

    return 1;

}

/* Set P=Q */
/* SU=16 */
void ECP_BN254_copy(ECP_BN254 *P, ECP_BN254 *Q) {
    FP_BN254_copy(&(P->x), &(Q->x));
    FP_BN254_copy(&(P->y), &(Q->y));
    FP_BN254_copy(&(P->z), &(Q->z));
}

/* Set P=-Q */
/* SU=8 */
void ECP_BN254_neg(ECP_BN254 *P) {
    FP_BN254_neg(&(P->y), &(P->y));
    FP_BN254_norm(&(P->y));
}

/* Set P=O */
void ECP_BN254_inf(ECP_BN254 *P) {
    FP_BN254_zero(&(P->x));
    FP_BN254_one(&(P->y));
    FP_BN254_zero(&(P->z));
}

/* Calculate right Hand Side of curve equation y^2=RHS */
/* SU=56 */
void ECP_BN254_rhs(FP_BN254 *v, FP_BN254 *x) {
    /* x^3+Ax+B */
    FP_BN254 t;
    FP_BN254_sqr(&t, x);
    FP_BN254_mul(&t, &t, x);

    if (CURVE_A_BN254 == -3) {
        FP_BN254_neg(v, x);
        FP_BN254_norm(v);
        FP_BN254_imul(v, v, -CURVE_A_BN254);
        FP_BN254_norm(v);
        FP_BN254_add(v, &t, v);
    } else FP_BN254_copy(v, &t);

    FP_BN254_rcopy(&t, CURVE_B_BN254);

    FP_BN254_add(v, &t, v);
    FP_BN254_reduce(v);
}

/* Extract (x,y) and return sign of y. If x and y are the same return only x */
/* SU=16 */
int ECP_BN254_get(BIG_256_56 x, BIG_256_56 y, ECP_BN254 *P) {
    ECP_BN254 W;
    int s;
    ECP_BN254_copy(&W, P);
    ECP_BN254_affine(&W);

    if (ECP_BN254_isinf(&W)) return -1;

    FP_BN254_redc(y, &(W.y));
    s = BIG_256_56_parity(y);

    FP_BN254_redc(x, &(W.x));

    return s;
}

/* Set P=(x,{y}) */
/* SU=96 */
int ECP_BN254_set(ECP_BN254 *P, BIG_256_56 x, BIG_256_56 y) {
    FP_BN254 rhs, y2;

    FP_BN254_nres(&y2, y);
    FP_BN254_sqr(&y2, &y2);
    FP_BN254_reduce(&y2);

    FP_BN254_nres(&rhs, x);
    ECP_BN254_rhs(&rhs, &rhs);

    if (!FP_BN254_equals(&y2, &rhs)) {
        ECP_BN254_inf(P);
        return 0;
    }

    FP_BN254_nres(&(P->x), x);
    FP_BN254_nres(&(P->y), y);
    FP_BN254_one(&(P->z));
    return 1;
}

/* Set P=(x,y), where y is calculated from x with sign s */
/* SU=136 */
int ECP_BN254_setx(ECP_BN254 *P, BIG_256_56 x, int s) {
    FP_BN254 rhs;
    BIG_256_56 t, m;
    BIG_256_56_rcopy(m, Modulus_BN254);

    FP_BN254_nres(&rhs, x);

    ECP_BN254_rhs(&rhs, &rhs);

    FP_BN254_redc(t, &rhs);
    if (BIG_256_56_jacobi(t, m) != 1) {
        ECP_BN254_inf(P);
        return 0;
    }

    FP_BN254_nres(&(P->x), x);
    FP_BN254_sqrt(&(P->y), &rhs);

    FP_BN254_redc(t, &(P->y));

    if (BIG_256_56_parity(t) != s)
        FP_BN254_neg(&(P->y), &(P->y));
    FP_BN254_reduce(&(P->y));
    FP_BN254_one(&(P->z));
    return 1;
}


void ECP_BN254_cfp(ECP_BN254 *P) {
    /* multiply point by curves cofactor */
    BIG_256_56 c;
    int cf = CURVE_Cof_I_BN254;
    if (cf == 1) return;
    if (cf == 4) {
        ECP_BN254_dbl(P);
        ECP_BN254_dbl(P);
        return;
    }
    if (cf == 8) {
        ECP_BN254_dbl(P);
        ECP_BN254_dbl(P);
        ECP_BN254_dbl(P);
        return;
    }
    BIG_256_56_rcopy(c, CURVE_Cof_BN254);
    ECP_BN254_mul(P, c);
    return;
}

/* map BIG to point on curve of correct order */
/* The BIG should be the output of some hash function */

void ECP_BN254_mapit(ECP_BN254 *P, octet *W) {
    BIG_256_56 q, x;
    BIG_256_56_fromBytes(x, W->val);
    BIG_256_56_rcopy(q, Modulus_BN254);
    BIG_256_56_mod(x, q);

    for (;;) {
        for (;;) {
            ECP_BN254_setx(P, x, 0);
            BIG_256_56_inc(x, 1);
            BIG_256_56_norm(x);
            if (!ECP_BN254_isinf(P)) break;
        }
        ECP_BN254_cfp(P);
        if (!ECP_BN254_isinf(P)) break;
    }
}

/* Convert P to Affine, from (x,y,z) to (x,y) */
/* SU=160 */
void ECP_BN254_affine(ECP_BN254 *P) {
    FP_BN254 one, iz;
    if (ECP_BN254_isinf(P)) return;
    FP_BN254_one(&one);
    if (FP_BN254_equals(&(P->z), &one)) return;

    FP_BN254_inv(&iz, &(P->z));
    FP_BN254_mul(&(P->x), &(P->x), &iz);
    FP_BN254_mul(&(P->y), &(P->y), &iz);
    FP_BN254_reduce(&(P->y));

    FP_BN254_reduce(&(P->x));
    FP_BN254_copy(&(P->z), &one);
}

/* SU=120 */
void ECP_BN254_outputxyz(ECP_BN254 *P) {
    BIG_256_56 x, z;
    if (ECP_BN254_isinf(P)) {
        printf("Infinity\n");
        return;
    }
    FP_BN254_reduce(&(P->x));
    FP_BN254_redc(x, &(P->x));
    FP_BN254_reduce(&(P->z));
    FP_BN254_redc(z, &(P->z));

    BIG_256_56 y;
    FP_BN254_reduce(&(P->y));
    FP_BN254_redc(y, &(P->y));
    printf("(");
    BIG_256_56_output(x);
    printf(",");
    BIG_256_56_output(y);
    printf(",");
    BIG_256_56_output(z);
    printf(")\n");
}

/* SU=16 */
/* Output point P */
void ECP_BN254_output(ECP_BN254 *P) {
    BIG_256_56 x;
    if (ECP_BN254_isinf(P)) {
        printf("Infinity\n");
        return;
    }
    ECP_BN254_affine(P);

    BIG_256_56 y;
    FP_BN254_redc(x, &(P->x));
    FP_BN254_redc(y, &(P->y));
    printf("(");
    BIG_256_56_output(x);
    printf(",");
    BIG_256_56_output(y);
    printf(")\n");
}

/* SU=16 */
/* Output point P */
void ECP_BN254_rawoutput(ECP_BN254 *P) {
    BIG_256_56 x, z;

    BIG_256_56 y;
    FP_BN254_redc(x, &(P->x));
    FP_BN254_redc(y, &(P->y));
    FP_BN254_redc(z, &(P->z));
    printf("(");
    BIG_256_56_output(x);
    printf(",");
    BIG_256_56_output(y);
    printf(",");
    BIG_256_56_output(z);
    printf(")\n");
}

/* SU=88 */
/* Convert P to octet string */
void ECP_BN254_toOctet(octet *W, ECP_BN254 *P, bool compress) {
    BIG_256_56 x, y;
    ECP_BN254_get(x, y, P);
    if (compress) {
        W->val[0] = 0x02;
        if (BIG_256_56_parity(y) == 1) W->val[0] = 0x03;
        W->len = MODBYTES_256_56 + 1;
        BIG_256_56_toBytes(&(W->val[1]), x);
    } else {
        W->val[0] = 4;
        W->len = 2 * MODBYTES_256_56 + 1;
        BIG_256_56_toBytes(&(W->val[1]), x);
        BIG_256_56_toBytes(&(W->val[MODBYTES_256_56 + 1]), y);
    }
}

/* SU=88 */
/* Restore P from octet string */
int ECP_BN254_fromOctet(ECP_BN254 *P, octet *W) {
    BIG_256_56 x, y;
    int typ = W->val[0];
    BIG_256_56_fromBytes(x, &(W->val[1]));
    if (typ == 0x04) {
        BIG_256_56_fromBytes(y, &(W->val[MODBYTES_256_56 + 1]));
        if (ECP_BN254_set(P, x, y)) return 1;
    }
    if (typ == 0x02 || typ == 0x03) {
        if (ECP_BN254_setx(P, x, typ & 1)) return 1;
    }
    return 0;
}


/* Set P=2P */
/* SU=272 */
void ECP_BN254_dbl(ECP_BN254 *P) {
    FP_BN254 t0, t1, t2, t3, x3, y3, z3, b;

    if (CURVE_A_BN254 == 0) {
        FP_BN254_sqr(&t0, &(P->y));                    //t0.sqr();
        FP_BN254_mul(&t1, &(P->y), &(P->z));            //t1.mul(z);

        FP_BN254_sqr(&t2, &(P->z));                    //t2.sqr();
        FP_BN254_add(&(P->z), &t0, &t0);        //z.add(t0);
        FP_BN254_norm(&(P->z));                    //z.norm();
        FP_BN254_add(&(P->z), &(P->z), &(P->z));    //z.add(z);
        FP_BN254_add(&(P->z), &(P->z), &(P->z));    //z.add(z);
        FP_BN254_norm(&(P->z));                    //z.norm();

        FP_BN254_imul(&t2, &t2, 3 * CURVE_B_I_BN254);        //t2.imul(3*ROM.CURVE_B_I);
        FP_BN254_mul(&x3, &t2, &(P->z));            //x3.mul(z);

        FP_BN254_add(&y3, &t0, &t2);                //y3.add(t2);
        FP_BN254_norm(&y3);                        //y3.norm();
        FP_BN254_mul(&(P->z), &(P->z), &t1);        //z.mul(t1);

        FP_BN254_add(&t1, &t2, &t2);                //t1.add(t2);
        FP_BN254_add(&t2, &t2, &t1);                //t2.add(t1);
        FP_BN254_sub(&t0, &t0, &t2);                //t0.sub(t2);
        FP_BN254_norm(&t0);                        //t0.norm();
        FP_BN254_mul(&y3, &y3, &t0);                //y3.mul(t0);
        FP_BN254_add(&y3, &y3, &x3);                //y3.add(x3);
        FP_BN254_mul(&t1, &(P->x), &(P->y));            //t1.mul(y);

        FP_BN254_norm(&t0);                    //x.norm();
        FP_BN254_mul(&(P->x), &t0, &t1);        //x.mul(t1);
        FP_BN254_add(&(P->x), &(P->x), &(P->x));    //x.add(x);
        FP_BN254_norm(&(P->x));                    //x.norm();
        FP_BN254_copy(&(P->y), &y3);                //y.copy(y3);
        FP_BN254_norm(&(P->y));                    //y.norm();
    } else // its -3
    {

        if (CURVE_B_I_BN254 == 0)                    //if (ROM.CURVE_B_I==0)
            FP_BN254_rcopy(&b, CURVE_B_BN254);        //b.copy(new FP(new BIG(ROM.CURVE_B)));

        FP_BN254_sqr(&t0, &(P->x));                    //t0.sqr();  //1    x^2
        FP_BN254_sqr(&t1, &(P->y));                    //t1.sqr();  //2    y^2
        FP_BN254_sqr(&t2, &(P->z));                    //t2.sqr();  //3

        FP_BN254_mul(&t3, &(P->x), &(P->y));            //t3.mul(y); //4
        FP_BN254_add(&t3, &t3, &t3);                //t3.add(t3);
        FP_BN254_norm(&t3);                        //t3.norm();//5

        FP_BN254_mul(&z3, &(P->z), &(P->x));            //z3.mul(x);   //6
        FP_BN254_add(&z3, &z3, &z3);                //z3.add(z3);
        FP_BN254_norm(&z3);                        //z3.norm();//7

        if (CURVE_B_I_BN254 == 0)                        //if (ROM.CURVE_B_I==0)
            FP_BN254_mul(&y3, &t2, &b);                //y3.mul(b); //8
        else
            FP_BN254_imul(&y3, &t2, CURVE_B_I_BN254);    //y3.imul(ROM.CURVE_B_I);

        FP_BN254_sub(&y3, &y3, &z3);                //y3.sub(z3); //y3.norm(); //9  ***
        FP_BN254_add(&x3, &y3, &y3);                //x3.add(y3);
        FP_BN254_norm(&x3);                        //x3.norm();//10

        FP_BN254_add(&y3, &y3, &x3);                //y3.add(x3); //y3.norm();//11
        FP_BN254_sub(&x3, &t1, &y3);                //x3.sub(y3);
        FP_BN254_norm(&x3);                        //x3.norm();//12
        FP_BN254_add(&y3, &y3, &t1);                //y3.add(t1);
        FP_BN254_norm(&y3);                        //y3.norm();//13
        FP_BN254_mul(&y3, &y3, &x3);                //y3.mul(x3); //14
        FP_BN254_mul(&x3, &x3, &t3);                //x3.mul(t3); //15
        FP_BN254_add(&t3, &t2, &t2);                //t3.add(t2);  //16
        FP_BN254_add(&t2, &t2, &t3);                //t2.add(t3); //17

        if (CURVE_B_I_BN254 == 0)                    //if (ROM.CURVE_B_I==0)
            FP_BN254_mul(&z3, &z3, &b);                //z3.mul(b); //18
        else
            FP_BN254_imul(&z3, &z3, CURVE_B_I_BN254);    //z3.imul(ROM.CURVE_B_I);

        FP_BN254_sub(&z3, &z3, &t2);                //z3.sub(t2); //z3.norm();//19
        FP_BN254_sub(&z3, &z3, &t0);                //z3.sub(t0);
        FP_BN254_norm(&z3);                        //z3.norm();//20  ***
        FP_BN254_add(&t3, &z3, &z3);                //t3.add(z3); //t3.norm();//21

        FP_BN254_add(&z3, &z3, &t3);                //z3.add(t3);
        FP_BN254_norm(&z3);                        //z3.norm(); //22
        FP_BN254_add(&t3, &t0, &t0);                //t3.add(t0); //t3.norm(); //23
        FP_BN254_add(&t0, &t0, &t3);                //t0.add(t3); //t0.norm();//24
        FP_BN254_sub(&t0, &t0, &t2);                //t0.sub(t2);
        FP_BN254_norm(&t0);                        //t0.norm();//25

        FP_BN254_mul(&t0, &t0, &z3);                //t0.mul(z3);//26
        FP_BN254_add(&y3, &y3, &t0);                //y3.add(t0); //y3.norm();//27
        FP_BN254_mul(&t0, &(P->y), &(P->z));            //t0.mul(z);//28
        FP_BN254_add(&t0, &t0, &t0);                //t0.add(t0);
        FP_BN254_norm(&t0);                        //t0.norm(); //29
        FP_BN254_mul(&z3, &z3, &t0);                //z3.mul(t0);//30
        FP_BN254_sub(&(P->x), &x3, &z3);                //x3.sub(z3); //x3.norm();//31
        FP_BN254_add(&t0, &t0, &t0);                //t0.add(t0);
        FP_BN254_norm(&t0);                        //t0.norm();//32
        FP_BN254_add(&t1, &t1, &t1);                //t1.add(t1);
        FP_BN254_norm(&t1);                        //t1.norm();//33
        FP_BN254_mul(&(P->z), &t0, &t1);                //z3.mul(t1);//34

        FP_BN254_norm(&(P->x));                    //x.norm();
        FP_BN254_copy(&(P->y), &y3);                //y.copy(y3);
        FP_BN254_norm(&(P->y));                    //y.norm();
        FP_BN254_norm(&(P->z));                    //z.norm();
    }
}

/* Set P+=Q */
/* SU=248 */
void ECP_BN254_add(ECP_BN254 *P, ECP_BN254 *Q) {
    int b3;
    FP_BN254 t0, t1, t2, t3, t4, x3, y3, z3, b;

    if (CURVE_A_BN254 == 0) {
        b3 = 3 * CURVE_B_I_BN254;                    //int b=3*ROM.CURVE_B_I;
        FP_BN254_mul(&t0, &(P->x), &(Q->x));        //t0.mul(Q.x);
        FP_BN254_mul(&t1, &(P->y), &(Q->y));        //t1.mul(Q.y);
        FP_BN254_mul(&t2, &(P->z), &(Q->z));        //t2.mul(Q.z);
        FP_BN254_add(&t3, &(P->x), &(P->y));        //t3.add(y);
        FP_BN254_norm(&t3);                    //t3.norm();

        FP_BN254_add(&t4, &(Q->x), &(Q->y));        //t4.add(Q.y);
        FP_BN254_norm(&t4);                    //t4.norm();
        FP_BN254_mul(&t3, &t3, &t4);            //t3.mul(t4);
        FP_BN254_add(&t4, &t0, &t1);            //t4.add(t1);

        FP_BN254_sub(&t3, &t3, &t4);            //t3.sub(t4);
        FP_BN254_norm(&t3);                    //t3.norm();
        FP_BN254_add(&t4, &(P->y), &(P->z));        //t4.add(z);
        FP_BN254_norm(&t4);                    //t4.norm();
        FP_BN254_add(&x3, &(Q->y), &(Q->z));        //x3.add(Q.z);
        FP_BN254_norm(&x3);                    //x3.norm();

        FP_BN254_mul(&t4, &t4, &x3);            //t4.mul(x3);
        FP_BN254_add(&x3, &t1, &t2);            //x3.add(t2);

        FP_BN254_sub(&t4, &t4, &x3);            //t4.sub(x3);
        FP_BN254_norm(&t4);                    //t4.norm();
        FP_BN254_add(&x3, &(P->x), &(P->z));        //x3.add(z);
        FP_BN254_norm(&x3);                    //x3.norm();
        FP_BN254_add(&y3, &(Q->x), &(Q->z));        //y3.add(Q.z);
        FP_BN254_norm(&y3);                    //y3.norm();
        FP_BN254_mul(&x3, &x3, &y3);            //x3.mul(y3);

        FP_BN254_add(&y3, &t0, &t2);            //y3.add(t2);
        FP_BN254_sub(&y3, &x3, &y3);            //y3.rsub(x3);
        FP_BN254_norm(&y3);                    //y3.norm();
        FP_BN254_add(&x3, &t0, &t0);            //x3.add(t0);
        FP_BN254_add(&t0, &t0, &x3);            //t0.add(x3);
        FP_BN254_norm(&t0);                    //t0.norm();
        FP_BN254_imul(&t2, &t2, b3);                //t2.imul(b);

        FP_BN254_add(&z3, &t1, &t2);            //z3.add(t2);
        FP_BN254_norm(&z3);                    //z3.norm();
        FP_BN254_sub(&t1, &t1, &t2);            //t1.sub(t2);
        FP_BN254_norm(&t1);                    //t1.norm();
        FP_BN254_imul(&y3, &y3, b3);                //y3.imul(b);

        FP_BN254_mul(&x3, &y3, &t4);            //x3.mul(t4);
        FP_BN254_mul(&t2, &t3, &t1);            //t2.mul(t1);
        FP_BN254_sub(&(P->x), &t2, &x3);            //x3.rsub(t2);
        FP_BN254_mul(&y3, &y3, &t0);            //y3.mul(t0);
        FP_BN254_mul(&t1, &t1, &z3);            //t1.mul(z3);
        FP_BN254_add(&(P->y), &y3, &t1);            //y3.add(t1);
        FP_BN254_mul(&t0, &t0, &t3);            //t0.mul(t3);
        FP_BN254_mul(&z3, &z3, &t4);            //z3.mul(t4);
        FP_BN254_add(&(P->z), &z3, &t0);            //z3.add(t0);

        FP_BN254_norm(&(P->x));                //x.norm();
        FP_BN254_norm(&(P->y));                //y.norm();
        FP_BN254_norm(&(P->z));                //z.norm();
    } else {

        if (CURVE_B_I_BN254 == 0)                //if (ROM.CURVE_B_I==0)
            FP_BN254_rcopy(&b, CURVE_B_BN254);    //b.copy(new FP(new BIG(ROM.CURVE_B)));

        FP_BN254_mul(&t0, &(P->x), &(Q->x));        //t0.mul(Q.x); //1
        FP_BN254_mul(&t1, &(P->y), &(Q->y));        //t1.mul(Q.y); //2
        FP_BN254_mul(&t2, &(P->z), &(Q->z));        //t2.mul(Q.z); //3

        FP_BN254_add(&t3, &(P->x), &(P->y));        //t3.add(y);
        FP_BN254_norm(&t3);                    //t3.norm(); //4
        FP_BN254_add(&t4, &(Q->x), &(Q->y));        //t4.add(Q.y);
        FP_BN254_norm(&t4);                    //t4.norm();//5
        FP_BN254_mul(&t3, &t3, &t4);            //t3.mul(t4);//6

        FP_BN254_add(&t4, &t0, &t1);            //t4.add(t1); //t4.norm(); //7
        FP_BN254_sub(&t3, &t3, &t4);            //t3.sub(t4);
        FP_BN254_norm(&t3);                    //t3.norm(); //8
        FP_BN254_add(&t4, &(P->y), &(P->z));        //t4.add(z);
        FP_BN254_norm(&t4);                    //t4.norm();//9
        FP_BN254_add(&x3, &(Q->y), &(Q->z));        //x3.add(Q.z);
        FP_BN254_norm(&x3);                    //x3.norm();//10
        FP_BN254_mul(&t4, &t4, &x3);            //t4.mul(x3); //11
        FP_BN254_add(&x3, &t1, &t2);            //x3.add(t2); //x3.norm();//12

        FP_BN254_sub(&t4, &t4, &x3);            //t4.sub(x3);
        FP_BN254_norm(&t4);                    //t4.norm();//13
        FP_BN254_add(&x3, &(P->x), &(P->z));        //x3.add(z);
        FP_BN254_norm(&x3);                    //x3.norm(); //14
        FP_BN254_add(&y3, &(Q->x), &(Q->z));        //y3.add(Q.z);
        FP_BN254_norm(&y3);                    //y3.norm();//15

        FP_BN254_mul(&x3, &x3, &y3);            //x3.mul(y3); //16
        FP_BN254_add(&y3, &t0, &t2);            //y3.add(t2); //y3.norm();//17

        FP_BN254_sub(&y3, &x3, &y3);            //y3.rsub(x3);
        FP_BN254_norm(&y3);                    //y3.norm(); //18

        if (CURVE_B_I_BN254 == 0)                //if (ROM.CURVE_B_I==0)
            FP_BN254_mul(&z3, &t2, &b);            //z3.mul(b); //18
        else
            FP_BN254_imul(&z3, &t2, CURVE_B_I_BN254);    //z3.imul(ROM.CURVE_B_I);

        FP_BN254_sub(&x3, &y3, &z3);            //x3.sub(z3);
        FP_BN254_norm(&x3);                    //x3.norm(); //20
        FP_BN254_add(&z3, &x3, &x3);            //z3.add(x3); //z3.norm(); //21

        FP_BN254_add(&x3, &x3, &z3);            //x3.add(z3); //x3.norm(); //22
        FP_BN254_sub(&z3, &t1, &x3);            //z3.sub(x3);
        FP_BN254_norm(&z3);                    //z3.norm(); //23
        FP_BN254_add(&x3, &x3, &t1);            //x3.add(t1);
        FP_BN254_norm(&x3);                    //x3.norm(); //24

        if (CURVE_B_I_BN254 == 0)                //if (ROM.CURVE_B_I==0)
            FP_BN254_mul(&y3, &y3, &b);            //y3.mul(b); //18
        else
            FP_BN254_imul(&y3, &y3, CURVE_B_I_BN254);    //y3.imul(ROM.CURVE_B_I);

        FP_BN254_add(&t1, &t2, &t2);            //t1.add(t2); //t1.norm();//26
        FP_BN254_add(&t2, &t2, &t1);            //t2.add(t1); //t2.norm();//27

        FP_BN254_sub(&y3, &y3, &t2);            //y3.sub(t2); //y3.norm(); //28

        FP_BN254_sub(&y3, &y3, &t0);            //y3.sub(t0);
        FP_BN254_norm(&y3);                    //y3.norm(); //29
        FP_BN254_add(&t1, &y3, &y3);            //t1.add(y3); //t1.norm();//30
        FP_BN254_add(&y3, &y3, &t1);            //y3.add(t1);
        FP_BN254_norm(&y3);                    //y3.norm(); //31

        FP_BN254_add(&t1, &t0, &t0);            //t1.add(t0); //t1.norm(); //32
        FP_BN254_add(&t0, &t0, &t1);            //t0.add(t1); //t0.norm();//33
        FP_BN254_sub(&t0, &t0, &t2);            //t0.sub(t2);
        FP_BN254_norm(&t0);                    //t0.norm();//34

        FP_BN254_mul(&t1, &t4, &y3);            //t1.mul(y3);//35
        FP_BN254_mul(&t2, &t0, &y3);            //t2.mul(y3);//36
        FP_BN254_mul(&y3, &x3, &z3);            //y3.mul(z3);//37
        FP_BN254_add(&(P->y), &y3, &t2);            //y3.add(t2); //y3.norm();//38
        FP_BN254_mul(&x3, &x3, &t3);            //x3.mul(t3);//39
        FP_BN254_sub(&(P->x), &x3, &t1);            //x3.sub(t1);//40
        FP_BN254_mul(&z3, &z3, &t4);            //z3.mul(t4);//41

        FP_BN254_mul(&t1, &t3, &t0);            //t1.mul(t0);//42
        FP_BN254_add(&(P->z), &z3, &t1);            //z3.add(t1);
        FP_BN254_norm(&(P->x));                //x.norm();

        FP_BN254_norm(&(P->y));                //y.norm();
        FP_BN254_norm(&(P->z));                //z.norm();
    }
}

/* Set P-=Q */
/* SU=16 */
void ECP_BN254_sub(ECP_BN254 *P, ECP_BN254 *Q) {
    ECP_BN254 NQ;
    ECP_BN254_copy(&NQ, Q);
    ECP_BN254_neg(&NQ);
    ECP_BN254_add(P, &NQ);
}

/* constant time multiply by small integer of length bts - use ladder */
void ECP_BN254_pinmul(ECP_BN254 *P, int e, int bts) {
    int i, b;
    ECP_BN254 R0, R1;

    ECP_BN254_affine(P);
    ECP_BN254_inf(&R0);
    ECP_BN254_copy(&R1, P);

    for (i = bts - 1; i >= 0; i--) {
        b = (e >> i) & 1;
        ECP_BN254_copy(P, &R1);
        ECP_BN254_add(P, &R0);
        ECP_BN254_cswap(&R0, &R1, b);
        ECP_BN254_copy(&R1, P);
        ECP_BN254_dbl(&R0);
        ECP_BN254_cswap(&R0, &R1, b);
    }
    ECP_BN254_copy(P, &R0);
    ECP_BN254_affine(P);
}

/* Set P=r*P */
/* SU=424 */
void ECP_BN254_mul(ECP_BN254 *P, BIG_256_56 e) {
    /* fixed size windows */
    int i, nb, s, ns;
    BIG_256_56 mt, t;
    ECP_BN254 Q, W[8], C;
    sign8 w[1 + (NLEN_256_56 * BASEBITS_256_56 + 3) / 4];

    if (ECP_BN254_isinf(P)) return;
    if (BIG_256_56_iszilch(e)) {
        ECP_BN254_inf(P);
        return;
    }

    ECP_BN254_affine(P);

    /* precompute table */

    ECP_BN254_copy(&Q, P);
    ECP_BN254_dbl(&Q);

    ECP_BN254_copy(&W[0], P);

    for (i = 1; i < 8; i++) {
        ECP_BN254_copy(&W[i], &W[i - 1]);
        ECP_BN254_add(&W[i], &Q);
    }

//printf("W[1]= ");ECP_output(&W[1]); printf("\n");

    /* make exponent odd - add 2P if even, P if odd */
    BIG_256_56_copy(t, e);
    s = BIG_256_56_parity(t);
    BIG_256_56_inc(t, 1);
    BIG_256_56_norm(t);
    ns = BIG_256_56_parity(t);
    BIG_256_56_copy(mt, t);
    BIG_256_56_inc(mt, 1);
    BIG_256_56_norm(mt);
    BIG_256_56_cmove(t, mt, s);
    ECP_BN254_cmove(&Q, P, ns);
    ECP_BN254_copy(&C, &Q);

    nb = 1 + (BIG_256_56_nbits(t) + 3) / 4;

    /* convert exponent to signed 4-bit window */
    for (i = 0; i < nb; i++) {
        w[i] = BIG_256_56_lastbits(t, 5) - 16;
        BIG_256_56_dec(t, w[i]);
        BIG_256_56_norm(t);
        BIG_256_56_fshr(t, 4);
    }
    w[nb] = BIG_256_56_lastbits(t, 5);

    ECP_BN254_copy(P, &W[(w[nb] - 1) / 2]);
    for (i = nb - 1; i >= 0; i--) {
        ECP_BN254_select(&Q, W, w[i]);
        ECP_BN254_dbl(P);
        ECP_BN254_dbl(P);
        ECP_BN254_dbl(P);
        ECP_BN254_dbl(P);
        ECP_BN254_add(P, &Q);
    }
    ECP_BN254_sub(P, &C); /* apply correction */
    ECP_BN254_affine(P);
}

/* Set P=eP+fQ double multiplication */
/* constant time - as useful for GLV method in pairings */
/* SU=456 */

void ECP_BN254_mul2(ECP_BN254 *P, ECP_BN254 *Q, BIG_256_56 e, BIG_256_56 f) {
    BIG_256_56 te, tf, mt;
    ECP_BN254 S, T, W[8], C;
    sign8 w[1 + (NLEN_256_56 * BASEBITS_256_56 + 1) / 2];
    int i, a, b, s, ns, nb;

    BIG_256_56_copy(te, e);
    BIG_256_56_copy(tf, f);

    /* precompute table */
    ECP_BN254_copy(&W[1], P);
    ECP_BN254_sub(&W[1], Q);  /* P+Q */
    ECP_BN254_copy(&W[2], P);
    ECP_BN254_add(&W[2], Q);  /* P-Q */
    ECP_BN254_copy(&S, Q);
    ECP_BN254_dbl(&S);  /* S=2Q */
    ECP_BN254_copy(&W[0], &W[1]);
    ECP_BN254_sub(&W[0], &S);
    ECP_BN254_copy(&W[3], &W[2]);
    ECP_BN254_add(&W[3], &S);
    ECP_BN254_copy(&T, P);
    ECP_BN254_dbl(&T); /* T=2P */
    ECP_BN254_copy(&W[5], &W[1]);
    ECP_BN254_add(&W[5], &T);
    ECP_BN254_copy(&W[6], &W[2]);
    ECP_BN254_add(&W[6], &T);
    ECP_BN254_copy(&W[4], &W[5]);
    ECP_BN254_sub(&W[4], &S);
    ECP_BN254_copy(&W[7], &W[6]);
    ECP_BN254_add(&W[7], &S);

    /* if multiplier is odd, add 2, else add 1 to multiplier, and add 2P or P to correction */

    s = BIG_256_56_parity(te);
    BIG_256_56_inc(te, 1);
    BIG_256_56_norm(te);
    ns = BIG_256_56_parity(te);
    BIG_256_56_copy(mt, te);
    BIG_256_56_inc(mt, 1);
    BIG_256_56_norm(mt);
    BIG_256_56_cmove(te, mt, s);
    ECP_BN254_cmove(&T, P, ns);
    ECP_BN254_copy(&C, &T);

    s = BIG_256_56_parity(tf);
    BIG_256_56_inc(tf, 1);
    BIG_256_56_norm(tf);
    ns = BIG_256_56_parity(tf);
    BIG_256_56_copy(mt, tf);
    BIG_256_56_inc(mt, 1);
    BIG_256_56_norm(mt);
    BIG_256_56_cmove(tf, mt, s);
    ECP_BN254_cmove(&S, Q, ns);
    ECP_BN254_add(&C, &S);

    BIG_256_56_add(mt, te, tf);
    BIG_256_56_norm(mt);
    nb = 1 + (BIG_256_56_nbits(mt) + 1) / 2;

    /* convert exponent to signed 2-bit window */
    for (i = 0; i < nb; i++) {
        a = BIG_256_56_lastbits(te, 3) - 4;
        BIG_256_56_dec(te, a);
        BIG_256_56_norm(te);
        BIG_256_56_fshr(te, 2);
        b = BIG_256_56_lastbits(tf, 3) - 4;
        BIG_256_56_dec(tf, b);
        BIG_256_56_norm(tf);
        BIG_256_56_fshr(tf, 2);
        w[i] = 4 * a + b;
    }
    w[nb] = (4 * BIG_256_56_lastbits(te, 3) + BIG_256_56_lastbits(tf, 3));

    ECP_BN254_copy(P, &W[(w[nb] - 1) / 2]);
    for (i = nb - 1; i >= 0; i--) {
        ECP_BN254_select(&T, W, w[i]);
        ECP_BN254_dbl(P);
        ECP_BN254_dbl(P);
        ECP_BN254_add(P, &T);
    }
    ECP_BN254_sub(P, &C); /* apply correction */
    ECP_BN254_affine(P);
}

void ECP_BN254_generator(ECP_BN254 *G) {
    BIG_256_56 x, y;
    BIG_256_56_rcopy(x, CURVE_Gx_BN254);
    BIG_256_56_rcopy(y, CURVE_Gy_BN254);
    ECP_BN254_set(G, x, y);
}

#ifdef HAS_MAIN

#include "time.h"
#include "randapi.h"
int main()
{
    int i;
    ECP_BN254 G,P;
    csprng RNG;
    BIG_256_56 r,s,x,y,b,m,w,q;

    unsigned long ran;

    char raw[100];
    octet RAW = {0, sizeof(raw), raw};             // Crypto Strong RNG

    time((time_t *)&ran);

    RAW.len = 100;				// fake random seed source
    RAW.val[0] = ran;
    RAW.val[1] = ran >> 8;
    RAW.val[2] = ran >> 16;
    RAW.val[3] = ran >> 24;
    for (i = 4; i < 100; i++) RAW.val[i] = i;

    CREATE_CSPRNG(&RNG, &RAW);  // initialise strong RNG


    BIG_256_56_rcopy(x,CURVE_Gx_BN254);
    BIG_256_56_rcopy(y,CURVE_Gy_BN254);
    BIG_256_56_rcopy(m,Modulus_BN254);

    printf("x= ");
    BIG_256_56_output(x);
    printf("\n");
    printf("y= ");
    BIG_256_56_output(y);
    printf("\n");
    //RNG_seed(&RNG,3,"abc");
    ECP_BN254_set(&G,x,y);
    if (ECP_BN254_isinf(&G)) printf("Failed to set - point not on curve\n");
    else printf("set success\n");

    ECP_BN254_output(&G);

    BIG_256_56_rcopy(r,CURVE_Order_BN254); //BIG_dec(r,7);
    printf("r= ");
    BIG_256_56_output(r);
    printf("\n");

    ECP_BN254_copy(&P,&G);

    ECP_BN254_mul(&P,r);

    ECP_BN254_output(&P);
//exit(0);
    BIG_256_56_randomnum(w,r,&RNG);
    BIG_256_56_mod(w,r);

    ECP_BN254_copy(&P,&G);
    ECP_BN254_mul(&P,w);

    ECP_BN254_output(&P);

    return 0;
}

#endif
