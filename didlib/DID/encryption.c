// Copyright 2021 The SecureUnionID Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "encryption.h"

char parag1string[]="032523648240000001ba344d80000000086121000000000013a700000000000012";
char parag2string[]="061a10bb519eb62feb8d8c7e8c61edb6a4648bbb4898bf0d91ee4224c803fb2b0516aaf9ba737833310aa78c5982aa5b1f4d746bae3784b70d8c34c1e7d54cf3021897a06baf93439a90e096698c822329bd0ae6bdbe09bd19f0e07891cd2b9a0ebb2b0e7c8b15268f6d4456f5f38d37b09006ffd739c9578a2d1aec6b3ace9b";

/* hash the string and map it into G1 element */
void BN254_HASHIT(ECP_BN254 *P, char *m)
{
    int i;
    sha3 hs;
    char h[MODBYTES_256_56];
    octet HM = {MODBYTES_256_56, sizeof(h), h};
    // get random value of 32 bytes with sha3
    SHA3_init(&hs, SHAKE256);
    for (i = 0; m[i] != 0; i++)
        SHA3_process(&hs, m[i]);
    SHA3_shake(&hs, HM.val, MODBYTES_256_56);
    // map the octet object HM into G1
    ECP_BN254_mapit(P, &HM);
}

/* serialize G1 element */
void g1_serialize(ECP_BN254 g1, char *g1string)
{
    int i;
    char c[2 * MODBYTES_256_56 + 1];
    octet C = {0, sizeof(c), c};
    // convert the G1 element into octec format
    ECP_BN254_toOctet(&C, &g1, true);
    for (i = 0; i < C.len; i++)
    {
        // format
        sprintf(g1string + i * 2, "%02x", (unsigned char)C.val[i]);
    }
    g1string[2 * C.len] = '\0';
}

/* deserialize G1 */
void g1_deserialize(ECP_BN254 *g1, char *g1string)
{
    int j;
    char c[2 * MODBYTES_256_56 + 1];
    unsigned int bytes;
    int counter = 0;
    octet C = {0, sizeof(c), c};
    C.len = MODBYTES_256_56 + 1;
    char temp[2];
    // convert the string into octec format
    for (j = 0; j < 2 * C.len; j += 2)
    {
        if (0 == j % 2)
        {
            temp[0] = g1string[j];
            temp[1] = g1string[j + 1];
            sscanf(temp, "%02x", &bytes);
            C.val[counter] = bytes;
            counter++;
        }
    }
    // convert octec format into G1 element
    ECP_BN254_fromOctet(g1, &C);
}

/* serialize G2 element */
void g2_serialize(ECP2_BN254 g2, char *g2string)
{
    int i;
    char di[4 * MODBYTES_256_56];
    octet Di = {0, sizeof(di), di};
    // convert the G2 element into octec format
    ECP2_BN254_toOctet(&Di, &g2);
    for (i = 0; i < Di.len; i++)
    {
        // format
        sprintf(g2string + i * 2, "%02x", (unsigned char)Di.val[i]);
    }
    g2string[2 * Di.len] = '\0';
}

/* deserialize G2 */
void g2_deserialize(ECP2_BN254 *g2, char *g2string)
{
    int j;
    char di[4 * MODBYTES_256_56];
    octet Di = {0, sizeof(di), di};
    Di.len = sizeof(di);
    int counter = 0;
    char temp[2];
    unsigned int bytes;
    // convert the string into octec format
    for (j = 0; j < 2 * Di.len; j += 2)
    {
        if (0 == j % 2)
        {
            temp[0] = g2string[j];
            temp[1] = g2string[j + 1];
            sscanf(temp, "%02x", &bytes);
            Di.val[counter] = bytes;
            counter++;
        }
    }
     // convert octec format into G2 element
    ECP2_BN254_fromOctet(g2, &Di);
}

/* serialize BIG element */
void big_serialize(BIG_256_56 big, char *bigstring)
{
    int i;
    char bigbyte[MODBYTES_256_56];
    BIG_256_56_toBytes(bigbyte, big);
    for (i = 0; i < MODBYTES_256_56; i++)
    {
        //格式化输入str
        sprintf(bigstring + i * 2, "%02x", (unsigned char)bigbyte[i]);
    }
    bigstring[2 * MODBYTES_256_56] = '\0';
}

/* deserialize BIG element */
void big_deserialize(BIG_256_56 big, char *bigstring)
{
    char bigbyte[MODBYTES_256_56];
    int counter = 0,j;
    char temp[2];
    unsigned int bytes;
    // convert the string into bytes[]
    for (j = 0; j < 2 * MODBYTES_256_56; j += 2)
    {
        if (0 == j % 2)
        {
            temp[0] = bigstring[j];
            temp[1] = bigstring[j + 1];
            sscanf(temp, "%02x", &bytes);
            bigbyte[counter] = bytes;
            counter++;
        }
    }
    // convert the bytes[] to BIG element
    BIG_256_56_fromBytes(big, bigbyte);
}

