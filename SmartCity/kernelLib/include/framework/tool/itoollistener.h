//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ���Ͽռ���Ϣƽ̨
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
	//IToolListener�ࣺ���߼����࣬Ϊ�û���ȡ����׽�����ڲ���Ϣ��һ��ص�����
	class VR_PIPEFRAMEWORK_EXPORT IToolListener
    {
    public:

		IToolListener();

		virtual ~IToolListener();

		/*
			���߼�����Ӧ�¼�,Ϊ�û���׽�����ڴ�ִ�еĽ��
			pResult : Ϊ�ڴ�ִ�в����󷵻صĽ��
		*/
        virtual void onEvent(Common::IObject* pResult) = 0;
    };
}

#endif  //_ITOOLLISTENER_H