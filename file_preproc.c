#include"token.h"

/*
从txt中读取一个规则集,txt文件使用绝对路径
@rule_path:规则的存储路径
@rule_set：返回存储规则的二维数组

UTF-8编码下一个中文占3个节https://zhidao.baidu.com/question/9196796.html
*/
void rule_read(char *rule_path,char (*rule_set)[17]) {
	char szTest[17] = { 0 };
	//int len = 0;
	FILE* fp = fopen(rule_path, "r");
	if (NULL == fp){
		printf("failed to open 1.txt\n");
		return;
	}
	int array_cnt = 0;
	while (!feof(fp)){
		memset(szTest, 0, sizeof(szTest));
		fgets(szTest, 17, fp); // 包含了换行符          
		//printf("%s", szTest);
		//printf("-----%d\n", strlen(szTest));
		memcpy(rule_set[array_cnt], szTest,strlen(szTest)-1);
		//strcpy(rule_set[array_cnt], szTest);
		array_cnt++;
	}
	fclose(fp);
	printf("\n");
}




//int main() {
//	printf("规则集的读取------------------------------------\n");
//	char* rule_path = "E:\\c_project\\rule_set\\1.txt";
//	char rule_set[20][17];
//	memset(rule_set, 0, sizeof(rule_set));
//	for (int i = 0; i < 20; i++) {
//		printf("规则%d:%s\n", i, rule_set[i]);
//	}
//	rule_read(rule_path,rule_set);
//	for (int i = 0; i < 20; i++) {
//		printf("规则%d:%s  %d\n", i, rule_set[i],strlen(rule_set[i]));//strlen计算字符串长度时候不计算’\0’在内
//	}
//	printf("规则集的读取------------------------------------\n");
//
//	system("pause");
//	return 0;
//}