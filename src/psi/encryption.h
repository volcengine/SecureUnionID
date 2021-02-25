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

#ifndef DID_ENCRYPTION_H
#define DID_ENCRYPTION_H
#include "../crypto/ecp_BN254.h"
#include "../crypto/ecp2_BN254.h"
#include "../crypto/fn_BN254.h"
#include "../crypto/pair_BN254.h"
#include "../crypto/hkdf.h"
#include "fcntl.h"
#include "string.h"
#include "time.h"
#define G1LENTH (MODBYTES_256_56+1)
#define G2LENTH 4*MODBYTES_256_56

#define NULLPOINTER 0
#define MALLOCERROR 1
#define SUCCESS 2
#define FAIL 3

typedef struct{
    ECP_BN254 PK1;
    ECP2_BN254 PK2;
}ECC_PK;

typedef struct {
    ECP_BN254 g1;
    ECP2_BN254 g2;
}ECC_para;

// Hash func: {0,1}->G1
void HASHIT(char *hashstring, char *m);

//generate system para
/*int Genpara(char *parag1string, char *parag2string);*/

//set system para
/*void Setpara();*/

//generate master key
int MasterKeygen(unsigned long ran, char *masterkey );

//generate key pairs
int Keygen(char *masterkey, char *dspid, char *pkg1string, char *pkg2string, char *skstring);

//generate system key
int System_Keygen(char **pkig1string, char **pkig2string, int numofmedia, char *sysg1string, char *sysg2string);

//Blinding for each did
int Blinding(char *did, unsigned long seed, char *betastring, char *Mstring);

//Encoding for each did
int Enc(char *skstring, char *Mstring, char *btistring);

//Unblinding for each did
int Unblinding(char **btistring, int numofmedia, char *betastring, char *sysg1string,  char *btstring);

//Verifying bti received from each media
//btistring represents the different btis received from different media i, pkistring represents different pki of corresponding media i
int verify_individual(char **btistring, char **pkig1string, char **pkig2string, char *did, int numofmedia, char *betastring);

//check whether there are some medias cheating from multiple final unblinded results
//btstring represents the string which is composed of several results of different dids
int batch_verify(char **btstring, char **did, char *sysg2string, int numofdid);

#endif