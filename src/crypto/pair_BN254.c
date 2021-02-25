
/* AMCL BN Curve pairing functions */

//#define HAS_MAIN

#include "pair_BN254.h"

/* Line function */
static void PAIR_BN254_line(FP12_BN254 *v, ECP2_BN254 *A, ECP2_BN254 *B, FP_BN254 *Qx, FP_BN254 *Qy) {
    FP2_BN254 X1, Y1, T1, T2;
    FP2_BN254 XX, YY, ZZ, YZ;
    FP4_BN254 a, b, c;

    if (A == B) {
        /* doubling */
        FP2_BN254_copy(&XX, &(A->x));    //FP2 XX=new FP2(A.getx());  //X
        FP2_BN254_copy(&YY, &(A->y));    //FP2 YY=new FP2(A.gety());  //Y
        FP2_BN254_copy(&ZZ, &(A->z));    //FP2 ZZ=new FP2(A.getz());  //Z


        FP2_BN254_copy(&YZ, &YY);        //FP2 YZ=new FP2(YY);        //Y
        FP2_BN254_mul(&YZ, &YZ, &ZZ);        //YZ.mul(ZZ);                //YZ
        FP2_BN254_sqr(&XX, &XX);        //XX.sqr();	               //X^2
        FP2_BN254_sqr(&YY, &YY);        //YY.sqr();	               //Y^2
        FP2_BN254_sqr(&ZZ, &ZZ);        //ZZ.sqr();			       //Z^2

        FP2_BN254_imul(&YZ, &YZ, 4);    //YZ.imul(4);
        FP2_BN254_neg(&YZ, &YZ);        //YZ.neg();
        FP2_BN254_norm(&YZ);            //YZ.norm();       //-4YZ

        FP2_BN254_imul(&XX, &XX, 6);                    //6X^2
        FP2_BN254_pmul(&XX, &XX, Qx);                   //6X^2.Xs

        FP2_BN254_imul(&ZZ, &ZZ, 3 * CURVE_B_I_BN254);    //3Bz^2

        FP2_BN254_pmul(&YZ, &YZ, Qy);    //-4YZ.Ys
        FP2_BN254_div_ip2(&ZZ);        //6(b/i)z^2

        FP2_BN254_norm(&ZZ);            // 6bi.Z^2

        FP2_BN254_add(&YY, &YY, &YY);    // 2y^2
        FP2_BN254_sub(&ZZ, &ZZ, &YY);    //
        FP2_BN254_norm(&ZZ);            // 6b.Z^2-2Y^2

        FP4_BN254_from_FP2s(&a, &YZ, &ZZ); // -4YZ.Ys | 6b.Z^2-2Y^2 | 6X^2.Xs
        FP4_BN254_from_FP2(&b, &XX);
        FP4_BN254_zero(&c);

        ECP2_BN254_dbl(A);                //A.dbl();
    } else {
        /* addition */

        FP2_BN254_copy(&X1, &(A->x));        //FP2 X1=new FP2(A.getx());    // X1
        FP2_BN254_copy(&Y1, &(A->y));        //FP2 Y1=new FP2(A.gety());    // Y1
        FP2_BN254_copy(&T1, &(A->z));        //FP2 T1=new FP2(A.getz());    // Z1

        FP2_BN254_copy(&T2, &T1);        //FP2 T2=new FP2(A.getz());    // Z1

        FP2_BN254_mul(&T1, &T1, &(B->y));    //T1.mul(B.gety());    // T1=Z1.Y2
        FP2_BN254_mul(&T2, &T2, &(B->x));    //T2.mul(B.getx());    // T2=Z1.X2

        FP2_BN254_sub(&X1, &X1, &T2);        //X1.sub(T2);
        FP2_BN254_norm(&X1);                //X1.norm();  // X1=X1-Z1.X2
        FP2_BN254_sub(&Y1, &Y1, &T1);        //Y1.sub(T1);
        FP2_BN254_norm(&Y1);                //Y1.norm();  // Y1=Y1-Z1.Y2

        FP2_BN254_copy(&T1, &X1);            //T1.copy(X1);            // T1=X1-Z1.X2

        FP2_BN254_pmul(&X1, &X1, Qy);        //X1.pmul(Qy);            // X1=(X1-Z1.X2).Ys

        FP2_BN254_mul(&T1, &T1, &(B->y));    //T1.mul(B.gety());       // T1=(X1-Z1.X2).Y2

        FP2_BN254_copy(&T2, &Y1);            //T2.copy(Y1);            // T2=Y1-Z1.Y2
        FP2_BN254_mul(&T2, &T2, &(B->x));    //T2.mul(B.getx());       // T2=(Y1-Z1.Y2).X2
        FP2_BN254_sub(&T2, &T2, &T1);        //T2.sub(T1);
        FP2_BN254_norm(&T2);                //T2.norm();          // T2=(Y1-Z1.Y2).X2 - (X1-Z1.X2).Y2
        FP2_BN254_pmul(&Y1, &Y1, Qx);        //Y1.pmul(Qx);
        FP2_BN254_neg(&Y1, &Y1);            //Y1.neg();
        FP2_BN254_norm(&Y1);                //Y1.norm(); // Y1=-(Y1-Z1.Y2).Xs

        FP4_BN254_from_FP2s(&a, &X1, &T2);    // (X1-Z1.X2).Ys  |  (Y1-Z1.Y2).X2 - (X1-Z1.X2).Y2  | - (Y1-Z1.Y2).Xs
        FP4_BN254_from_FP2(&b, &Y1);        //b=new FP4(Y1);
        FP4_BN254_zero(&c);
        ECP2_BN254_add(A, B);            //A.add(B);
    }

    FP12_BN254_from_FP4s(v, &a, &b, &c);
    v->type = AMCL_FP_SPARSER;
}


