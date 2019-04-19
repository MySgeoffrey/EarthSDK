#ifndef _POLYLINEEDITOR_H
#define _POLYLINEEDITOR_H

#include "geometryeditor/geometryeditor.h"

namespace GeometryEditor
{
	/// <summary>
	/// 折线编辑类
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IPolylineEditor : public IGeometryEditor
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		IPolylineEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IPolylineEditor();
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

	};
}

#endif//_POLYLINEEDITOR_H

