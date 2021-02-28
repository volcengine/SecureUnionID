**SecureUnionID C接口和使用说明**
=======================================
**C接口说明**
^^^^^^^^^^^^^
- **哈希生成**
     **HASHIT(hashstring, m)**

     该接口用于将m转换为群G1上的元素对应的序列化字符串hashstring。

     参数说明：
            ::

              m           需要哈希的字符串
              hashstring  序列化字符串

- **主密钥生成**
      **MasterKeygen(ran, masterkey)**

      该接口用于随机生成512bit的主密钥，该密钥用于后续派生对应DSP的公私钥对。

      参数说明：
            ::

              ran        随机数种子
              masterkey  生成的主密钥

      错误返回：成功 或 “masterkey空指针”错误

- **密钥生成**
     **Keygen(masterKey, dspId, pkg1string, pkg2string, skstring)**

     该接口用于生成对应DSP的公私钥对。

     参数说明：
            ::

              masterKey              主密钥
              dspId                  对应DSP的标识符
              pkg1string/pkg2string  生成的在群G1和G2上的两个公钥对应的序列化字符串
              skstring               生成的私钥对应的序列化字符串

     错误返回：成功 或 “参数空指针”错误

- **系统公钥生成**
     **System_Keygen(pkig1string, pkig2string, numofmedia, sysg1string, sysg2string)**

     该接口用于生成某DSP对应的系统公钥。

     参数说明：
            ::

              pkig1string/pkig2string   所有媒体对应某DSP的公钥对应的序列化字符串组成的数组
              numofmedia                参与媒体的个数
              sysg1string/sysg2string   对应DSP的系统公钥对应的序列化字符串

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误

- **盲化**
     **Blinding(did, seed, betastring, Mstring)**

     该接口用于生成用于盲化的随机数和did盲化后的结果。

     参数说明：
            ::

              seed          随机数种子
              did           需要盲化的did字符串
              betastring    盲化随机数对应的序列化字符串
              Mstring       盲化后结果对应的序列化字符串

     错误返回：成功 或 “参数空指针”错误

- **加密**
     **Enc(skstring, Mstring, btistring)**

     该接口用于加密字符串。

     参数说明：
            ::

              skstring    私钥对应的序列化字符串
              Mstring     要加密的字符串，一般为哈希后的did或盲化后的did
              btistring   加密后的字符串

     错误返回：成功 或 “参数空指针”错误

- **去盲化**
     **Unblinding(btistring, numofmedia, betastring, sysg1string, btstring)**

     该接口用于将收到的消息密文合并，并去掉随机数，得到最终的密文。

     参数说明：
            ::

                  btistring    来自各个媒体方的加密字符串组成的数组
                  numofmedia   参与的媒体个数
                  betastring   当时盲化所用的随机数对应的序列化字符串
                  sysg1string  在G1上的公钥对应的序列化字符串
                  btstring     最终的加密字符串

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误

- **单独验证**
     **verify_individual(btistring, pkig1string, pkig2string, did, numofmedia, betastring)**

     该接口用于单独验证接收到来自各个媒体方的密文是否正确。

     参数说明：
            ::
            
              btistring                  来自各个媒体方的加密字符串组成的数组
              pkig1string/pkig2string    各个媒体方公钥对应的序列化字符串组成的数组
              did                        did明文字符串
              numofmedia                 参与的媒体个数
              betastring                 当时盲化所用的随机数对应的序列化字符串

     Note：btistring和pkig1string/pkig2string每一位一一对应，绑定在同一个媒体上。

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误 或 作弊媒体编号的相反数

- **批量验证**
     **batch_verify(btstring, did, sysg2string, numofdid)**

     该接口用于批量验证去盲后的密文是否正确。

     参数说明：
            ::

              btstring       多个去盲后的did密文字符串组成的数组
              did            多个did字符串组成的数组
              sysg2string    在G2上的系统公钥对应的序列化字符串
              numofdid       did的个数

     Note：btstring和did每一位一一对应。

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误

**C样例**
^^^^^^^^^^

