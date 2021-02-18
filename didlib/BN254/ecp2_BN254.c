/* AMCL Weierstrass elliptic curve functions over FP2 */
/* SU=m, m is Stack Usage */

#include "ecp2_BN254.h"
#include "ecp_BN254.h"

int ECP2_BN254_isinf(ECP2_BN254 *P) {
    return (FP2_BN254_iszilch(&(P->x)) & FP2_BN254_iszilch(&(P->z)));
}

/* Set P=Q */
/* SU= 16 */
void ECP2_BN254_copy(ECP2_BN254 *P, ECP2_BN254 *Q) {
    FP2_BN254_copy(&(P->x), &(Q->x));
    FP2_BN254_copy(&(P->y), &(Q->y));
    FP2_BN254_copy(&(P->z), &(Q->z));
}

/* set P to Infinity */
/* SU= 8 */
void ECP2_BN254_inf(ECP2_BN254 *P) {
    FP2_BN254_zero(&(P->x));
    FP2_BN254_one(&(P->y));
    FP2_BN254_zero(&(P->z));
}

/* Conditional move Q to P dependant on d */
static void ECP2_BN254_cmove(ECP2_BN254 *P, ECP2_BN254 *Q, int d) {
    FP2_BN254_cmove(&(P->x), &(Q->x), d);
    FP2_BN254_cmove(&(P->y), &(Q->y), d);
    FP2_BN254_cmove(&(P->z), &(Q->z), d);
}

/* return 1 if b==c, no branching */
static int ECP2_teq(sign32 b, sign32 c) {
    sign32 x = b ^c;
    x -= 1;  // if x=0, x now -1
    return (int) ((x >> 31) & 1);
}

/* Constant time select from pre-computed table */
static void ECP2_BN254_select(ECP2_BN254 *P, ECP2_BN254 W[], sign32 b) {
    ECP2_BN254 MP;
    sign32 m = b >> 31;
    sign32 babs = (b ^ m) - m;

    babs = (babs - 1) / 2;

    ECP2_BN254_cmove(P, &W[0], ECP2_teq(babs, 0));  // conditional move
    ECP2_BN254_cmove(P, &W[1], ECP2_teq(babs, 1));
    ECP2_BN254_cmove(P, &W[2], ECP2_teq(babs, 2));
    ECP2_BN254_cmove(P, &W[3], ECP2_teq(babs, 3));
    ECP2_BN254_cmove(P, &W[4], ECP2_teq(babs, 4));
    ECP2_BN254_cmove(P, &W[5], ECP2_teq(babs, 5));
    ECP2_BN254_cmove(P, &W[6], ECP2_teq(babs, 6));
    ECP2_BN254_cmove(P, &W[7], ECP2_teq(babs, 7));

    ECP2_BN254_copy(&MP, P);
    ECP2_BN254_neg(&MP);  // minus P
    ECP2_BN254_cmove(P, &MP, (int) (m & 1));
}

/* return 1 if P==Q, else 0 */
/* SU= 312 */
int ECP2_BN254_equals(ECP2_BN254 *P, ECP2_BN254 *Q) {
    FP2_BN254 a, b;

    FP2_BN254_mul(&a, &(P->x), &(Q->z));
    FP2_BN254_mul(&b, &(Q->x), &(P->z));
    if (!FP2_BN254_equals(&a, &b)) return 0;

    FP2_BN254_mul(&a, &(P->y), &(Q->z));
    FP2_BN254_mul(&b, &(Q->y), &(P->z));
    if (!FP2_BN254_equals(&a, &b)) return 0;
    return 1;
}

/* Make P affine (so z=1) */
/* SU= 232 */
void ECP2_BN254_affine(ECP2_BN254 *P) {
    FP2_BN254 one, iz;
    if (ECP2_BN254_isinf(P)) return;

    FP2_BN254_one(&one);
    if (FP2_BN254_isunity(&(P->z))) {
        FP2_BN254_reduce(&(P->x));
        FP2_BN254_reduce(&(P->y));
        return;
    }

    FP2_BN254_inv(&iz, &(P->z));
    FP2_BN254_mul(&(P->x), &(P->x), &iz);
    FP2_BN254_mul(&(P->y), &(P->y), &iz);

    FP2_BN254_reduce(&(P->x));
    FP2_BN254_reduce(&(P->y));
    FP2_BN254_copy(&(P->z), &one);
}

