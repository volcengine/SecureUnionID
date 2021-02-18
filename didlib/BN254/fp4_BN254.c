/*
	Licensed to the Apache Software Foundation (ASF) under one
	or more contributor license agreements.  See the NOTICE file
	distributed with this work for additional information
	regarding copyright ownership.  The ASF licenses this file
	to you under the Apache License, Version 2.0 (the
	"License"); you may not use this file except in compliance
	with the License.  You may obtain a copy of the License at

	  http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing,
	software distributed under the License is distributed on an
	"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
	KIND, either express or implied.  See the License for the
	specific language governing permissions and limitations
	under the License.
*/

/* AMCL Fp^4 functions */
/* SU=m, m is Stack Usage (no lazy )*/

/* FP4 elements are of the form a+ib, where i is sqrt(-1+sqrt(-1)) */

#include "fp4_BN254.h"

/* test x==0 ? */
/* SU= 8 */
int FP4_BN254_iszilch(FP4_BN254 *x) {
    if (FP2_BN254_iszilch(&(x->a)) && FP2_BN254_iszilch(&(x->b))) return 1;
    return 0;
}

/* test x==1 ? */
/* SU= 8 */
int FP4_BN254_isunity(FP4_BN254 *x) {
    if (FP2_BN254_isunity(&(x->a)) && FP2_BN254_iszilch(&(x->b))) return 1;
    return 0;
}

/* test is w real? That is in a+ib test b is zero */
int FP4_BN254_isreal(FP4_BN254 *w) {
    return FP2_BN254_iszilch(&(w->b));
}

/* return 1 if x==y, else 0 */
/* SU= 16 */
int FP4_BN254_equals(FP4_BN254 *x, FP4_BN254 *y) {
    if (FP2_BN254_equals(&(x->a), &(y->a)) && FP2_BN254_equals(&(x->b), &(y->b)))
        return 1;
    return 0;
}

/* set FP4 from two FP2s */
/* SU= 16 */
void FP4_BN254_from_FP2s(FP4_BN254 *w, FP2_BN254 *x, FP2_BN254 *y) {
    FP2_BN254_copy(&(w->a), x);
    FP2_BN254_copy(&(w->b), y);
}

/* set FP4 from FP2 */
/* SU= 8 */
void FP4_BN254_from_FP2(FP4_BN254 *w, FP2_BN254 *x) {
    FP2_BN254_copy(&(w->a), x);
    FP2_BN254_zero(&(w->b));
}

/* set high part of FP4 from FP2 */
/* SU= 8 */
void FP4_BN254_from_FP2H(FP4_BN254 *w, FP2_BN254 *x) {
    FP2_BN254_copy(&(w->b), x);
    FP2_BN254_zero(&(w->a));
}

/* FP4 copy w=x */
/* SU= 16 */
void FP4_BN254_copy(FP4_BN254 *w, FP4_BN254 *x) {
    if (w == x) return;
    FP2_BN254_copy(&(w->a), &(x->a));
    FP2_BN254_copy(&(w->b), &(x->b));
}

/* FP4 w=0 */
/* SU= 8 */
void FP4_BN254_zero(FP4_BN254 *w) {
    FP2_BN254_zero(&(w->a));
    FP2_BN254_zero(&(w->b));
}

/* FP4 w=1 */
/* SU= 8 */
void FP4_BN254_one(FP4_BN254 *w) {
    FP2_BN254_one(&(w->a));
    FP2_BN254_zero(&(w->b));
}

/* Set w=-x */
/* SU= 160 */
void FP4_BN254_neg(FP4_BN254 *w, FP4_BN254 *x) {
    /* Just one field neg */
    FP2_BN254 m, t;
    FP4_BN254_norm(x);
    FP2_BN254_add(&m, &(x->a), &(x->b));
    FP2_BN254_neg(&m, &m);
    FP2_BN254_add(&t, &m, &(x->b));
    FP2_BN254_add(&(w->b), &m, &(x->a));
    FP2_BN254_copy(&(w->a), &t);
    FP4_BN254_norm(w);
}

/* Set w=conj(x) */
/* SU= 16 */
void FP4_BN254_conj(FP4_BN254 *w, FP4_BN254 *x) {
    FP2_BN254_copy(&(w->a), &(x->a));
    FP2_BN254_neg(&(w->b), &(x->b));
    FP4_BN254_norm(w);
}

