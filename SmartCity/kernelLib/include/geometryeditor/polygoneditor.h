#ifndef _POLYGONEDITOREX_H
#define _POLYGONEDITOREX_H

#include "geometryeditor/polylineeditor.h"

namespace GeometryEditor
{
	/// <summary>
	/// ����α༭��
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IPolygonEditor 
		: public IPolylineEditor
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IPolygonEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IPolygonEditor();
		/// <summary>
		/// ��ʼ��
		/// </summary>
		virtual void initial();

		virtual Common::ISceneObject* generateSceneObject();

	protected:

		// ��γ����Ϣ
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

