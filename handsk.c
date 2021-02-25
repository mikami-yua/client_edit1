#include"handsk.h"
#include"ot.h"
#include <time.h>
SOCKET clientSOCKET;//全局的socket

int random_unique = 11;

void get_msg() {
	char recv_buff[1024];
	int r;
	while (1) {
		memset(recv_buff, 0, sizeof(recv_buff));
		r = recv(clientSOCKET, recv_buff, 1023, NULL);
		if (r > 0) {
			recv_buff[r] = 0;
			printf("recv from serv:%s\n", recv_buff);
		}
	}
}

void main_loop() {
	//1.请求协议版本
	WSADATA wsaDATA;
	WSAStartup(MAKEWORD(2, 2), &wsaDATA);
	if (LOBYTE(wsaDATA.wVersion) != 2 || HIBYTE(wsaDATA.wVersion) != 2) {
		printf("protocol error!\n");
		return -1;
	}
	//2.创建socket
	clientSOCKET = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数1 通信协议的类型  参数2 tcp、udp  参数3 保护方式
	if (SOCKET_ERROR == clientSOCKET) {//SOCKET_ERROR=-1
		printf("init socket error\n");
		WSACleanup();
		return -2;
	}

	//3.获取服务器协议地址族（手动确定服务器的，只需要和服务器写的一样即可）
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//协议版本
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//ip地址的本质是个整数可以使用本机ip（使用别的ip会导致绑定失败）
	addr.sin_port = htons(10086);//端口65536  10000左右的

	//4.连接服务器
	int r = connect(clientSOCKET, (struct sockaddr*)&addr, sizeof(addr));
	if (-1 == r) {
		printf("connect error\n");
		return -1;
	}

	//5.通信
	char buff[1024];
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)get_msg, NULL, NULL, NULL);//创建线程，第三个参数写函数名，其他全部填NULL
	//子线程的作用主要就是接收消息
	while (1) {
		memset(buff, 0, 1024);
		printf("please input:");
		scanf("%s", buff);
		r = send(clientSOCKET, buff, strlen(buff), NULL);
	}
}


void get_random_str(char* random_str, const int random_len)
{
	srand(time(NULL)+random_unique);
	int i;
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
	random_unique++;
}

/*
函数连接服务器，简化所有使用socket的函数
*/
void socket_conn() {
	//1.请求协议版本
	WSADATA wsaDATA;
	WSAStartup(MAKEWORD(2, 2), &wsaDATA);
	if (LOBYTE(wsaDATA.wVersion) != 2 || HIBYTE(wsaDATA.wVersion) != 2) {
		printf("protocol error!\n");
		return -1;
	}
	//2.创建socket
	clientSOCKET = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数1 通信协议的类型  参数2 tcp、udp  参数3 保护方式
	if (SOCKET_ERROR == clientSOCKET) {//SOCKET_ERROR=-1
		printf("init socket error\n");
		WSACleanup();
		return -2;
	}

	//3.获取服务器协议地址族（手动确定服务器的，只需要和服务器写的一样即可）
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//协议版本
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//ip地址的本质是个整数可以使用本机ip（使用别的ip会导致绑定失败）
	addr.sin_port = htons(10086);//端口65536  10000左右的

	//4.连接服务器
	int r = connect(clientSOCKET, (struct sockaddr*)&addr, sizeof(addr));//addr就是服务器的地址族
	if (-1 == r) {
		printf("connect error\n");
		return -1;
	}
}

/*
socket 清理函数
*/
void socket_clean() {
	closesocket(clientSOCKET);
	WSACleanup();
}