/* extract x, y from point P */
/* SU= 16 */
int ECP2_BN254_get(FP2_BN254 *x, FP2_BN254 *y, ECP2_BN254 *P) {
    ECP2_BN254 W;
    ECP2_BN254_copy(&W, P);
    ECP2_BN254_affine(&W);
    if (ECP2_BN254_isinf(&W)) return -1;
    FP2_BN254_copy(y, &(W.y));
    FP2_BN254_copy(x, &(W.x));
    return 0;
}

/* SU= 152 */
/* Output point P */
void ECP2_BN254_output(ECP2_BN254 *P) {
    FP2_BN254 x, y;
    if (ECP2_BN254_isinf(P)) {
        printf("Infinity\n");
        return;
    }
    ECP2_BN254_get(&x, &y, P);
    printf("(");
    FP2_BN254_output(&x);
    printf(",");
    FP2_BN254_output(&y);
    printf(")\n");
}

/* SU= 232 */
void ECP2_BN254_outputxyz(ECP2_BN254 *P) {
    ECP2_BN254 Q;
    if (ECP2_BN254_isinf(P)) {
        printf("Infinity\n");
        return;
    }
    ECP2_BN254_copy(&Q, P);
    printf("(");
    FP2_BN254_output(&(Q.x));
    printf(",");
    FP2_BN254_output(&(Q.y));
    printf(",");
    FP2_BN254_output(&(Q.z));
    printf(")\n");
}

/* SU= 168 */
/* Convert Q to octet string */
void ECP2_BN254_toOctet(octet *W, ECP2_BN254 *Q) {
    BIG_256_56 b;
    FP2_BN254 qx, qy;
    ECP2_BN254_get(&qx, &qy, Q);

    FP_BN254_redc(b, &(qx.a));
    BIG_256_56_toBytes(&(W->val[0]), b);
    FP_BN254_redc(b, &(qx.b));
    BIG_256_56_toBytes(&(W->val[MODBYTES_256_56]), b);
    FP_BN254_redc(b, &(qy.a));
    BIG_256_56_toBytes(&(W->val[2 * MODBYTES_256_56]), b);
    FP_BN254_redc(b, &(qy.b));
    BIG_256_56_toBytes(&(W->val[3 * MODBYTES_256_56]), b);

    W->len = 4 * MODBYTES_256_56;

}

/* SU= 176 */
/* restore Q from octet string */
int ECP2_BN254_fromOctet(ECP2_BN254 *Q, octet *W) {
    BIG_256_56 b;
    FP2_BN254 qx, qy;
    BIG_256_56_fromBytes(b, &(W->val[0]));
    FP_BN254_nres(&(qx.a), b);
    BIG_256_56_fromBytes(b, &(W->val[MODBYTES_256_56]));
    FP_BN254_nres(&(qx.b), b);
    BIG_256_56_fromBytes(b, &(W->val[2 * MODBYTES_256_56]));
    FP_BN254_nres(&(qy.a), b);
    BIG_256_56_fromBytes(b, &(W->val[3 * MODBYTES_256_56]));
    FP_BN254_nres(&(qy.b), b);

    if (ECP2_BN254_set(Q, &qx, &qy)) return 1;
    return 0;
}

/* SU= 128 */
/* Calculate RHS of twisted curve equation x^3+B/i or x^3+Bi*/
void ECP2_BN254_rhs(FP2_BN254 *rhs, FP2_BN254 *x) {
    /* calculate RHS of elliptic curve equation */
    FP2_BN254 t;
    BIG_256_56 b;
    FP2_BN254_sqr(&t, x);

    FP2_BN254_mul(rhs, &t, x);

    /* Assuming CURVE_A=0 */

    BIG_256_56_rcopy(b, CURVE_B_BN254);

    FP2_BN254_from_BIG(&t, b);

#if SEXTIC_TWIST_BN254 == D_TYPE
    FP2_BN254_div_ip(&t);   /* IMPORTANT - here we use the correct SEXTIC twist of the curve */
#endif

#if SEXTIC_TWIST_BN254 == M_TYPE
    FP2_BN254_norm(&t);
    FP2_BN254_mul_ip(&t);   /* IMPORTANT - here we use the correct SEXTIC twist of the curve */
    FP2_BN254_norm(&t);

#endif


    FP2_BN254_add(rhs, &t, rhs);
    FP2_BN254_reduce(rhs);
}