.. code-block:: c

      // 本样例模拟字节和一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证
      // 字节为服务端，负责加密
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
          unsigned long ran = 0;
          int fd,result;

          //生成随机数种子
          if ((fd = open("/dev/random", O_RDONLY)) > 0)
          {
               read(fd, &ran, sizeof(ran));
          }

          //字节生成主密钥和公私钥对
          MasterKeygen(ran,masterkey);
          Keygen(masterkey,dspID,pkg1string,pkg2string,skstring);

          //DSP生成系统参数
          pkig1string[0] = pkg1string;
          pkig2string[0] = pkg2string;
          System_Keygen(pkig1string,pkig2string,1,sysg1string,sysg2string);

          //DSP进行盲化
          read(fd, &ran, sizeof(ran));
          Blinding(did,ran,betastring,Mstring);
          read(fd, &ran, sizeof(ran));
          close(fd);
          Blinding(did1,ran,betastring1,Mstring1);

          //字节加密
          Enc(skstring,Mstring,cipherstring);
          Enc(skstring,Mstring1,cipherstring1);

          //DSP进行去盲化
          cipherstrings[0] = cipherstring;
          Unblinding(cipherstrings,1,betastring,sysg1string,cipher);
          cipherstrings[0] = cipherstring1;
          Unblinding(cipherstrings,1,betastring1,sysg1string,cipher1);

          ciphers[0] = cipher;
          ciphers[1] = cipher1;
          dids[0] = did;
          dids[1] = did1;

          //验证
          if(batch_verify(ciphers,dids,sysg2string,2) != SUCCESS){
               cipherstrings[0] = cipherstring;
               result = verify_individual(cipherstrings,pkig1string,pkig2string,did,1,betastring);
               if(result<0)
                    printf("No.%d media cheat on 1th did!\n",-result);

               cipherstrings[0] = cipherstring1;
               result = verify_individual(cipherstrings,pkig1string,pkig2string,did1,1,betastring1);
               if(result<0)
                    printf("No.%d media cheat on 2th did!\n",-result);
          }
          else{
               printf("no one cheat!\n");
          }
     }

**SecureUnionID JAVA接口和使用说明**
=======================================
**JAVA接口说明**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
接口存在于类SecureUnionID中, 该类调用底层的C语言的接口，因此使用之前需要先加载SecureUnionID库。
System.loadLibrary("SecureUnionID");

- **主密钥生成**
      **int MasterKeygen(long seed, byte[] masterKey)**

      该接口生成主密钥。输入为随机数种子seed, 输出存储到masterKey中。

      参数说明：
            ::

              seed        随机数种子
              masterKey   生成的主密钥, 长度为64

      错误返回：成功 或 “masterKey空指针”错误

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

     错误返回：成功 或 “参数空指针”错误

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

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误

- **盲化**
     **int Blinding(String deviceId, long seed, byte[] betaValue, byte[] blindResult)**

     该接口用于生成用于盲化的随机数和did盲化后的结果。

     参数说明：
            ::

              deviceId      需要盲化的did字符串
              seed          随机数种子
              betaValue     盲化随机数对应的序列化字符串, 长度为65
              blindResult   盲化后的结果, 长度为67

     错误返回：成功 或 “参数空指针”错误

- **加密**
     **int Enc(byte[] privateKey, byte[] plainText, byte[] cipherText)**

     该接口用于加密字符串。

     参数说明：
            ::

              privateKey    私钥, 长度为32
              plainText     要加密的字符串, 一般为哈希后的设备ID或盲化后的设备ID, 长度为67
              cipherText    加密后的密文, 长度为67

     错误返回：成功 或 “参数空指针”错误

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

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误

- **单独验证**
     **int verify_individual(String[] cipherTextArray, String[] publicKeyOfG1Array, String[] publicKeyOfG2Array, String deviceId, int numofMedia, byte[] betaValue)**

     该接口用于单独验证接收到来自各个媒体方的密文是否正确。

     参数说明：
            ::
            
              cipherTextArray           来自各个媒体方的加密字符串组成的数组
              publicKeyOfG1Array        所有媒体对应某DSP的在群G1上的公钥
              publicKeyOfG2Array        所有媒体对应某DSP的在群G2上的公钥
              deviceId                  设备ID
              numofMedia                参与的媒体个数
              betaValue                 当时盲化所用的随机数对应的序列化字符串, 长度为67

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误 或 作弊媒体编号的相反数

