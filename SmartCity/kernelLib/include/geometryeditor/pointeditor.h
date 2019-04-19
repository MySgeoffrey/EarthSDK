#ifndef _POINTEDITOR_H
#define _POINTEDITOR_H

#include "geometryeditor/geometryeditor.h"
#include "geographic/geopoint.h"

namespace GeometryEditor
{
	/// <summary>
	/// 点编辑类
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IPointEditor : public IGeometryEditor
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		IPointEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IPointEditor();
		/// <summary>
		/// 初始化
		/// </summary>
		virtual void initial();

		virtual Common::ISceneObject* generateSceneObject();
	protected:

		virtual void performDoublePicked(osg::Vec3d pos);

		// 经纬度信息
		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

		virtual void performPickedXYZ(osg::Vec3d pos);

		virtual void performMovingXYZ(osg::Vec3d pos);

		virtual void performRightPicked();

		virtual void performDeleteDrawing();
	protected:
		Graphic::CGeoPoint* mpGeoPoint;

	};
}

#endif//_POLYLINEEDITOR_H