/* prepare ate parameter, n=6u+2 (BN) or n=u (BLS), n3=3*n */
int PAIR_BN254_nbits(BIG_256_56 n3, BIG_256_56 n) {
    BIG_256_56 x;
    BIG_256_56_rcopy(x, CURVE_Bnx_BN254);

    BIG_256_56_pmul(n, x, 6);
    BIG_256_56_dec(n, 2);

    BIG_256_56_norm(n);
    BIG_256_56_pmul(n3, n, 3);
    BIG_256_56_norm(n3);

    return BIG_256_56_nbits(n3);
}

/*
	For multi-pairing, product of n pairings
	1. Declare FP12 array of length number of bits in Ate parameter
	2. Initialise this array by calling PAIR_initmp()
	3. Accumulate each pairing by calling PAIR_another() n times
	4. Call PAIR_miller()
	5. Call final exponentiation PAIR_fexp()
*/

/* prepare for multi-pairing */
void PAIR_BN254_initmp(FP12_BN254 r[]) {
    int i;
    for (i = ATE_BITS_BN254 - 1; i >= 0; i--)
        FP12_BN254_one(&r[i]);
    return;
}

/* basic Miller loop */
void PAIR_BN254_miller(FP12_BN254 *res, FP12_BN254 r[]) {
    int i;
    FP12_BN254_one(res);
    for (i = ATE_BITS_BN254 - 1; i >= 1; i--) {
        FP12_BN254_sqr(res, res);
        FP12_BN254_ssmul(res, &r[i]);
    }

    FP12_BN254_conj(res, res);
    FP12_BN254_ssmul(res, &r[0]);
    return;
}

