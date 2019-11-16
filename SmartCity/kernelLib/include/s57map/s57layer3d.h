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
		/// 构造函数
		/// </summary>
		IMyS57Layer3D();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyS57Layer3D(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyS57Layer3D();

		virtual bool draw(IMyDrawArgs* pArgs);
	};

}

#endif //_S57MAP3D_H