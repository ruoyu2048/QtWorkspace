#include "Crypto.h"
#include <openssl/pem.h>
#include "base64.h"

/**
 * @brief RSA最大加密明文大小
 */
const int Crypto::MAX_ENCRYPT_BLOCK = 117;

/**
 * @brief RSA最大解密密文大小
 */
const int Crypto::MAX_DECRYPT_BLOCK = 128;


Crypto::Crypto():
    rsakeypair(nullptr){
}

Crypto::~Crypto() {
    EVP_PKEY_free(rsakeypair);
}

bool Crypto::generateRsaKeyPair()
{
    EVP_PKEY_CTX *context = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

    if( EVP_PKEY_keygen_init(context) <= 0) {
        return false;
    }

    if( EVP_PKEY_CTX_set_rsa_keygen_bits(context, RSA_KEYLEN) <= 0 ) {
        return false;
    }

    if( EVP_PKEY_keygen(context, &rsakeypair) <= 0 ) {
        return false;
    }

    EVP_PKEY_CTX_free(context);

    return true;
}

bool Crypto::writeRsaKeyPairToFile(char *file)
{


    return true;
}

bool Crypto::getRsaPublicKey(unsigned char **publicKey){
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(bio, rsakeypair);
    return bio2string(bio, publicKey);
}

bool Crypto::setRsaPublicKey(unsigned char *publicKey, size_t publicKeyLength)
{
    if( !rsakeypair ){
        generateRsaKeyPair();
    }

    BIO *bio = BIO_new(BIO_s_mem());
    if(BIO_write(bio, publicKey, (int)publicKeyLength) != (int)publicKeyLength) {
        return false;
    }

    PEM_read_bio_PUBKEY(bio, &rsakeypair, NULL, NULL);
    BIO_free_all(bio);

    return true;
}

bool Crypto::getRsaPrivateKey(unsigned char **privateKey) {
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PrivateKey(bio, rsakeypair, NULL, NULL, 0, 0, NULL);
    return bio2string(bio, privateKey);
}

bool Crypto::setRsaPrivateKey(unsigned char *publicKey, size_t publicKeyLength)
{
    if( !rsakeypair ){
        generateRsaKeyPair();
    }

    BIO *bio = BIO_new(BIO_s_mem());

    if(BIO_write(bio, publicKey, publicKeyLength) != (int)publicKeyLength) {
        return false;
    }
    PEM_read_bio_PrivateKey(bio, &rsakeypair, NULL, NULL);
    BIO_free_all(bio);

    return true;
}

bool Crypto::rsaEncrypt(string &plaintext, string &ciphertext_base64)
{
    if( !rsakeypair )
        return false;

    RSA* rsa = EVP_PKEY_get1_RSA(rsakeypair);
    if( !rsa ){
        return false;
    }

    size_t rsa_len = static_cast<size_t>(RSA_size(rsa));

    bool flag = true;
    int  index  = 0;
    int  offset = 0;
    int  inputLen = static_cast<int>(plaintext.length());
    printf("[encrypt]input_plaintext_length:%d\n"
           "[encrypt]segment:%d\n"
           "[encrypt]input_plaintext==>\n%s\n", inputLen, inputLen/MAX_ENCRYPT_BLOCK, plaintext.c_str());

    string ciphertext_cache;
    while ( inputLen-offset > 0 ) {
        unsigned char* encMsg=nullptr;
        encMsg = new unsigned char[rsa_len + 1]();
        int encLen = -1;
        if( inputLen-offset > MAX_ENCRYPT_BLOCK ){
            encLen = RSA_public_encrypt(MAX_ENCRYPT_BLOCK,
                                        (unsigned char *)plaintext.c_str()+offset,
                                        encMsg, rsa, RSA_PKCS1_PADDING);
        }
        else{
            encLen = RSA_public_encrypt(static_cast<int>(inputLen-offset),
                                        (unsigned char *)plaintext.c_str()+offset,
                                        encMsg, rsa, RSA_PKCS1_PADDING);
        }

        if( -1 == encLen ){
            delete [] encMsg;
            encMsg=nullptr;
            flag = false;
            break;
        }

        string cacheblock((char*)(encMsg),encLen);
        ciphertext_cache.append(cacheblock);

        index++;
        offset = index*MAX_ENCRYPT_BLOCK;
        delete [] encMsg;
        encMsg = nullptr;
    }
    RSA_free(rsa);

    if( !flag ){
        return false;
    }

    ciphertext_base64 = base64_encode((unsigned char*)ciphertext_cache.c_str(),ciphertext_cache.length());

    return true;
}

bool Crypto::rsaDecrypt(string &ciphertext_base64, string &plaintext)
{
    if( !rsakeypair )
        return false;

    RSA* rsa = EVP_PKEY_get1_RSA(rsakeypair);
    if( !rsa )
        return false;

    size_t rsa_len = static_cast<size_t>(RSA_size(rsa));

    bool flag = true;
    int  index  = 0;
    int  offset = 0;

    string ciptertext = base64_decode(ciphertext_base64);
    int inputLen = ciptertext.length();
    printf("\n"
           "[decrypt]input_ciphertext_len:%d\n"
           "[decrypt]segment:%d\n\n",
           inputLen,
           inputLen/MAX_DECRYPT_BLOCK);

    plaintext.clear();
    while ( inputLen-offset > 0 ) {
        int decLen = -1;
        unsigned char* decMsg=nullptr;
        decMsg = new unsigned char[rsa_len+1]();
        if( inputLen-offset > MAX_DECRYPT_BLOCK ){
            decLen = RSA_private_decrypt(MAX_DECRYPT_BLOCK,
                                         (unsigned char *)ciptertext.c_str()+offset,
                                         decMsg, rsa, RSA_PKCS1_PADDING);
        }
        else{
            decLen = RSA_private_decrypt(inputLen-offset,
                                         (unsigned char *)ciptertext.c_str()+offset,
                                         decMsg, rsa, RSA_PKCS1_PADDING);
        }

        if( -1 == decLen ){
            delete [] decMsg;
            decMsg=nullptr;
            flag = false;
            break;
        }

        string cacheblock((char*)decMsg, decLen);
        plaintext.append(cacheblock);

        index++;
        offset = index*MAX_DECRYPT_BLOCK;
        delete [] decMsg;
        decMsg = nullptr;

    }
    printf("[decrypt]decrypted_plaintext_len:%d\n"
           "[decrypt]plaintext:\n%s\n", plaintext.length(), plaintext.c_str());
    RSA_free(rsa);

    return true;
}