/* Set P=(x,y). Return 1 if (x,y) is on the curve, else return 0*/
/* SU= 232 */
int ECP2_BN254_set(ECP2_BN254 *P, FP2_BN254 *x, FP2_BN254 *y) {
    FP2_BN254 rhs, y2;

    FP2_BN254_sqr(&y2, y);
    ECP2_BN254_rhs(&rhs, x);

    if (!FP2_BN254_equals(&y2, &rhs)) {
        ECP2_BN254_inf(P);
        return 0;
    }

    FP2_BN254_copy(&(P->x), x);
    FP2_BN254_copy(&(P->y), y);

    FP2_BN254_one(&(P->z));
    return 1;
}

/* Set P=(x,y). Return 1 if (x,.) is on the curve, else return 0 */
/* SU= 232 */
int ECP2_BN254_setx(ECP2_BN254 *P, FP2_BN254 *x) {
    FP2_BN254 y;
    ECP2_BN254_rhs(&y, x);

    if (!FP2_BN254_sqrt(&y, &y)) {
        ECP2_BN254_inf(P);
        return 0;
    }

    FP2_BN254_copy(&(P->x), x);
    FP2_BN254_copy(&(P->y), &y);
    FP2_BN254_one(&(P->z));
    return 1;
}

/* Set P=-P */
/* SU= 8 */
void ECP2_BN254_neg(ECP2_BN254 *P) {
    FP2_BN254_norm(&(P->y));
    FP2_BN254_neg(&(P->y), &(P->y));
    FP2_BN254_norm(&(P->y));
}

/* R+=R */
/* return -1 for Infinity, 0 for addition, 1 for doubling */
/* SU= 448 */
int ECP2_BN254_dbl(ECP2_BN254 *P) {
    FP2_BN254 t0, t1, t2, iy, x3, y3;

    FP2_BN254_copy(&iy, &(P->y));        //FP2 iy=new FP2(y);
#if SEXTIC_TWIST_BN254 == D_TYPE
    FP2_BN254_mul_ip(&iy);            //iy.mul_ip();
    FP2_BN254_norm(&iy);                //iy.norm();
#endif
    FP2_BN254_sqr(&t0, &(P->y));            //t0.sqr();
#if SEXTIC_TWIST_BN254 == D_TYPE
    FP2_BN254_mul_ip(&t0);            //t0.mul_ip();
#endif
    FP2_BN254_mul(&t1, &iy, &(P->z));    //t1.mul(z);
    FP2_BN254_sqr(&t2, &(P->z));                //t2.sqr();

    FP2_BN254_add(&(P->z), &t0, &t0);    //z.add(t0);
    FP2_BN254_norm(&(P->z));                //z.norm();
    FP2_BN254_add(&(P->z), &(P->z), &(P->z));    //z.add(z);
    FP2_BN254_add(&(P->z), &(P->z), &(P->z));    //z.add(z);
    FP2_BN254_norm(&(P->z));            //z.norm();

    FP2_BN254_imul(&t2, &t2, 3 * CURVE_B_I_BN254);    //t2.imul(3*ROM.CURVE_B_I);
#if SEXTIC_TWIST_BN254 == M_TYPE
    FP2_BN254_mul_ip(&t2);
    FP2_BN254_norm(&t2);
#endif

    FP2_BN254_mul(&x3, &t2, &(P->z));    //x3.mul(z);

    FP2_BN254_add(&y3, &t0, &t2);        //y3.add(t2);
    FP2_BN254_norm(&y3);                //y3.norm();
    FP2_BN254_mul(&(P->z), &(P->z), &t1);    //z.mul(t1);

    FP2_BN254_add(&t1, &t2, &t2);        //t1.add(t2);
    FP2_BN254_add(&t2, &t2, &t1);        //t2.add(t1);
    FP2_BN254_norm(&t2);                //t2.norm();
    FP2_BN254_sub(&t0, &t0, &t2);        //t0.sub(t2);
    FP2_BN254_norm(&t0);                //t0.norm();                           //y^2-9bz^2
    FP2_BN254_mul(&y3, &y3, &t0);        //y3.mul(t0);
    FP2_BN254_add(&(P->y), &y3, &x3);        //y3.add(x3);                          //(y^2+3z*2)(y^2-9z^2)+3b.z^2.8y^2
    FP2_BN254_mul(&t1, &(P->x), &iy);        //t1.mul(iy);						//
    FP2_BN254_norm(&t0);            //x.norm();
    FP2_BN254_mul(&(P->x), &t0, &t1);    //x.mul(t1);
    FP2_BN254_add(&(P->x), &(P->x), &(P->x));    //x.add(x);       //(y^2-9bz^2)xy2

    FP2_BN254_norm(&(P->x));            //x.norm();
    FP2_BN254_norm(&(P->y));            //y.norm();

    return 1;
}

