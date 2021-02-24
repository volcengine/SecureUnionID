%module secureunionid
%{
#include "../../didlib/DID/encryption.h"
%}

%include various.i

// Hash func: {0,1}->G1
void HASHIT(char *hashstring, char *m);
    
// generate master key
int MasterKeygen(unsigned long ran, char *BYTE);
    
// generate key pairs
int Keygen(char *BYTE, char *dspid, char *BYTE, char *BYTE, char *BYTE);
    
// generate system key
int System_Keygen(char **STRING_ARRAY, char **STRING_ARRAY, int numofmedia, char *BYTE, char *BYTE);
    
// Blinding for each did
int Blinding(char *did, unsigned long seed, char *BYTE, char *BYTE);
    
// Encoding for each did
int Enc(char *BYTE, char *BYTE, char *BYTE);
    
// Unblinding for each did
int Unblinding(char **STRING_ARRAY, int numofmedia, char *BYTE, char *BYTE,  char *BYTE);
    
// Verifying bti received from each media
// btistring represents the different btis received from different media i, pkistring represents different pki of corresponding media i
int verify_individual(char **STRING_ARRAY, char **STRING_ARRAY, char **STRING_ARRAY, char *did, int numofmedia, char *BYTE);
    
// check whether there are some medias cheating from multiple final unblinded results
// btstring represents the string which is composed of several results of different dids
int batch_verify(char **STRING_ARRAY, char **STRING_ARRAY, char *BYTE, int numofdid);