/*
一个函数进行ot-----ot函数的入口应该是两个随机的字符串，没有出口，出口在serv端
发送的参数在一个结构体中，接收的数据也在一个结构体中
在一个结构体中开销小，实现时需要解决：1.buff的长度问题，buff中使用分隔符分割各个位的问题

需要提供参数：真实消息1，真实消息2.什么都不得到
*/
void ot_msg(char* irl_msg1,char * irl_msg2) {
	//char* irl_msg1 = "hello lili";//真实消息
	//char* irl_msg2 = "are you ok?";

	int r;
	char send1[1024];//包装ot第一次传送的buff
	memset(send1, 0, sizeof(send1));

	char nn[100];
	char ee[100];
	char dd[100];//dd保存在本地
	ot_send_rsa_msg(nn, ee, dd);
	char msg_r1[100];
	char msg_r2[100];
	ot_send_rand_msg(msg_r1, msg_r2);//得到两个随即消息

	//int offset = 0;
	strcat(send1, nn);
	strcat(send1, "/");
	strcat(send1, ee);
	strcat(send1, "/");
	strcat(send1, msg_r1);
	strcat(send1, "/");
	strcat(send1, msg_r2);
	r = send(clientSOCKET, send1, strlen(send1), NULL);
	//printf("%s\n", send1);
	//接收v
	char v[100];
	memset(v, 0, sizeof(v));
	recv(clientSOCKET, v, 99, NULL);
	printf("get rev:%s\n", v);
	//printf("v:%s\n", v);
	char k1[100];
	char k2[100];
	ot_compute_ki_msg(k1, k2, v, msg_r1, msg_r2, dd, nn);
	//printf("%s\n", k1);
	//printf("%s\n", k2);
	//对真实消息进行加密
	char enmsg1[100];
	char enmsg2[100];
	ot_encode_msg(enmsg1, enmsg2, k1, k2, irl_msg1, irl_msg2);
	//把两个真实消息发送给serv
	char send2[1024];
	memset(send2, 0, sizeof(send2));

	//真实消息含有\0，需要手动拷贝
	
	printf("%s\n", enmsg1);
	printf("%s\n", enmsg2);

	strcat(send2, enmsg1);
	strcat(send2, "/");
	strcat(send2, enmsg2);
	send(clientSOCKET, send2, strlen(send2), NULL);
	//【问题】：问什么需要debug两次，exe文件才会更新。猜测：和工程的组织方式有关
}

void binarystring(char c)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		if (c & 0x80) putchar('1'); else putchar('0');
		c <<= 1;
	}
}


/*
实现函数：
对于一个128位的字符串，生成对应的128个0、1  在ot中这256个消息都属于真实信息，需要serv进行选择
itoa(整型数据，目标字符串，进制)不能自动补0


这个函数应该在serv端，serv端根据rule生成的01，输入01

大ot函数，每次输入128个0、1
client端每次输入256个随机串对应这些0.1
*/
void string2bin(char *cc) {
	char bin[129];//多一位放\0
	memset(bin, 0, sizeof(bin));
	for (int i = 0; i < strlen(cc); i++) {
		char c = cc[i];
		for (int j = 0; j < 8; j++) {
			if (c & 0x80) bin[i * 8 + j] = '1';
			else bin[i * 8 + j] = '0';
			c <<= 1;
		}
	}
	printf("bin:%s\nbinlen:%d", bin, strlen(bin));
}



/*
生成永不改变的r，128bit
@random_r:生成的128bit数（以0101的方式放在char中）

只要char是满的，不是aecii码式的填充，就能直接异或
*/
void generate_r(char *random_r) {
	//BIGNUM* rnd;
	//rnd = BN_new();
	////BN_random
	//int bits = 128;
	//int top = 0;
	//int bottom = 0;
	//BN_rand(rnd, bits, top, bottom);
	//char show2[17];
	//memset(show2, 0, sizeof(show2));
	//BN_bn2bin(rnd, show2);
	//while (strlen(show2) != 16)
	//{
	//	BN_rand(rnd, bits, top, bottom);
	//	memset(show2, 0, sizeof(show2));
	//	BN_bn2bin(rnd, show2);
	//}
	////printf("%s\n", show2);//嶏_Mg菕gZOQd ~"_
	////printf("%d\n", strlen(show2));//16
	//strcpy(random_r, show2);
	///*
	//每个char都异或一个char-------------------代码块测试可行，用char*保存生成的01串即可
	//
	//char* irl_msg1 = "abcdefghijkl1234";
	//char buf[17];
	//memset(buf, 0, sizeof(buf));
	//for (int i = 0; i < 16; i++) {
	//	buf[i] = irl_msg1[i] ^ show2[i];
	//}
	//printf("%s\n",buf);
	//char ans[17];
	//memset(ans, 0, sizeof(ans));
	//for (int i = 0; i < 16; i++) {
	//	ans[i] = buf[i] ^ show2[i];
	//}
	//printf("%s\n", ans);
	//*/
	//BN_free(rnd);//而是每4位组成一个十进制数储存在to中

	get_random_str(random_r, 16);
}

