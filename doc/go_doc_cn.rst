**SecureUnionID GO接口说明及性能测试**
=======================================
**Go接口说明**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- **随机数种子生成**
      **SeedGen()**

      该接口用于生成并返回一个unit32类型的随机数种子，该种子用于后续的随机数生成。

      错误返回：nil 或 “文件打开失败”错误 或 “二进制读取”错误
- **随机数种子生成**
      **RandomSeed()**

      该接口用于生成并返回一个[64]byte类型的随机数种子，该种子用于后续的随机数生成。

      错误返回：nil 或 “随机数生成错误”错误
- **主密钥生成**
      **MasterKeyGen(seed)**

      该接口用于随机生成512bit的主密钥，该密钥用于后续派生对应DSP的公私钥对。

      参数说明:
            ::

                  seed 随机数种子

      错误返回：nil 或 “masterkey空指针”错误
- **主密钥生成**
      **GenMasterKey(seed)**

      该接口用于随机生成512bit的主密钥，该密钥用于后续派生对应DSP的公私钥对。

      参数说明:
            ::

                  seed 随机数种子 (64 byte)

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
- **盲化**
     **Blindv2(seed, did)** 

     该接口用于生成did盲化后的结果，同时返回用于盲化的随机数。

     参数说明：
            ::

              seed    随机数种子 (64 byte)
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
            dspID = "1234567890"
            did = "123456789012345"
            did1 = "123456789054321"

            //字节方生成密钥
            seed,_ := core.RandomSeed()
            masterKeyBT,_ := core.GenMasterKey(seed)
            keyPairBT,_ := core.Keygen(masterKeyBT,dspID)

            // DSP生成系统密钥
            var pki []core.Group
            pki = append(pki,keyPairBT.PK)
            sysPk,_ := core.SystemKeygen(pki)

            // 生成客户端和服务端的实例
            clt := core.NewClientFromInput(sysPk)
            sevBT := core.NewSeverFromInput(keyPairBT.SK)

            // DSP进行盲化操作
            seed,_ = core.RandomSeed()
            randVal,M,_ = clt.Blindv2(seed,did)
            seed,_ = core.RandomSeed()  
            randVal1,M1,_ := clt.Blindv2(seed,did1)
            
            // 字节方加密接收到的消息
            cipherBT1,_ := sevBT.Enc(M)
            cipherBT2,_ := sevBT.Enc(M1)

            // DSP解盲
            var cipheri []string
            cipheri = append(cipheri,cipherBT1)
            bt,_ = clt.Unblind(randVal,cipheri)
            cipheri = cipheri[0:0]
            cipheri = append(cipheri,cipherBT2)
            bt1,_ := clt.Unblind(randVal1,cipheri)
            
            cipheri = cipheri[0:0]
            cipheri = append(cipheri,cipherBT1+cipherBT2)

            // 验证
            var cipher []string
            cipher = append(cipher,bt)
            cipher = append(cipher,bt1)
            var dids []string
            dids = append(dids,did)
            dids = append(dids,did1)
            var randVals []string
            randVals = append(randVals,randVal)
            randVals = append(randVals,randVal1)
            result,result1,_ := clt.Verify(cipheri,pki,cipher,dids,randVals)
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

主密钥生成 0.28ms

密钥生成 0.73ms

下表为随着媒体数变化，各个模块的计算开销，其中-表示不随之变化。（单位ms）

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
| Verify       | 1.96 | `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`| `\ -`|
+--------------+------+------+------+------+------+------+------+------+


下表为随着DID数变化，各个模块的计算开销，其中-表示不随之变化。（单位ms）

这里盲化、加密和解盲可以多线程进行，测试时未使用多线程。

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
| Verify       | 1.96 | 2.39 | 4.53 | 6.90 | 9.52 | 11.98 | 14.43 | 17.02 |
+--------------+------+------+------+------+------+-------+-------+-------+