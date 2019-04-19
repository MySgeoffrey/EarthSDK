#ifndef _POLYGONEDITOREX_H
#define _POLYGONEDITOREX_H

#include "geometryeditor/polylineeditor.h"

namespace GeometryEditor
{
	/// <summary>
	/// 多边形编辑类
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IPolygonEditor 
		: public IPolylineEditor
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		IPolygonEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IPolygonEditor();
		/// <summary>
		/// 初始化
		/// </summary>
		virtual void initial();

		virtual Common::ISceneObject* generateSceneObject();

	protected:

		// 经纬度信息
		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

		virtual void performPickedXYZ(osg::Vec3d pos);

		virtual void performMovingXYZ(osg::Vec3d pos);

		virtual void performRightPicked();

		virtual void performDeleteDrawing();

	protected:
		

	};
}

#endif//_POLYGONEDITOREX_H

