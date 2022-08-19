**SecureUnionID Python Interface and Usage Description**
=======================================
**Python Interface Description**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The interface is implemented in the `SecureUnionID.py`. It calls the low level interface of C language.
So you should `import SecureUnionID` first

- **Random Number generator**
      **rnd = SecureUnionID.genRandSeed()**

     This interface is used to generate 64 byte random number.

     Return Result Description:
            ::

              rnd[0]   length of generated random number（in Byte）
              rnd[1]   geneated random number

- **Master Key Generation**
      **masterKey = SecureUnionID.genMasterKey(seed)**

     This interface is used to randomly generate a 512-bit master key.

     Parameter Description:
            ::

              seed:        random number seed (64 byte)
     
     Return Result Description:
            ::

              masterKey[0]:   success or not, 2 means success; others means failure
              masterKey[1]:   generated master key

- **Master Key Generation**
      **masterKey = SecureUnionID.MasterKeygen(seed)**

     This interface is used to randomly generate a 512-bit master key.

     Parameter Description:
            ::

              seed:        random number seed
     
     Return Result Description:
            ::

              masterKey[0]:   success or not, 2 means success; others means failure
              masterKey[1]:   generated master key


- **Key Generation**
     **keys = SecureUnionID.Keygen(masterKey, dspId)**

    This interface is used to generate the public and private key pair corresponding to the DSP.

     Parameter Description:
            ::

              masterKey:              the master key
              dspId:                  the identifier of the corresponding DSP

     Return Result Description:
            ::

              keys[0]:                 success or not, 2 means success; others means failure
              keys[1]:                 the generated public key on group G1
              keys[2]:                 the generated public key on group G2
              keys[3]:                 the generated private key


- **System Public Key Generation**
      **systemKey = SecureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array , numofMedia)**

    Parameter Description:
            ::

              publicKeyOfG1Array    an array of the public keys of a DSP of all the media on group G1
              publicKeyOfG2Array:   an array of the public keys of a DSP of all the media on group G2
              numofMedia:           the number of participating media
     
     Return Result Description:
            ::

              systemKey[0]:             success or not, 2 means success; others means failure
              systemKey[1]:             the system public key of the DSP on group G1
              systemKey[2]:             the system public key of the DSP on group G2

- **Blinding**
     **blind = SecureUnionID.Blind(deviceId, seed)**

     This interface is used to blind the device identifier using the random seed.

     Parameter Description:
            ::

              deviceId:           the device identifier to be blinded
              seed:               the random number seed (64 bytes)

     Return Result Description:
            ::  

              blind[0]:             success or not, 2 means success; others means failure
              blind[1]:             the serialized string corresponding to the random number used for blinding
              blind[2]:             the serialized string corresponding to the blinded result

- **Blinding**
     **blind = SecureUnionID.Blinding(deviceId, seed)**

     This interface is used to blind the device identifier using the random seed.

     Parameter Description:
            ::

              deviceId:           the device identifier to be blinded
              seed:               the random number seed

     Return Result Description:
            ::  

              blind[0]:             success or not, 2 means success; others means failure
              blind[1]:             the serialized string corresponding to the random number used for blinding
              blind[2]:             the serialized string corresponding to the blinded result


- **Encryption**
     **cipherText = SecureUnionID.Enc(privateKey, plainText)**

     This interface is used to encrypt strings.

     Parameter Description:
            ::

              privateKey:               the private key
              plainText:                the string to be encrypted

     Return Result Description:
            ::  

              cipherText[0]:              success or not, 2 means success; others means failure
              cipherText[1]:              the encrypted string


- **Unblinding**
     **unblind = SecureUnionID.Unblinding(cipherTextArray, numofMedia, betaValue, systemKeyOfG1)**

     This interface is used to merge the received message ciphertext and remove the random number to obtain the final ciphertext.

     Parameter Description:
            ::

              cipherTextArray:            an array of encrypted strings from various media 
              numofMedia:                 the number of participating media
              betaValue:                  the serialized string corresponding to the random number used for blinding
              systemKeyOfG1:              the serialized string corresponding to the public key on group G1
          
      Return Result Description:
            ::  

              unblindCipherText[0]:       success or not, 2 means success; others means failure
              unblindCipherText[1]:       the final encrypted string