bool Crypto::aesEncrypt(string &key, string &plaintext, string &ciphertext_base64)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    int ret = EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(),
                                 nullptr,
                                 reinterpret_cast<const unsigned char*>(key.c_str()),
                                 nullptr);
    string result;
    result.resize(plaintext.length()+AES_BLOCK_SIZE,'\0');

    int nUpdateLen = 0;
    ret = EVP_EncryptUpdate(ctx, (unsigned char*)result.c_str(),
                            &nUpdateLen,
                            reinterpret_cast<const unsigned char*>(plaintext.data()),
                            static_cast<int>(plaintext.length()));
    int nFinalLen = 0;
    ret = EVP_EncryptFinal_ex(ctx, (unsigned char*)result.c_str() + nUpdateLen, &nFinalLen);
    ret = EVP_CIPHER_CTX_cleanup(ctx);
    EVP_CIPHER_CTX_free(ctx);

    string ciphertext(result.c_str(),static_cast<size_t>(nUpdateLen+nFinalLen));

    //ASCI to BASE64
    ciphertext_base64 = base64_encode(ciphertext);

    return true;
}

bool Crypto::aesDecrypt(string &key, string &ciphertext_base64, string &plaintext)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    int ret = EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(),
                                 nullptr,
                                 reinterpret_cast<const unsigned char*>(key.c_str()),
                                 nullptr);

    string result;
    //BASE64 to ASCI
    string ciphertext = base64_decode(ciphertext_base64);
    result.resize(ciphertext.length()+AES_BLOCK_SIZE,'\0');

    int nUpdateLen = 0;
    ret = EVP_DecryptUpdate(ctx, (unsigned char*)result.c_str(),
                            &nUpdateLen,
                            reinterpret_cast<const unsigned char*>(ciphertext.data()),
                            static_cast<int>(ciphertext.length()));
    int nFinalLen = 0;
    ret = EVP_DecryptFinal_ex(ctx, (unsigned char*)result.c_str() + nUpdateLen, &nFinalLen);
    ret = EVP_CIPHER_CTX_cleanup(ctx);

    EVP_CIPHER_CTX_free(ctx);

    plaintext.append(result.c_str(),static_cast<size_t>(nUpdateLen+nFinalLen));

    return true;;
}

int Crypto::writeKeyToFile(FILE *file, int key) {

    return SUCCESS;
}

string &Crypto::formatRSAKey(string &key, KeyType keyType)
{
    trimed(key);
    size_t nKeyLen = key.size();
    for(size_t i = 64; i < nKeyLen; i+=64)
    {
        if(key[i] != '\n'){
            key.insert(i, "\n");
        }
        i++;
    }

    string strHead="-----BEGIN PUBLIC KEY-----\n";
    string strTail="\n-----END PUBLIC KEY-----\n";
    if( Private == keyType ){
        strHead="-----BEGIN PRIVATE KEY-----\n";
        strTail="\n-----END PRIVATE KEY-----\n";
    }

    if( string::npos==key.find(strHead) ){
        key.insert(0, strHead);
    }

    if( string::npos==key.find(strTail) ){
        key.append(strTail);
    }
    return key;
}

string &Crypto::unformatRASKey(string &key, KeyType keyType)
{
    trimed(key);
    //删除换行符

    string::size_type nPos = key.find('\n');
    while ( string::npos != nPos ) {
        if ( string::npos != nPos ) {
            key.replace(nPos, 1, "");
            nPos = key.find('\n');
        }
    }

    string strHead="-----BEGIN PUBLIC KEY-----";
    string strTail="-----END PUBLIC KEY-----";

    if( Private == keyType ){
        strHead="-----BEGIN PRIVATE KEY-----";
        strTail="-----END PRIVATE KEY-----";
    }

    nPos = key.find(strHead);
    if( string::npos != nPos ){
        key.erase(nPos,strHead.length());
    }

    nPos = key.find(strTail);
    if( string::npos != nPos ){
        key.erase(nPos,strTail.length());
    }

    return key;
}

string &Crypto::trimed(string &s)
{
    if( s.empty() )
        return s;

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ")+1);

    return s;
}

string &Crypto::trimedLeft(string &s)
{
    if( s.empty() )
        return s;

    s.erase(0,s.find_first_not_of(" "));

    return s;
}

string &Crypto::trimedRight(string &s)
{
    if( s.empty() )
        return s;

    s.erase(s.find_last_not_of(" ")+1);

    return s;
}

bool Crypto::bio2string(BIO *bio, unsigned char **string)
{
    size_t bioLength = BIO_pending(bio);
    *string = (unsigned char*)malloc(bioLength + 1);
    if(string == NULL) {
        return false;
    }
    BIO_read(bio, *string, bioLength);
    // Insert the NUL terminator
    (*string)[bioLength] = '\0';
    BIO_free_all(bio);
    return true;
}
