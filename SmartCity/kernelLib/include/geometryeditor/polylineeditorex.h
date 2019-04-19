#ifndef _POLYLINEEDITOREX_H
#define _POLYLINEEDITOREX_H

#include "geometryeditor/polygoneditor.h"

namespace GeometryEditor
{
	/// <summary>
	/// ��ͷ�༭��չ��
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IArrowEditor
		: public IPolygonEditor
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IArrowEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IArrowEditor();

	protected:

		// ��γ����Ϣ
		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

		bool createArrow(std::vector<IEditPoint*>& originPoints,
			std::vector<osg::Vec3d>& results);

	};
}

#endif//_POLYLINEEDITOREX_H