/*
生成128个向量，使它们的异或之和为random_r
@char *random_r:固定的异或之和值
@char** r_vector:返回128个随机串，每个串的长度是128

直接生成时间太长了，不可行，随机生成127组，最后一组匹配
*/
void generate_r_vector(char (*r_vector)[17],char *random_r) {
	//BIGNUM* vector[127];
	//char r_v[128][17];
	//char flag[17];//判断是否一致
	//memset(flag, 0, sizeof(flag));
	//int bits = 128;
	//int top = 0;
	//int bottom = 0;
	//for (int i = 0; i < 127; i++) {
	//	vector[i] = BN_new();
	//	BN_rand(vector[i], bits, top, bottom);
	//	memset(r_v[i], 0, sizeof(r_v[i]));
	//	BN_bn2bin(vector[i], r_v[i]);
	//}
	//memset(r_v[127], 0, sizeof(r_v[127]));
	//for (int i = 0; i < 127; i++) {
	//	for (int j = 0; j < 16; j++) {
	//		flag[j] = flag[j] ^ r_v[i][j];
	//	}
	//}
	//for (int i = 0; i < 16; i++) {
	//	r_v[127][i] = flag[i] ^ random_r[i];
	//}
	///*char ans[17];
	//memset(ans, 0, sizeof(ans));
	//for (int i = 0; i < 128; i++) {
	//	for (int j = 0; j < 16; j++) {
	//		ans[j] = ans[j] ^ r_v[i][j];
	//	}
	//}
	//printf("===%s\n", ans);*/
	//
	//for (int i = 0; i < 128; i++) {
	//	memcpy(r_vector[i], r_v[i], 17);//逐字节拷贝解决问题strcpy会出现问题，原因未知
	//}
	char flag[17];//判断是否一致
	memset(flag, 0, sizeof(flag));
	for (int i = 0; i < 127; i++) {
		get_random_str(r_vector[i], 16);
	}
	for (int i = 0; i < 127; i++) {
		for (int j = 0; j < 16; j++) {
			flag[j] = flag[j] ^ r_vector[i][j];
		}
	}
	for (int i = 0; i < 16; i++) {
		r_vector[127][i] = flag[i] ^ random_r[i];
	}
}




/*
产生连接key数字的函数，输入ip，产生128对key,都存放在一个结构体中HAND_KEY
@key:返回的带有ip和key_array的结构体
@ipadd：输入的ip地址
*/
void generate_key_array(HAND_KEY *key,char *ipadd) {//这里的key就是外面真实的key，不需要copy即可传值
	key->ipaddr = ipadd;
	char a1[17], a2[17];
	//生成128对 17 长的随机串
	for (int i = 0; i < 128; i++) {
		memset(a1, 0, sizeof(a1));
		memset(a2, 0, sizeof(a2));
		
		generate_r(a1);
		generate_r(a2);
		
		memcpy(key->key_array[i][0], a1, sizeof(a1));
		memcpy(key->key_array[i][1], a2, sizeof(a2));
	}
}

/*
进行128次 每次两个128bit字符串的 ot
现在已有的ot可以进行一次2选1，黄的论文：128次每次得到128bit字符串的ot
需要进行128次ot
可能存在问题：上次ot没有结束，就开始进行下一次ot了

把128组char* 数组放在ot_msg中，另一边接收128个


两个参数：真实消息向量1，真实消息向量2
*/
void ot_128_send(char (*irl_megs1)[17], char(*irl_megs2)[17]) {
	char buf1[17], buf2[17];
	for (int i = 0; i < 128; i++) {
		//memset(buf1, 0, sizeof(buf1));
		//memset(buf2, 0, sizeof(buf2));
		//memcpy(buf1, irl_megs1[i], 17);
		//memcpy(buf2, irl_megs2[i], 17);
		//printf("%d\n", i);
		//printf("%s\n", irl_megs1[i]);
		//printf("%s\n", irl_megs2[i]);
		//printf("%s\n", buf1);
		//printf("%s\n", buf2);
		//printf("\n");
		ot_msg(irl_megs1[i], irl_megs2[i]);
	}
}

