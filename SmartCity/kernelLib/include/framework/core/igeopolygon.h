//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ���Ͽռ���Ϣƽ̨
//  @ File Name : igeopolygon.h
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#if !defined(_IGEOPOLYGON_H)
#define _IGEOPOLYGON_H

#include "framework/common/isceneobject.h"



namespace Core
{
	/// <summary>
	/// IGeoPolygon�ࣺ����������࣬ͨ��һ����������֯����Ⱦ�����
	/// </summary>
	class VR_PIPEFRAMEWORK_EXPORT IGeoPolygon : public Common::ISceneObject
    {
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		IGeoPolygon();
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IGeoPolygon(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IGeoPolygon();

		virtual bool contain(const osg::Vec3d& geoPoint);
    };
}

#endif  //_IGEOPOLYGON_H