/* Accumulate another set of line functions for n-pairing */
void PAIR_BN254_another(FP12_BN254 r[], ECP2_BN254 *PV, ECP_BN254 *QV) {
    int i, nb, bt;
    BIG_256_56 n, n3;
    FP12_BN254 lv, lv2;
    ECP2_BN254 A, NP, P;
    ECP_BN254 Q;
    FP_BN254 Qx, Qy;

    ECP2_BN254 K;
    FP2_BN254 X;
    FP_BN254_rcopy(&Qx, Fra_BN254);
    FP_BN254_rcopy(&Qy, Frb_BN254);
    FP2_BN254_from_FPs(&X, &Qx, &Qy);

    if (ECP_BN254_isinf(QV)) return;

    nb = PAIR_BN254_nbits(n3, n);

    ECP2_BN254_copy(&P, PV);
    ECP_BN254_copy(&Q, QV);

    ECP2_BN254_affine(&P);
    ECP_BN254_affine(&Q);

    FP_BN254_copy(&Qx, &(Q.x));
    FP_BN254_copy(&Qy, &(Q.y));

    ECP2_BN254_copy(&A, &P);
    ECP2_BN254_copy(&NP, &P);
    ECP2_BN254_neg(&NP);

    for (i = nb - 2; i >= 1; i--) {
        PAIR_BN254_line(&lv, &A, &A, &Qx, &Qy);

        bt = BIG_256_56_bit(n3, i) - BIG_256_56_bit(n, i); // bt=BIG_bit(n,i);
        if (bt == 1) {
            PAIR_BN254_line(&lv2, &A, &P, &Qx, &Qy);
            FP12_BN254_smul(&lv, &lv2);
        }
        if (bt == -1) {
            PAIR_BN254_line(&lv2, &A, &NP, &Qx, &Qy);
            FP12_BN254_smul(&lv, &lv2);
        }
        FP12_BN254_ssmul(&r[i], &lv);
    }

    ECP2_BN254_neg(&A);

    ECP2_BN254_copy(&K, &P);
    ECP2_BN254_frob(&K, &X);
    PAIR_BN254_line(&lv, &A, &K, &Qx, &Qy);
    ECP2_BN254_frob(&K, &X);
    ECP2_BN254_neg(&K);
    PAIR_BN254_line(&lv2, &A, &K, &Qx, &Qy);
    FP12_BN254_smul(&lv, &lv2);
    FP12_BN254_ssmul(&r[0], &lv);
}

/* Optimal R-ate pairing r=e(P,Q) */
void PAIR_BN254_ate(FP12_BN254 *r, ECP2_BN254 *P1, ECP_BN254 *Q1) {
    BIG_256_56 n, n3;
    FP_BN254 Qx, Qy;
    int i, nb, bt;
    ECP2_BN254 A, NP, P;
    ECP_BN254 Q;
    FP12_BN254 lv, lv2;
    ECP2_BN254 KA;
    FP2_BN254 X;

    FP_BN254_rcopy(&Qx, Fra_BN254);
    FP_BN254_rcopy(&Qy, Frb_BN254);
    FP2_BN254_from_FPs(&X, &Qx, &Qy);

    FP12_BN254_one(r);
    if (ECP_BN254_isinf(Q1)) return;

    nb = PAIR_BN254_nbits(n3, n);

    ECP2_BN254_copy(&P, P1);
    ECP_BN254_copy(&Q, Q1);

    ECP2_BN254_affine(&P);
    ECP_BN254_affine(&Q);

    FP_BN254_copy(&Qx, &(Q.x));
    FP_BN254_copy(&Qy, &(Q.y));

    ECP2_BN254_copy(&A, &P);
    ECP2_BN254_copy(&NP, &P);
    ECP2_BN254_neg(&NP);



    /* Main Miller Loop */
    for (i = nb - 2; i >= 1; i--)   //0
    {
        FP12_BN254_sqr(r, r);
        PAIR_BN254_line(&lv, &A, &A, &Qx, &Qy);

        bt = BIG_256_56_bit(n3, i) - BIG_256_56_bit(n, i); // bt=BIG_bit(n,i);
        if (bt == 1) {
            PAIR_BN254_line(&lv2, &A, &P, &Qx, &Qy);
            FP12_BN254_smul(&lv, &lv2);
        }
        if (bt == -1) {
            PAIR_BN254_line(&lv2, &A, &NP, &Qx, &Qy);
            FP12_BN254_smul(&lv, &lv2);
        }
        FP12_BN254_ssmul(r, &lv);

    }

    FP12_BN254_conj(r, r);
    /* R-ate fixup required for BN curves */
    ECP2_BN254_neg(&A);

    ECP2_BN254_copy(&KA, &P);
    ECP2_BN254_frob(&KA, &X);
    PAIR_BN254_line(&lv, &A, &KA, &Qx, &Qy);
    ECP2_BN254_frob(&KA, &X);
    ECP2_BN254_neg(&KA);
    PAIR_BN254_line(&lv2, &A, &KA, &Qx, &Qy);
    FP12_BN254_smul(&lv, &lv2);
    FP12_BN254_ssmul(r, &lv);
}

