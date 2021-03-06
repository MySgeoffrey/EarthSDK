//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : itoollistener.h
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#if !defined(_ITOOLLISTENER_H)
#define _ITOOLLISTENER_H

#include "framework/common/iobject.h"


namespace Tool
{
	//IToolListener类：工具监听类，为用户获取、捕捉场景内部消息的一般回调机制
	class VR_PIPEFRAMEWORK_EXPORT IToolListener
    {
    public:

		IToolListener();

		virtual ~IToolListener();

		/*
			工具监听响应事件,为用户捕捉工具内存执行的结果
			pResult : 为内存执行操作后返回的结果
		*/
        virtual void onEvent(Common::IObject* pResult) = 0;
    };
}

#endif  //_ITOOLLISTENER_H
