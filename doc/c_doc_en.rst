**SecureUnionID C Interface and Usage Description**
====================================================================
**C Interface Description**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- **Hash Generation**
     **HASHIT(hashstring, m)**

     This interface is used to convert m into the serialized string hashstring corresponding to the element on the group G1.

     Parameter Description:
            ::

              m:           the string to be hashed
              hashstring:  serialized string

- **Random Number Generation**
      **genRandSeed(ran)**

      This interface is used to generate 64 bytes random number.

     Parameter Description:
            ::

              ran        the generated random number

      Return：the length (in bytes) of generated random number

- **Master Key Generation**
      **MasterKeygen(ran, masterkey)**

     This interface is used to randomly generate a 512-bit master key, which is used to subsequently derive the corresponding DSP public-private key pair.

     Parameter Description:
            ::

              ran:        random number seed
              masterkey:  generated master key

     Error Return: success or "masterkey null pointer" error

- **Master Key Generation**
      **genMasterKey(ran, masterkey)**

     This interface is used to randomly generate a 512-bit master key, which is used to subsequently derive the corresponding DSP public-private key pair.

     Parameter Description:
            ::

              ran:        random number seed (64 byte)
              masterkey:  generated master key

     Error Return: success or "masterkey null pointer" error

- **Key Generation**
     **Keygen(masterKey, dspId, pkg1string, pkg2string, skstring)**

     This interface is used to generate the public and private key pair corresponding to the DSP.

     Parameter Description:
            ::

              masterKey:              master key
              dspId:                  the identifier of the corresponding DSP
              pkg1string/pkg2string:  Generated serialized strings corresponding to the two public keys on groups G1 and G2
              skstring:               The serialized string corresponding to the generated private key

     Error Return: success or "parameter null pointer" error

- **System Public Key Generation**
     **System_Keygen(pkig1string, pkig2string, numofmedia, sysg1string, sysg2string)**

     This interface is used to generate the system public key corresponding to a DSP.

     Parameter Description:
            ::

              pkig1string/pkig2string:   an array of serialized strings corresponding to the public keys of a DSP of all the media
              numofmedia:                the number of participating media
              sysg1string/sysg2string:   the serialized string corresponding to the system public key of the DSP

     Error return: success or "parameter null pointer" error or "allocation space" error

- **Blinding**
     **Blinding(did, seed, betastring, Mstring)**

     This interface is used to generate random numbers for blinding and the blinded did.

     Parameter Description:
            ::

              seed:          random number seed
              did:           the did string to be blinded
              betastring:    The serialized string corresponding to the random number used for blinding
              Mstring:       The serialized string corresponding to the blinded result

     Error Return: success or "parameter null pointer" error

- **Blinding**
     **Blind(did, seed, betastring, Mstring)**

     This interface is used to generate random numbers for blinding and the blinded did.

     Parameter Description:
            ::

              seed:          random number seed (64 byte)
              did:           the did string to be blinded
              betastring:    The serialized string corresponding to the random number used for blinding
              Mstring:       The serialized string corresponding to the blinded result

     Error Return: success or "parameter null pointer" error

- **Encryption**
     **Enc(skstring, Mstring, btistring)**

     This interface is used to encrypt strings.

     Parameter Description:
            ::

              skstring:    the serialized string corresponding to the private key
              Mstring:     the string to be encrypted, generally the hashed did or blinded did
              btistring:   encrypted string

     Error Return: success or "parameter null pointer" error

- **Unblinding**
     **Unblinding(btistring, numofmedia, betastring, sysg1string, btstring)**

     This interface is used to merge the received message ciphertext and remove the random number to obtain the final ciphertext.

     Parameter Description:
            ::

                  btistring:    an array of encrypted strings from various media 
                  numofmedia:   the number of participating media
                  betastring:   the serialized string corresponding to the random number used for blinding
                  sysg1string:  the serialized string corresponding to the public key on group G1
                  btstring:     the final encrypted string

     Error Return: success or "parameter null pointer" error or "allocation space" error

**C Demo**
^^^^^^^^^^^^^

.. code-block:: c

     // This demo simulates the situation where Bytedance communicates with a DSP.
     // In this demo, the DSP is client and is responsible for blinding, unblinding
     // and verifying. Bytedance is server and is responsible for encryption.
     int main(){
          char did[16] = "123456789012345";
          char did1[16] = "123456789054321";
          char dspID[11] = "1234567890";
          char pkg1string[2*G1LENTH+1];
          char pkg2string[2*G2LENTH+1];
          char sysg1string[2*G1LENTH+1];
          char sysg2string[2*G2LENTH+1];
          char masterkey[64];
          char skstring[32];
          char *pkig1string[1], *pkig2string[1], *ciphers[2], *dids[2], *cipherstrings[1];
          char betastring[2*32+1],betastring1[2*32+1];
          char Mstring[2*G1LENTH+1],Mstring1[2*G1LENTH+1];
          char cipherstring[2*G1LENTH+1],cipherstring1[2*G1LENTH+1];
          char cipher[2*G1LENTH+1],cipher1[2*G1LENTH+1];
          char ran[64];
          int fd,result;

          // Generate random number seed
          genRandSeed(ran);

          // Bytedance generates keys.
          genMasterKey(ran,masterkey);
          Keygen(masterkey,dspID,pkg1string,pkg2string,skstring);

          // DSP generates the system key.
          pkig1string[0] = pkg1string;
          pkig2string[0] = pkg2string;
          System_Keygen(pkig1string,pkig2string,1,sysg1string,sysg2string);

          // DSP conducts blinding operation.
          genRandSeed(ran);
          Blind(did,ran,betastring,Mstring);
          genRandSeed(ran);
          Blind(did1,ran,betastring1,Mstring1);

          // Bytedance encrypts the received messages.
          Enc(skstring,Mstring,cipherstring);
          Enc(skstring,Mstring1,cipherstring1);

          // DSP unblinds the received ciphers.
          cipherstrings[0] = cipherstring;
          Unblinding(cipherstrings,1,betastring,sysg1string,cipher);
          cipherstrings[0] = cipherstring1;
          Unblinding(cipherstrings,1,betastring1,sysg1string,cipher1);

          ciphers[0] = cipher;
          ciphers[1] = cipher1;
          dids[0] = did;
          dids[1] = did1;

     }