/* Optimal R-ate double pairing e(P,Q).e(R,S) */
void PAIR_BN254_double_ate(FP12_BN254 *r, ECP2_BN254 *P1, ECP_BN254 *Q1, ECP2_BN254 *R1, ECP_BN254 *S1) {
    BIG_256_56 n, n3;
    FP_BN254 Qx, Qy, Sx, Sy;
    int i, nb, bt;
    ECP2_BN254 A, B, NP, NR, P, R;
    ECP_BN254 Q, S;
    FP12_BN254 lv, lv2;

    FP2_BN254 X;
    ECP2_BN254 K;

    FP_BN254_rcopy(&Qx, Fra_BN254);
    FP_BN254_rcopy(&Qy, Frb_BN254);
    FP2_BN254_from_FPs(&X, &Qx, &Qy);

    if (ECP_BN254_isinf(Q1)) {
        PAIR_BN254_ate(r, R1, S1);
        return;
    }
    if (ECP_BN254_isinf(S1)) {
        PAIR_BN254_ate(r, P1, Q1);
        return;
    }
    nb = PAIR_BN254_nbits(n3, n);

    ECP2_BN254_copy(&P, P1);
    ECP_BN254_copy(&Q, Q1);

    ECP2_BN254_affine(&P);
    ECP_BN254_affine(&Q);

    ECP2_BN254_copy(&R, R1);
    ECP_BN254_copy(&S, S1);

    ECP2_BN254_affine(&R);
    ECP_BN254_affine(&S);

    FP_BN254_copy(&Qx, &(Q.x));
    FP_BN254_copy(&Qy, &(Q.y));

    FP_BN254_copy(&Sx, &(S.x));
    FP_BN254_copy(&Sy, &(S.y));

    ECP2_BN254_copy(&A, &P);
    ECP2_BN254_copy(&B, &R);

    ECP2_BN254_copy(&NP, &P);
    ECP2_BN254_neg(&NP);
    ECP2_BN254_copy(&NR, &R);
    ECP2_BN254_neg(&NR);

    FP12_BN254_one(r);

    /* Main Miller Loop */
    for (i = nb - 2; i >= 1; i--) {
        FP12_BN254_sqr(r, r);
        PAIR_BN254_line(&lv, &A, &A, &Qx, &Qy);
        PAIR_BN254_line(&lv2, &B, &B, &Sx, &Sy);
        FP12_BN254_smul(&lv, &lv2);
        FP12_BN254_ssmul(r, &lv);

        bt = BIG_256_56_bit(n3, i) - BIG_256_56_bit(n, i); // bt=BIG_bit(n,i);
        if (bt == 1) {
            PAIR_BN254_line(&lv, &A, &P, &Qx, &Qy);
            PAIR_BN254_line(&lv2, &B, &R, &Sx, &Sy);
            FP12_BN254_smul(&lv, &lv2);
            FP12_BN254_ssmul(r, &lv);
        }
        if (bt == -1) {
            PAIR_BN254_line(&lv, &A, &NP, &Qx, &Qy);
            PAIR_BN254_line(&lv2, &B, &NR, &Sx, &Sy);
            FP12_BN254_smul(&lv, &lv2);
            FP12_BN254_ssmul(r, &lv);
        }

    }
    /* R-ate fixup required for BN curves */

    FP12_BN254_conj(r, r);

    ECP2_BN254_neg(&A);
    ECP2_BN254_neg(&B);

    ECP2_BN254_copy(&K, &P);
    ECP2_BN254_frob(&K, &X);
    PAIR_BN254_line(&lv, &A, &K, &Qx, &Qy);
    ECP2_BN254_frob(&K, &X);
    ECP2_BN254_neg(&K);
    PAIR_BN254_line(&lv2, &A, &K, &Qx, &Qy);
    FP12_BN254_smul(&lv, &lv2);
    FP12_BN254_ssmul(r, &lv);

    ECP2_BN254_copy(&K, &R);
    ECP2_BN254_frob(&K, &X);
    PAIR_BN254_line(&lv, &B, &K, &Sx, &Sy);
    ECP2_BN254_frob(&K, &X);
    ECP2_BN254_neg(&K);
    PAIR_BN254_line(&lv2, &B, &K, &Sx, &Sy);
    FP12_BN254_smul(&lv, &lv2);
    FP12_BN254_ssmul(r, &lv);
}

