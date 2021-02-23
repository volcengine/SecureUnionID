**SecureUnionID Interface Description and Performance Evaluation**
====================================================================
**Go Interface Description**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- **Random Number Seed Generation**
     **SeedGen()**

     This interface is used to generate and return a random number seed of unit32 type, which is used for subsequent random number generation.

     Error Return: nil or "file open failure" error or "binary read" error 
- **Master Key Generation**
     **MasterKeyGen(seed)**

     This interface is used to randomly generate a 512-bit master key, which is used to subsequently derive the corresponding DSP public-private key pair.

     Parameter Description:
          ::

               seed: random number seed

     Error Return: nil or "masterkey null pointer" error
- **Key Generation**
     **Keygen(masterKey, dspId)**

     This interface is used to generate the public and private key pair corresponding to the DSP.

     Parameter Description:
            ::

              masterKey:  master key
              dspId:      the identifier of the corresponding DSP

     Error Return: nil or "parameter null pointer" error
- **System Public Key Generation**
     **SystemKeygen(pki)**

     This interface is used to generate the system public key corresponding to a DSP.

     Parameter Description:
            ::

              pki:  a slice which is composed of the public keys corresponding to a DSP of all the media

     Error Return: nil or "parameter null pointer" error or "allocation space" error
- **Hash Generation**
     **HashToG1(did)**

     This interface is used to convert did into an element on group G1.

     Parameter Description:
            ::

              did:  the did string to be hashed
     
- **Blinding**
     **Blind(seed, did)** 

     This interface is used to generate the blinded result of the did and return a random number used for blinding.

     Parameter Description:
            ::

              seed:    random number seed
              did:     the did string to be blinded

     Error Return: nil or "parameter null pointer" error
- **Encryption**
     **Enc(message)**

     This interface is used to encrypt received messages.

     Parameter Description:
            ::
            
              message: the received message, here it is usually the blinded did

     Error Return: nil or "parameter null pointer" error or "parameter format" error
- **Unblinding**
     **Unblind(randVal, cipheri)**

     This interface is used to merge the received message ciphertext and remove the random number to obtain the final ciphertext.

     Parameter Description:
            ::

              randVal:  the random number used for blinding
              cipheri:  the blinded did ciphertext received from various media

     Error Return: nil or "parameter null pointer" error or "allocation space" error
- **Verification**
     **Verify(cipheri, pki, cipher, dids, ranVal)**

     This interface is used to verify whether the received ciphertext is correct.

     Parameter Description:
            ::
            
              cipheri:    a slice which is composed of strings which are the concatenation of one or multiple blinded did ciphertexts received from various media
              pki:        public key of each media
              cipher:     a slice composed of multiple merged and unblinded did ciphertexts
              dids:       a slice composed of multiple did strings
              randVal:    a slice composed of random numbers used for blinding

     Note：cipheri and pki have a one-to-one correspondence with each digit and are bound to the same medium; dids and randVal have a one-to-one correspondence with each digit, and randVal stores the random number used for blinding the corresponding position of dids.

     Returned Parameters：int int error 
          ::

           The first parameter represents the opposite number of the cheating medium number, which is 2 when there is no cheating, 1 for space errors, and 0 for null pointer errors.

           The second parameter represents the opposite of the cheating DID number, which is 2 when there is no cheating, 1 for space errors, and 0 for null pointer errors.

     Error Return: nil or "parameter null pointer" error or "allocation space" error

**GO Demo**
^^^^^^^^^^^^^