/* Set w=-conj(x) */
/* SU= 16 */
void FP4_BN254_nconj(FP4_BN254 *w, FP4_BN254 *x) {
    FP2_BN254_copy(&(w->b), &(x->b));
    FP2_BN254_neg(&(w->a), &(x->a));
    FP4_BN254_norm(w);
}

/* Set w=x+y */
/* SU= 16 */
void FP4_BN254_add(FP4_BN254 *w, FP4_BN254 *x, FP4_BN254 *y) {
    FP2_BN254_add(&(w->a), &(x->a), &(y->a));
    FP2_BN254_add(&(w->b), &(x->b), &(y->b));
}

/* Set w=x-y */
/* Input y MUST be normed */
void FP4_BN254_sub(FP4_BN254 *w, FP4_BN254 *x, FP4_BN254 *y) {
    FP4_BN254 my;
    FP4_BN254_neg(&my, y);
    FP4_BN254_add(w, x, &my);
}
/* SU= 8 */
/* reduce all components of w mod Modulus */
void FP4_BN254_reduce(FP4_BN254 *w) {
    FP2_BN254_reduce(&(w->a));
    FP2_BN254_reduce(&(w->b));
}

/* SU= 8 */
/* normalise all elements of w */
void FP4_BN254_norm(FP4_BN254 *w) {
    FP2_BN254_norm(&(w->a));
    FP2_BN254_norm(&(w->b));
}

/* Set w=s*x, where s is FP2 */
/* SU= 16 */
void FP4_BN254_pmul(FP4_BN254 *w, FP4_BN254 *x, FP2_BN254 *s) {
    FP2_BN254_mul(&(w->a), &(x->a), s);
    FP2_BN254_mul(&(w->b), &(x->b), s);
}

/* Set w=s*x, where s is FP */
void FP4_BN254_qmul(FP4_BN254 *w, FP4_BN254 *x, FP_BN254 *s) {
    FP2_BN254_pmul(&(w->a), &(x->a), s);
    FP2_BN254_pmul(&(w->b), &(x->b), s);
}

/* SU= 16 */
/* Set w=s*x, where s is int */
void FP4_BN254_imul(FP4_BN254 *w, FP4_BN254 *x, int s) {
    FP2_BN254_imul(&(w->a), &(x->a), s);
    FP2_BN254_imul(&(w->b), &(x->b), s);
}

/* Set w=x^2 */
/* Input MUST be normed  */
void FP4_BN254_sqr(FP4_BN254 *w, FP4_BN254 *x) {
    FP2_BN254 t1, t2, t3;

    FP2_BN254_mul(&t3, &(x->a), &(x->b)); /* norms x */
    FP2_BN254_copy(&t2, &(x->b));
    FP2_BN254_add(&t1, &(x->a), &(x->b));
    FP2_BN254_mul_ip(&t2);

    FP2_BN254_add(&t2, &(x->a), &t2);

    FP2_BN254_norm(&t1);  // 2
    FP2_BN254_norm(&t2);  // 2

    FP2_BN254_mul(&(w->a), &t1, &t2);

    FP2_BN254_copy(&t2, &t3);
    FP2_BN254_mul_ip(&t2);

    FP2_BN254_add(&t2, &t2, &t3);

    FP2_BN254_norm(&t2);  // 2
    FP2_BN254_neg(&t2, &t2);
    FP2_BN254_add(&(w->a), &(w->a), &t2);  /* a=(a+b)(a+i^2.b)-i^2.ab-ab = a*a+ib*ib */
    FP2_BN254_add(&(w->b), &t3, &t3);  /* b=2ab */

    FP4_BN254_norm(w);
}

/* Set w=x*y */
/* Inputs MUST be normed  */
void FP4_BN254_mul(FP4_BN254 *w, FP4_BN254 *x, FP4_BN254 *y) {

    FP2_BN254 t1, t2, t3, t4;
    FP2_BN254_mul(&t1, &(x->a), &(y->a));
    FP2_BN254_mul(&t2, &(x->b), &(y->b));

    FP2_BN254_add(&t3, &(y->b), &(y->a));
    FP2_BN254_add(&t4, &(x->b), &(x->a));

    FP2_BN254_norm(&t4); // 2
    FP2_BN254_norm(&t3); // 2

    FP2_BN254_mul(&t4, &t4, &t3); /* (xa+xb)(ya+yb) */

    FP2_BN254_neg(&t3, &t1);  // 1
    FP2_BN254_add(&t4, &t4, &t3);  //t4E=3
    FP2_BN254_norm(&t4);

    FP2_BN254_neg(&t3, &t2);  // 1
    FP2_BN254_add(&(w->b), &t4, &t3); //wbE=3

    FP2_BN254_mul_ip(&t2);
    FP2_BN254_add(&(w->a), &t2, &t1);

    FP4_BN254_norm(w);
}

