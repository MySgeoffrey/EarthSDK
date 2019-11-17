//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : imymapmanager.h
//  @ Date : 2017/10/20
//  @ Author : sgeoffrey
//
//


#if !defined(_IMYMAPMANAGER_H)
#define _IMYMAPMANAGER_H

#include "mychartmaplib/mychartmaplib_global.h"
#include "mychartmaplib/imydrawable.h"

namespace MyChart
{
	class IMyMap;
	class IMyGeneralMap;
	/// <summary>
	/// 地图管理类
	/// </summary>
	class MYCHARTMAPLIB_EXPORT IMyMapManager 
		: public MyChart::IMyDrawable
	{
	public:

		/// <summary>
		/// 构造函数
		/// </summary>
		IMyMapManager();
		
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyMapManager();

		/// <summary>
		/// 获取地图管理类实例
		/// </summary>
		/// <returns>管理类单例</returns>
		static IMyMapManager* instance();

		/// <summary>
		/// 释放地图管理类实例
		/// </summary>
		static void releaseInstance();
		
		/// <summary>
		/// 获取地图集
		/// </summary>
		/// <returns>地图集</returns>
		std::vector<IMyMap*>& getMaps();

		/// <summary>
		/// 获取通用地图（图层集）
		/// </summary>
		IMyGeneralMap* getGeneralMap();

		/// <summary>
		/// 关闭地图
		/// </summary>
		/// <param name="name">地图名称</param>
		/// <returns>是否成功关闭</returns>
		bool closeMap(const QString& name);

		IMyMap* getMap(const QString& name);

		/// <summary>
		/// 关闭所有地图
		/// </summary>
		/// <returns>是否成功关闭</returns>
		bool closeAllMaps();

		/// <summary>
		/// 获取最新绘制的位图
		/// </summary>
		/// <returns>位图</returns>
		virtual QPixmap& getImage();

		/// <summary>
		/// 绘制
		/// </summary>
		/// <param name="pArgs">绘制参数</param>
		/// <returns>是否绘制成功</returns>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual bool drawGeneralMap(IMyDrawArgs* pArgs);

		/// <summary>
		/// 将地图集数据保存至索引文件
		/// </summary>
		/// <param name="filePath">索引文件路径</param>
		/// <returns>是否保存成功</returns>
		virtual bool toFile(const QString& filePath);

		/// <summary>
		/// 从索引文件中恢复地图集数据
		/// </summary>
		/// <param name="filePath">索引文件路径</param>
		/// <param name="fileList">文件路径列表</param>
		/// <returns>是否恢复成功</returns>
		virtual bool fromFile(const QString& filePath,QStringList& fileList);

	protected:
		std::vector<IMyMap*> mMaps;
		IMyGeneralMap* mpGeneralMap;
		QPixmap mBackgroundImage;
	};
}

#endif  //_IMYMAPMANAGER_H