/* Set P+=Q */
/* SU= 400 */
int ECP2_BN254_add(ECP2_BN254 *P, ECP2_BN254 *Q) {
    FP2_BN254 t0, t1, t2, t3, t4, x3, y3, z3;
    int b3 = 3 * CURVE_B_I_BN254;

    FP2_BN254_mul(&t0, &(P->x), &(Q->x));    //t0.mul(Q.x);         // x.Q.x
    FP2_BN254_mul(&t1, &(P->y), &(Q->y));    //t1.mul(Q.y);		 // y.Q.y

    FP2_BN254_mul(&t2, &(P->z), &(Q->z));    //t2.mul(Q.z);
    FP2_BN254_add(&t3, &(P->x), &(P->y));    //t3.add(y);
    FP2_BN254_norm(&t3);                //t3.norm();          //t3=X1+Y1

    FP2_BN254_add(&t4, &(Q->x), &(Q->y));    //t4.add(Q.y);
    FP2_BN254_norm(&t4);                //t4.norm();			//t4=X2+Y2
    FP2_BN254_mul(&t3, &t3, &t4);        //t3.mul(t4);						//t3=(X1+Y1)(X2+Y2)
    FP2_BN254_add(&t4, &t0, &t1);        //t4.add(t1);		//t4=X1.X2+Y1.Y2

    FP2_BN254_sub(&t3, &t3, &t4);        //t3.sub(t4);
    FP2_BN254_norm(&t3);                //t3.norm();
#if SEXTIC_TWIST_BN254 == D_TYPE
    FP2_BN254_mul_ip(&t3);            //t3.mul_ip();
    FP2_BN254_norm(&t3);                //t3.norm();         //t3=(X1+Y1)(X2+Y2)-(X1.X2+Y1.Y2) = X1.Y2+X2.Y1
#endif
    FP2_BN254_add(&t4, &(P->y), &(P->z));    //t4.add(z);
    FP2_BN254_norm(&t4);                //t4.norm();			//t4=Y1+Z1
    FP2_BN254_add(&x3, &(Q->y), &(Q->z));    //x3.add(Q.z);
    FP2_BN254_norm(&x3);                //x3.norm();			//x3=Y2+Z2

    FP2_BN254_mul(&t4, &t4, &x3);        //t4.mul(x3);						//t4=(Y1+Z1)(Y2+Z2)
    FP2_BN254_add(&x3, &t1, &t2);        //x3.add(t2);						//X3=Y1.Y2+Z1.Z2

    FP2_BN254_sub(&t4, &t4, &x3);        //t4.sub(x3);
    FP2_BN254_norm(&t4);                //t4.norm();
#if SEXTIC_TWIST_BN254 == D_TYPE
    FP2_BN254_mul_ip(&t4);            //t4.mul_ip();
    FP2_BN254_norm(&t4);                //t4.norm();          //t4=(Y1+Z1)(Y2+Z2) - (Y1.Y2+Z1.Z2) = Y1.Z2+Y2.Z1
#endif
    FP2_BN254_add(&x3, &(P->x), &(P->z));    //x3.add(z);
    FP2_BN254_norm(&x3);                //x3.norm();	// x3=X1+Z1
    FP2_BN254_add(&y3, &(Q->x), &(Q->z));    //y3.add(Q.z);
    FP2_BN254_norm(&y3);                //y3.norm();				// y3=X2+Z2
    FP2_BN254_mul(&x3, &x3, &y3);        //x3.mul(y3);							// x3=(X1+Z1)(X2+Z2)
    FP2_BN254_add(&y3, &t0, &t2);        //y3.add(t2);							// y3=X1.X2+Z1+Z2
    FP2_BN254_sub(&y3, &x3, &y3);        //y3.rsub(x3);
    FP2_BN254_norm(&y3);                //y3.norm();				// y3=(X1+Z1)(X2+Z2) - (X1.X2+Z1.Z2) = X1.Z2+X2.Z1
#if SEXTIC_TWIST_BN254 == D_TYPE
    FP2_BN254_mul_ip(&t0);            //t0.mul_ip();
    FP2_BN254_norm(&t0);                //t0.norm(); // x.Q.x
    FP2_BN254_mul_ip(&t1);            //t1.mul_ip();
    FP2_BN254_norm(&t1);                //t1.norm(); // y.Q.y
#endif
    FP2_BN254_add(&x3, &t0, &t0);        //x3.add(t0);
    FP2_BN254_add(&t0, &t0, &x3);        //t0.add(x3);
    FP2_BN254_norm(&t0);                //t0.norm();
    FP2_BN254_imul(&t2, &t2, b3);        //t2.imul(b);
#if SEXTIC_TWIST_BN254 == M_TYPE
    FP2_BN254_mul_ip(&t2);
    FP2_BN254_norm(&t2);
#endif
    FP2_BN254_add(&z3, &t1, &t2);        //z3.add(t2);
    FP2_BN254_norm(&z3);                //z3.norm();
    FP2_BN254_sub(&t1, &t1, &t2);        //t1.sub(t2);
    FP2_BN254_norm(&t1);                //t1.norm();
    FP2_BN254_imul(&y3, &y3, b3);        //y3.imul(b);
#if SEXTIC_TWIST_BN254 == M_TYPE
    FP2_BN254_mul_ip(&y3);
    FP2_BN254_norm(&y3);
#endif
    FP2_BN254_mul(&x3, &y3, &t4);        //x3.mul(t4);
    FP2_BN254_mul(&t2, &t3, &t1);        //t2.mul(t1);
    FP2_BN254_sub(&(P->x), &t2, &x3);        //x3.rsub(t2);
    FP2_BN254_mul(&y3, &y3, &t0);        //y3.mul(t0);
    FP2_BN254_mul(&t1, &t1, &z3);        //t1.mul(z3);
    FP2_BN254_add(&(P->y), &y3, &t1);        //y3.add(t1);
    FP2_BN254_mul(&t0, &t0, &t3);        //t0.mul(t3);
    FP2_BN254_mul(&z3, &z3, &t4);        //z3.mul(t4);
    FP2_BN254_add(&(P->z), &z3, &t0);        //z3.add(t0);

    FP2_BN254_norm(&(P->x));            //x.norm();
    FP2_BN254_norm(&(P->y));            //y.norm();
    FP2_BN254_norm(&(P->z));            //z.norm();

    return 0;
}