/* hash the string and map it into G1 string */
void HASHIT(char *hashstring, char *m)
{
    int i;
    ECP_BN254 P;
    sha3 hs;
    char h[MODBYTES_256_56];
    octet HM = {MODBYTES_256_56, sizeof(h), h};
    // get random value of 32 bytes with sha3
    SHA3_init(&hs, SHAKE256);
    for (i = 0; m[i] != 0; i++)
        SHA3_process(&hs, m[i]);
    SHA3_shake(&hs, HM.val, MODBYTES_256_56);
    // map the octet object HM into G1
    ECP_BN254_mapit(&P, &HM);
    g1_serialize(P,hashstring);
}

/**---------------------------dividing line-----------------------------**/

/*int Genpara(char *parag1string, char *parag2string)
{
    if(!parag1string || !parag2string){
        //printf("genpara: null pointer!\n");
        return 0;
    }
    // generate the generator g1 and g2 for G1 and G2
    ECP_BN254_generator(&(para.g1));
    ECP2_BN254_generator(&(para.g2));
    // serialize the system parameters and store it into parastring
    g1_serialize(para.g1, parag1string);
    g2_serialize(para.g2, parag2string);
    return 1;
}*/

/*void Setpara()
{
    //deserialize parastring and set para
    g1_deserialize(&(para.g1), parag1string);
    g2_deserialize(&(para.g2), parag2string);
}*/

/* generate the master key */
int MasterKeygen(unsigned long ran, char *masterkey ){
    int i;
    char raw[100];
    csprng rng;
    octet RAW = {0, sizeof(raw), raw};
    if(!masterkey){
        return NULLPOINTER;
    }

    RAW.len = 100; // fake random seed source
    RAW.val[0] = ran;
    RAW.val[1] = ran >> 8;
    RAW.val[2] = ran >> 16;
    RAW.val[3] = ran >> 24; // fake random seed source
    for (i = 4; i < 100; i++)
        RAW.val[i] = i;
    CREATE_CSPRNG(&rng, &RAW); // initialise strong RNG

    for (i = 0; i < 64; i++)
        masterkey[i] = RAND_byte(&rng);
    return SUCCESS;
}

/* derive the public key for each dsp according to the masterkey
   [in]: masterkey: the master key
         dspId: dsp id
   [out]: pkg1string:  pkg2string   the derived public key
*/
int Keygen(char *masterkey, char *dspId, char *pkg1string, char *pkg2string, char *skstring)
{
    char g1string[G1LENTH * 2 + 1], g2string[G2LENTH * 2 + 1];
    ECC_PK pk;
    ECC_para para;
    BIG_256_56 alpha;
    BIG_256_56 q;
    if(!masterkey || !dspId || !pkg1string || !pkg2string|| !skstring){
        return NULLPOINTER;
    }

    HKDF((unsigned char *)masterkey, 64, (unsigned char *)dspId, strlen(dspId), (unsigned char *) skstring);
    BIG_256_56_fromBytes(alpha, skstring);
    BIG_256_56_rcopy(q, CURVE_Order_BN254);
    BIG_256_56_mod(alpha,q);

    //P K1=g1^alpha
    g1_deserialize(&(para.g1),parag1string);
    ECP_BN254_copy(&pk.PK1, &(para.g1));
    ECP_BN254_mul(&pk.PK1, alpha);

    // PK2=g2^alpha
    g2_deserialize(&(para.g2),parag2string);
    ECP2_BN254_copy(&pk.PK2, &(para.g2));
    ECP2_BN254_mul(&pk.PK2, alpha);

    // serialize the public key and store it into pkstring */
    g1_serialize(pk.PK1, pkg1string);
    g2_serialize(pk.PK2, pkg2string);
    return SUCCESS;
}

