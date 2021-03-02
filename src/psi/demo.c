 #include<stdio.h>
 #include"encryption.h"
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