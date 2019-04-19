#ifndef _POLYLINEEDITOREX_H
#define _POLYLINEEDITOREX_H

#include "geometryeditor/polygoneditor.h"

namespace GeometryEditor
{
	/// <summary>
	/// 箭头编辑扩展类
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IArrowEditor
		: public IPolygonEditor
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		IArrowEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IArrowEditor();

	protected:

		// 经纬度信息
		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

		bool createArrow(std::vector<IEditPoint*>& originPoints,
			std::vector<osg::Vec3d>& results);

	};
}

#endif//_POLYLINEEDITOREX_H