/*
client端进行多少次128ot是由serv端确定的
serv端根据rule的数量发送进行多少组ot的请求，serv端发送ot数量，client端接收到数量后进行ot，每个r都新生成k是不变的
返回serv希望进行ot的次数
*/
int client_handsk_ot() {
	int nums;
	//memset(nums, 0, sizeof(nums));
	recv(clientSOCKET,&nums, sizeof(nums), NULL);
	printf("ot times:%d\n", nums);
	return nums;
}


int main() {//socket的设置函数独立出来（至少在解决128ot之后再考虑这个问题）
	//main_loop();
	
	socket_conn();
	printf("service readlly!\n");
	char* irl_msg1 = "abcdefghijkl1234";//真实消息 显示的16实际长度是15 有\0占一位
	char* irl_msg2 = "are you ok?";
	//ot_msg(irl_msg1,irl_msg2);
	//string2bin(irl_msg1);

	//char random[17];
	//memset(random, 0, sizeof(random));
	//generate_r(random);
	//char r_v[128][17];
	//memset(r_v, 0, sizeof(r_v));
	//generate_r_vector(r_v,random);
	/*
	//char ans[17];test
	//memset(ans, 0, sizeof(ans));
	//for (int i = 0; i < 128; i++) {
	//	for (int j = 0; j < 16; j++) {
	//		ans[j] = ans[j] ^ r_v[i][j];
	//	}
	//}
	//printf("%s\n", ans);
	//printf("%d\n", strcmp(ans, random));
	*/

	int loop_time=client_handsk_ot();

	HAND_KEY handkey;
	char* ipadd = "123.12.3.4";
	generate_key_array(&handkey, ipadd);//可以直接得到
	


	/*验证生成的随机密钥数组
	for (int i = 0; i < 128; i++) {
		printf("%s\n", handkey.key_array[i][0]);
		printf("%s\n", handkey.key_array[i][1]);
		printf("\n");
	}
	*/

	/*128ot的基本步骤
	char random[17];
	memset(random, 0, sizeof(random));
	generate_r(random);
	printf("random:%s\n", random);
	char r_v1[128][17];
	memset(r_v1, 0, sizeof(r_v1));
	generate_r_vector(r_v1,random);//生成了一组
	char r_v2[128][17];
	memset(r_v2, 0, sizeof(r_v2));
	generate_r_vector(r_v2, random);//生成了一组
	//生成第二组 client在本地测试生成的两组是否能异或成目标值
	*/

	/*验证生成128个随机向量能否异或成一个
	char ans[17];
	memset(ans, 0, sizeof(ans));
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 16; j++) {
			ans[j] = ans[j] ^ r_v1[i][j];
		}
	}
	printf("%s\n", ans);
	printf("ans:%d\n", strcmp(ans, random));

	memset(ans, 0, sizeof(ans));
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 16; j++) {
			ans[j] = ans[j] ^ r_v2[i][j];
		}
	}
	printf("%s\n", ans);
	printf("%d\n", strcmp(ans, random));
	*/
	//使用ot发送
	//ot_128_send(r_v1, r_v2);
	
	//测试多次128ot的可行性
	/*
	memset(random, 0, sizeof(random));
	generate_r(random);
	printf("random:%s\n", random);
	memset(r_v1, 0, sizeof(r_v1));
	generate_r_vector(r_v1, random);//生成了一组
	memset(r_v2, 0, sizeof(r_v2));
	generate_r_vector(r_v2, random);//生成了一组
	ot_128_send(r_v1, r_v2);
	*/
	char random[17];
	memset(random, 0, sizeof(random));
	generate_r(random);
	printf("random:%s\n", random);
	char r_v1[128][17];
	memset(r_v1, 0, sizeof(r_v1));
	generate_r_vector(r_v1, random);//生成了一组
	char r_v2[128][17];
	memset(r_v2, 0, sizeof(r_v2));
	generate_r_vector(r_v2, random);//生成了一组
	for (int i = 0; i < loop_time; i++) {
		memset(random, 0, sizeof(random));
		generate_r(random);
		printf("random:%s\n", random);
		memset(r_v1, 0, sizeof(r_v1));
		generate_r_vector(r_v1, random);//生成了一组
		memset(r_v2, 0, sizeof(r_v2));
		generate_r_vector(r_v2, random);//生成了一组
		ot_128_send(r_v1, r_v2);
		ot_128_send(r_v1, r_v2);
	}

	socket_clean();
	system("pause");
	
	return 0;
}




//78min