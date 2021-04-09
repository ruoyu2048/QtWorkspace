#include <iostream>

using namespace std;

#include "Crypto.h"
#include "base64.h"

int main()
{
    Crypto crypto;
    //crypto.generateRsaKeyPair();
    //解决windows下汉字输乱码问题
    system("chcp 65001");
    string pub="MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCAWzPZcXYilgx7yv2YJZz6nSq79sINBZeIuEkNcAuuEAEfNKOG+rWftVW9u7cSbDFIBBFWUge522+ygvRaFka+QNZui21W/BFb+Tv6QmOB0PYgt3w1t2IqbWgWD7/vfIxBEqWBs0FyTUp2AWzmBvvD+Z1fSmUTh3pAEgtn2DCwHQIDAQAB";
    string pri="MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAIBbM9lxdiKWDHvK/ZglnPqdKrv2wg0Fl4i4SQ1wC64QAR80o4b6tZ+1Vb27txJsMUgEEVZSB7nbb7KC9FoWRr5A1m6LbVb8EVv5O/pCY4HQ9iC3fDW3YiptaBYPv+98jEESpYGzQXJNSnYBbOYG+8P5nV9KZROHekASC2fYMLAdAgMBAAECgYArJ/8H5jk1KcQqvzaCuGrObJeYuT/HhxDLHEMbrIeQz7rA1j8BPDxi5WJdt8a2//+V2iBGKLCGUfx9RcCIpaLFBwdaQu9YI6Bzkyri6drou/4hP5vjo+Ii+pjy5Yv+MHRA94uTkxB56aH4RRl+GVNNCiVXlgD7QePjQezYwOOgYQJBAPkUcPMB/Kxc5MrfAEYLA+Qwry9C4v3v11IY3Gh7qPJljPh75GfRBBbawwsR2WsHWVIpRpSWmWj39yz2m7GXXOkCQQCD7CAS7ECW9lA4GvCFKO7dCs+g3STcEgmIonMw5LW9peoXw1FOaatPT8M8+M5f09vsWs65NzKT5+qPURwPxukVAkEA27xEyJ0AI67vyGli/GfLTwwEAFWpmwxzkGGOUD6HlRAxhSUHIu8qBF+JocJlZB5fljQDqCITxJ682FXWlJ1LwQJAJrkXzRYl+2n/sAhQU3Xpb16uBXUETcjJnZ65mz93DVz3MtR1OEQSMeyizXHm+f/xDRa2mW1XXkzz7++KAiOL5QJBALDVERgCFWzk3QnZgerAeBGm7pBRWBUsJhlDOTB0OBQjSYcSpUBszgYroZM2EAsx3Ks33zKeJd5E5LJAGGRXgCk=";

    //    string pub="MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDeD0NvdIaqV7fL8isaJjYwQxM4Vb9EKoOHwVnRejag708/JHKilQKL9WprR1t0tiPNungYkMHVdbBrlGIXL2XyZXGVOs3hQy9UMThZp3hpY/ViM4LwvWDnCqgKHbpuIJ9ABMgy2wOm+ir8hSJTrHS4qotVInldp98iuSxsT6SKowIDAQAB";
    //    string pri="MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAN4PQ290hqpXt8vyKxomNjBDEzhVv0Qqg4fBWdF6NqDvTz8kcqKVAov1amtHW3S2I826eBiQwdV1sGuUYhcvZfJlcZU6zeFDL1QxOFmneGlj9WIzgvC9YOcKqAodum4gn0AEyDLbA6b6KvyFIlOsdLiqi1UieV2n3yK5LGxPpIqjAgMBAAECgYEAv59YSBO8Kb7sYlgmDbivxlH7gf+sv7wYKMwXIAdgoNwh/JENQ3Nm8TPkk5LOPxb2KpAhFsHEItoeVZeEVhGW3ExcAC2Iz52XjcVG+nEoF/Wh6CgbqH8zrDCmKmZf9D0z8Y7fgq8jkTEkL6yDhsC2kGxwMJn3Vd880ne8n9culsECQQDxE/DZeNGxduj4XGzUztOZzk6uGSFKJpXWb+4YzI+s40tF6ZkoptSFJbOoiGFERMFmDSAadUPqjtZVc/e/8AFTAkEA6832whuFnQEqOYjhZ/U4Z/q0/uHYYb7AZSQ95OyW8/bYIiuXctWgtL/lf6ehN0jZJtmINleN9o3z4cqwyOSXcQJATf+5qWMh1V03u0OMIr5YUCcj5chTsP4S9LJ6QqPMyI8xx4jw+OohUIMGQbz+W2wzdqVAgc73Dmjwh+1hd0Rb6QJAFaGjTVMxAqIkuXpxExRPxFofWHJnYzAYfi7A7ss1XdP7J8dP8VzfjTU9XGSISYYJRaMW98U4ZLHI+aGabhNqQQJAOEkJLAed1h7X5hS1sEhbS76/wKRKJU7e11QlJVgoq9z7kfs6wvRbW0VCZzijHOBzyKYUzv0fXLC/lteygWSBCQ==";

    crypto.formatRSAKey(pub);
    printf("1:%s\n",pub.c_str());
    size_t xx=strlen(pub.c_str());
    crypto.setRsaPublicKey((unsigned char*)pub.c_str(),xx);
    unsigned char* rpub=NULL;
    crypto.getRsaPublicKey(&rpub);
    printf("2:%s",rpub);

    crypto.formatRSAKey(pri,Private);
    printf("1:%s\n",pri.c_str());
    xx=strlen(pri.c_str());
    crypto.setRsaPrivateKey((unsigned char*)pri.c_str(),xx);
    unsigned char* rpri=NULL;
    crypto.getRsaPrivateKey(&rpri);
    printf("2:%s",rpri);

    string ori="2020-12-04T11:35:15.815";
    string out;
    crypto.rsaSign(ori,out);
    //out="G9LYjo+lAVFtYARjTRWQO37mylJsx52WpbqMJAnIRvehtr3/qAouEr908JDs9HItBAF6rVMASxZ2GhlC7mmFmqjX/TnCleHYote2XU9i6bdygkstudBHQoyc21xIJ6FQ+99UblOSZBMukISkhLpDexHWeHF7837vmCyLrFyLM00=";
    if( crypto.rsaVerify(ori,out) ){
        printf("ttut successfuly!\n");
    }

    //    string sEncTest="c1xt2moyJnBvYnzxCtZcFzahb3NVIgKawBF2w2okBHGyQaMlTmHb3Kjj85/PmMRpnH/C0a4IfSoABMFuZw3oPoBeEiZoneKkimvqGyCJcDRoF3mkueDP2Rjqwb/lqLJlbh+JGN+lu8+h2kSRHTrDCw5uY0KEtAlvJIxj/GryS4g=";
    //    //string sEncTest="ZIwNRujoVHfuqWre32vcJwdK86pJEYvK8a/o/U8YdayY91CfL4zx87tFd9otvmCC1pFq3ZuckuiqcOkKkUNbEADits80iVbEXZFXxn6ll7O+YhX5bw2ekJvlTtWNY14Tqca24PcWxVtIzKtfTivbtG2sVtLoPBCU1A+dTBkNMYc=";
    //    //string sEncTest="VGJ47YTuVPSDHkP32V1wphbNzl1jxOCj6Pc62OpF4PypLYXu2P3KiqXluCq+SW6X93V2C5R8hf7Z9mrLHEEAXoQliwccFA3jWa7gs5K7bNoIAPFKOOPs9EWq6I8rbsXAEs7d3W+pw9uqMLYO2+yBas0uoE22JKHDuh/qPGRwThw=";

    //    string plaintext,ciptxt;
    //    string test;
    //    for(int i=0;i<20;i++ ){
    //        test += to_string(i+1);
    //        test += " China is beautiful country!\n";
    //    }

    //    FILE* file = nullptr;
    //    if( 0 != fopen_s(&file,"test.json","r") ){
    //        printf("open test file failed!\n");
    //        return 0;
    //    }

    //    char cBuf[4096]={'\0'};
    //    fread(cBuf, 1, sizeof(cBuf)-1, file);
    //    test = cBuf;
    //    fclose(file);

    //    crypto.rsaEncrypt(test,ciptxt);
    //    crypto.rsaDecrypt(ciptxt,plaintext);

    //    string strTest="Hello World";
    //    string enc;

    //    crypto.aesEncrypt(strTest,enc);
    //    //enc="892C3AB398E294F85BBD8421417D8BFAF6DDA52D95982BC5070982F1C244CE0F";
    //    string dec;
    //    crypto.aesDecrypt(enc,dec);
    //    printf("%s",dec.c_str());

    //    string key="abcdabcdabcdabcd";
    //    string txt="123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng123456abcdefgonpgoiqnwpoensdng";
    //    string strcip_b64;
    //    crypto.aesEncrypt(key,txt,strcip_b64);
    //    printf("%s\n\n",strcip_b64.c_str());
    //    string plaintxt;
    //    crypto.aesDecrypt(key,strcip_b64,plaintxt);
    //    printf("%s\n\n",plaintxt.c_str());

    return 0;
}


