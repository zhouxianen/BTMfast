#include "readFiles.h"
#include <fstream>
#include <string>
void getFiles( string path, vector<string>& files )
{
	//文件句柄
	long   hFile   =   0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

void getFilesName( string path, vector<string>& files )
{
	//文件句柄
	long   hFile   =   0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					//getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
						getFiles( fileinfo.name, files );
			}
			else
			{
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
				files.push_back(fileinfo.name);
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

char* str2char(string src) 
{
	char *dst = new char[255];
	int i; 
	for(i=0;i <=src.length();i++) 
		dst[i]=src[i]; 
	dst[i] = '\0';
	return dst;
}


//获取所有的文件名
void GetAllFiles( string path, vector<string>& files)  
{  
 
	long   hFile   =   0;  
	//文件信息  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
	{  
		do  
		{   
			if((fileinfo.attrib &  _A_SUBDIR))  
			{  
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					GetAllFiles( p.assign(path).append("\\").append(fileinfo.name), files ); 
				}
			}  
			else  
			{  
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
			} 
 
		}while(_findnext(hFile, &fileinfo)  == 0);  
 
		_findclose(hFile); 
	} 
 
}  
 
//获取特定格式的文件名
void GetAllFormatFiles( string path, vector<string>& files,string format)  
{  
	//文件句柄  
	long   hFile   =   0;  
	//文件信息  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(),&fileinfo)) !=  -1)  
	{  
		do  
		{    
			if((fileinfo.attrib &  _A_SUBDIR))  
			{  
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					GetAllFormatFiles( p.assign(path).append("\\").append(fileinfo.name), files,format); 
				}
			}  
			else  
			{  
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
			}  
		}while(_findnext(hFile, &fileinfo)  == 0);  
 
		_findclose(hFile); 
	} 
} 

//如果字符串中都是整数
void string2int(string s, int* pdata)
{
	bool temp=false;		//读取一个数据标志位
	int data=0;				//分离的一个数据
	int m=0;				//数组索引值
	for(int i=0;i<s.length();i++)
	{
		while((s[i]>='0')&&(s[i]<='9'))		//当前字符是数据，并一直读后面的数据，只要遇到不是数字为止
		{
			temp=true;		//读数据标志位置位
			data*=10;
			data+=(s[i]-'0');		//字符在系统以ASCII码存储，要得到其实际值必须减去‘0’的ASCII值
			i++;
		}
		//刚读取了数据
		if(temp)		//判断是否完全读取一个数据
		{
			pdata[m]=data;		//赋值
			m++;
			data=0;		
			temp=false;		//标志位复位
		}
	}
}


//如果字符串中有小数
void string2float(string s, double* pdata)
{
	bool temp=false;
	int ndata=0;		//整数部分
	double	fdata=0;	//小数部分
	int n=0;			//小数部分的位数
	int m=0;
	for(int i=0;i<s.length();i++)
	{
		while((s[i]>='0')&&(s[i]<='9')||(s[i]=='.'))		//当前字符是数据或者是小数点
		{
			temp=true;
			if(s[i]=='.')		//遇到了小数点
			{
				//不停的读取后面的小数
				i++;		//小数后的下一位
				while((s[i]>='0')&&(s[i]<='9'))
				{
					n--;
					fdata += (pow(10, n)*(s[i]-'0'));
					i++;
				}
			}
			//没有遇到小数点
			else
			{
				ndata*=10;
				ndata+=(s[i]-'0');
				i++;
			}
		}
		//刚读取了数据
		if(temp)
		{
			pdata[m]=ndata+fdata;
			m++;
			ndata=0;
			fdata=0;
			n=0;
			temp=false;
		}
	}
}
