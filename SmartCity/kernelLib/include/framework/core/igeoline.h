//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ���Ͽռ���Ϣƽ̨
//  @ File Name : igeoline.h
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#if !defined(_IGEOLINE_H)
#define _IGEOLINE_H

#include "framework/common/isceneobject.h"



namespace Core
{
	/// <summary>
	/// IGeoLine�ࣺ��������֯����Ⱦ����
	/// ��������ռ�����ߴ�,��һϵ�е����깹�ɡ�
	/// </summary>
	class VR_PIPEFRAMEWORK_EXPORT IGeoLine : public Common::ISceneObject
    {
    public:

		/// <summary>
		/// ���캯��
		/// </summary>
		IGeoLine();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IGeoLine(osgEarth::MapNode* pRefMapNode);

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IGeoLine();

		/// <summary>
		/// ��ȡ����ͼ��
		/// </summary>
		virtual std::vector<osg::Vec3d>& getGeometry();

	protected:
		std::vector<osg::Vec3d> m_vecPoint;

    };
}

#endif  //_IGEOLINE_H