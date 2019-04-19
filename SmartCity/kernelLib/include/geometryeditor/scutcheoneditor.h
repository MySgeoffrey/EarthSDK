#ifndef _SCUTCHEONEDITOR_H
#define _SCUTCHEONEDITOR_H

#include "geometryeditor/geometryeditor.h"
#include "geoobject/geoscutcheon.h"

namespace GeometryEditor
{
	/// <summary>
	/// ��༭��
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IScutcheonEditor : public IGeometryEditor
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IScutcheonEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IScutcheonEditor();
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
		Geo::CGeoScutcheon* mpScutcheon;

	};
}

#endif//_SCUTCHEONEDITOR_H