/* calculate the public key of the system according to the public key of each media
   [in]: pkig1string, pkig2string: the public key for the media
         numofmedia: the number of the media
   [out]: sysg1string, sysg2string: the pubic key of the system
*/
int System_Keygen(char **pkig1string, char **pkig2string, int numofmedia, char *sysg1string, char *sysg2string)
{
    ECC_PK sys_pk;
    int i;
    char g1string[G1LENTH * 2 + 1], g2string[G2LENTH * 2 + 1];
    if(!pkig1string || !pkig2string || !sysg1string || !sysg2string){
        return NULLPOINTER;
    }
    // set the system parameter to be zero, ie, the infinity pointer.
    ECP_BN254_inf(&sys_pk.PK1);
    ECP2_BN254_inf(&sys_pk.PK2);
    ECC_PK *pki=(ECC_PK *)malloc(sizeof(ECC_PK)*numofmedia);
    if (!pki) {
        return MALLOCERROR;
    }

    for (i = 0; i < numofmedia; i++)
    {
        if(!pkig1string[i] || !pkig2string[i]){
            return NULLPOINTER;
        }
        // deserialize the public key for the i th media.
        g1_deserialize(&(pki[i].PK1), pkig1string[i]);
        g2_deserialize(&(pki[i].PK2), pkig2string[i]);
        // compute the public key for the system
        ECP_BN254_add(&sys_pk.PK1, &pki[i].PK1);
        ECP2_BN254_add(&sys_pk.PK2, &pki[i].PK2);
    }
    g1_serialize(sys_pk.PK1, sysg1string);
    g2_serialize(sys_pk.PK2, sysg2string);

    free(pki);
    return SUCCESS;
}

/* blinding
   [in]: did: device ID
         seed: the random seed for generating the value for blinding
   [out] betastring: the random value for the blinding
         Mstring: the result of the blinding
*/
int Blinding(char *did, unsigned long seed, char *betastring, char *Mstring)
{
    char raw[100];
    octet RAW = {0, sizeof(raw), raw};
    ECP_BN254 hashdid;
    BIG_256_56 q, bigbeta;
    ECC_para para;
    ECP_BN254 M;
    int i;
    if(!did || !betastring || !Mstring){
        return NULLPOINTER;
    }

    // generate the random seed.
    csprng rng;
    RAW.len = 100; // fake random seed source
    RAW.val[0] = seed;
    RAW.val[1] = seed >> 8;
    RAW.val[2] = seed >> 16;
    RAW.val[3] = seed >> 24; // fake random seed source
    for (i = 4; i < 100; i++)
        RAW.val[i] = i;
    CREATE_CSPRNG(&rng, &RAW); // initialise strong RNG

    // generate the random BIG nummber for blinding
    BIG_256_56_rcopy(q, CURVE_Order_BN254);
    BIG_256_56_randomnum(bigbeta, q, &rng);
    big_serialize(bigbeta, betastring);

    //hashdid=H(did)
    BN254_HASHIT(&hashdid, did);
    //M=hashdid*g1^biabeta
    g1_deserialize(&(para.g1),parag1string);
    ECP_BN254_copy(&M, &(para.g1));
    ECP_BN254_mul(&M, bigbeta);
    ECP_BN254_add(&M, &hashdid);

    // serialize M
    g1_serialize(M, Mstring);
    return SUCCESS;
}

/* encryption
   [in]: skstring: private key
            dspId: dsp id
          Mstring: the plaintext
   [out]: btistring: cipertext
*/
int Enc(char *skstring,  char *Mstring, char *btistring)
{
    BIG_256_56 bigski,q;
    ECP_BN254 M;
    ECP_BN254 bti;
    int i;

    if(!skstring || !Mstring || !btistring){
        return NULLPOINTER;
    }

    BIG_256_56_fromBytes(bigski, skstring);
    BIG_256_56_rcopy(q, CURVE_Order_BN254);
    BIG_256_56_mod(bigski,q);

    // deserialize Mstring and store it into M
    g1_deserialize(&M, Mstring);

    // bti = M^ski, ie, bti = M^alpha
    ECP_BN254_copy(&bti, &M);
    ECP_BN254_mul(&bti, bigski);

    // serialize bti and store it into btistring
    g1_serialize(bti, btistring);
    return SUCCESS;
}

/* verify the individual media */
int verify_individual(char **btistring, char **pkig1string, char **pkig2string, char *did, int numofmedia, char *betastring)
{
    ECP_BN254 hashdid, unblinded, *bti;
    ECC_PK *pki;
    BIG_256_56 bigbeta;
    ECC_para para;
    FP12_BN254 e_bt_g2, e_did_pk;
    int i;
    if(!btistring || !pkig1string || !pkig2string){
        return NULLPOINTER;
    }

    if((bti=(ECP_BN254 *)malloc(sizeof(ECP_BN254)*numofmedia))==NULL){
        return MALLOCERROR;
    }
    if((pki=(ECC_PK *)malloc(sizeof(ECC_PK)*numofmedia))==NULL){
        return MALLOCERROR;
    }

    BN254_HASHIT(&hashdid, did);
    // deserialize the betastring and store it into bigbeta
    big_deserialize(bigbeta, betastring);

    for (i = 0; i < numofmedia; i++)
    {
        if(!btistring[i] || !pkig1string[i] || !pkig2string[i]){
            return NULLPOINTER;
        }
        // deserialize bti and pki
        g1_deserialize(&pki[i].PK1, pkig1string[i]);
        g2_deserialize(&pki[i].PK2, pkig2string[i]);
        g1_deserialize(&bti[i], btistring[i]);
        // unblinded=bti*pki_1^(-bigbeta)
        ECP_BN254_copy(&unblinded, &pki[i].PK1);
        ECP_BN254_mul(&unblinded, bigbeta);
        ECP_BN254_neg(&unblinded);
        ECP_BN254_add(&unblinded, &bti[i]);

        // e_bt_g2 = e(unblinded,g2)
        g2_deserialize(&(para.g2),parag2string);
        PAIR_BN254_ate(&e_bt_g2, &(para.g2), &unblinded);
        PAIR_BN254_fexp(&e_bt_g2);
        // e_did_pk = e(H(did),pki_2)
        PAIR_BN254_ate(&e_did_pk, &pki[i].PK2, &hashdid);
        PAIR_BN254_fexp(&e_did_pk);
        // if not equal, output the number of the cheating media.
        if (!FP12_BN254_equals(&e_did_pk, &e_bt_g2))
            return (-(i+1));
    }
    free(bti);
    free(pki);
    return SUCCESS;
}