/* Set P-=Q */
/* SU= 16 */
void ECP2_BN254_sub(ECP2_BN254 *P, ECP2_BN254 *Q) {
    ECP2_BN254 NQ;
    ECP2_BN254_copy(&NQ, Q);
    ECP2_BN254_neg(&NQ);
    ECP2_BN254_add(P, &NQ);
}

/* P*=e */
/* SU= 280 */
void ECP2_BN254_mul(ECP2_BN254 *P, BIG_256_56 e) {
    /* fixed size windows */
    int i, nb, s, ns;
    BIG_256_56 mt, t;
    ECP2_BN254 Q, W[8], C;
    sign8 w[1 + (NLEN_256_56 * BASEBITS_256_56 + 3) / 4];

    if (ECP2_BN254_isinf(P)) return;

    /* precompute table */

    ECP2_BN254_copy(&Q, P);
    ECP2_BN254_dbl(&Q);
    ECP2_BN254_copy(&W[0], P);

    for (i = 1; i < 8; i++) {
        ECP2_BN254_copy(&W[i], &W[i - 1]);
        ECP2_BN254_add(&W[i], &Q);
    }

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
    ECP2_BN254_cmove(&Q, P, ns);
    ECP2_BN254_copy(&C, &Q);

    nb = 1 + (BIG_256_56_nbits(t) + 3) / 4;

    /* convert exponent to signed 4-bit window */
    for (i = 0; i < nb; i++) {
        w[i] = BIG_256_56_lastbits(t, 5) - 16;
        BIG_256_56_dec(t, w[i]);
        BIG_256_56_norm(t);
        BIG_256_56_fshr(t, 4);
    }
    w[nb] = BIG_256_56_lastbits(t, 5);

    ECP2_BN254_copy(P, &W[(w[nb] - 1) / 2]);
    for (i = nb - 1; i >= 0; i--) {
        ECP2_BN254_select(&Q, W, w[i]);
        ECP2_BN254_dbl(P);
        ECP2_BN254_dbl(P);
        ECP2_BN254_dbl(P);
        ECP2_BN254_dbl(P);
        ECP2_BN254_add(P, &Q);
    }
    ECP2_BN254_sub(P, &C); /* apply correction */
    ECP2_BN254_affine(P);
}

