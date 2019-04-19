#ifndef _POLYLINEEDITOR_H
#define _POLYLINEEDITOR_H

#include "geometryeditor/geometryeditor.h"

namespace GeometryEditor
{
	/// <summary>
	/// ���߱༭��
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IPolylineEditor : public IGeometryEditor
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IPolylineEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IPolylineEditor();
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

	};
}

#endif//_POLYLINEEDITOR_H

