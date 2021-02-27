#include"token.h"
#include<openssl/md5.h>

/*
计算字符串的hash值  使用md5函数可以生成128bit的哈希值
*/
void hash_compute() {
    MD5_CTX ctx;
    char* data = "12345";
    char buf[33] = { '\0' };
    char tmp[3] = { '\0' };
    unsigned char md5[16]; //md5加密结果为16个范围为0-255的数，所以用的unsigned char，要变为16进制，需要32个char
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, strlen(data));
    MD5_Final(md5, &ctx);
    
    for (int i = 0; i < 16; i++) {
        printf("%c", md5[i]);
    }
    printf("\n");
    for (int i = 0;i < 16; i++){
        sprintf(tmp, "%02x", md5[i]);
        strcat(buf, tmp);
    }
    printf("%s", buf);
}



int main() {
	printf("hello world\n");
    hash_compute();
	system("pause");
	return 0;
}