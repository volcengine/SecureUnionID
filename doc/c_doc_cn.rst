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

- **随机数生成**
      **genRandSeed(ran)**

      该接口用于生成长度为64字节的随机数。

      参数说明：
            ::

              ran        生成的随机数

      返回：所生成的随机数长度

- **主密钥生成**
      **MasterKeygen(ran, masterkey)**

      该接口用于随机生成512bit的主密钥，该密钥用于后续派生对应DSP的公私钥对。

      参数说明：
            ::

              ran        随机数种子
              masterkey  生成的主密钥

      错误返回：成功 或 “masterkey空指针”错误

- **主密钥生成**
      **genMasterKey(ran, masterkey)**

      该接口用于随机生成512bit的主密钥，该密钥用于后续派生对应DSP的公私钥对。

      参数说明：
            ::

              ran        随机数种子(64 byte)
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

- **盲化**
     **Blind(did, seed, betastring, Mstring)**

     该接口用于生成用于盲化的随机数和did盲化后的结果。

     参数说明：
            ::

              seed          随机数种子(64 byte)
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
          char ran[64];
          int fd,result;

          //生成随机数种子
          genRandSeed(ran);

          //字节生成主密钥和公私钥对
          genMasterKey(ran,masterkey);
          Keygen(masterkey,dspID,pkg1string,pkg2string,skstring);

          //DSP生成系统参数
          pkig1string[0] = pkg1string;
          pkig2string[0] = pkg2string;
          System_Keygen(pkig1string,pkig2string,1,sysg1string,sysg2string);

          //DSP进行盲化
          genRandSeed(ran);
          Blind(did,ran,betastring,Mstring);
          genRandSeed(ran);
          Blind(did1,ran,betastring1,Mstring1);

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

     }