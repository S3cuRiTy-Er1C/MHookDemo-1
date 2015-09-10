#ifndef __FastLink_H_
#define __FastLink_H_
#include "stdio.h"
#include <unistd.h>
#include "Dump_Dex/DexUtil.H"
/*
************************************************************
*				Link
*Dex通用类
************************************************************
*/
typedef struct Link{
	struct Link* next = NULL;
	struct Base	 BS;
/*
************************************************************
*				AddToLast
*将数据添加到链表
*如果不保存连接最后指，使用AddToLast是一个非常耗时的操作
*所以使用一个AddLink函数
************************************************************
*/
	static void* AddToLast(void* inSrc,void* in_next){
		if(NULL == inSrc)return in_next;
		Link* Src = (Link*)inSrc;
		while(NULL != Src->next){
			Src = Src->next;
		}
		Src->next = (Link*)in_next;
		return inSrc;
	}
/*
************************************************************
*				AddLink
*将数据添加到链表
*只是设置链表指向，返回下一个链表
************************************************************
*/

	static void* AddLink(void* inSrc,void* in_next){
		if((NULL == inSrc)||(NULL == in_next))return NULL;
		Link* Src = (Link*)inSrc;
		Src->next = (Link*)in_next;
		return in_next;
	}
/*
************************************************************
*				GetSize
*将数据添加到链表
************************************************************
*/
	static size_t GetSize(void* inlink){
		Link* link =(Link*)inlink;
		size_t mSize = 0;
		while(link != NULL){
			mSize += link->BS.MemSize;
			link = link->next;
		}
		return mSize;
	}
/*
************************************************************
*				GetSize
*将数据添加到链表
************************************************************
*/
	static size_t GetCount(void* inlink){
		Link* link =(Link*)inlink;
		size_t mSize = 0;
		while(link != NULL){
			mSize++;
			link = link->next;
		}
		return mSize;
	}
/*
************************************************************
*				GetLastLink
*将数据添加到链表
************************************************************
*/
	static void* GetLastLink(void* inlink){
		Link* link =(Link*)inlink;
		if(link == NULL)return NULL;
		while(link->next != NULL){
			link = link->next;
		}
		return link;
	}
}Link;

template <typename Type_>
class FastLink{
public:
	Type_* First = NULL;
	Type_* Last = NULL;
	Type_* pType = NULL;
/*
************************************************************
*				复位局部指针
************************************************************
*/
	void Reset(){
		pType = First;
	}
/*
************************************************************
*				获取链表宽度
************************************************************
*/
	size_t GetSize(){
		Link* mType = (Link*)First;
		size_t mSize = 0;
		while(mType != NULL){
			mSize++;
			mType = mType->next;
		}
		return mSize;
	}
/*
************************************************************
*				获取链表宽度
************************************************************
*/
	size_t GetMemSize(){
		Link* mType = (Link*)First;
		size_t mSize = 0;
		while(mType != NULL){
			mSize+= mType->BS.MemSize;
			mType = mType->next;
		}
		return mSize;
	}
/*
************************************************************
*				获取局部指针变量
************************************************************
*/
	Type_* Get(){
		return pType;
	}
/*
************************************************************
*				获取局部指针变量
************************************************************
*/
	Type_* get(int inNo){
		Link* mType = (Link*)First;
		for(int m_i=0;m_i < inNo;m_i++){
			if(mType == NULL)return NULL;
			mType = mType->next;
		}
		return (Type_*)mType;
	}
/*
************************************************************
*将局部指针指向下一个数据
************************************************************
*/
	void Next(){
		Link* link =(Link*)pType;
		if(pType == NULL)return ;
		link = link->next;
		pType = (Type_*)link;
	}
/*
************************************************************
*向内存列表中添加一个数据
************************************************************
*/
	void Add(Type_* inType){
		if(First == NULL){
			First = inType;
			Last = inType;
			return ;
		}
		Link* link =(Link*)Last;
		link->next = (Link*)inType;
		link = link->next;
		Last = (Type_*)link;
	}
};
#endif