/* output FP4 in format [a,b] */
/* SU= 8 */
void FP4_BN254_output(FP4_BN254 *w) {
    printf("[");
    FP2_BN254_output(&(w->a));
    printf(",");
    FP2_BN254_output(&(w->b));
    printf("]");
}

/* SU= 8 */
void FP4_BN254_rawoutput(FP4_BN254 *w) {
    printf("[");
    FP2_BN254_rawoutput(&(w->a));
    printf(",");
    FP2_BN254_rawoutput(&(w->b));
    printf("]");
}

/* Set w=1/x */
/* SU= 160 */
void FP4_BN254_inv(FP4_BN254 *w, FP4_BN254 *x) {
    FP2_BN254 t1, t2;
    FP2_BN254_sqr(&t1, &(x->a));
    FP2_BN254_sqr(&t2, &(x->b));
    FP2_BN254_mul_ip(&t2);
    FP2_BN254_norm(&t2);
    FP2_BN254_sub(&t1, &t1, &t2);
    FP2_BN254_inv(&t1, &t1);
    FP2_BN254_mul(&(w->a), &t1, &(x->a));
    FP2_BN254_neg(&t1, &t1);
    FP2_BN254_norm(&t1);
    FP2_BN254_mul(&(w->b), &t1, &(x->b));
}

/* w*=i where i = sqrt(-1+sqrt(-1)) */
/* SU= 200 */
void FP4_BN254_times_i(FP4_BN254 *w) {
    FP_BN254 z;
    FP2_BN254 s, t;
    FP2_BN254_copy(&t, &(w->b));

    FP2_BN254_copy(&s, &t);

    FP_BN254_copy(&z, &(s.a));
    FP_BN254_neg(&(s.a), &(s.b));
    FP_BN254_copy(&(s.b), &z);

    FP2_BN254_add(&t, &t, &s);

    FP2_BN254_copy(&(w->b), &(w->a));
    FP2_BN254_copy(&(w->a), &t);
    FP4_BN254_norm(w);
}

/* Set w=w^p using Frobenius */
/* SU= 16 */
void FP4_BN254_frob(FP4_BN254 *w, FP2_BN254 *f) {
    FP2_BN254_conj(&(w->a), &(w->a));
    FP2_BN254_conj(&(w->b), &(w->b));
    FP2_BN254_mul(&(w->b), f, &(w->b));
}

/* Set r=a^b mod m */
/* SU= 240 */
void FP4_BN254_pow(FP4_BN254 *r, FP4_BN254 *a, BIG_256_56 b) {
    FP4_BN254 w;
    BIG_256_56 z, zilch;
    int bt;

    BIG_256_56_zero(zilch);

    BIG_256_56_copy(z, b);
    BIG_256_56_norm(z);
    FP4_BN254_copy(&w, a);
    FP4_BN254_norm(&w);
    FP4_BN254_one(r);

    while (1) {
        bt = BIG_256_56_parity(z);
        BIG_256_56_shr(z, 1);
        if (bt) FP4_BN254_mul(r, r, &w);
        if (BIG_256_56_comp(z, zilch) == 0) break;
        FP4_BN254_sqr(&w, &w);
    }
    FP4_BN254_reduce(r);
}

/* SU= 304 */
/* XTR xtr_a function */
void FP4_BN254_xtr_A(FP4_BN254 *r, FP4_BN254 *w, FP4_BN254 *x, FP4_BN254 *y, FP4_BN254 *z) {
    FP4_BN254 t1, t2;
    FP4_BN254_copy(r, x);
    FP4_BN254_sub(&t1, w, y);
    FP4_BN254_norm(&t1);
    FP4_BN254_pmul(&t1, &t1, &(r->a));
    FP4_BN254_add(&t2, w, y);
    FP4_BN254_norm(&t2);
    FP4_BN254_pmul(&t2, &t2, &(r->b));
    FP4_BN254_times_i(&t2);

    FP4_BN254_add(r, &t1, &t2);
    FP4_BN254_add(r, r, z);

    FP4_BN254_reduce(r);
}