/* final exponentiation - keep separate for multi-pairings and to avoid thrashing stack */
void PAIR_BN254_fexp(FP12_BN254 *r) {
    FP2_BN254 X;
    BIG_256_56 x;
    FP_BN254 a, b;
    FP12_BN254 t0, y0, y1, y2, y3;

    BIG_256_56_rcopy(x, CURVE_Bnx_BN254);
    FP_BN254_rcopy(&a, Fra_BN254);
    FP_BN254_rcopy(&b, Frb_BN254);
    FP2_BN254_from_FPs(&X, &a, &b);

    /* Easy part of final exp */

    FP12_BN254_inv(&t0, r);
    FP12_BN254_conj(r, r);

    FP12_BN254_mul(r, &t0);
    FP12_BN254_copy(&t0, r);

    FP12_BN254_frob(r, &X);
    FP12_BN254_frob(r, &X);
    FP12_BN254_mul(r, &t0);

//    if (FP12_BN254_isunity(r))
//    {
//        FP12_BN254_zero(r);
//        return;
//    }

    /* Hard part of final exp - see Duquesne & Ghamman eprint 2015/192.pdf */
    FP12_BN254_pow(&t0, r, x); // t0=f^-u
    FP12_BN254_usqr(&y3, &t0); // y3=t0^2
    FP12_BN254_copy(&y0, &t0);
    FP12_BN254_mul(&y0, &y3); // y0=t0*y3
    FP12_BN254_copy(&y2, &y3);
    FP12_BN254_frob(&y2, &X); // y2=y3^p
    FP12_BN254_mul(&y2, &y3); //y2=y2*y3
    FP12_BN254_usqr(&y2, &y2); //y2=y2^2
    FP12_BN254_mul(&y2, &y3); // y2=y2*y3

    FP12_BN254_pow(&t0, &y0, x);  //t0=y0^-u
    FP12_BN254_conj(&y0, r);     //y0=~r
    FP12_BN254_copy(&y1, &t0);
    FP12_BN254_frob(&y1, &X);
    FP12_BN254_frob(&y1, &X); //y1=t0^p^2
    FP12_BN254_mul(&y1, &y0); // y1=y0*y1
    FP12_BN254_conj(&t0, &t0); // t0=~t0
    FP12_BN254_copy(&y3, &t0);
    FP12_BN254_frob(&y3, &X); //y3=t0^p
    FP12_BN254_mul(&y3, &t0); // y3=t0*y3
    FP12_BN254_usqr(&t0, &t0); // t0=t0^2
    FP12_BN254_mul(&y1, &t0); // y1=t0*y1

    FP12_BN254_pow(&t0, &y3, x); // t0=y3^-u
    FP12_BN254_usqr(&t0, &t0); //t0=t0^2
    FP12_BN254_conj(&t0, &t0); //t0=~t0
    FP12_BN254_mul(&y3, &t0); // y3=t0*y3

    FP12_BN254_frob(r, &X);
    FP12_BN254_copy(&y0, r);
    FP12_BN254_frob(r, &X);
    FP12_BN254_mul(&y0, r);
    FP12_BN254_frob(r, &X);
    FP12_BN254_mul(&y0, r);

    FP12_BN254_usqr(r, &y3);  //r=y3^2
    FP12_BN254_mul(r, &y2);   //r=y2*r
    FP12_BN254_copy(&y3, r);
    FP12_BN254_mul(&y3, &y0); // y3=r*y0
    FP12_BN254_mul(r, &y1); // r=r*y1
    FP12_BN254_usqr(r, r); // r=r^2
    FP12_BN254_mul(r, &y3); // r=r*y3
    FP12_BN254_reduce(r);

}

#ifdef USE_GLV_BN254

