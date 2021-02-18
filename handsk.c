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
*/
void ot_msg() {
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
	printf("%s", send1);
	while (1)
	{
		;
	}
	WSACleanup();

	//【问题】：问什么需要debug两次，exe文件才会更新。猜测：和工程的组织方式有关

	//5.通信
	//char buff[1024];
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)get_msg, NULL, NULL, NULL);//创建线程，第三个参数写函数名，其他全部填NULL

	//子线程的作用主要就是接收消息
	/*while (1) {
		memset(buff, 0, 1024);
		printf("please input:");
		scanf("%s", buff);
		r = send(clientSOCKET, buff, strlen(buff), NULL);
	}*/
	/*本地ot的过程
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
    
    //k1,k2作为key对真正的两个msg进行加密
    
	printf("******************************msg encode******************************\n");
	char enmsg1[100];
	char enmsg2[100];
	char* irl_msg1 = "hello lili";
	char* irl_msg2 = "are you ok?";
	ot_encode_msg(enmsg1, enmsg2, k1, k2, irl_msg1, irl_msg2);
	printf("%s\n", enmsg1);
	printf("%s\n", enmsg2);
	printf("******************************msg decode******************************\n");
	char demsg1[100];
	char demsg2[100];
	ot_decode_msg(demsg1, demsg2, enmsg1, enmsg2, k);
	printf("%s\n", demsg1);
	printf("%s\n", demsg2);
	*/

	
}

int main() {
	//main_loop();
	ot_msg();


	return 0;
}

//78min