- **Individual Verification**
     **result = SecureUnionID.verify_individual(cipherTextArray, publicKeyOfG1Array, publicKeyOfG2Array, deviceId, numofMedia, betaValue)**
     

     This interface is used to individually verify whether the ciphertext received from each media party is correct.

     Parameter Description:
            ::
            
              cipherTextArray:            an array of encrypted strings from various media
              publicKeyOfG1Array          an array of the public keys on group G1 
              publicKeyOfG2Array          an array of the public keys on group G2
              deviceId:                   the device identifier
              numofMedia:                 the number of participating media
              betaValue:                  the serialized string corresponding to the random number used for blinding

      Return Result Description:
            ::  

              result :       success or not, 2 means success; others means failure


- **Batch Verification**
     **result = SecureUnionID.batch_verify(unblindCipherArray, allDeviceIds, systemKeyOfG2, numofDeviceIds)**

     This interface is used to batch verify whether the ciphertext after deblinding is correct.

     Parameter Description:
            ::

              unblindCipherArray:       an array of multiple unblinded ciphertext strings of device identifiers
              allDeviceIds:             an array of multiple device identifiers
              systemKeyOfG2:            the system public key on group G2
              numofDeviceIds:           the number of device identifiers

     Return Result Description:
            ::  

              result :       success or not, 2 means success; others means failure


**example**
^^^^^^^^^^

.. code-block:: python

     /// This demo simulates the situation where Bytedance communicates with a DSP.
     // In this demo, the DSP is client and is responsible for blinding, unblinding
     // and verifying. Bytedance is server and is responsible for encryption.
    import SecureUnionID
     import random
     import sys

     print("--------------------------------------------------")
     print("Step 0: generate master key.")


     # generate random seed.
     r = SecureUnionID.genRandSeed()

     # generate master key.
     masterKey = SecureUnionID.genMasterKey(r[1])
     r = masterKey[0]
     if r != 2:
          print ("generate master key error, error number: %d" % (r))
          exit(1)
     print("OK")


     # generate public/private keys.
     print("--------------------------------------------------")
     print("Step 1: generate public key and private key")
     # generate public/private keys.
     sys.version_info[0]
     dspId = b"1234567890"
     if (sys.version_info.major == 2):
          dspId = "1234567890"

     keys = SecureUnionID.Keygen(masterKey[1], dspId)
     r = keys[0]
     if r != 2:
          print ("generate public private keys error, error number: %d" % (r))
          exit(1)
     print("OK")

     # generate system keys.
     print("--------------------------------------------------")
     print("Step 2: generate the system key")

     publicKeyOfG1Array = [keys[1]]
     publicKeyOfG2Array = [keys[2]]

     # generate system keys.
     systemKey = SecureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array , 1)
     r = systemKey[0]
     if r != 2:
          print ("generate system key error, error number: %d" % (r))
          exit(1)
     print("OK")

     # blinding.
     print("--------------------------------------------------")
     print("Step 3: blind")
     # blinding.
     plaintext = b"123456789012345"
     if (sys.version_info.major == 2):
          plaintext = "123456789012345"

     r = SecureUnionID.genRandSeed()
     blind = SecureUnionID.Blind(plaintext, r[1])
     r = blind[0]
     if r != 2:
          print ("blind error, error number: %d" % (r))
          exit(1)
     print("OK")

     # encryption
     print("--------------------------------------------------")
     print("Step 4: encrypt")
     enc = SecureUnionID.Enc(keys[3], blind[2])
     r = enc[0]
     if r != 2:
          print ("encrypt error, error number: %d" % (r))
          exit(1)
     print("OK")

     # unblinding
     print("--------------------------------------------------")
     print("Step 5: unblind")
     cipherTextArray = [enc[1]]
     unblind = SecureUnionID.Unblinding(cipherTextArray, 1, blind[1], systemKey[1])
     r = unblind[0]
     if r != 2:
          print ("unbind error, error number: %d" % (r))
          exit(1)

     print("OK")

     unblindCipherArray = [unblind[1]]
     print("--------------------------------------------------");
     print("Step 6: verify");

     allDeviceIds = [plaintext]
     r = SecureUnionID.batch_verify(unblindCipherArray, allDeviceIds, systemKey[2], 1)
     if r != 2:
          print ("verify error, error number: %d" % (r))
          exit(1)
     print("OK")