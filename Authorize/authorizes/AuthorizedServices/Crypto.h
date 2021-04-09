#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <stdio.h>
#include <string>


#define RSA_KEYLEN 1024

enum KeyType{
    Public,
    Private
};

using namespace std;

class Crypto {
public:
    Crypto();
    ~Crypto();

    /***************************************************************/
    /*                             RSA                             */
    /***************************************************************/
    bool generateRsaKeyPair();

    bool writeRsaKeyPairToFile(char* file);

    bool getRsaPublicKey(unsigned char **publicKey);

    bool setRsaPublicKey(unsigned char *publicKey, size_t publicKeyLength);

    bool getRsaPrivateKey(unsigned char **privateKey);

    bool setRsaPrivateKey(unsigned char *publicKey, size_t publicKeyLength);

    bool rsaSign(string &orignal,string &sign_text_base64);

    bool rsaVerify(string &orignal,string &sign_text_base64);

    /**
     * @brief rsaEncrypt         RSA公钥加密
     * @param plaintext          待加密的明文
     * @param ciphertext_base64  加密后的BASE64格式密文
     * @return
     */
    bool rsaEncrypt( string &plaintext, string &ciphertext_base64 );

    /**
     * @brief rsaDecrypt        RSA私钥解密
     * @param ciphertext_base64 待解密的BASE64格式密文(NO_BASE64)
     * @param plaintext         解密后的明文
     * @return
     */
    bool rsaDecrypt( string &ciphertext_base64,  string &plaintext );


    /***************************************************************/
    /*                             AES                             */
    /***************************************************************/
    bool aesEncrypt( string &key, string &plaintext, string &ciphertext_base64 );

    bool aesDecrypt( string &key, string &ciphertext_base64,  string &plaintext );


    /***************************************************************/
    /*                          others                             */
    /***************************************************************/
    bool writeKeyToFile(FILE *file, int key);

    /**
     * @brief formatRSAKey 格式化RSA密钥字符串(添加RSA密钥的头和尾，以及BASE64格式的换行符)
     * @param key BASE64格式的字符串
     * @param keyType 需要格式的类型
     * @return 格式化后的RAS密钥字符串
     */
    string &formatRSAKey( string &key, KeyType keyType = Public );

    /**
     * @brief unformatRASKey RSA密钥字符串去格式化(移除RSA密钥的头和尾，以及BASE64格式的换行符)
     * @param key 含有添加RSA密钥的头和尾的BASE64格式的字符串
     * @param keyType 需要格式的类型
     * @return 去格式化后的RAS密钥字符串
     */
    string &unformatRASKey( string &key, KeyType keyType = Public );

    /**
     * @brief trimed 删除字符串首尾空格
     * @param s
     * @return
     */
    string &trimed(string &s);

    /**
     * @brief trimedLeft 删除字符串首位空格
     * @param s
     * @return
     */
    string &trimedLeft(string &s);

    /**
     * @brief trimedRight 删除字符串尾部空格
     * @param s
     * @return
     */
    string &trimedRight(string &s);

private:
    bool bio2string(BIO *bio, unsigned char **string);

private:
    //RSA
    /**
     * RSA最大加密明文大小
     */
    static const int MAX_ENCRYPT_BLOCK;

    /**
     * RSA最大解密密文大小
     */
    static const int MAX_DECRYPT_BLOCK;
    EVP_PKEY* rsakeypair;

    //AES

};

#endif