/* GLV method */
static void glv(BIG_256_56 u[2], BIG_256_56 e) {
    int i, j;
    BIG_256_56 v[2], t, q;
    DBIG_256_56 d;
    BIG_256_56_rcopy(q, CURVE_Order_BN254);
    for (i = 0; i < 2; i++) {
        BIG_256_56_rcopy(t, CURVE_W_BN254[i]);
        BIG_256_56_mul(d, t, e);
        BIG_256_56_ddiv(v[i], d, q);
        BIG_256_56_zero(u[i]);
    }
    BIG_256_56_copy(u[0], e);
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            BIG_256_56_rcopy(t, CURVE_SB_BN254[j][i]);
            BIG_256_56_modmul(t, v[j], t, q);
            BIG_256_56_add(u[i], u[i], q);
            BIG_256_56_sub(u[i], u[i], t);
            BIG_256_56_mod(u[i], q);
        }

    return;
}

#endif // USE_GLV

/* Galbraith & Scott Method */
static void gs(BIG_256_56 u[4], BIG_256_56 e) {
    int i;
    int j;
    BIG_256_56 v[4], t, q;
    DBIG_256_56 d;
    BIG_256_56_rcopy(q, CURVE_Order_BN254);
    for (i = 0; i < 4; i++) {
        BIG_256_56_rcopy(t, CURVE_WB_BN254[i]);
        BIG_256_56_mul(d, t, e);
        BIG_256_56_ddiv(v[i], d, q);
        BIG_256_56_zero(u[i]);
    }

    BIG_256_56_copy(u[0], e);
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {
            BIG_256_56_rcopy(t, CURVE_BB_BN254[j][i]);
            BIG_256_56_modmul(t, v[j], t, q);
            BIG_256_56_add(u[i], u[i], q);
            BIG_256_56_sub(u[i], u[i], t);
            BIG_256_56_mod(u[i], q);
        }

    return;
}

/* Multiply P by e in group G1 */
void PAIR_BN254_G1mul(ECP_BN254 *P, BIG_256_56 e) {
#ifdef USE_GLV_BN254   /* Note this method is patented */
    int np, nn;
    ECP_BN254 Q;
    FP_BN254 cru;
    BIG_256_56 t, q;
    BIG_256_56 u[2];

    BIG_256_56_rcopy(q, CURVE_Order_BN254);
    glv(u, e);

    ECP_BN254_copy(&Q, P);
    ECP_BN254_affine(&Q);
    FP_BN254_rcopy(&cru, CURVE_Cru_BN254);
    FP_BN254_mul(&(Q.x), &(Q.x), &cru);

    /* note that -a.B = a.(-B). Use a or -a depending on which is smaller */

    np = BIG_256_56_nbits(u[0]);
    BIG_256_56_modneg(t, u[0], q);
    nn = BIG_256_56_nbits(t);
    if (nn < np) {
        BIG_256_56_copy(u[0], t);
        ECP_BN254_neg(P);
    }

    np = BIG_256_56_nbits(u[1]);
    BIG_256_56_modneg(t, u[1], q);
    nn = BIG_256_56_nbits(t);
    if (nn < np) {
        BIG_256_56_copy(u[1], t);
        ECP_BN254_neg(&Q);
    }
    BIG_256_56_norm(u[0]);
    BIG_256_56_norm(u[1]);
    ECP_BN254_mul2(P, &Q, u[0], u[1]);

#else
    ECP_BN254_mul(P,e);
#endif
}

/* Multiply P by e in group G2 */
void PAIR_BN254_G2mul(ECP2_BN254 *P, BIG_256_56 e) {
#ifdef USE_GS_G2_BN254   /* Well I didn't patent it :) */
    int i, np, nn;
    ECP2_BN254 Q[4];
    FP2_BN254 X;
    FP_BN254 fx, fy;
    BIG_256_56 x, y, u[4];

    FP_BN254_rcopy(&fx, Fra_BN254);
    FP_BN254_rcopy(&fy, Frb_BN254);
    FP2_BN254_from_FPs(&X, &fx, &fy);

    BIG_256_56_rcopy(y, CURVE_Order_BN254);
    gs(u, e);

    ECP2_BN254_copy(&Q[0], P);
    for (i = 1; i < 4; i++) {
        ECP2_BN254_copy(&Q[i], &Q[i - 1]);
        ECP2_BN254_frob(&Q[i], &X);
    }

    for (i = 0; i < 4; i++) {
        np = BIG_256_56_nbits(u[i]);
        BIG_256_56_modneg(x, u[i], y);
        nn = BIG_256_56_nbits(x);
        if (nn < np) {
            BIG_256_56_copy(u[i], x);
            ECP2_BN254_neg(&Q[i]);
        }
        BIG_256_56_norm(u[i]);
    }

    ECP2_BN254_mul4(P, Q, u);

#else
    ECP2_BN254_mul(P,e);
#endif
}

