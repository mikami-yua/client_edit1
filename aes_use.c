#include"aes.h"
#include"ot.h"
/*
实现任意长度的aes加解密

*/

int main(void)
{
    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */

     /* A 128 bit key */
    unsigned char* key = (unsigned char*)"0123456789012345";//这里的key将来扩充成数组，用于不断的生成会话密钥，验证多次连接程序的可靠性（6789012345678901）

    /* A 128 bit IV */
    unsigned char* iv = (unsigned char*)"0123456789012345";//AES的CBC加密模式，默认iv是16个0（这个称为初始化向量），由于是分组加密，所以下一组的iv，就用前一组的加密shu的密文来充当。

    /* Message to be encrypted */
    unsigned char* plaintext =
        (unsigned char*)"The quick brown fox jumps over the lazy dog";

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    unsigned char ciphertext[512];

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[512];

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
    ciphertext_len = encrypt(plaintext, strlen((char*)plaintext), key, iv,
        ciphertext);

    /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp(stdout, (const char*)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
        decryptedtext);

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    /* Show the decrypted text */
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);

    printf("*************************************RSA******************************\n");
    char nn[100];
    char ee[100];
    char dd[100];
    ot_send_rsa_msg(nn,ee,dd);
    //printf("%s\n", aa);
    //printf("%s\n", bb);
    //printf("%s\n", cc);
    //printf("%d\n", strlen(cc)); strlen(cc)显示真实长度
    printf("******************************RANDOM MSG******************************\n");
    char msg_r1[100];//128位
    char msg_r2[100];
    ot_send_rand_msg(msg_r1,msg_r2);
    printf("%s\n", msg_r1);
    printf("%s\n", msg_r2);
    printf("******************************compute v******************************\n");
    char v[100];
    char k[100];
    ot_recv_compute_v(v,k,nn,ee,msg_r1,msg_r2);
    printf("V:%s\n", v);
    printf("K:%s\n", k);
    printf("******************************compute ki******************************\n");
    char k1[100];
    char k2[100];
    ot_compute_ki_msg(k1, k2,v,msg_r1,msg_r2,dd,nn);
    printf("%s\n", k1);
    printf("%s\n", k2);
    //printf("%d\n", strlen(k1));
    /*
    k1,k2作为key对真正的两个msg进行加密
    */
    printf("******************************msg encode******************************\n");
    char enmsg1[100];
    char enmsg2[100];
    char* irl_msg1 = "hello lili";
    char* irl_msg2 = "are you ok?";
    ot_encode_msg(enmsg1,enmsg2,k1,k2,irl_msg1,irl_msg2);
    printf("%s\n", enmsg1);
    printf("%s\n", enmsg2);
    printf("******************************msg decode******************************\n");
    char demsg1[100];
    char demsg2[100];
    ot_decode_msg(demsg1,demsg2,enmsg1,enmsg2,k);
    printf("%s\n", demsg1);
    printf("%s\n", demsg2);
    


    return 0;
}
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}
/*
加密函数
@plaintext：明文
@plaintext_len：明文的长度
@key：加密密钥k
@iv：初始化向量
@ciphertext：【最终结果】密文
*/
int encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key,
    unsigned char* iv, unsigned char* ciphertext)
{
    EVP_CIPHER_CTX* ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}
/*
解密函数
@ciphertext：密文
@ciphertext_len：密文的长度
@key：加密密钥k
@iv：初始化向量
@plaintext：【最终结果】明文
*/
int decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key,
    unsigned char* iv, unsigned char* plaintext)
{
    EVP_CIPHER_CTX* ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

/*
加密函数(128位)
@plaintext：明文
@plaintext_len：明文的长度
@key：加密密钥k
@iv：初始化向量
@ciphertext：【最终结果】密文
*/
int encrypt_lite(unsigned char* plaintext, int plaintext_len, unsigned char* key,
    unsigned char* iv, unsigned char* ciphertext)
{
    EVP_CIPHER_CTX* ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 128 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}
/*
解密函数（128位）
@ciphertext：密文
@ciphertext_len：密文的长度
@key：加密密钥k
@iv：初始化向量
@plaintext：【最终结果】明文
*/
int decrypt_lite(unsigned char* ciphertext, int ciphertext_len, unsigned char* key,
    unsigned char* iv, unsigned char* plaintext)
{
    EVP_CIPHER_CTX* ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 128 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}