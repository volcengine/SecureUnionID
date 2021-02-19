**SecureUnionID接口说明及性能测试**
=======================================
**Go接口说明**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- **随机数种子生成**
      **SeedGen()**

      该接口用于生成并返回一个unit32类型的随机数种子，该种子用于后续的随机数生成。

      错误返回：nil 或 “文件打开失败”错误 或 “二进制读取”错误
- **主密钥生成**
      **MasterKeyGen(seed)**

      该接口用于随机生成512bit的主密钥，该密钥用于后续派生对应DSP的公私钥对。

      参数说明:
            ::

                  seed 随机数种子

      错误返回：nil 或 “masterkey空指针”错误
- **密钥生成**
     **Keygen(masterKey, dspId)**

     该接口用于生成对应DSP的公私钥对。

     参数说明:
            ::

              masterKey  主密钥
              dspId      对应DSP的标识符

     错误返回：nil 或 “参数空指针”错误

- **系统公钥生成**
     **SystemKeygen(pki)**

     该接口用于生成某DSP对应的系统公钥。

     参数说明：
            ::

              pki  所有媒体对应某DSP的公钥组成的Slice

     错误返回：nil 或 “参数空指针”错误 或 “分配空间”错误
- **哈希生成**
     **HashToG1(did)**

     该接口用于将did转换为群G1上的元素。

     参数说明：
            ::

              did  需要哈希的did字符串
     
- **盲化**
     **Blind(seed, did)** 

     该接口用于生成did盲化后的结果，同时返回用于盲化的随机数。

     参数说明：
            ::

              seed    随机数种子
              did     需要盲化的did字符串

     错误返回：nil 或 “参数空指针”错误
- **加密**
     **Enc(message)**

     该接口用于加密接收到的消息。

     参数说明：
            ::
            
              message 接收到的消息，这里通常为did盲化后的结果

     错误返回：nil 或 “参数空指针”错误 或 “参数格式”错误
- **去盲化**
     **Unblind(randVal, cipheri)**

     该接口用于将收到的消息密文合并，并去掉随机数，得到最终的密文。

     参数说明：
            ::

              randVal  当时盲化所用的随机数
              cipheri  从各个媒体处收到的对应盲化did的密文

     错误返回：nil 或 “参数空指针”错误 或 “分配空间”错误
- **验证**
     **Verify(cipheri, pki, cipher, dids, ranVal)**

     该接口用于验证接收到的密文是否正确。

     参数说明：
            ::
            
              cipheri    从各个媒体处收到的一个或多个盲化did的密文连接起来的字符串
              pki        各个媒体的公钥
              cipher     多个合并并去盲后的did密文组成的Slice
              dids       多个did组成的Slice
              randVal    当时加盲所使用的随机数组成的Slice

     Note：cipheri和pki每一位一一对应，绑定在同一个媒体上；dids和randVal每一位一一对应，randVal存的为dids对应位置加盲时使用的随机数。

     返回参数：int int error 

      第一个参数表示作弊媒体编号的相反数，没作弊时为2，空间出错为1，空指针错误为0

      第二个参数表示作弊DID编号的相反数，没作弊时为2，空间出错为1，空指针错误为0

     错误返回：nil 或 “参数空指针”错误 或 “分配空间”错误

**GO样例**
^^^^^^^^^^

