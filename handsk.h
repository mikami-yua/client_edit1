#pragma once
#ifndef HANDSK_H
#define HANDSK_H
/*  CS架构
* s							c
1.请求协议版本				请求协议版本
2.创建socket				创建socket
3.创建协议地址族（ip端口协议） 获取服务器的协议地址族
4.绑定
5.监听
6.等待客户端连接			4.连接服务器
7.通信						5，通信
8.关闭socket				6.关闭socket
9.清理协议					7.清理协议

多线程解决多客户端通信问题
	客户端连接到服务器，启动一个线程和客户端通信
	线程：接收客户端发送的数据
	把数据发送给其他的客户端


*/

#include"im_client.h"

typedef struct handsk_key {
	char* ipaddr;//连接服务器时由client提供serv的ip，这里提前判断以下是否已有即可
	char key_array[128][2][17];//128对，每对两个，每个128bit 16个char长
}HAND_KEY;

void ot_msg(char* irl_msg1, char* irl_msg2);

#endif // !HANDSK_H
