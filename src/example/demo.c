 #include<stdio.h>
 #include"../psi/encryption.h"
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
          char betastring[2*32+1], betastring1[2*32+1];
          char Mstring[2*G1LENTH+1], Mstring1[2*G1LENTH+1];
          char cipherstring[2*G1LENTH+1], cipherstring1[2*G1LENTH+1];
          char cipher[2*G1LENTH+1], cipher1[2*G1LENTH+1];
          char ran[64];
          int fd,result;

          //生成随机数种子
          int r = genRandSeed(ran);
          if (r != 64)
          {
               printf("gen random number error\n");
               return 0;
          }

          //字节生成主密钥和公私钥对
          printf("--------------------------------------------------\n");
          printf("Step 0: generate master key.\n");
          r = genMasterKey(ran,masterkey);
          if (r != SUCCESS) {
               printf("generate master key error, error number %d\n", r);
               return 0;
          }

          printf("OK\n");

          printf("--------------------------------------------------\n");
          printf("Step 1: generate public key and private key\n");
          r = Keygen(masterkey,dspID,pkg1string,pkg2string,skstring);
          if (r != SUCCESS) {
               printf("generate public private keys error, error number: %d", r);
               return 0;
          }
          printf("OK\n");

          //DSP生成系统参数
          printf("--------------------------------------------------\n");
          printf("Step 2: generate the system key\n");
          pkig1string[0] = pkg1string;
          pkig2string[0] = pkg2string;
          r = System_Keygen(pkig1string,pkig2string,1,sysg1string,sysg2string);
          if (r != SUCCESS) {
               printf("generate system key error, error number: %d", r);
               return 0;
          }
          printf("OK\n");
          //DSP进行盲化
          printf("--------------------------------------------------\n");
          printf("Step 3: blind\n");
          r = genRandSeed(ran);
          if (r != 64)
          {
               printf("gen random number error\n");
               return 0;
          }
          r = Blind(did,ran,betastring,Mstring);
          if (r != SUCCESS) {
               printf("blind error, error number: %d", r);
               return 0;
          }
          r = genRandSeed(ran);
          if (r != 64)
          {
               printf("gen random number error\n");
               return 0;
          }
          r = Blind(did1,ran,betastring1,Mstring1);
          if (r != SUCCESS) {
               printf("blind error, error number: %d", r);
               return 0;
          }
          printf("OK\n");
          //字节加密
          printf("--------------------------------------------------\n");
          printf("Step 4: encrypt\n");
          r = Enc(skstring,Mstring,cipherstring);
          if (r != SUCCESS) {
               printf("encrypt error, error number: %d", r);
               return 0;
          }
          r = Enc(skstring,Mstring1,cipherstring1);
          if (r != SUCCESS) {
               printf("encrypt error, error number: %d", r);
               return 0;
          }
          printf("OK\n");
          //DSP进行去盲化
          printf("--------------------------------------------------\n");
          printf("Step 5: unblind\n");
          cipherstrings[0] = cipherstring;
          r = Unblinding(cipherstrings,1,betastring,sysg1string,cipher);
          if (r != SUCCESS) {
               printf("unblind error, error number: %d", r);
               return 0;
          }
          cipherstrings[0] = cipherstring1;
          r = Unblinding(cipherstrings,1,betastring1,sysg1string,cipher1);
          if (r != SUCCESS) {
               printf("encrypt error, error number: %d", r);
               return 0;
          }
          printf("OK\n");

          ciphers[0] = cipher;
          ciphers[1] = cipher1;
          dids[0] = did;
          dids[1] = did1;

          //验证
          printf("--------------------------------------------------\n");
          printf("Step 6: verify\n");

          //验证正确性
          for(int i=0; i<2; i++)
          {
               HASHIT(Mstring, dids[i]);
               r = Enc(skstring,Mstring,cipherstring);
               if (r != SUCCESS) {
                    printf("encrypt error, error number: %d\n", r);
                    return 0;
               }
               if (memcmp(cipherstring, ciphers[i], 2*G1LENTH+1) != 0) {
                    printf("[did check fail]encrypt != blind + encrypt + unblind\n");
                    return 0;
               }
          }
          printf("[did check ok] encrypt = blind + encrypt + unblind\n");
     }