.. code-block:: go

      // 本样例模拟字节和一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证
      // 字节为服务端，负责加密

      func main() {
            var dspID,did,did1,randVal,M,bt string
            dspID="1234567890"
            did="123456789012345"
            did1="123456789054321"

            //字节方生成密钥
            seed,_:=core.SeedGen()
            masterKeyBT,_:=core.MasterKeyGen(seed)
            keyPairBT,_:=core.Keygen(masterKeyBT,dspID)

            // DSP生成系统密钥
            var pki []core.Group
            pki=append(pki,keyPairBT.PK)
            sysPk,_:=core.SystemKeygen(pki)

            // 生成客户端和服务端的实例
            clt:=core.NewClientFromInput(sysPk)
            sevBT:=core.NewSeverFromInput(keyPairBT.SK)

            // DSP进行盲化操作
            randVal,M,_=clt.Blind(seed,did)  
            randVal1,M1,_:=clt.Blind(seed,did1)
            
            // 字节方加密接收到的消息
            cipherBT1,_:=sevBT.Enc(M)
            cipherBT2,_:=sevBT.Enc(M1)

            // DSP解盲
            var cipheri []string
            cipheri=append(cipheri,cipherBT1)
            bt,_=clt.Unblind(randVal,cipheri)
            cipheri=cipheri[0:0]
            cipheri=append(cipheri,cipherBT2)
            bt1,_:=clt.Unblind(randVal1,cipheri)
            
            cipheri=cipheri[0:0]
            cipheri=append(cipheri,cipherBT1+cipherBT2)

            // 验证
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
                  fmt.Printf("No.%d media cheat on %dth did!\n", -result, -result1)
            }

      }

**性能测试**
^^^^^^^^^^^^^

主密钥生成 0.22ms

密钥生成 0.76ms

下表为随着媒体数变化，各个模块的计算开销，其中-表示不随之变化。（单位ms）

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


下表为随着DID数变化，各个模块的计算开销，其中-表示不随之变化。（单位ms）

这里盲化、加密和解盲可以多线程进行，测试时未使用多线性。

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


为了方便其他语言接口的二次开发，下面给出底层C语言接口的相关说明。

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
              Mstring     要加密的字符串，一般为哈希后的did
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

     返回参数：成功 或 “参数空指针”错误 或 “分配空间”错误 或 作弊媒体编号的相反数

- **批量验证**
     **batch_verify(btstring, did, sysg2string, numofdid)**

     该接口用于批量验证去盲后的密文是否正确。

     参数说明：
            ::

              btstring       多个去盲后的did密文字符串组成的数组
              did            多个did字符串组成的数字
              sysg2string    在G2上的系统公钥对应的序列化字符串
              numofdid       did的个数

     Note：btstring和did每一位一一对应。

     返回参数：成功 或 “参数空指针”错误 或 “分配空间”错误

**C样例**
^^^^^^^^^^

.. code-block:: c

      // 本样例模拟字节和一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证
      // 字节为服务端，负责加密
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
          char *ranbyte=(char *)malloc(sizeof(ran));
          int fd,result;

          //生成随机数种子
          if ((fd = open ("/dev/random", O_RDONLY)) > 0)
          {
               read (fd, &ran, sizeof (ran));
          }
          close (fd);

          //字节生成主密钥和公私钥对
          MasterKeygen(ran,masterkey);
          Keygen(masterkey,dspID,pkg1string,pkg2string,skstring);

          //DSP生成系统参数
          pkig1string[0]=pkg1string;
          pkig2string[0]=pkg2string;
          System_Keygen(pkig1string,pkig2string,1,sysg1string,sysg2string);

          //DSP进行盲化
          Blinding(did,betastring,ran,Mstring);
          Blinding(did1,betastring1,ran,Mstring1);

          //字节加密
          Enc(skstring,Mstring,cipherstring);
          Enc(skstring,Mstring1,cipherstring1);

          //DSP进行去盲化
          cipherstrings[0]=cipherstring;
          Unblinding(cipherstrings,1,betastring,sysg1string,cipher);
          cipherstrings[0]=cipherstring1;
          Unblinding(cipherstrings,1,betastring1,sysg1string,cipher1);

          ciphers[0]=cipher;
          ciphers[1]=cipher1;
          dids[0]=did;
          dids[1]=did1;

          //验证
          if(batch_verify(ciphers,dids,sysg2string,2)!=SUCCESS){
               cipherstrings[0]=cipherstring;
               result=verify_individual(cipherstrings,pkig1string,pkig2string,did,1,betastring);
               if(result<0)
                    printf("No.%d media cheat on 1th did!\n",-result);

               cipherstrings[0]=cipherstring1;
               result=verify_individual(cipherstrings,pkig1string,pkig2string,did1,1,betastring1);
               if(result<0)
                    printf("No.%d media cheat on 2th did!\n",-result);
          }
          else{
               printf("no one cheat!\n");
          }
     }