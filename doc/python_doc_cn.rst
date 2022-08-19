**SecureUnionID Python接口和使用说明**
=======================================
**Python接口说明**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
接口存在于SecureUnionID.py中, 该类调用底层的C语言的接口，所以需要先`import SecureUnionID`
- **随机数生成**
      **rnd = SecureUnionID.genRandSeed()**

      该接口生成长度为64字节的随机数。输出存储到rnd中。

      返回结果说明：
            ::

              rnd[0]   所生成随机数的长度（in Byte）
              rnd[1]   生成的随机数, 长度为64

- **主密钥生成**
      **masterKey = SecureUnionID.genMasterKey(seed)**

      该接口生成主密钥。输入为随机数种子seed, 输出存储到masterKey中。

      参数说明：
            ::

              seed        随机数种子 (64 byte)

      返回结果说明：
            ::

              masterKey[0]   表示是否成功(当为2时表示成功，其他数值为失败)
              masterKey[1]   生成的主密钥, 长度为64

- **主密钥生成**
      **masterKey = SecureUnionID.MasterKeygen(seed)**

      该接口生成主密钥。输入为随机数种子seed, 输出存储到masterKey中。

      参数说明：
            ::

              seed        随机数种子

      返回结果说明：
            ::

              masterKey[0]   表示是否成功(当为2时表示成功，其他数值为失败)
              masterKey[1]   生成的主密钥, 长度为64


- **密钥生成**
     **keys = SecureUnionID.Keygen(masterKey, dspId)**

     该接口用于生成对应DSP的公私钥对。

     参数说明：
          ::

            masterKey              主密钥, 长度为64
            dspId                  对应DSP的标识符

     返回结果说明：
          ::

            keys[0]          表示是否成功(当为2时表示成功，其他数值为失败);
            keys[1]          生成的在群G1的公钥, 长度为67
            keys[2]          生成的在群G2的公钥, 长度为257
            keys[3]          生成的私钥, 长度为32


- **系统公钥生成**
     **systemKey = SecureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array , numofMedia)**

     该接口用于生成某DSP对应的系统公钥。

     参数说明：
            ::

              publicKeyOfG1Array     所有媒体对应某DSP的在群G1上的公钥
              publicKeyOfG2Array     所有媒体对应某DSP的在群G2上的公钥
              numofMedia             参与媒体的个数

     返回结果说明：
            ::

              systemKey[0]         表示是否成功(当为2时表示成功，其他数值为失败);
              systemKey[1]         对应DSP的在群G1上的系统公钥, 长度为67
              systemKey[2]         对应DSP的在群G2上的系统公钥, 长度为257


- **盲化**
     **blind = SecureUnionID.Blinding(deviceId, seed)**

     该接口用于生成用于盲化的随机数和did盲化后的结果。

     参数说明：
            ::

              deviceId      需要盲化的did字符串
              seed          随机数种子

     返回结果说明：
            ::

              blind[0]       表示是否成功(当为2时表示成功，其他数值为失败);     
              blind[1]       盲化随机数对应的序列化字符串, 长度为65
              blind[2]       盲化后的结果, 长度为67

- **盲化**
     **blind = SecureUnionID.Blind(deviceId, seed)**

     该接口用于生成用于盲化的随机数和did盲化后的结果。

     参数说明：
            ::

              deviceId      需要盲化的did字符串
              seed          随机数种子，长度为64字节的随机数

     返回结果说明：
            ::

              blind[0]       表示是否成功(当为2时表示成功，其他数值为失败);     
              blind[1]       盲化随机数对应的序列化字符串, 长度为65
              blind[2]       盲化后的结果, 长度为67

- **加密**
     **cipherText = SecureUnionID.Enc(privateKey, plainText)**

     该接口用于加密字符串。

     参数说明：
            ::

              privateKey      私钥, 长度为32
              plainText       要加密的字符串, 一般为哈希后的设备ID或盲化后的设备ID, 长度为67

     返回结果说明：
            ::

               cipherText[0]    表示是否成功(当为2时表示成功，其他数值为失败);
               cipherText[1]    加密后的密文, 长度为67

- **去盲化**
     **unblindCipherText = SecureUnionID.Unblinding(cipherTextArray, numofMedia, betaValue, systemKeyOfG1)**

     该接口用于将收到的消息密文合并，并去掉随机数，得到最终的密文。

     参数说明：
            ::

              cipherTextArray    来自各个媒体方的加密字符串组成的数组
              numofMedia         参与的媒体个数
              betaValue          当时盲化所用的随机数对应的序列化字符串
              systemKeyOfG1      在G1上的系统公钥, 长度为67

     返回结果说明：
            ::
                  
               unblindCipherText[0]  表示是否成功(当为2时表示成功，其他数值为失败);
               unblindCipherText[1]  最终的的加密的字符串, 长度为67

**示例**
^^^^^^^^^^

.. code-block:: python

     // 本样例模拟字节和一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证
     // 字节为服务端，负责加密
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