#ifndef _POINTEDITOR_H
#define _POINTEDITOR_H

#include "geometryeditor/geometryeditor.h"
#include "geographic/geopoint.h"

namespace GeometryEditor
{
	/// <summary>
	/// ��༭��
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IPointEditor : public IGeometryEditor
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IPointEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IPointEditor();
		/// <summary>
		/// ��ʼ��
		/// </summary>
		virtual void initial();

		virtual Common::ISceneObject* generateSceneObject();
	protected:

		virtual void performDoublePicked(osg::Vec3d pos);

		// ��γ����Ϣ
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

