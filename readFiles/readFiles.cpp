#include "readFiles.h"
#include <fstream>
#include <string>
void getFiles( string path, vector<string>& files )
{
	//�ļ����
	long   hFile   =   0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
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
	//�ļ����
	long   hFile   =   0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
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


//��ȡ���е��ļ���
void GetAllFiles( string path, vector<string>& files)  
{  
 
	long   hFile   =   0;  
	//�ļ���Ϣ  
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
 
//��ȡ�ض���ʽ���ļ���
void GetAllFormatFiles( string path, vector<string>& files,string format)  
{  
	//�ļ����  
	long   hFile   =   0;  
	//�ļ���Ϣ  
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

//����ַ����ж�������
void string2int(string s, int* pdata)
{
	bool temp=false;		//��ȡһ�����ݱ�־λ
	int data=0;				//�����һ������
	int m=0;				//��������ֵ
	for(int i=0;i<s.length();i++)
	{
		while((s[i]>='0')&&(s[i]<='9'))		//��ǰ�ַ������ݣ���һֱ����������ݣ�ֻҪ������������Ϊֹ
		{
			temp=true;		//�����ݱ�־λ��λ
			data*=10;
			data+=(s[i]-'0');		//�ַ���ϵͳ��ASCII��洢��Ҫ�õ���ʵ��ֵ�����ȥ��0����ASCIIֵ
			i++;
		}
		//�ն�ȡ������
		if(temp)		//�ж��Ƿ���ȫ��ȡһ������
		{
			pdata[m]=data;		//��ֵ
			m++;
			data=0;		
			temp=false;		//��־λ��λ
		}
	}
}


//����ַ�������С��
void string2float(string s, double* pdata)
{
	bool temp=false;
	int ndata=0;		//��������
	double	fdata=0;	//С������
	int n=0;			//С�����ֵ�λ��
	int m=0;
	for(int i=0;i<s.length();i++)
	{
		while((s[i]>='0')&&(s[i]<='9')||(s[i]=='.'))		//��ǰ�ַ������ݻ�����С����
		{
			temp=true;
			if(s[i]=='.')		//������С����
			{
				//��ͣ�Ķ�ȡ�����С��
				i++;		//С�������һλ
				while((s[i]>='0')&&(s[i]<='9'))
				{
					n--;
					fdata += (pow(10, n)*(s[i]-'0'));
					i++;
				}
			}
			//û������С����
			else
			{
				ndata*=10;
				ndata+=(s[i]-'0');
				i++;
			}
		}
		//�ն�ȡ������
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
