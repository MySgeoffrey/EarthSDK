#ifndef _S57LAYER3D_H
#define _S57LAYER3D_H

#include "s57map/s57mapconfig.h"
#include "mychartdrawlib/imys57layer.h"

namespace MyChart
{
	class VR_S57MAP_EXPORT IMyS57Layer3D
		: public IMyS57Layer
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		IMyS57Layer3D();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		IMyS57Layer3D(const QString& name);

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IMyS57Layer3D();

		virtual bool draw(IMyDrawArgs* pArgs);
	};

}

#endif //_S57MAP3D_H