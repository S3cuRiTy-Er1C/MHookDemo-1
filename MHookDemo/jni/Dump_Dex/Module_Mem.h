#ifndef __Module_Mem_H_
#define __Module_Mem_H_
#include "stdio.h"
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include "Common/Utils.H"
//__________________________________________________________
//内存分割模块
typedef struct str_ModMem{
	//下一个内存分割模块
	struct str_ModMem* next;
	//数据内存地址
	unsigned char*	Addr;
	//加载基本地址
	unsigned char*	LoadBase;
	//内存名字
	char* 	name;
	//内存宽度
	size_t 	Length;
	//文件偏移地址
	size_t	Offset;
	//
	size_t	Base;
}str_ModMem;
//__________________________________________________________
class Mod_Mem{
public:
str_ModMem* ModMem = NULL;
/*
************************************************************
*				Szie
*获取整个内存宽度
************************************************************
*/
	unsigned int Size()
	{
		size_t mLength = 0;
		str_ModMem* mMod = ModMem;
		while(mMod != NULL){
			mLength += mMod->Length;
			mMod = mMod->next;
		}
		return mLength;
	}
/*
************************************************************
*根据模块名字过获取子模块指针
************************************************************
*/
	str_ModMem* getByName(const char* inName){
		str_ModMem* mMod = ModMem;
		while(mMod != NULL){
			if(mMod->name != NULL){
				if(strcmp(mMod->name,inName) == 0){
					return mMod;
				}
			}
			mMod = mMod->next;
		}
		return NULL;
	}
/*
************************************************************
*获取最后一个内存模块列表
************************************************************
*/
	str_ModMem* Last()
	{
		str_ModMem* mMod = ModMem;
		if(mMod == NULL)return NULL;
		while(mMod->next != NULL){
			mMod = mMod->next;
		}
		return mMod;
	}
/*
************************************************************
*新建一个内存模块，并且将这内存模块添加到尾部
************************************************************
*/
	str_ModMem* newMem(size_t in_Length)
	{
		str_ModMem* mMem = Last();
		//判断是否是第一次设置内存快
		if(mMem == NULL){
			ModMem = (str_ModMem*)malloc(sizeof(str_ModMem)+1);
			mMem = ModMem;
		}else{
			mMem->next = (str_ModMem*)malloc(sizeof(str_ModMem)+1);
			mMem = mMem->next;
		}
		memset(mMem,0,sizeof(str_ModMem)+1);
		//将偏移地址设置为之前宽度
		mMem->Offset = Size();
		//设置地址
		mMem->Addr = (unsigned char*)malloc(in_Length);
		memset(mMem->Addr,0,in_Length);
		mMem->Length = in_Length;
		return mMem;
	}
/*
************************************************************
*新建一个内存模块，并且将这内存模块添加到尾部
************************************************************
*/
	str_ModMem* newMem(void* inAddr,size_t in_Length)
	{
		str_ModMem* mMem = newMem(in_Length);
		memcpy(mMem->Addr,inAddr,in_Length);
		return mMem;
	}
/*
************************************************************
*新建一个内存模块，并且将这内存模块添加到尾部
************************************************************
*/
	str_ModMem* newNameMem(const char* inName,size_t in_Length)
	{
		str_ModMem* mMem = newMem(in_Length);
		//设置名字
		mMem->name = strdup(inName);
		return mMem;
	}
/*
************************************************************
*保存成文件
************************************************************
*/
	void SaveFile(char* appName,char* inFile){
		//return;
		char* filePath = (char*)malloc(1024);
		memset(filePath,0,1024);
		sprintf(filePath,"/sdcard/DumpDex/%s",inFile);
		FILE* mfd=fopen(filePath,"wb");
		//查找可写文件路径
		if(!mfd){
			DEXLOG("[Dump]Fail SD Path:%s",filePath);
			DEXLOG("[Dump]SD Open file fail!");
			memset(filePath,0,1024);
			if(appName != NULL){
				sprintf(filePath,"/data/data/%s/%s",appName,inFile);
				mfd=fopen(filePath,"wb");
				if(!mfd){
					DEXLOG("[Dump]Fail SD Path:%s",filePath);
					DEXLOG("[Dump]SD Open file fail!");
					free(filePath);
					return;
				}
			}else{
				DEXLOG("[Dump]Fail appName Is Null");
				free(filePath);
				return ;
			}
		}
		str_ModMem* mMod = ModMem;
		while(mMod != NULL){
			if(mMod->name != NULL){
				DEXLOG("[Dump]正在下载:%s",mMod->name);
			}else{
				DEXLOG("[Dump]正在下载:NULL");
			}
			fwrite(mMod->Addr,1,mMod->Length,mfd);
			mMod = mMod->next;
		}
		//释放至于资源
		fclose(mfd);
		DEXLOG("[Dump]Dump File Path:%s",filePath);
		free(filePath);
	}
/*
************************************************************
*获取有多少个内存块
************************************************************
*/
	size_t getCountBlock(){
		size_t mCount = 0;
		str_ModMem* mMod = ModMem;
		while(mMod != NULL){
			mCount++;
		}
		return mCount;
	}
/*
************************************************************
*获取有多少个内存块
************************************************************
*/
	static void SaveFile(char* appName,char* inFile,void* inAddr,size_t inLen){
		char* filePath = (char*)malloc(1024);
		memset(filePath,0,1024);
		sprintf(filePath,"/sdcard/DumpDex/%s",inFile);
		FILE* mfd=fopen(filePath,"wb");
		//查找可写文件路径
		if(!mfd){
			DEXLOG("[Dump]Fail SD Path:%s",filePath);
			DEXLOG("[Dump]SD Open file fail!");
			memset(filePath,0,1024);
			if(appName != NULL){
				sprintf(filePath,"/data/data/%s/%s",appName,inFile);
				mfd=fopen(filePath,"wb");
				if(!mfd){
					DEXLOG("[Dump]Fail SD Path:%s",filePath);
					DEXLOG("[Dump]SD Open file fail!");
					free(filePath);
					return;
				}
			}else{
				DEXLOG("[Dump]Fail appName Is Null");
				free(filePath);
				return ;
			}
		}
		fwrite(inAddr,1,inLen,mfd);
		fclose(mfd);
		DEXLOG("[Dump]Dump File Path:%s",filePath);
		free(filePath);
	}
};
//
#endif
