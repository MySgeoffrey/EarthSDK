#ifndef _SCUTCHEONEDITOR_H
#define _SCUTCHEONEDITOR_H

#include "geometryeditor/geometryeditor.h"
#include "geoobject/geoscutcheon.h"

namespace GeometryEditor
{
	/// <summary>
	/// 点编辑类
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IScutcheonEditor : public IGeometryEditor
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		IScutcheonEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IScutcheonEditor();
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
		Geo::CGeoScutcheon* mpScutcheon;

	};
}

#endif//_SCUTCHEONEDITOR_H

