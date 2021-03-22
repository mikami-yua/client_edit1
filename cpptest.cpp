#include"cpptest.h"
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include<string>
#include<fstream>
#include "cppjieba/Jieba.hpp"
#include"cppjieba/KeywordExtractor.hpp"
/*
使用结巴需要
拷贝include dept dict到项目目录
在项目属性的include中设置设置include dept

如果使用分词，如何防止客户端不规矩的分词（好像原来的版本也没有办法防止client不规矩的取token）
*/

/*
在文件中写msg，通过函数读取，转化为token
使用停用词，过滤无用的token
*/

/*
c++下怎么完成初始化代码块
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

/*
使用停用词过滤提取关键祠
*/
void test67() {
	char ch0[] = { "我喜欢吃苹果和apple banana" };
	char ch1[] = { "报告提出,今年国内生产总值预期增长6%以上。设定这一预期目标，是考虑了经济运行恢复情况，有利于引导各方面集中精力推进改革创新、推动高质量发展。我国2019年预期目标增长6% —6.5 % ，2020年为应对突如其来的新冠肺炎疫情，引导各方面集中精力抓好“六稳”“六保”，没有提出GDP增速量化目标，但依然实现了2.3 % 的增长，是全球唯一实现正增长的主要经济体。这为今年的发展奠定了良好基础。同时，我国超大规模经济体优势决定了经济发展具有较强的韧性和后劲，且2020年基数相对较低，因而有实力、有底气实现今年GDP增长6 % 以上的目标。GDP预期目标，也体现了保持经济运行在合理区间的要求，与今后目标平稳衔接，有利于实现可持续健康发展。6 % 以上，既有基于抓好“六稳”“六保”的底线要求，又蕴含高质量发展的内在要求，就是既要保持一定的增速，又不将速度放在第一位，更重要的是提升增长质量，有效防范化解各类风险隐患，实现更高质量、更有效率、更可持续、更为安全的发展。a" };
	const char* const DICT_PATH = "dict/jieba.dict.utf8";
	const char* const HMM_PATH = "dict/hmm_model.utf8";
	const char* const USER_DICT_PATH = "dict/user.dict.utf8";
	const char* const IDF_PATH = "dict/idf.utf8";
	const char* const STOP_WORD_PATH = "dict/stop_words.utf8";
	cout << "加载dict" << endl;
	cppjieba::KeywordExtractor we(DICT_PATH, HMM_PATH,IDF_PATH, STOP_WORD_PATH);//可以使用5个参数，加入自定义字典
	vector<string> words;
	string s = ch1;
	cout << "切分" << endl;
	size_t size = 250;
	we.Extract(s, words, size);
	for (vector<string>::iterator vit = words.begin(); vit != words.end(); vit++) {
		cout << *vit << " ";
	}
}

/*
从文件中读取msg
@filename:消息的存储路径
@return:返回一个消息的string，注意在使用完后free这个string
*/
char* msg_read(char* filename) {
	char* text;
	FILE* pf = fopen(filename, "r");
	fseek(pf, 0, SEEK_END);
	long lSize = ftell(pf);
	// 用完后需要将内存free掉
	text = (char*)malloc(lSize + 1);
	rewind(pf);
	fread(text, sizeof(char), lSize, pf);
	text[lSize] = '\0';
	return text;
}

/*
对传入的消息使用分词技术，并返回一个string数组
*/
void test68(char *msg_path) {
	char* zf_msg;
	zf_msg = msg_read(msg_path);

	///-------------------------------------------------------
	const char* const DICT_PATH = "dict/jieba.dict.utf8";
	const char* const HMM_PATH = "dict/hmm_model.utf8";
	const char* const USER_DICT_PATH = "dict/user.dict.utf8";
	const char* const IDF_PATH = "dict/idf.utf8";
	const char* const STOP_WORD_PATH = "dict/stop_words.utf8";
	cout << "加载dict" << endl;
	cppjieba::KeywordExtractor we(DICT_PATH, HMM_PATH, IDF_PATH, STOP_WORD_PATH);//可以使用5个参数，加入自定义字典
	vector<string> words;
	string s = zf_msg;
	cout << "切分" << endl;
	size_t size = 300;
	we.Extract(s, words, size);
	for (vector<string>::iterator vit = words.begin(); vit != words.end(); vit++) {
		cout << *vit << " ";
	}
}
