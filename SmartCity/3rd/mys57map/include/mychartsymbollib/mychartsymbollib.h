#ifndef MYCHARTSYMBOLLIB_H
#define MYCHARTSYMBOLLIB_H

#include "mychartsymbollib/mychartsymbollib_global.h"
#include <vector>
#include <map>

namespace MyChart
{
	class CColorTable;
	class CBitmapSymbol;
	class CBitmapPattern;
	class CVectorSymbol;
	class CVectorLine;
	class CVectorPattern;
	class CLookupTable;
	class CLayerInfo;
	/// <summary>
	/// 海图基础符号库解析类
	/// </summary>
	class MYCHARTSYMBOLLIB_EXPORT CS57Symbollib
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CS57Symbollib();

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CS57Symbollib();

		/// <summary>
		/// 加载s57海图符号库存
		/// </summary>
		/// <returns>是否成功解析基础符号库/returns>
		bool initial();

		/// <summary>
		/// 卸载s57海图符号库存
		/// </summary>
		void uninitial();

		/************************************************************************/
		/*判别符号库中的符号类型                                                 */
		/************************************************************************/

		/// <summary>
		/// 合并Bitmaps符号信息
		/// </summary>
		/// <param name="bitmapText">符号字符串数据</param>
		void mergeBitmaps(QString bitmapText);

		/// <summary>
		/// 合并Vectors符号信息
		/// </summary>
		/// <param name="vectorText">符号字符串数据</param>
		void mergeVectors(QString vectorText);

		/// <summary>
		/// 判断是否为Bitmap类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isBitmap(QString component);

		/// <summary>
		/// 判断是否为BitmapSymbol类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isBitmapSymbol(QString component);

		/// <summary>
		/// 判断是否为BitmapPattern类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isBitmapPattern(QString component);

		/// <summary>
		/// 判断是否为Vector类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isVector(QString component);

		/// <summary>
		/// 判断是否为VectorSymbol类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isVectorSymbol(QString component);

		/// <summary>
		/// 判断是否为VectorPattern类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isVectorPattern(QString component);

		/// <summary>
		/// 判断是否为VectorLine类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isVectorLine(QString component);

		/// <summary>
		/// 判断是否为ColorTable类型
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isColorTable(QString component);

		/// <summary>
		/// 判断是否为Lines类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isLines(QString component);

		/// <summary>
		/// 判断是否为PaperChart类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isPaperChart(QString component);

		/// <summary>
		/// 判断是否为Simplified类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isSimplified(QString component);

		/// <summary>
		/// 判断是否为PlainBoundaries类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isPlainBoundaries(QString component);

		/// <summary>
		/// 判断是否为SymbolizedBoundaries类型符号
		/// </summary>
		/// <param name="component">判断条件</param>
		/// <returns>true：是，false:否</returns>
		bool isSymbolizedBoundaries(QString component);

		/// <summary>
		/// 基础符号解析类实例
		/// </summary>
		/// <returns>单例对象</returns>
		static CS57Symbollib* instance();

		/// <summary>
		/// 释放基础符号解析类实例
		/// </summary>
		static void releaseInstance();

		static QString stripLenFromString(QString original);

		static QString trim(QString string , char c);

	private:

		static CS57Symbollib* s_s57symbilLib;

		/************************************************************************/
		/* 符号库信息                                                            */
		/************************************************************************/
		//五种符号类型
	public: 
		static const QString PAPER_CHART;///<纸质符号
		static const QString SIMPLIFIED;///<简单符号
		static const QString LINES;///<线符号
		static const QString PLAIN_BOUNDARIES;///<简单边界符号
		static const QString SYMBOLIZED_BOUNDARIES;///<符号化边界符号

		static const char Term;
		static const QString Nil;

	public:
		std::vector<QString> Datacomponents;
		QString Remarks;
		std::map<QString, int> Test;
		std::map<QString, CColorTable*> ColorTables;
		std::map<QString, CBitmapSymbol*> SymbolBitmaps;
		std::map<QString, CBitmapPattern*> PatternBitmaps;
		std::map<QString, CVectorSymbol*> SymbolVectors;
		std::map<QString, CVectorLine*> LineVectors;
		std::map<QString, CVectorPattern*> PatternVectors;
		std::map<QString, std::map<int, CLookupTable*>*> LookupTables;
		std::map<QString, CVectorLine*> LC;
		std::vector<CLayerInfo*> LayerInfo;
		int NextId;
		CColorTable* DefaultColorTable;
	};

}


#endif // MYCHARTSYMBOLLIB_H
