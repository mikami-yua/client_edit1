#pragma once
#ifndef FILE_PREPROC
#define FILE_PREPROC

#include"im_client.h"
/*
文件的预处理模块
客户端：从txt中读取msg
服务器端：从txt中读取rule set
*/


void rule_read(char* rule_path, char(*rule_set)[17]);


#endif // !FILE_PREPROC
