//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : imys57shaperecord.h
//  @ Date : 2017/10/20
//  @ Author : sgeoffrey
//
//


#if !defined(_IMYS57SHAPERECORD_H)
#define _IMYS57SHAPERECORD_H

#include "mycharts57datalib/mycharts57datalib_global.h"
#include "mychartmaplib/imyshaperecord.h"

namespace MyChart
{
	class IMyLayer;

	/// <summary>
	/// S57图形要素类
	/// </summary>
	class MYCHARTS57DATALIB_EXPORT IMyS57ShapeRecord 
		: public IMyShapeRecord
	{
	public:

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="geometryType">几何类型</param>
		IMyS57ShapeRecord(const GeometryType& geometryType);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyS57ShapeRecord();

	public:
		IMyLayer* ParentLayer;///<父图层
		float IndexOfRenderPriority;///<渲染优先级索引
		int IndexInOriginLayerList;///<在原图层中的索引
		bool IsShow;///<是否参与绘制

	};

}

#endif  //_IMYS57SHAPERECORD_H
