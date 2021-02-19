#include"handsk.h"
#include"ot.h"
SOCKET clientSOCKET;//全局的socket
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

/*
一个函数进行ot-----ot函数的入口应该是两个随机的字符串，没有出口，出口在serv端
发送的参数在一个结构体中，接收的数据也在一个结构体中
在一个结构体中开销小，实现时需要解决：1.buff的长度问题，buff中使用分隔符分割各个位的问题

需要提供参数：真实消息1，真实消息2.什么都不得到
*/
void ot_msg(char* irl_msg1,char * irl_msg2) {
	//char* irl_msg1 = "hello lili";//真实消息
	//char* irl_msg2 = "are you ok?";

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
	printf("%s\n", send1);
	//接收v
	char v[100];
	memset(v, 0, sizeof(v));
	recv(clientSOCKET, v, 99, NULL);
	printf("v:%s\n", v);
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
	strcat(send2, enmsg1);
	strcat(send2, "/");
	strcat(send2, enmsg2);
	send(clientSOCKET, send2, strlen(send2), NULL);

	
	WSACleanup();

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


int main() {
	//main_loop();
	char* irl_msg1 = "abcdefghijkl1234";//真实消息 显示的16实际长度是15 有\0占一位
	char* irl_msg2 = "are you ok?";
	//ot_msg(irl_msg1,irl_msg2);
	string2bin(irl_msg1);

	system("pause");
	return 0;
}

//78min