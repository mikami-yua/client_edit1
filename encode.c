#include"encode.h"

void get_random_w(char* random_str)
{
	srand(time(NULL));
	int i;
	int random_len = 16;
	for (i = 0; i < random_len; ++i)
	{
		switch ((rand() % 3))
		{
		case 1:
			random_str[i] = 'A' + rand() % 26;
			break;
		case 2:
			random_str[i] = 'a' + rand() % 26;
			break;
		default:
			random_str[i] = '0' + rand() % 10;
			break;
		}
	}
	random_str[random_len] = '\0';
}


/*
加密模块，主要用于计算alpha，bata，以及alpha_
假定有一个token，一个加密的token应该时16个字节（前面使用hash把长度固定了）
ps：关于ot的想法，ot仅出现在需要进行setup的步骤，不需要setup的时候不需要进行ot。ot的目的也仅仅是为了确定规则
*/
void get_alpha(char* token,char* alpha) {
	char w[17];
	memset(w, 0, sizeof(w));
	get_random_w(w);
	for (int i = 0; i < 16; i++) {
		alpha[i] = w[i] ^ token[i];
	}
	alpha[16] = '\0';
}


int main() {
	char* a_token = "aaaabbbbccccdddd";
	char b_token[17];
	memset(b_token, 0, sizeof(17));
	get_alpha(a_token, &b_token);//调用函数时注意取地址
	printf("%s\n", b_token);
	printf("%d\n", sizeof(b_token));
	printf("hello");
	system("pause");
}