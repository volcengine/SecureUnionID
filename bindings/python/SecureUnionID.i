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
    
// generate master key
%cstring_bounded_output(char* BYTE, 1024);
int 

(unsigned long ran, char *BYTE);
    
// generate key pairs
int Keygen(char *masterKey, char *dspid, char *BYTE, char *BYTE, char *BYTE);

//generate master key
int MasterKeygen(unsigned long ran, char *BYTE);
    
// generate system key
int System_Keygen(char **STRING_ARRAY, char **STRING_ARRAY, int numofmedia, char *BYTE, char *BYTE);
    
// Blinding for each did
int Blinding(char *did, unsigned long seed, char *BYTE, char *BYTE);
    
// Encoding for each did
int Enc(char *skstring, char *Mstring, char *BYTE);
    
// Unblinding for each did
int Unblinding(char **STRING_ARRAY, int numofmedia, char *betastring, char *sysg1string, char *BYTE);
    
// Verifying bti received from each media
// btistring represents the different btis received from different media i, pkistring represents different pki of corresponding media i
int verify_individual(char **STRING_ARRAY, char **STRING_ARRAY, char **STRING_ARRAY, char *did, int numofmedia, char *BYTE);
    
// check whether there are some medias cheating from multiple final unblinded results
// btstring represents the string which is composed of several results of different dids
int batch_verify(char **STRING_ARRAY, char **STRING_ARRAY, char *sysg2string, int numofdid);
