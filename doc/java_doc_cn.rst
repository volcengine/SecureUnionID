**SecureUnionID JAVA接口和使用说明**
=======================================
**JAVA接口说明**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
接口存在于类SecureUnionID中, 该类调用底层的C语言的接口，因此使用之前需要先加载SecureUnionID库。
System.loadLibrary("SecureUnionID");

- **随机数生成**
      **int randomSeed(byte[] rnd)**

      该接口生成随机数, 输出存储到rnd中。

      参数说明：
            ::
              rnd        输出随机数种子（长度为64）

      返回：生成的随机数种子长度

- **主密钥生成**
      **int MasterKeygen(byte[] seed, byte[] masterKey)**

      该接口生成主密钥。输入为随机数种子seed, 输出存储到masterKey中。

      参数说明：
            ::

              seed        随机数种子（长度为64）
              masterKey   生成的主密钥, 长度为64

      返回：0代表成功; 小于0代表失败

- **主密钥生成**
      **int MasterKeygen(long seed, byte[] masterKey)**

      该接口生成主密钥。输入为随机数种子seed, 输出存储到masterKey中。

      参数说明：
            ::

              seed        随机数种子
              masterKey   生成的主密钥, 长度为64

      错误返回：0代表成功; 小于0代表失败

- **哈希生成**
     **HASHIT(hashstring, m)**

     该接口用于将m转换为群G1上的元素对应的序列化字符串hashstring。

     参数说明：
            ::

              m           需要哈希的字符串
              hashstring  序列化哈希值

- **密钥生成**
     **int Keygen(byte[] masterKey, String dspId, byte[] publicKeyOfG1, byte[] publicKeyOfG2, byte[] privateKey);**

     该接口用于生成对应DSP的公私钥对。前两个参数对应输入; 后面三个参数对应输出。

     参数说明：
            ::

              masterKey              主密钥, 长度为64
              dspId                  对应DSP的标识符
              publicKeyOfG1          生成的在群G1的公钥, 长度为67
              publicKeyOfG2          生成的在群G2的公钥, 长度为257
              privateKey             生成的私钥, 长度为32

     错误返回：0代表成功; 小于0代表失败

- **系统公钥生成**
     **int System_Keygen(String[] publicKeyOfG1Array, String[] publicKeyOfG2Array, int numofMedia, byte[] systemKeyOfG1, byte[] systemKeyOfG2)**

     该接口用于生成某DSP对应的系统公钥。前三个参数对应输入; 后面两个参数对应输出

     参数说明：
            ::

              publicKeyOfG1Array     所有媒体对应某DSP的在群G1上的公钥
              publicKeyOfG2Array     所有媒体对应某DSP的在群G2上的公钥
              numofMedia             参与媒体的个数
              systemKeyOfG1          对应DSP的在群G1上的系统公钥, 长度为67
              systemKeyOfG2          对应DSP的在群G2上的系统公钥, 长度为257

     错误返回：0代表成功; 小于0代表失败

- **盲化**
     **int Blinding(String deviceId, long seed, byte[] betaValue, byte[] blindResult)**

     该接口用于生成用于盲化的随机数和did盲化后的结果。

     参数说明：
            ::

              deviceId      需要盲化的did字符串
              seed          随机数种子
              betaValue     盲化随机数对应的序列化字符串, 长度为65
              blindResult   盲化后的结果, 长度为67

     错误返回：0代表成功; 小于0代表失败

- **盲化**
     **int Blind(String deviceId, byte[] seed, byte[] betaValue, byte[] blindResult)**

     该接口用于生成用于盲化的随机数和did盲化后的结果。

     参数说明：
            ::

              deviceId      需要盲化的did字符串
              seed          随机数种子（长度为64字节）
              betaValue     盲化随机数对应的序列化字符串, 长度为65
              blindResult   盲化后的结果, 长度为67

     错误返回：0代表成功; 小于0代表失败

- **加密**
     **int Enc(byte[] privateKey, byte[] plainText, byte[] cipherText)**

     该接口用于加密字符串。

     参数说明：
            ::

              privateKey    私钥, 长度为32
              plainText     要加密的字符串, 一般为哈希后的设备ID或盲化后的设备ID, 长度为67
              cipherText    加密后的密文, 长度为67

     错误返回：0代表成功; 小于0代表失败

- **去盲化**
     **int Unblinding(String[] cipherTextArray, int numofMedia, byte[]  betaValue, byte[] systemKeyOfG1, byte[] unblindCipherText)**

     该接口用于将收到的消息密文合并，并去掉随机数，得到最终的密文。

     参数说明：
            ::

                  cipherTextArray    来自各个媒体方的加密字符串组成的数组
                  numofMedia         参与的媒体个数
                  betaValue          当时盲化所用的随机数对应的序列化字符串
                  systemKeyOfG1      在G1上的系统公钥, 长度为67
                  unblindCipherText  最终的的加密的字符串, 长度为67

     错误返回：0代表成功; 小于0代表失败

**示例**
^^^^^^^^^^

.. code-block:: java

     // 本样例模拟字节和一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证
     // 字节为服务端，负责加密
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
          byte randSeed [] = new byte[MASTER_KEY_LEN];
          byte masterKey[] = new byte[MASTER_KEY_LEN];

          System.out.println("--------------------------------------------------");
          System.out.println("Step 0: generate master key.");

          int r = secureUnionID.randomSeed(randSeed);
          //System.out.printf("random seed is %d\n", randSeed);
          r = secureUnionID.MasterKeygen(randSeed, masterKey);
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
          r = secureUnionID.randomSeed(randSeed);
          r = secureUnionID.Blind(deviceId0, randSeed, betaValue0, blindResult0);
          if (r != 0) {
               System.out.printf("blinding error %d\n", r);
               return;
          }
          System.out.printf("blind result for device id 0: %s\n", bytesToHex(blindResult0));

          byte betaValue1[] = new byte[2 * PRIVATE_KEY_LEN + 1];
          byte blindResult1[] = new byte[PUBKEY_G1_LEN];
          r = secureUnionID.randomSeed(randSeed);
          r = secureUnionID.Blind(deviceId1, randSeed, betaValue1, blindResult1);
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
    }