/* Calculates q.P using Frobenius constant X */
/* SU= 96 */
void ECP2_BN254_frob(ECP2_BN254 *P, FP2_BN254 *X) {
    FP2_BN254 X2;

    FP2_BN254_sqr(&X2, X);
    FP2_BN254_conj(&(P->x), &(P->x));
    FP2_BN254_conj(&(P->y), &(P->y));
    FP2_BN254_conj(&(P->z), &(P->z));
    FP2_BN254_reduce(&(P->z));

    FP2_BN254_mul(&(P->x), &X2, &(P->x));
    FP2_BN254_mul(&(P->y), &X2, &(P->y));
    FP2_BN254_mul(&(P->y), X, &(P->y));

}


// Bos & Costello https://eprint.iacr.org/2013/458.pdf
// Faz-Hernandez & Longa & Sanchez  https://eprint.iacr.org/2013/158.pdf
// Side channel attack secure

void ECP2_BN254_mul4(ECP2_BN254 *P, ECP2_BN254 Q[4], BIG_256_56 u[4]) {
    int i, j, k, nb, pb, bt;
    ECP2_BN254 T[8], W;
    BIG_256_56 t[4], mt;
    sign8 w[NLEN_256_56 * BASEBITS_256_56 + 1];
    sign8 s[NLEN_256_56 * BASEBITS_256_56 + 1];

    for (i = 0; i < 4; i++) {
        BIG_256_56_copy(t[i], u[i]);
    }

// Precomputed table
    ECP2_BN254_copy(&T[0], &Q[0]); // Q[0]
    ECP2_BN254_copy(&T[1], &T[0]);
    ECP2_BN254_add(&T[1], &Q[1]);    // Q[0]+Q[1]
    ECP2_BN254_copy(&T[2], &T[0]);
    ECP2_BN254_add(&T[2], &Q[2]);    // Q[0]+Q[2]
    ECP2_BN254_copy(&T[3], &T[1]);
    ECP2_BN254_add(&T[3], &Q[2]);    // Q[0]+Q[1]+Q[2]
    ECP2_BN254_copy(&T[4], &T[0]);
    ECP2_BN254_add(&T[4], &Q[3]);  // Q[0]+Q[3]
    ECP2_BN254_copy(&T[5], &T[1]);
    ECP2_BN254_add(&T[5], &Q[3]);    // Q[0]+Q[1]+Q[3]
    ECP2_BN254_copy(&T[6], &T[2]);
    ECP2_BN254_add(&T[6], &Q[3]);    // Q[0]+Q[2]+Q[3]
    ECP2_BN254_copy(&T[7], &T[3]);
    ECP2_BN254_add(&T[7], &Q[3]);    // Q[0]+Q[1]+Q[2]+Q[3]

// Make it odd
    pb = 1 - BIG_256_56_parity(t[0]);
    BIG_256_56_inc(t[0], pb);
    BIG_256_56_norm(t[0]);

// Number of bits
    BIG_256_56_zero(mt);
    for (i = 0; i < 4; i++) {
        BIG_256_56_or(mt, mt, t[i]);
    }
    nb = 1 + BIG_256_56_nbits(mt);

// Sign pivot
    s[nb - 1] = 1;
    for (i = 0; i < nb - 1; i++) {
        BIG_256_56_fshr(t[0], 1);
        s[i] = 2 * BIG_256_56_parity(t[0]) - 1;
    }

// Recoded exponent
    for (i = 0; i < nb; i++) {
        w[i] = 0;
        k = 1;
        for (j = 1; j < 4; j++) {
            bt = s[i] * BIG_256_56_parity(t[j]);
            BIG_256_56_fshr(t[j], 1);

            BIG_256_56_dec(t[j], (bt >> 1));
            BIG_256_56_norm(t[j]);
            w[i] += bt * k;
            k *= 2;
        }
    }

// Main loop
    ECP2_BN254_select(P, T, 2 * w[nb - 1] + 1);
    for (i = nb - 2; i >= 0; i--) {
        ECP2_BN254_select(&W, T, 2 * w[i] + s[i]);
        ECP2_BN254_dbl(P);
        ECP2_BN254_add(P, &W);
    }

// apply correction
    ECP2_BN254_copy(&W, P);
    ECP2_BN254_sub(&W, &Q[0]);
    ECP2_BN254_cmove(P, &W, pb);

    ECP2_BN254_affine(P);
}

