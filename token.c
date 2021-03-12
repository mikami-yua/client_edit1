#include"token.h"
#include<openssl/md5.h>
#include"cpptest.h"

//#include "cppjieba/Jieba.hpp"


/*
实现类似java静态代码块的功能
*/
//#define main main(){init();/##/

/*
计算字符串的hash值  使用md5函数可以生成128bit的哈希值
*/
void hash_compute(char* data,unsigned char *ret) {
    MD5_CTX ctx;
    char buf[33] = { '\0' };
    char tmp[3] = { '\0' };
    unsigned char md5[17] = {'\0'}; //md5加密结果为16个范围为0-255的数，所以用的unsigned char，要变为16进制，需要32个char
    memset(md5, 0, sizeof(md5));
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, strlen(data));
    MD5_Final(md5, &ctx);
    memset(ret, 0, 17);
    memcpy(ret, md5, 17);
    //for (int i = 0; i < 16; i++) {
    //    printf("%x ", md5[i]);
    //}
    //printf("\n");

    ///for (int i = 0; i < 16; i++) {
    ///    printf("%c", md5[i]);
    ///}
    ///printf("%c\n", '|');
    //printf("\n%d\n",strlen(md5));
    //for (int i = 0;i < 16; i++){
    //    sprintf(tmp, "%02x", md5[i]);
    //    strcat(buf, tmp);
    //}
    //printf("%s", buf);
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

//把一个超长字符串（一段话）转换为token串-----使用分词技术
void jieba_test() {
    
    

}

////////////////////////////////////////////////////////////////////

//
//void CutWithoutTagNameDemo() {
//    printf("CutWithoutTagNameDemo:\n");
//    // init will take a few seconds to load dicts.
//    Jieba handle = NewJieba(DICT_PATH, HMM_PATH, USER_DICT, IDF_PATH, STOP_WORDS_PATH);
//
//    const char* s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。l";
//    size_t len = strlen(s);
//    CJiebaWord* words = CutWithoutTagName(handle, s, len, "x");
//    CJiebaWord* x;
//    for (x = words; x->word; x++) {
//        printf("%*.*s\n", x->len, x->len, x->word);
//    }
//    FreeWords(words);
//    FreeJieba(handle);
//}
//
//void ExtractDemo() {
//    printf("ExtractDemo:\n");
//
//    // init will take a few seconds to load dicts.
//    Extractor handle = NewExtractor(DICT_PATH,
//        HMM_PATH,
//        IDF_PATH,
//        STOP_WORDS_PATH,
//        USER_DICT);
//
//    const char* s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。l";
//    size_t top_n = 5;
//    CJiebaWord* words = Extract(handle, s, strlen(s), top_n);
//    CJiebaWord* x;
//    for (x = words; x && x->word; x++) {
//        printf("%*.*s\n", x->len, x->len, x->word);
//    }
//    FreeWords(words);
//    FreeExtractor(handle);
//}
//
//void UserWordDemo()
//{
//    printf("UserWordDemo:\n");
//    Jieba handle = NewJieba(DICT_PATH, HMM_PATH, USER_DICT, IDF_PATH, STOP_WORDS_PATH);
//
//    const char* s = "人艰不拆";
//    size_t len = strlen(s);
//
//    CJiebaWord* words = Cut(handle, s, len);
//    CJiebaWord* x;
//    for (x = words; x->word; x++) {
//        printf("%*.*s\n", x->len, x->len, x->word);
//    }
//    FreeWords(words);
//
//    JiebaInsertUserWord(handle, "人艰不拆");
//    words = Cut(handle, s, len);
//    for (x = words; x->word; x++) {
//        printf("%*.*s\n", x->len, x->len, x->word);
//    }
//    FreeWords(words);
//
//    FreeJieba(handle);
//}


////////////////////////////////////////////////////////////////////

/*
需要对不足16byte的字符串补偿-----并且返回无符号的字符数组
@msg：任意长度的token
@untoken_16:返回16字节的无符号数组(算上\0 17长度)
方案1：使用0补齐
方案2：使用md5计算为16字节------暂定使用方案2
*/
void get_string_16(char *msg, unsigned char* untoken_16) {
    unsigned char show[17] = { '\0' };
    memset(show, 0, 17);
    hash_compute(msg, show);
    memset(untoken_16, 0, 16);
    memcpy(untoken_16, show, sizeof(show));
}


int main() {
    char* str = "规则集k";
    unsigned char show[17] = {'\0'};
    get_string_16(str, show);

    
    for (int i = 0; i < 16; i++) {
        printf("%x ", show[i]);
    }
    printf("\n");
    //get_random_table(11);

    test66();
    /*
    c中调用cpp函数：https://www.cnblogs.com/zj-blogs/p/11161953.html
    */
    test67();

	system("pause");
	return 0;
}