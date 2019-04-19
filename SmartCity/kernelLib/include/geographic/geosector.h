//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : cgeosector.h
//  @ Date : 2018/7/28
//  @ Author : sgeoffrey
//  @platform:���Ͽռ���Ϣƽ̨
//


#if !defined(_CGEOSECTOR_H)
#define _CGEOSECTOR_H

#include "geographic/geocirlce.h"
#include "geographic/geographicconfig.h"

namespace Graphic
{
	class VR_GEOGRAPHIC_EXPORT CGeoSector
		: public CGeoCircle
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CGeoSector();
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		CGeoSector(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~CGeoSector();

	protected:
		virtual bool transformRenderGeometry(std::vector<osg::Vec3d>& outRenderGeometry);
	};
}
#endif  //_CGEOSECTOR_H