/* SU= 152 */
/* XTR xtr_d function */
void FP4_BN254_xtr_D(FP4_BN254 *r, FP4_BN254 *x) {
    FP4_BN254 w;
    FP4_BN254_copy(r, x);
    FP4_BN254_conj(&w, r);
    FP4_BN254_add(&w, &w, &w);
    FP4_BN254_sqr(r, r);
    FP4_BN254_norm(&w);
    FP4_BN254_sub(r, r, &w);
    FP4_BN254_reduce(r);    /* reduce here as multiple calls trigger automatic reductions */
}

/* SU= 728 */
/* r=x^n using XTR method on traces of FP12s */
void FP4_BN254_xtr_pow(FP4_BN254 *r, FP4_BN254 *x, BIG_256_56 n) {
    int i, par, nb;
    BIG_256_56 v;
    FP2_BN254 w;
    FP4_BN254 t, a, b, c, sf;

    BIG_256_56_zero(v);
    BIG_256_56_inc(v, 3);
    BIG_256_56_norm(v);
    FP2_BN254_from_BIG(&w, v);
    FP4_BN254_from_FP2(&a, &w);

    FP4_BN254_copy(&sf, x);
    FP4_BN254_norm(&sf);
    FP4_BN254_copy(&b, &sf);
    FP4_BN254_xtr_D(&c, &sf);

    par = BIG_256_56_parity(n);
    BIG_256_56_copy(v, n);
    BIG_256_56_norm(v);
    BIG_256_56_shr(v, 1);
    if (par == 0) {
        BIG_256_56_dec(v, 1);
        BIG_256_56_norm(v);
    }

    nb = BIG_256_56_nbits(v);
    for (i = nb - 1; i >= 0; i--) {
        if (!BIG_256_56_bit(v, i)) {
            FP4_BN254_copy(&t, &b);
            FP4_BN254_conj(&sf, &sf);
            FP4_BN254_conj(&c, &c);
            FP4_BN254_xtr_A(&b, &a, &b, &sf, &c);
            FP4_BN254_conj(&sf, &sf);
            FP4_BN254_xtr_D(&c, &t);
            FP4_BN254_xtr_D(&a, &a);
        } else {
            FP4_BN254_conj(&t, &a);
            FP4_BN254_xtr_D(&a, &b);
            FP4_BN254_xtr_A(&b, &c, &b, &sf, &t);
            FP4_BN254_xtr_D(&c, &c);
        }
    }

    if (par == 0) FP4_BN254_copy(r, &c);
    else FP4_BN254_copy(r, &b);
    FP4_BN254_reduce(r);
}

