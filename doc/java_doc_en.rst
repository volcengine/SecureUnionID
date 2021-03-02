**SecureUnionID JAVA Interface and Usage Description**
=======================================
**JAVA Interface Description**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The interface is implemented in the class `SecureUnionID`. This class calls the low level interface of C language.
So you should load the `SecureUnionID` library as the following:

System.loadLibrary("SecureUnionID");

- **Master Key Generation**
      **int MasterKeygen(long seed, byte[] masterKey)**

     This interface is used to randomly generate a 512-bit master key.

     Parameter Description:
            ::

              seed:        random number seed
              masterKey:   generated master key

     Error Return: 0 means success; less than 0 means failure

- **Key Generation**
     **int Keygen(byte[] masterKey, String dspId, byte[] publicKeyOfG1, byte[] publicKeyOfG2, byte[] privateKey)**

    This interface is used to generate the public and private key pair corresponding to the DSP.

     Parameter Description:
            ::

              masterKey:              the master key
              dspId:                  the identifier of the corresponding DSP
              publicKeyOfG1           the generated public key on group G1
              publicKeyOfG2           the generated public key on group G2
              privateKey:             the generated private key

     Error Return: 0 means success; less than 0 means failure

- **System Public Key Generation**
     **int System_Keygen(String[] publicKeyOfG1Array, String[] publicKeyOfG2Array, int numofMedia, byte[] systemKeyOfG1, byte[] systemKeyOfG2)**

    Parameter Description:
            ::

              publicKeyOfG1Array    an array of the public keys of a DSP of all the media on group G1
              publicKeyOfG2Array:   an array of the public keys of a DSP of all the media on group G2
              numofMedia:           the number of participating media
              systemKeyOfG1:        the system public key of the DSP on group G1
              systemKeyOfG2:        the system public key of the DSP on group G2

     Error return: 0 means success; less than 0 means failure
     
- **Blinding**
     **int Blinding(String deviceId, long seed, byte[] betaValue, byte[] blindResult)**

     This interface is used to blind the device identifier using the random seed.

     Parameter Description:
            ::

              deviceId:           the device identifier to be blinded
              seed:               the random number seed  
              betaValue:          the serialized string corresponding to the random number used for blinding
              blindResult:        the serialized string corresponding to the blinded result

     Error Return: 0 means success; less than 0 means failure

- **Encryption**
     **int Enc(byte[] privateKey, byte[] plainText, byte[] cipherText)**

     This interface is used to encrypt strings.

     Parameter Description:
            ::

              privateKey:    the private key
              plainText:     the string to be encrypted
              cipherText:    the encrypted string

     Error Return: 0 means success; less than 0 means failure

- **Unblinding**
     **int Unblinding(String[] cipherTextArray, int numofMedia, byte[]  betaValue, byte[] systemKeyOfG1, byte[] unblindCipherText)**

     This interface is used to merge the received message ciphertext and remove the random number to obtain the final ciphertext.

     Parameter Description:
            ::

              cipherTextArray:    an array of encrypted strings from various media 
              numofMedia:         the number of participating media
              betaValue:          the serialized string corresponding to the random number used for blinding
              systemKeyOfG1:      the serialized string corresponding to the public key on group G1
              unblindCipherText:  the final encrypted string

     Error Return: 0 means success; less than 0 means failure

- **Individual Verification**
     **int verify_individual(String[] cipherTextArray, String[] publicKeyOfG1Array, String[] publicKeyOfG2Array, String deviceId, int numofMedia, byte[] betaValue)**

     This interface is used to individually verify whether the ciphertext received from each media party is correct.

     Parameter Description:
            ::
            
              cipherTextArray:            an array of encrypted strings from various media
              publicKeyOfG1Array          an array of the public keys on group G1 
              publicKeyOfG2Array          an array of the public keys on group G2
              deviceId:                   the device identifier
              numofMedia:                 the number of participating media
              betaValue:                  the serialized string corresponding to the random number used for blinding

     Error Return: 0 means success; less than 0 means failure; greater than 0 means the cheating media number

