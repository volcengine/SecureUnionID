**SecureUnionID Go Interface Description and Performance Evaluation**
========================================================================
**Go Interface Description**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- **Random Number Seed Generation**
     **SeedGen()**

     This interface is used to generate and return a random number seed of unit32 type, which is used for subsequent random number generation.

     Error Return: nil or "file open failure" error or "binary read" error
- **Random Number Seed Generation**
     **RandomSeed()**

     This interface is used to generate and return a random number seed of [64]byte type, which is used for subsequent random number generation.

     Error Return: nil or "generate random error" error
- **Master Key Generation**
     **MasterKeyGen(seed)**

     This interface is used to randomly generate a 512-bit master key, which is used to subsequently derive the corresponding DSP public-private key pair.

     Parameter Description:
          ::

               seed: random number seed

     Error Return: nil or "masterkey null pointer" error
- **Master Key Generation**
     **GenMasterKey(seed)**

     This interface is used to randomly generate a 512-bit master key, which is used to subsequently derive the corresponding DSP public-private key pair.

     Parameter Description:
          ::

               seed: random number seed(64 byte)

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
- **Blinding**
     **Blindv2(seed, did)** 

     This interface is used to generate the blinded result of the did and return a random number used for blinding.

     Parameter Description:
            ::

              seed:    random number seed (64 byte)
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

**GO Demo**
^^^^^^^^^^^^^

.. code-block:: go

      // This demo simulates the situation where Bytedance communicates with a DSP.
      // In this demo, the DSP is client and is responsible for blinding, unblinding
      // and verifying. Bytedance is server and is responsible for encryption.

      func main() {
            var dspID,did,did1,randVal,M,bt string
            dspID = "1234567890"
            did = "123456789012345"
            did1 = "123456789054321"

            // Bytedance generates keys.
            seed,_ := core.RandomSeed()
            masterKeyBT,_ := core.GenMasterKey(seed)
            keyPairBT,_ := core.Keygen(masterKeyBT,dspID)

            // DSP generates the system key.
            var pki []core.Group
            pki = append(pki,keyPairBT.PK)
            sysPk,_ := core.SystemKeygen(pki)

            // Generate instances of client and server.
            clt := core.NewClientFromInput(sysPk)
            sevBT := core.NewSeverFromInput(keyPairBT.SK)

            // DSP conducts blinding operation.
            seed,_ = core.RandomSeed()
            randVal,M,_ = clt.Blindv2(seed,did)
            seed,_ = core.RandomSeed()  
            randVal1,M1,_ := clt.Blindv2(seed,did1)
            
            // Bytedance encrypts the received messages.
            cipherBT1,_ := sevBT.Enc(M)
            cipherBT2,_ := sevBT.Enc(M1)

            // DSP unblinds the received ciphers.
            var cipheri []string
            cipheri = append(cipheri,cipherBT1)
            bt,_ = clt.Unblind(randVal,cipheri)
            cipheri = cipheri[0:0]
            cipheri = append(cipheri,cipherBT2)
            bt1,_ := clt.Unblind(randVal1,cipheri)
            
            cipheri = cipheri[0:0]
            cipheri = append(cipheri,cipherBT1+cipherBT2)

      }

**Benchmark**
^^^^^^^^^^^^^^
::

 MasterKeyGen 0.28ms
 Keygen 0.73ms

The following table shows the calculation overhead of each module as the number of media changes, where - means no change. (unit: ms)

+--------------+------+------+------+------+------+------+------+------+
| numofmedia   | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    |
+--------------+------+------+------+------+------+------+------+------+
| SystemKeygen | 0.11 | 0.15 | 0.19 | 0.22 | 0.26 | 0.30 | 0.32 | 0.37 |
+--------------+------+------+------+------+------+------+------+------+
| Blindv2      | 0.61 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+
| Enc          | 0.23 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+
| Unblind      | 0.29 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+


The following table shows the calculation overhead of each module as the number of dids changes, where - means no change. (unit: ms)

Here blinding, encryption, and unblinding can be performed in multiple threads, which is not used in the test.

+--------------+------+------+------+------+------+-------+-------+-------+
| numofdid     | 1    | 10   | 50   | 100  | 150  | 200   | 250   | 300   |
+--------------+------+------+------+------+------+-------+-------+-------+
| SystemKeygen | 0.11 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -` | `\ -` | `\ -` |
+--------------+------+------+------+------+------+-------+-------+-------+
| Blindv2      | 0.61 | 6.30 | 30.99| 61.67| 92.00| 120.23| 150.57| 185.70|
+--------------+------+------+------+------+------+-------+-------+-------+
| Enc          | 0.23 | 2.26 | 11.29| 24.00| 34.38| 44.68 | 55.61 | 67.32 |
+--------------+------+------+------+------+------+-------+-------+-------+
| Unblind      | 0.29 | 2.94 | 15.03| 30.03| 44.71| 60.01 | 74.20 | 89.56 |
+--------------+------+------+------+------+------+-------+-------+-------+