/* SU= 872 */
/* r=ck^a.cl^n using XTR double exponentiation method on traces of FP12s. See Stam thesis. */
void FP4_BN254_xtr_pow2(FP4_BN254 *r, FP4_BN254 *ck, FP4_BN254 *cl, FP4_BN254 *ckml, FP4_BN254 *ckm2l, BIG_256_56 a,
                        BIG_256_56 b) {
    int i, f2;
    BIG_256_56 d, e, w;
    FP4_BN254 t, cu, cv, cumv, cum2v;


    BIG_256_56_copy(e, a);
    BIG_256_56_copy(d, b);
    BIG_256_56_norm(e);
    BIG_256_56_norm(d);
    FP4_BN254_copy(&cu, ck);
    FP4_BN254_copy(&cv, cl);
    FP4_BN254_copy(&cumv, ckml);
    FP4_BN254_copy(&cum2v, ckm2l);

    f2 = 0;
    while (BIG_256_56_parity(d) == 0 && BIG_256_56_parity(e) == 0) {
        BIG_256_56_shr(d, 1);
        BIG_256_56_shr(e, 1);
        f2++;
    }
    while (BIG_256_56_comp(d, e) != 0) {
        if (BIG_256_56_comp(d, e) > 0) {
            BIG_256_56_imul(w, e, 4);
            BIG_256_56_norm(w);
            if (BIG_256_56_comp(d, w) <= 0) {
                BIG_256_56_copy(w, d);
                BIG_256_56_copy(d, e);
                BIG_256_56_sub(e, w, e);
                BIG_256_56_norm(e);
                FP4_BN254_xtr_A(&t, &cu, &cv, &cumv, &cum2v);
                FP4_BN254_conj(&cum2v, &cumv);
                FP4_BN254_copy(&cumv, &cv);
                FP4_BN254_copy(&cv, &cu);
                FP4_BN254_copy(&cu, &t);
            } else if (BIG_256_56_parity(d) == 0) {
                BIG_256_56_shr(d, 1);
                FP4_BN254_conj(r, &cum2v);
                FP4_BN254_xtr_A(&t, &cu, &cumv, &cv, r);
                FP4_BN254_xtr_D(&cum2v, &cumv);
                FP4_BN254_copy(&cumv, &t);
                FP4_BN254_xtr_D(&cu, &cu);
            } else if (BIG_256_56_parity(e) == 1) {
                BIG_256_56_sub(d, d, e);
                BIG_256_56_norm(d);
                BIG_256_56_shr(d, 1);
                FP4_BN254_xtr_A(&t, &cu, &cv, &cumv, &cum2v);
                FP4_BN254_xtr_D(&cu, &cu);
                FP4_BN254_xtr_D(&cum2v, &cv);
                FP4_BN254_conj(&cum2v, &cum2v);
                FP4_BN254_copy(&cv, &t);
            } else {
                BIG_256_56_copy(w, d);
                BIG_256_56_copy(d, e);
                BIG_256_56_shr(d, 1);
                BIG_256_56_copy(e, w);
                FP4_BN254_xtr_D(&t, &cumv);
                FP4_BN254_conj(&cumv, &cum2v);
                FP4_BN254_conj(&cum2v, &t);
                FP4_BN254_xtr_D(&t, &cv);
                FP4_BN254_copy(&cv, &cu);
                FP4_BN254_copy(&cu, &t);
            }
        }
        if (BIG_256_56_comp(d, e) < 0) {
            BIG_256_56_imul(w, d, 4);
            BIG_256_56_norm(w);
            if (BIG_256_56_comp(e, w) <= 0) {
                BIG_256_56_sub(e, e, d);
                BIG_256_56_norm(e);
                FP4_BN254_xtr_A(&t, &cu, &cv, &cumv, &cum2v);
                FP4_BN254_copy(&cum2v, &cumv);
                FP4_BN254_copy(&cumv, &cu);
                FP4_BN254_copy(&cu, &t);
            } else if (BIG_256_56_parity(e) == 0) {
                BIG_256_56_copy(w, d);
                BIG_256_56_copy(d, e);
                BIG_256_56_shr(d, 1);
                BIG_256_56_copy(e, w);
                FP4_BN254_xtr_D(&t, &cumv);
                FP4_BN254_conj(&cumv, &cum2v);
                FP4_BN254_conj(&cum2v, &t);
                FP4_BN254_xtr_D(&t, &cv);
                FP4_BN254_copy(&cv, &cu);
                FP4_BN254_copy(&cu, &t);
            } else if (BIG_256_56_parity(d) == 1) {
                BIG_256_56_copy(w, e);
                BIG_256_56_copy(e, d);
                BIG_256_56_sub(w, w, d);
                BIG_256_56_norm(w);
                BIG_256_56_copy(d, w);
                BIG_256_56_shr(d, 1);
                FP4_BN254_xtr_A(&t, &cu, &cv, &cumv, &cum2v);
                FP4_BN254_conj(&cumv, &cumv);
                FP4_BN254_xtr_D(&cum2v, &cu);
                FP4_BN254_conj(&cum2v, &cum2v);
                FP4_BN254_xtr_D(&cu, &cv);
                FP4_BN254_copy(&cv, &t);
            } else {
                BIG_256_56_shr(d, 1);
                FP4_BN254_conj(r, &cum2v);
                FP4_BN254_xtr_A(&t, &cu, &cumv, &cv, r);
                FP4_BN254_xtr_D(&cum2v, &cumv);
                FP4_BN254_copy(&cumv, &t);
                FP4_BN254_xtr_D(&cu, &cu);
            }
        }
    }
    FP4_BN254_xtr_A(r, &cu, &cv, &cumv, &cum2v);
    for (i = 0; i < f2; i++) FP4_BN254_xtr_D(r, r);
    FP4_BN254_xtr_pow(r, r, d);
}

/* Move b to a if d=1 */
void FP4_BN254_cmove(FP4_BN254 *f, FP4_BN254 *g, int d) {
    FP2_BN254_cmove(&(f->a), &(g->a), d);
    FP2_BN254_cmove(&(f->b), &(g->b), d);
}

/* New stuff for ECp4 support */

/* Set w=x/2 */
void FP4_BN254_div2(FP4_BN254 *w, FP4_BN254 *x) {
    FP2_BN254_div2(&(w->a), &(x->a));
    FP2_BN254_div2(&(w->b), &(x->b));
}

