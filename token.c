#include"token.h"
#include<openssl/md5.h>


/*
计算字符串的hash值  使用md5函数可以生成128bit的哈希值
*/
void hash_compute() {
    MD5_CTX ctx;
    char* data = "12345iouashdjkjzx";
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


//生成随机表
/*
产生随机表中的一行（一个随机数）
@seed：随机种子
@random_row：返回的随机数
*/
void get_random_row(int seed,char *random_row) {
    srand(seed);
    int len = 16;//128位的随机字符串
    for (int i = 0; i < len; i++) {
        switch ((rand() % 3))
        {
        case 1:
            random_row[i] = 'A' + rand() % 26;
            break;
        case 2:
            random_row[i] = 'a' + rand() % 26;
            break;
        default:
            random_row[i] = '0' + rand() % 10;
            break;
        }
    }
    random_row[len] = '\0';
}
/*生成随机数表
@or_seed：原始种子
*/
void get_random_table(int or_seed) {
    char random_str[100][17];
    memset(random_str, 0, sizeof(random_str));
    for (int i = 0; i < 100; i++) {
        get_random_row(or_seed + i, random_str[i]);
    }
    for (int i = 0; i < 100; i++) {
        printf("%s\n", random_str[i]);
    }
}

//把一个超长字符串（一段话）转换为token串


int main() {
	printf("hello world\n");
    hash_compute();
    get_random_table(11);
	system("pause");
	return 0;
}