#include"cpptest.h"
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include<string>
#include<fstream>
#include "cppjieba/Jieba.hpp"
/*
使用结巴需要
拷贝include dept dict到项目目录
在项目属性的include中设置设置include dept

如果使用分词，如何防止客户端不规矩的分词（好像原来的版本也没有办法防止client不规矩的取token）
*/


void test66() {
	char ch0[] = { "我喜欢吃苹果和apple banana" };
	const char* const DICT_PATH = "dict/jieba.dict.utf8";
	const char* const HMM_PATH = "dict/hmm_model.utf8";
	const char* const USER_DICT_PATH = "dict/user.dict.utf8";
	const char* const IDF_PATH = "dict/idf.utf8";
	const char* const STOP_WORD_PATH = "dict/stop_words.utf8";
	printf("加载dict...\n");
	
	cppjieba::Jieba jieba(DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH);
	printf("加载完成...\n");
	vector<string> words;
	vector<cppjieba::Word> jiebawords;
	string s = ch0;
	string result;
	cout << "切分" << endl;
	jieba.Cut(s, words, true);


	for (vector<string>::iterator vit = words.begin(); vit != words.end(); vit++) {
		cout << *vit << " ";
	}
	

	//ofstream file("result.txt");
	//file << limonp::Join(words.begin(), words.end(), "/") << endl;
	//file.close();
}