/* batch verify the result */
int batch_verify(char **btstring, char **did, char *sysg2string, int numofdid)
{
    ECP_BN254 *hashdid,*bt;
    FP12_BN254 e_bt_g2, e_did_pk;
    ECP_BN254 btprod, didprod;
    ECP2_BN254 sys_pkg2;
    ECC_para para;
    int i;
    if(!btstring || !did || !sysg2string){
        return NULLPOINTER;
    }

    if((hashdid=(ECP_BN254 *)malloc(sizeof(ECP_BN254)*numofdid))==NULL){
        return MALLOCERROR;
    }
    if((bt=(ECP_BN254 *)malloc(sizeof(ECP_BN254)*numofdid))==NULL){
        return MALLOCERROR;
    }

    ECP_BN254_inf(&btprod);
    ECP_BN254_inf(&didprod);
    for (i = 0; i < numofdid; i++)
    {
        if(!btstring[i] || !did[i]){
            return NULLPOINTER;
        }
        g1_deserialize(&bt[i], btstring[i]);
        BN254_HASHIT(&hashdid[i], did[i]);
        //btprod=btprod*bt[i]
        ECP_BN254_add(&btprod, &bt[i]);
        //didprod=didprod*hashdid[i]
        ECP_BN254_add(&didprod, &hashdid[i]);
    }

    //e_bt_g2 = e(btprod,g2)
    g2_deserialize(&(para.g2),parag2string);
    PAIR_BN254_ate(&e_bt_g2, &(para.g2), &btprod);
    PAIR_BN254_fexp(&e_bt_g2);
    //e_did_pk = e(didprod,pki_2)
    g2_deserialize(&sys_pkg2, sysg2string);
    PAIR_BN254_ate(&e_did_pk, &sys_pkg2, &didprod);
    PAIR_BN254_fexp(&e_did_pk);

    free(hashdid);
    free(bt);
    if(FP12_BN254_equals(&e_did_pk, &e_bt_g2))
        return SUCCESS;
    else return FAIL;
}

/* unblinding
   [in]: btistring: the cipher before unblinding
         numofmedia: the number of the media
         sysg1string: the system public key
         betastring: the value for unblinding
   [out]: btistring: the output after unblinding
*/
int Unblinding(char **btistring, int numofmedia, char *betastring, char *sysg1string, char *btstring)
{
    ECP_BN254 sys_pkg1;
    ECP_BN254 *bti, bt;
    BIG_256_56 bigbeta;
    int i;
    if(!btistring || !betastring || !sysg1string){
        return NULLPOINTER;
    }

    if((bti=(ECP_BN254 *)malloc(sizeof(ECP_BN254)*numofmedia))==NULL){
        return MALLOCERROR;
    }

    // get the btistring of each media and serialize into the bti array.
    for (i = 0; i < numofmedia; i++)
    {
        if(!btistring[i]){
            return NULLPOINTER;
        }
        g1_deserialize(&bti[i], btistring[i]);
    }

    // deserialize betastring and store it into the BIG number bigbeta
    big_deserialize(bigbeta, betastring);

    // bt = PK^(-biabeta)
    g1_deserialize(&sys_pkg1, sysg1string);
    ECP_BN254_copy(&bt, &sys_pkg1);
    ECP_BN254_mul(&bt, bigbeta);
    ECP_BN254_neg(&bt);

    for (i = 0; i < numofmedia; i++)
    {
        // bt = bt*bti
        ECP_BN254_add(&bt, &bti[i]);
    }

    // store the result
    g1_serialize(bt, btstring);
    free(bti);
    return SUCCESS;
}

