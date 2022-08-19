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

%module SecureUnionID
%{
//#define SWIG_PYTHON_STRICT_BYTE_CHAR // this is for python3. for python2 it should be removed.
#include "../../src/psi/encryption.h"
%}

%include <typemaps.i>
%include <cstring.i>

// This tells SWIG to treat char ** as a special case
%typemap(in) char ** {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = (char **) malloc((size+1)*sizeof(char *));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyString_Check(o))
	$1[i] = PyString_AsString(PyList_GetItem($input,i));
      else {
	PyErr_SetString(PyExc_TypeError,"list must contain strings");
	free($1);
	return NULL;
      }
    }
    $1[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

// This cleans up the char ** array we malloc'd before the function call
%typemap(freearg) char ** {
  free((char *) $1);
}

// Hash func: {0,1}->G1
void HASHIT(char *hashstring, char *m);

// generate random seed
unsigned long randomSeed();
    
%cstring_bounded_output(char* outRnd, 64);
%cstring_bounded_output(char* outMasterKey, 64);
%cstring_bounded_output(char* outPublicKeyOfG1, 66);
%cstring_bounded_output(char* outPublicKeyOfG2, 256);
%cstring_bounded_output(char* outPrivateKey, 32);
%cstring_bounded_output(char* outSystemKeyOfG1, 66);
%cstring_bounded_output(char* outSystemKeyOfG2, 256);

%cstring_bounded_output(char* outBetaValue, 64);
%cstring_bounded_output(char* outBlindResult, 66);
%cstring_bounded_output(char* outEncResult, 66);
%cstring_bounded_output(char* outBlindResult, 66);
%cstring_bounded_output(char* outUnblindCipherText, 66);


// generate random seed
int genRandSeed(char *outRnd);

//generate master key
int MasterKeygen(unsigned long ran, char *outMasterKey);

//generate master key
int genMasterKey(char *rnd, char *outMasterKey);

// generate key pairs
int Keygen(char *masterKey, char *dspid, char *outPublicKeyOfG1, char *outPublicKeyOfG2, char *outPrivateKey);
    
// generate system key
int System_Keygen(char **STRING_ARRAY, char **STRING_ARRAY, int numofmedia, char *outSystemKeyOfG1, char *outSystemKeyOfG2);
    
// Blinding for each did
int Blinding(char *did, unsigned long seed, char *outBetaValue, char *outBlindResult);

// Blinding for each did
int Blind(char *did, char *seed, char *outBetaValue, char *outBlindResult);

// Encoding for each did
int Enc(char *skstring, char *Mstring, char *outEncResult);
    
// Unblinding for each did
int Unblinding(char **STRING_ARRAY, int numofmedia, char *betastring, char *sysg1string, char *outUnblindCipherText);