- **Batch Verification**
     **int batch_verify(String[] unblindCipherArray, String[] allDeviceIds, byte[] systemKeyOfG2, int numofDeviceIds)****

     This interface is used to batch verify whether the ciphertext after deblinding is correct.

     Parameter Description:
            ::

              unblindCipherArray:       an array of multiple unblinded ciphertext strings of device identifiers
              allDeviceIds:             an array of multiple device identifiers
              systemKeyOfG2:            the system public key on group G2
              numofDeviceIds:           the number of device identifiers

     Error Return: 0 means success; less than 0 means failure


**example**
^^^^^^^^^^

.. code-block:: java

     /// This demo simulates the situation where Bytedance communicates with a DSP.
     // In this demo, the DSP is client and is responsible for blinding, unblinding
     // and verifying. Bytedance is server and is responsible for encryption.
     class Demo {
          // master key length.
          public static final int MASTER_KEY_LEN = 64;
          // private key length.
          public static final int PRIVATE_KEY_LEN = 32;

          public static final int G1_LEN = 33;
          public static final int G2_LEN = 128;

          // the length of public key on G1 group
          public static final int PUBKEY_G1_LEN = G1_LEN * 2 + 1;
          // the length of public key on G2 group
          public static final int PUBKEY_G2_LEN = G2_LEN * 2 + 1;

          public static final int SUCCESS = 2;

          public static void main(String[] args) {
               // load the library libsecureunionid.so. It should be in the java library path
          System.loadLibrary("SecureUnionID");

          SecureUnionID secureUnionID = new SecureUnionID();

          // generate masterkey.
          // TODO: the first random seed coulde be generated by wrapping C lower code.
          byte masterKey[] = new byte[MASTER_KEY_LEN];

          System.out.println("--------------------------------------------------");
          System.out.println("Step 0: generate master key.");

          long randSeed = secureUnionID.randomSeed();
          //System.out.printf("random seed is %d\n", randSeed);
          int r = secureUnionID.MasterKeygen(randSeed, masterKey);
          if (r != 0) {
               System.out.printf("generate master key error %d\n", r);
               return;
          }
          System.out.printf("masterKey is: %s\n", bytesToHex(masterKey));

          String dspID = "1234567890";

          // generate public/private keys.
          System.out.println("--------------------------------------------------");
          System.out.println("Step 1: generate public key and private key");
          byte publicKeyOfG1[] = new byte[PUBKEY_G1_LEN];
          byte publicKeyOfG2[] = new byte[PUBKEY_G2_LEN];
          byte privateKey[] = new byte[PRIVATE_KEY_LEN];
          r = secureUnionID.Keygen(masterKey, dspID, publicKeyOfG1, publicKeyOfG2, privateKey);
          if (r != 0) {
               System.out.printf("generate key error %d\n", r);
               return;
          }

          System.out.printf("publicKey of G1: is %s\n", bytesToHex(publicKeyOfG1));
          System.out.printf("publicKey of G2: is %s\n", bytesToHex(publicKeyOfG2));

          // generate system keys.
          System.out.println("--------------------------------------------------");
          System.out.println("Step 2: generate the system key");
          String publicKeyOfG1Array[] = new String[1];
          String publicKeyOfG2Array[] = new String[1];
          publicKeyOfG1Array[0] = new String(publicKeyOfG1);
          publicKeyOfG2Array[0] = new String(publicKeyOfG2);
          byte systemKeyOfG1[] = new byte[PUBKEY_G1_LEN];
          byte systemKeyOfG2[] = new byte[PUBKEY_G2_LEN];
          r = secureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array, 1, systemKeyOfG1, systemKeyOfG2);
          if (r != 0) {
               System.out.printf("generate system key error %d\n", r);
               return;
          }

          // the device id number to protect.
          String deviceId0 = "123456789012345";
          String deviceId1 = "123456789054321";

          // blinding.
          System.out.println("--------------------------------------------------");
          System.out.println("Step 3: blind");
          byte betaValue0[] = new byte[2 * PRIVATE_KEY_LEN + 1];
          byte blindResult0[] = new byte[PUBKEY_G1_LEN];
          randSeed = secureUnionID.randomSeed();
          r = secureUnionID.Blinding(deviceId0, randSeed, betaValue0, blindResult0);
          if (r != 0) {
               System.out.printf("blinding error %d\n", r);
               return;
          }
          System.out.printf("blind result for device id 0: %s\n", bytesToHex(blindResult0));

          byte betaValue1[] = new byte[2 * PRIVATE_KEY_LEN + 1];
          byte blindResult1[] = new byte[PUBKEY_G1_LEN];
          randSeed = secureUnionID.randomSeed();
          r = secureUnionID.Blinding(deviceId1, randSeed, betaValue1, blindResult1);
          if (r != 0) {
               System.out.printf("blinding error %d\n", r);
               return;
          }
          System.out.printf("blind result for device id 1: %s\n", bytesToHex(blindResult1));

          // encryption
          System.out.println("--------------------------------------------------");
          System.out.println("Step 4: encrypt");
          byte cipherText0[] = new byte[PUBKEY_G1_LEN];
          r = secureUnionID.Enc(privateKey, blindResult0, cipherText0);
          if (r != 0) {
               System.out.printf("encrypt error %d\n", r);
               return;
          }
          System.out.printf("encrypt result for device id 0: %s\n", bytesToHex(blindResult0));

          byte cipherText1[] = new byte[PUBKEY_G1_LEN];
          r = secureUnionID.Enc(privateKey, blindResult1, cipherText1);
          if (r != 0) {
               System.out.printf("encrypt error %d\n", r);
               return;
          }
          System.out.printf("encrypt result for device id 1: %s\n", bytesToHex(blindResult1));

          // unblinding
          System.out.println("--------------------------------------------------");
          System.out.println("Step 5: unblind");
          byte unblindCipherText0[] = new byte[PUBKEY_G1_LEN];
          String[] cipherTextArray = new String[1];
          cipherTextArray[0] = new String(cipherText0);
          r = secureUnionID.Unblinding(cipherTextArray, 1, betaValue0, systemKeyOfG1, unblindCipherText0);
          if (r != 0) {
               System.out.printf("unblinding error %d\n", r);
               return;
          }
          System.out.printf("unblind result for device id 0: %s\n", bytesToHex(unblindCipherText0));


          byte unblindCipherText1[] = new byte[PUBKEY_G1_LEN];
          cipherTextArray[0] = new String(cipherText1);
          r = secureUnionID.Unblinding(cipherTextArray, 1, betaValue1, systemKeyOfG1, unblindCipherText1);
          if (r != 0) {
               System.out.printf("unblinding error %d\n", r);
               return;
          }
          System.out.printf("unblind result for device id 1: %s\n", bytesToHex(unblindCipherText1));

          System.out.println("--------------------------------------------------");
          System.out.println("Step 6: verify");
          String unblindCipherArray[] = new String[2];
          unblindCipherArray[0] = new String(unblindCipherText0);
          unblindCipherArray[1] = new String(unblindCipherText1);

          String allDeviceIds[] = new String[2];
          allDeviceIds[0] = deviceId0;
          allDeviceIds[1] = deviceId1;
          // verify
          r = secureUnionID.batch_verify(unblindCipherArray, allDeviceIds, systemKeyOfG2, 2);

          if (r != 0) {
               cipherTextArray[0] = new String(cipherText0);
               int result = secureUnionID.verify_individual(cipherTextArray, publicKeyOfG1Array, publicKeyOfG2Array, deviceId0, 1, betaValue0);
               if (result != 0) {
                    System.out.println("Cheat on the first device id!\n");
               }
               else {
                    System.out.println("Cheat on the second device id!\n");
               }
          }
          else {
               System.out.println("Success!\n");
          }
       }
    }