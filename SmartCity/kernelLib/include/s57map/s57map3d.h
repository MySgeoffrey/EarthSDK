#ifndef _S57MAP3D_H
#define _S57MAP3D_H

#include "s57map/s57mapconfig.h"
#include "mychartdrawlib/imys57map.h"
#include "mychartdrawlib/imys57layer.h"
#include "osg/Group"

namespace MyChart
{
	class VR_S57MAP_EXPORT IMyS57Map3D
		: public IMyS57Map
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyS57Map3D();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		/// <param name="pData">绘制数据对象</param>
		IMyS57Map3D(const QString& name,IMyVectorMapData* pData);

		~IMyS57Map3D();

		void setRefRoot(osg::ref_ptr<osg::Group> in_pRefRoot); 

		virtual bool draw(IMyDrawArgs* pArgs);

	protected:
		bool constructDrawLayer();

		virtual bool draw(IMyS57Layer* pLayer, IMyDrawArgs* pArgs);

		virtual bool drawPolygonRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
	protected:
		osg::ref_ptr<osg::Group> mpRefRoot;
	};

}

#endif //_S57MAP3D_H