.. code-block:: go

      // This demo simulates the situation where Bytedance communicates with a DSP.
      // In this demo, the DSP is client and is responsible for blinding, unblinding
      // and verifying. Bytedance is server and is responsible for encryption.

      func main() {
            var dspID,did,did1,randVal,M,bt string
            dspID="1234567890"
            did="123456789012345"
            did1="123456789054321"

            // Bytedance generates keys.
            seed,_:=core.SeedGen()
            masterKeyBT,_:=core.MasterKeyGen(seed)
            keyPairBT,_:=core.Keygen(masterKeyBT,dspID)

            // DSP generates the system key.
            var pki []core.Group
            pki=append(pki,keyPairBT.PK)
            sysPk,_:=core.SystemKeygen(pki)

            // Generate instances of client and server.
            clt:=core.NewClientFromInput(sysPk)
            sevBT:=core.NewSeverFromInput(keyPairBT.SK)

            // DSP conducts blinding operation.
            seed,_=core.SeedGen()
            randVal,M,_=clt.Blind(seed,did)
            seed,_=core.SeedGen()  
            randVal1,M1,_:=clt.Blind(seed,did1)
            
            // Bytedance encrypts the received messages.
            cipherBT1,_:=sevBT.Enc(M)
            cipherBT2,_:=sevBT.Enc(M1)

            // DSP unblinds the received ciphers.
            var cipheri []string
            cipheri=append(cipheri,cipherBT1)
            bt,_=clt.Unblind(randVal,cipheri)
            cipheri=cipheri[0:0]
            cipheri=append(cipheri,cipherBT2)
            bt1,_:=clt.Unblind(randVal1,cipheri)
            
            cipheri=cipheri[0:0]
            cipheri=append(cipheri,cipherBT1+cipherBT2)

            // Verifying.
            var cipher []string
            cipher=append(cipher,bt)
            cipher=append(cipher,bt1)
            var dids []string
            dids=append(dids,did)
            dids=append(dids,did1)
            var randVals []string
            randVals=append(randVals,randVal)
            randVals=append(randVals,randVal1)
            result,result1,_:=clt.Verify(cipheri,pki,cipher,dids,randVals)
            if result == 2 {
                  fmt.Println("no one cheat!")
            } else if result == 0 || result == 1 {
                  fmt.Println("verify error!")
            } else {
                  fmt.Printf("No.%d medium cheat on %dth did!\n", -result, -result1)
            }

      }

**Benchmark**
^^^^^^^^^^^^^^
::

 MasterKeyGen 0.22ms
 Keygen 0.76ms

The following table shows the calculation overhead of each module as the number of media changes, where - means no change. (unit: ms)

+--------------+------+------+------+------+------+------+------+------+
| numofmedia   | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    |
+--------------+------+------+------+------+------+------+------+------+
| SystemKeygen | 0.12 | 0.15 | 0.19 | 0.23 | 0.27 | 0.30 | 0.34 | 0.37 |
+--------------+------+------+------+------+------+------+------+------+
| Blind        | 0.51 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+
| Enc          | 0.23 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+
| Unblind      | 0.30 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+
| Verify       | 1.96 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+


The following table shows the calculation overhead of each module as the number of dids changes, where - means no change. (unit: ms)

Here blinding, encryption, and unblinding can be performed in multiple threads, which is not used in the test.

