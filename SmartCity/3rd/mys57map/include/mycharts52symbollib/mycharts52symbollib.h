#ifndef MYCHARTS52SYMBOLLIB_H
#define MYCHARTS52SYMBOLLIB_H

#include "mycharts52symbollib/mycharts52symbollib_global.h"
#include "mychartsymbollib/lookuptable.h"

namespace MyChart
{
	class IMyS52LayerSymbol;
	class IMyS57LayerData;
	class IMyS57MapData;
	/// <summary>
	/// S52风格临时解析参数
	/// </summary>
	struct SymbolParserParam
	{
	public:
		SymbolParserParam()
		{
			SafeCountour = 10;
			IsSymbolizedBoundries = false;
			LookUpTable = NULL;
			LineModeOfSymbol = "LINES";
		}

		virtual ~SymbolParserParam()
		{

		}
		float SafeCountour;
		bool IsSymbolizedBoundries;
		QString AreaModeOfSymbol;
		QString LineModeOfSymbol;
		QString PointModeOfSymbol;
		QString Type;
        QString CurrentModeOfSymbol;
		int CoutInMode;
		std::map<int, CLookupTable*>* LookUpTable;

		std::vector<QString> Description;
		std::vector<QString> DisplayPriority;
		std::vector<QString> Radar;
		std::vector<QString> ImoDisplayCategory;
		std::vector<QString> ViewingGroup;
	};

	/// <summary>
	/// S52符号解析类
	/// </summary>
	class MYCHARTS52SYMBOLLIB_EXPORT CMyChartS52SymbolLib
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CMyChartS52SymbolLib();

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CMyChartS52SymbolLib();

		/// <summary>
		/// 单例对象
		/// </summary>
		/// <returns>单例对象</returns>
		static CMyChartS52SymbolLib* instance();

		/// <summary>
		/// 解析S52风格
		/// </summary>
		/// <param name="safeCountour">安全等深线</param>
		/// <param name="areaModeOfSymbol">面状符号类型</param>
		/// <param name="pointModeOfSymbol">点状符号类型</param>
		/// <param name="in_pS57Layer">当前S57图层</param>
		/// <param name="inout_pS52Symbol">返回的S52风格</param>
		/// <returns>是否解析成功</returns>
		bool getS52Symbol(
			float safeCountour,QString areaModeOfSymbol,QString pointModeOfSymbol,
			IMyS57LayerData* in_pS57Layer,
			IMyS52LayerSymbol*& inout_pS52Symbol);

	protected:
		void getBaseDescription(SymbolParserParam* pParseParam,
			IMyS57LayerData* in_pS57Layer,
			IMyS52LayerSymbol*& inout_pS52Symbol);

		void getPointSymbolizationInstruction(SymbolParserParam* pParseParam,
			IMyS57LayerData* in_pS57Layer,
			IMyS52LayerSymbol*& inout_pS52Symbol);

		void getLineSymbolizationInstruction(SymbolParserParam* pParseParam,
			IMyS57LayerData* in_pS57Layer,
			IMyS52LayerSymbol*& inout_pS52Symbol);

		void getAreaSymbolizationInstruction(SymbolParserParam* pParseParam,
			IMyS57LayerData* in_pS57Layer,
			IMyS52LayerSymbol*& inout_pS52Symbol);

	private:
		static CMyChartS52SymbolLib* s_instance;
	};
}
#endif // MYCHARTS52SYMBOLLIB_H
