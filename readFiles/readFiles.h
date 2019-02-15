#pragma once
#ifndef readFiles_H
#define readFiles_H
#include<iostream>
#include <vector>
#include<io.h>
//#include <fstream>
using namespace std;
//---begin, for reading file
void getFiles( string path, vector<string>& files );
void getFilesName( string path, vector<string>& files );
char* str2char(string src) ;
//---  end, for reading file

//获取特定格式的文件名
void GetAllFormatFiles( string path, vector<string>& files,string format)  ;
//获取所有的文件名
void GetAllFiles( string path, vector<string>& files) ;

//---begin, txt文本读取
//如果字符串中有小数
void string2float(string s, double* pdata) ;
//如果字符串中都是整数
void string2int(string s, int* pdata);
//---  end, txt文本读取

#endif