/* f=f^e */
void PAIR_BN254_GTpow(FP12_BN254 *f, BIG_256_56 e) {
#ifdef USE_GS_GT_BN254   /* Note that this option requires a lot of RAM! Maybe better to use compressed XTR method, see fp4.c */
    int i, np, nn;
    FP12_BN254 g[4];
    FP2_BN254 X;
    BIG_256_56 t, q;
    FP_BN254 fx, fy;
    BIG_256_56 u[4];

    FP_BN254_rcopy(&fx, Fra_BN254);
    FP_BN254_rcopy(&fy, Frb_BN254);
    FP2_BN254_from_FPs(&X, &fx, &fy);

    BIG_256_56_rcopy(q, CURVE_Order_BN254);
    gs(u, e);

    FP12_BN254_copy(&g[0], f);
    for (i = 1; i < 4; i++) {
        FP12_BN254_copy(&g[i], &g[i - 1]);
        FP12_BN254_frob(&g[i], &X);
    }

    for (i = 0; i < 4; i++) {
        np = BIG_256_56_nbits(u[i]);
        BIG_256_56_modneg(t, u[i], q);
        nn = BIG_256_56_nbits(t);
        if (nn < np) {
            BIG_256_56_copy(u[i], t);
            FP12_BN254_conj(&g[i], &g[i]);
        }
        BIG_256_56_norm(u[i]);
    }
    FP12_BN254_pow4(f, g, u);

#else
    FP12_BN254_pow(f,f,e);
#endif
}

#ifdef HAS_MAIN

int main()
{
    int i;
    char byt[32];
    csprng rng;
    BIG_256_56 xa,xb,ya,yb,w,a,b,t1,q,u[2],v[4],m,r;
    ECP2_BN254 P,G;
    ECP_BN254 Q,R;
    FP12_BN254 g,gp;
    FP4_BN254 t,c,cp,cpm1,cpm2;
    FP2_BN254 x,y,X;


    BIG_256_56_rcopy(a,Fra_BN254);
    BIG_256_56_rcopy(b,Fra_BN254);
    FP2_BN254_from_BIGs(&X,a,b);

    BIG_256_56_rcopy(xa,CURVE_Gx_BN254);
    BIG_256_56_rcopy(ya,CURVE_Gy_BN254);

    ECP_BN254_set(&Q,xa,ya);
    //if (Q.inf) printf("Failed to set - point not on curve\n");
    //else printf("G1 set success\n");

    printf("Q= ");
    ECP_BN254_output(&Q);
    printf("\n");

    BIG_256_56_rcopy(xa,CURVE_Pxa_BN254);
    BIG_256_56_rcopy(xb,CURVE_Pxb_BN254);
    BIG_256_56_rcopy(ya,CURVE_Pya_BN254);
    BIG_256_56_rcopy(yb,CURVE_Pyb_BN254);

    FP2_BN254_from_BIGs(&x,xa,xb);
    FP2_BN254_from_BIGs(&y,ya,yb);

    ECP2_BN254_set(&P,&x,&y);
    //if (P.inf) printf("Failed to set - point not on curve\n");
    //else printf("G2 set success\n");

    printf("P= ");
    ECP2_BN254_output(&P);
    printf("\n");

    for (i=0; i<1000; i++ )
    {
        PAIR_BN254_ate(&g,&P,&Q);
        PAIR_BN254_fexp(&g);
    }
    printf("g= ");
    FP12_BN254_output(&g);
    printf("\n");
}

#endif
