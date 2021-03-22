#pragma once
#ifndef CPPTEST_H
#define CPPTEST_H




#ifdef __cplusplus       //-----------标准写法-----------
extern "C" {             //-----------标准写法-----------
#endif                   //-----------标准写法-----------

	void test66();    //要被c函数调用
	void test67();
	void test68(char* msg_path);

#ifdef __cplusplus      //-----------标准写法-----------
}                       //-----------标准写法-----------
#endif                  //-----------标准写法-----------


#endif // !CPPTEST_H
