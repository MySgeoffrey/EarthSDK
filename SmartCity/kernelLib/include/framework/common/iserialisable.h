//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ���Ͽռ���Ϣƽ̨
//  @ File Name : iserialisable.h
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#if !defined(_ISERIALISABLE_H)
#define _ISERIALISABLE_H

#include "framework/vr_globeframework_global.h"

// �����л������ṩ���ݶ���������Դ֮���ͨ�Žӿ�
namespace Common
{
	/// <summary>
	/// ��Ч��
	/// </summary>
	class VR_PIPEFRAMEWORK_EXPORT ISerialisable
    {
    public:
		ISerialisable();

		virtual ~ISerialisable();
		virtual int insertToDb();
		virtual int updateToDb();
		virtual int deleteFromDb();
		virtual int queryFromDb();
    };
}

#endif  //_ISERIALISABLE_H