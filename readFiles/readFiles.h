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

//��ȡ�ض���ʽ���ļ���
void GetAllFormatFiles( string path, vector<string>& files,string format)  ;
//��ȡ���е��ļ���
void GetAllFiles( string path, vector<string>& files) ;

//---begin, txt�ı���ȡ
//����ַ�������С��
void string2float(string s, double* pdata) ;
//����ַ����ж�������
void string2int(string s, int* pdata);
//---  end, txt�ı���ȡ

#endif