+--------------+------+------+------+------+------+-------+-------+-------+
| numofdid     | 1    | 10   | 50   | 100  | 150  | 200   | 250   | 300   |
+--------------+------+------+------+------+------+-------+-------+-------+
| SystemKeygen | 0.09 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -` | `\ -` | `\ -` |
+--------------+------+------+------+------+------+-------+-------+-------+
| Blind        | 0.51 | 5.1  | 23.53| 51   | 78.87| 101.82| 127.51| 156.11|
+--------------+------+------+------+------+------+-------+-------+-------+
| Enc          | 0.23 | 2.26 | 11.50| 23.39| 34.62| 45.9  | 57.51 | 69.25 |
+--------------+------+------+------+------+------+-------+-------+-------+
| Unblind      | 0.30 | 2.8  | 13.68| 27.89| 42.03| 55.05 | 68.16 | 84.15 |
+--------------+------+------+------+------+------+-------+-------+-------+
| Verify       | 1.96 | 2.45 | 4.25 | 7    | 9.53 | 11.71 | 14.24 | 17.14 |
+--------------+------+------+------+------+------+-------+-------+-------+


In order to facilitate the secondary development of other language interfaces, the relevant description of the underlying C language interface is given below.

**C Interface Description**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- **Hash Generation**
     **HASHIT(hashstring, m)**

     This interface is used to convert m into the serialized string hashstring corresponding to the element on the group G1.

     Parameter Description:
            ::

              m:           the string to be hashed
              hashstring:  serialized string

- **Master Key Generation**
      **MasterKeygen(ran, masterkey)**

     This interface is used to randomly generate a 512-bit master key, which is used to subsequently derive the corresponding DSP public-private key pair.

     Parameter Description:
            ::

              ran:        random number seed
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

- **盲化**
     **Blinding(did, seed, betastring, Mstring)**

     This interface is used to generate random numbers for blinding and the blinded did.

     Parameter Description:
            ::

              seed:          random number seed
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

- **Individual Verification**
     **verify_individual(btistring, pkig1string, pkig2string, did, numofmedia, betastring)**

     This interface is used to individually verify whether the ciphertext received from each media party is correct.

     Parameter Description:
            ::
            
              btistring:                  an array of encrypted strings from various media
              pkig1string/pkig2string:    an array of serialized strings corresponding to the public keys of each medium
              did:                        did plaintext string
              numofmedia:                 the number of participating media
              betastring:                 the serialized string corresponding to the random number used for blinding

     Note：btistring和pkig1string/pkig2string每一位一一对应，绑定在同一个媒体上。

     Error Return: success or "parameter null pointer" error or "allocation space" error or the opposite of the cheating media number

- **Batch Verification**
     **batch_verify(btstring, did, sysg2string, numofdid)**

     This interface is used to batch verify whether the ciphertext after deblinding is correct.

     Parameter Description:
            ::

              btstring:       an array of multiple unblinded ciphertext strings of dids
              did:            an array of multiple did strings
              sysg2string:    the serialized string corresponding to the system public key on G2
              numofdid:       the number of dids

     Note: There is a one-to-one correspondence between btstring and did.

     Error Return: success or "parameter null pointer" error or "allocation space" error

**C Demo**
^^^^^^^^^^^^^

.. code-block:: c

     // This demo simulates the situation where Bytedance communicates with a DSP.
     // In this demo, the DSP is client and is responsible for blinding, unblinding
     // and verifying. Bytedance is server and is responsible for encryption.
     int main(){
          char did[16]="123456789012345";
          char did1[16]="123456789054321";
          char dspID[11]="1234567890";
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
          unsigned long ran = 0;
          int fd,result;

          // Generate random number seed
          if ((fd = open("/dev/random", O_RDONLY)) > 0)
          {
               read(fd, &ran, sizeof(ran));
          }

          // Bytedance generates keys.
          MasterKeygen(ran,masterkey);
          Keygen(masterkey,dspID,pkg1string,pkg2string,skstring);

          // DSP generates the system key.
          pkig1string[0]=pkg1string;
          pkig2string[0]=pkg2string;
          System_Keygen(pkig1string,pkig2string,1,sysg1string,sysg2string);

          // DSP conducts blinding operation.
          read(fd, &ran, sizeof(ran));
          Blinding(did,ran,betastring,Mstring);
          read(fd, &ran, sizeof(ran));
          close(fd);
          Blinding(did1,ran,betastring1,Mstring1);

          // Bytedance encrypts the received messages.
          Enc(skstring,Mstring,cipherstring);
          Enc(skstring,Mstring1,cipherstring1);

          // DSP unblinds the received ciphers.
          cipherstrings[0]=cipherstring;
          Unblinding(cipherstrings,1,betastring,sysg1string,cipher);
          cipherstrings[0]=cipherstring1;
          Unblinding(cipherstrings,1,betastring1,sysg1string,cipher1);

          ciphers[0]=cipher;
          ciphers[1]=cipher1;
          dids[0]=did;
          dids[1]=did1;

          // Verifying
          if(batch_verify(ciphers,dids,sysg2string,2)!=SUCCESS){
               cipherstrings[0]=cipherstring;
               result=verify_individual(cipherstrings,pkig1string,pkig2string,did,1,betastring);
               if(result<0)
                    printf("No.%d medium cheat on 1th did!\n",-result);

               cipherstrings[0]=cipherstring1;
               result=verify_individual(cipherstrings,pkig1string,pkig2string,did1,1,betastring1);
               if(result<0)
                    printf("No.%d medium cheat on 2th did!\n",-result);
          }
          else{
               printf("no one cheat!\n");
          }
     }