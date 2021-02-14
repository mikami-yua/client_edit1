
//int main1()
//{
//    OpenSSL_add_all_algorithms();
//    ERR_load_crypto_strings();
//
//    /* Set up the key and iv. Do I need to say to not hard code these in a real application? :-) */
//
//    /* A 256 bit key */
//    static const unsigned char key[] = "01234567890123456789012345678901";//这里的key将来扩充成数组，用于不断的生成会话密钥，验证多次连接程序的可靠性
//
//    /* A 128 bit IV */
//    static const unsigned char iv[] = "0123456789012345";
//
//    /* Message to be encrypted */
//    unsigned char plaintext[] = "The quick brown fox jumps over the lazy dog";
//
//    /* Some additional data to be authenticated */
//    static const unsigned char aad[] = "Some AAD data";
//
//    /* Buffer for ciphertext. Ensure the buffer is long enough for the
//     * ciphertext which may be longer than the plaintext, dependant on the
//     * algorithm and mode
//     */
//    unsigned char ciphertext[128];
//
//    /* Buffer for the decrypted text */
//    unsigned char decryptedtext[128];
//
//    /* Buffer for the tag */
//    unsigned char tag[16];
//
//
//    int decryptedtext_len = 0, ciphertext_len = 0;
//
//    /* Encrypt the plaintext */
//    ciphertext_len = encrypt(plaintext, strlen(plaintext), aad, strlen(aad), key, iv, ciphertext, tag);
//
//    /* Do something useful with the ciphertext here */
//    printf("Ciphertext is:\n");
//    BIO_dump_fp(stdout, ciphertext, ciphertext_len);
//    //printf("Ciphertext is:%s\n", ciphertext);
//    BIO_dump_fp(stdout, ciphertext, sizeof(ciphertext));
//    printf("Tag is:\n");
//    BIO_dump_fp(stdout, tag, 14);
//    //printf("Tag is:%s\n", tag);
//    printf("aad is:\n");
//    BIO_dump_fp(stdout, aad, sizeof(aad));//aad加密之后依然可见
//
//    /*
//    对于aad加密之后依然可见，现在有两种思路
//        1、消息和token合1，token放在aad的位置发送给服务器
//        2、尝试去除aad部分
//
//    */
//
//
//    /* Mess with stuff */
//    /* ciphertext[0] ^= 1; */
//    /* tag[0] ^= 1; */
//
//    /* Decrypt the ciphertext */
//    decryptedtext_len = decrypt(ciphertext, ciphertext_len, aad, strlen(aad), tag, key, iv, decryptedtext);
//
//    if (decryptedtext_len < 0)
//    {
//        /* Verify error */
//        printf("Decrypted text failed to verify\n");
//    }
//    else
//    {
//        /* Add a NULL terminator. We are expecting printable text */
//        decryptedtext[decryptedtext_len] = '\0';
//
//        /* Show the decrypted text */
//        printf("Decrypted text is:\n");
//        printf("%s\n", decryptedtext);
//        printf("Decrypted text is:\n");
//        printf("%s\n", aad);//aad可以被解密，可用来实现签名验证
//    }
//
//    /* Remove error strings */
//    ERR_free_strings();
//
//    return 0;
//}
//
//void handleErrors(void)
//{
//    unsigned long errCode;
//
//    printf("An error occurred\n");
//    while (errCode = ERR_get_error())
//    {
//        char* err = ERR_error_string(errCode, NULL);
//        printf("%s\n", err);
//    }
//    abort();
//}
//
//int encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* aad,
//    int aad_len, unsigned char* key, unsigned char* iv,
//    unsigned char* ciphertext, unsigned char* tag)
//{
//    EVP_CIPHER_CTX* ctx = NULL;
//    int len = 0, ciphertext_len = 0;
//
//    /* Create and initialise the context */
//    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//    /* Initialise the encryption operation. */
//    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
//        handleErrors();
//
//    /* Set IV length if default 12 bytes (96 bits) is not appropriate */
//    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
//        handleErrors();
//
//    /* Initialise key and IV */
//    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();
//
//    /* Provide any AAD data. This can be called zero or more times as
//     * required
//     */
//    if (aad && aad_len > 0)
//    {
//        if (1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
//            handleErrors();
//    }
//
//    /* Provide the message to be encrypted, and obtain the encrypted output.
//     * EVP_EncryptUpdate can be called multiple times if necessary
//     */
//    if (plaintext)
//    {
//        if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
//            handleErrors();
//
//        ciphertext_len = len;
//    }
//
//    /* Finalise the encryption. Normally ciphertext bytes may be written at
//     * this stage, but this does not occur in GCM mode
//     */
//    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
//    ciphertext_len += len;
//
//    /* Get the tag */
//    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
//        handleErrors();
//
//    /* Clean up */
//    EVP_CIPHER_CTX_free(ctx);
//
//    return ciphertext_len;
//}
//
//int decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* aad,
//    int aad_len, unsigned char* tag, unsigned char* key, unsigned char* iv,
//    unsigned char* plaintext)
//{
//    EVP_CIPHER_CTX* ctx = NULL;
//    int len = 0, plaintext_len = 0, ret;
//
//    /* Create and initialise the context */
//    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//    /* Initialise the decryption operation. */
//    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
//        handleErrors();
//
//    /* Set IV length. Not necessary if this is 12 bytes (96 bits) */
//    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
//        handleErrors();
//
//    /* Initialise key and IV */
//    if (!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();
//
//    /* Provide any AAD data. This can be called zero or more times as
//     * required
//     */
//    if (aad && aad_len > 0)
//    {
//        if (!EVP_DecryptUpdate(ctx, NULL, &len, aad, aad_len))
//            handleErrors();
//    }
//
//    /* Provide the message to be decrypted, and obtain the plaintext output.
//     * EVP_DecryptUpdate can be called multiple times if necessary
//     */
//    if (ciphertext)
//    {
//        if (!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
//            handleErrors();
//
//        plaintext_len = len;
//    }
//
//    /* Set expected tag value. Works in OpenSSL 1.0.1d and later */
//    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
//        handleErrors();
//
//    /* Finalise the decryption. A positive return value indicates success,
//     * anything else is a failure - the plaintext is not trustworthy.
//     */
//    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
//
//    /* Clean up */
//    EVP_CIPHER_CTX_free(ctx);
//
//    if (ret > 0)
//    {
//        /* Success */
//        plaintext_len += len;
//        return plaintext_len;
//    }
//    else
//    {
//        /* Verify failed */
//        return -1;
//    }
//}