/* Map to hash value to point on G2 from random BIG */
void ECP2_BN254_mapit(ECP2_BN254 *Q, octet *W) {
    BIG_256_56 q, one, Fx, Fy, x, hv;
    FP2_BN254 X;
#if (PAIRING_FRIENDLY_BN254 == BN)
    ECP2_BN254 T, K;
#elif (PAIRING_FRIENDLY_BN254 == BLS)
    ECP2_BN254 xQ, x2Q;
#endif
    BIG_256_56_fromBytes(hv, W->val);
    BIG_256_56_rcopy(q, Modulus_BN254);
    BIG_256_56_one(one);
    BIG_256_56_mod(hv, q);

    for (;;) {
        FP2_BN254_from_BIGs(&X, one, hv);
        if (ECP2_BN254_setx(Q, &X)) break;
        BIG_256_56_inc(hv, 1);
    }

    BIG_256_56_rcopy(Fx, Fra_BN254);
    BIG_256_56_rcopy(Fy, Frb_BN254);
    FP2_BN254_from_BIGs(&X, Fx, Fy);

#if SEXTIC_TWIST_BN254 == M_TYPE
    FP2_BN254_inv(&X,&X);
    FP2_BN254_norm(&X);
#endif

    BIG_256_56_rcopy(x, CURVE_Bnx_BN254);

#if (PAIRING_FRIENDLY_BN254 == BN)

    /* Faster Hashing to G2 - Fuentes-Castaneda, Knapp and Rodriguez-Henriquez */
    /* Q -> xQ + F(3xQ) + F(F(xQ)) + F(F(F(Q))). */
    ECP2_BN254_copy(&T, Q);
    ECP2_BN254_mul(&T, x);
#if SIGN_OF_X_BN254 == NEGATIVEX
    ECP2_BN254_neg(&T);   // our x is negative
#endif
    ECP2_BN254_copy(&K, &T);
    ECP2_BN254_dbl(&K);
    ECP2_BN254_add(&K, &T);

    ECP2_BN254_frob(&K, &X);
    ECP2_BN254_frob(Q, &X);
    ECP2_BN254_frob(Q, &X);
    ECP2_BN254_frob(Q, &X);
    ECP2_BN254_add(Q, &T);
    ECP2_BN254_add(Q, &K);
    ECP2_BN254_frob(&T, &X);
    ECP2_BN254_frob(&T, &X);
    ECP2_BN254_add(Q, &T);
    ECP2_BN254_affine(Q);

#elif (PAIRING_FRIENDLY_BN254 == BLS)

    /* Efficient hash maps to G2 on BLS curves - Budroni, Pintore */
    /* Q -> x2Q -xQ -Q +F(xQ -Q) +F(F(2Q)) */

    ECP2_BN254_copy(&xQ,Q);
    ECP2_BN254_mul(&xQ,x);

    ECP2_BN254_copy(&x2Q,&xQ);
    ECP2_BN254_mul(&x2Q,x);

#if SIGN_OF_X_BN254==NEGATIVEX
    ECP2_BN254_neg(&xQ);
#endif

    ECP2_BN254_sub(&x2Q,&xQ);
    ECP2_BN254_sub(&x2Q,Q);

    ECP2_BN254_sub(&xQ,Q);
    ECP2_BN254_frob(&xQ,&X);

    ECP2_BN254_dbl(Q);
    ECP2_BN254_frob(Q,&X);
    ECP2_BN254_frob(Q,&X);

    ECP2_BN254_add(Q,&x2Q);
    ECP2_BN254_add(Q,&xQ);

    ECP2_BN254_affine(Q);

#endif
}

void ECP2_BN254_generator(ECP2_BN254 *G) {
    FP2_BN254 wx, wy;

    FP_BN254_rcopy(&(wx.a), CURVE_Pxa_BN254);
    FP_BN254_rcopy(&(wx.b), CURVE_Pxb_BN254);
    FP_BN254_rcopy(&(wy.a), CURVE_Pya_BN254);
    FP_BN254_rcopy(&(wy.b), CURVE_Pyb_BN254);

    ECP2_BN254_set(G, &wx, &wy);
}
