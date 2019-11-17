#ifndef MYCHARTS57DATALIB_H
#define MYCHARTS57DATALIB_H

#include "mycharts57datalib/mycharts57datalib_global.h"

class OGRLayer;
class OGRFeature;
class OGRFeatureDefn;
namespace MyChart
{
	class IMyS57MapData;
	class IMyS57LayerData;
	class IMyShapeRecord;
	class IMyProperty;
	/// <summary>
	/// S57海图数据驱动类
	/// </summary>
	class MYCHARTS57DATALIB_EXPORT CMyShpDataDriver
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CMyShpDataDriver();

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CMyShpDataDriver();

		/// <summary>
		/// 读取地图数据
		/// </summary>
		/// <param name="chartFilePath">海图路径</param>
		/// <returns>海图数据</returns>
		virtual IMyS57MapData* readS57Map(const QString& chartFilePath);

		/// <summary>
		/// 读取图层数据
		/// </summary>
		/// <param name="pLayer">海图图层</param>
		/// <param name="pMap">海图对象</param>
		/// <returns>是否读取成功</returns>
		virtual bool readLayer(OGRLayer* pLayer, IMyS57MapData* pMap);

		/// <summary>
		/// 读取图层要素数据
		/// </summary>
		/// <param name="pFeature">要素</param>
		/// <param name="pFeatureDefn">要素定义对象</param>
		/// <param name="inout_pShapeRecord">要素对象</param>
		/// <param name="inout_pProperty">要素属性对象</param>
		/// <returns>是否读取成功</returns>
		virtual bool readFeature(OGRFeature* pFeature, OGRFeatureDefn* pFeatureDefn,
			IMyShapeRecord*& inout_pShapeRecord,QStringList*& inout_pProperty);

	private:

	};

	class MYCHARTS57DATALIB_EXPORT CMyChartS57DataDriver
		: public CMyShpDataDriver
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CMyChartS57DataDriver()
		{}

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CMyChartS57DataDriver()
		{}

		/// <summary>
		/// 读取海图数据
		/// </summary>
		/// <param name="chartFilePath">海图路径</param>
		/// <returns>海图数据</returns>
		virtual IMyS57MapData* readS57Map(const QString& chartFilePath);
	};
}


#endif // MYCHARTS57DATALIB_H