- **批量验证**
     **int batch_verify(String[] unblindCipherArray, String[] allDeviceIds, byte[] systemKeyOfG2, int numofDeviceIds)**

     该接口用于批量验证去盲后的密文是否正确。

     参数说明：
            ::

              unblindCipherArray   多个去盲后的did密文字符串组成的数组
              allDeviceIds         多个did字符串组成的数组
              systemKeyOfG2        对应DSP的在群G2上的系统公钥, 长度为257
              numofDeviceIds       设备ID的个数

     错误返回：成功 或 “参数空指针”错误 或 “分配空间”错误

**示例**
^^^^^^^^^^

.. code-block:: java

     // 本样例模拟字节和一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证
     // 字节为服务端，负责加密
     import java.util.Random;
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

          Random rand = new Random();
          int randSeed = rand.nextInt();
          secureUnionID.MasterKeygen(randSeed, masterKey);
          System.out.printf("masterKey is: %s\n", bytesToHex(masterKey));

          String dspID = "1234567890";

          // generate public/private keys.
          byte publicKeyOfG1[] = new byte[PUBKEY_G1_LEN];
          byte publicKeyOfG2[] = new byte[PUBKEY_G2_LEN];
          byte privateKey[] = new byte[PRIVATE_KEY_LEN];
          secureUnionID.Keygen(masterKey, dspID, publicKeyOfG1, publicKeyOfG2, privateKey);

          // generate system keys.
          String publicKeyOfG1Array[] = new String[1];
          String publicKeyOfG2Array[] = new String[1];
          publicKeyOfG1Array[0] = new String(publicKeyOfG1);
          publicKeyOfG2Array[0] = new String(publicKeyOfG2);
          byte systemKeyOfG1[] = new byte[PUBKEY_G1_LEN];
          byte systemKeyOfG2[] = new byte[PUBKEY_G2_LEN];
          secureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array, 1, systemKeyOfG1, systemKeyOfG2);

          // the device id number to protect.
          String deviceId0 = "123456789012345";
          String deviceId1 = "123456789054321";

          // blinding.
          byte betaValue0[] = new byte[2 * PRIVATE_KEY_LEN + 1];
          byte blindResult0[] = new byte[PUBKEY_G1_LEN];
          randSeed = rand.nextInt();
          secureUnionID.Blinding(deviceId0, randSeed, betaValue0, blindResult0);

          byte betaValue1[] = new byte[2 * PRIVATE_KEY_LEN + 1];
          byte blindResult1[] = new byte[PUBKEY_G1_LEN];
          randSeed = rand.nextInt();
          secureUnionID.Blinding(deviceId1, randSeed, betaValue1, blindResult1);

          // encryption
          byte cipherText0[] = new byte[PUBKEY_G1_LEN];
          secureUnionID.Enc(privateKey, blindResult0, cipherText0);

          byte cipherText1[] = new byte[PUBKEY_G1_LEN];
          secureUnionID.Enc(privateKey, blindResult1, cipherText1);

          // unblinding
          byte unblindCipherText0[] = new byte[PUBKEY_G1_LEN];
          String[] cipherTextArray = new String[1];
          cipherTextArray[0] = new String(cipherText0);
          secureUnionID.Unblinding(cipherTextArray, 1, betaValue0, systemKeyOfG1, unblindCipherText0);


          byte unblindCipherText1[] = new byte[PUBKEY_G1_LEN];
          cipherTextArray[0] = new String(cipherText1);
          secureUnionID.Unblinding(cipherTextArray, 1, betaValue1, systemKeyOfG1, unblindCipherText1);

          String unblindCipherArray[] = new String[2];
          unblindCipherArray[0] = new String(unblindCipherText0);
          unblindCipherArray[1] = new String(unblindCipherText1);

          String allDeviceIds[] = new String[2];
          allDeviceIds[0] = deviceId0;
          allDeviceIds[1] = deviceId1;
          // verify
          int r = secureUnionID.batch_verify(unblindCipherArray, allDeviceIds, systemKeyOfG2, 2);

          if (r != SUCCESS) {
               cipherTextArray[0] = new String(cipherText0);
               int result = secureUnionID.verify_individual(cipherTextArray, publicKeyOfG1Array, publicKeyOfG2Array, deviceId0, 1, betaValue0);
               if (result < 0) {
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