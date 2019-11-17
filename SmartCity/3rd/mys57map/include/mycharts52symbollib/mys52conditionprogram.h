#ifndef MYCHARTS52CONDITIONPROGREAM_H
#define MYCHARTS52CONDITIONPROGREAM_H

#include "mycharts52symbollib/mycharts52symbollib_global.h"
#include "mycharts52symbollib/imys52symbol.h"

namespace MyChart
{
	class IMyS52Symbol;
	class IMyS57LayerData;
	class IMyS57MapData;
	class IMyS57ShapeRecord;

	/// <summary>
	/// 解析S52条件符号时所用的临时参数类
	/// </summary>
	struct S52CspParam
	{
		S52CspParam()
		{
			NameOfCSP = "";
			S57Map = NULL;
			S57Layer = NULL;
			DepareArea = NULL;
			DrgareArea = NULL;
			RecordIndex = -1;
			S57ShapeRecord = NULL;
			RecordSymbols = NULL;
			SafeDepth = 10;

			ShallowContour = 2.0f;
			SafetyContour = 20.0f;
			DeepContour = 30.0f;
			Two_shades = false;
			ShallowPatten = false;
			IsSymbolizedBoundraries = true;//是否用带符号的边界
			IsShowTextDescription = true;//由Mariner来选择决定
			TimeTags = 1.0f;
			DistanceTags = 10.0f;
			IsFullLength = false;
			Result = NULL;
		}

		QString NameOfCSP;///<条件符号名称
		IMyS57MapData* S57Map;///<当前海图对象
		IMyS57LayerData* S57Layer;///<当前海图图层
		IMyS57LayerData* DepareArea;///<Depare图层
		IMyS57LayerData* DrgareArea;///<Drgare图层
		int RecordIndex;///<当前记录的索引
		IMyS57ShapeRecord* S57ShapeRecord;///<当前记录
		std::vector<IMyS52Symbol*>* RecordSymbols;///已经解析完成的符号集合
		IMyS52Symbol* Result;///<S52符号解析结果
		double SafeDepth;///<安全深度

		float ShallowContour		;///<Shallow等深线
		float SafetyContour			;///<安全等深线
		float DeepContour			;///<深度等深线
		bool Two_shades				;
		bool ShallowPatten			;
		bool IsSymbolizedBoundraries;
		bool IsShowTextDescription	;
		float TimeTags				;
		float DistanceTags			;
		bool IsFullLength			;
	};

	/// <summary>
	/// S52条件符号程序解析类
	/// </summary>
	class MYCHARTS52SYMBOLLIB_EXPORT CMyS52ConditionSymbolProgram
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CMyS52ConditionSymbolProgram();

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CMyS52ConditionSymbolProgram();

		/// <summary>
		/// S52条件符号程序实例
		/// </summary>
		/// <returns>单例对象</returns>
		static CMyS52ConditionSymbolProgram* instance();

		/// <summary>
		/// 按条件符号程序解析S52风格
		/// </summary>
		/// <param name="nameOfCSP">条件符号程序名称</param>
		/// <param name="pS57Map">海图对象</param>
		/// <param name="pS57Layer">海图图层</param>
		/// <param name="recordIndex">当前记录的索引</param>
		/// <param name="pS57ShapeRecord">当前记录</param>
		/// <param name="isSymbolizedBoundries">是否为符号化边界</param>
		/// <param name="safeCountour">安全等深线</param>
		/// <param name="pRecordSymbols">已经解析的符号集合</param>
		/// <returns>当前解析返回的S52风格</returns>
		IMyS52Symbol* parS52Symbol(
			QString nameOfCSP,
			IMyS57MapData* pS57Map,IMyS57LayerData* pS57Layer,
			const int& recordIndex,IMyS57ShapeRecord* pS57ShapeRecord,
			const bool& isSymbolizedBoundries,
			const double& safeCountour,
			std::vector<IMyS52Symbol*>* pRecordSymbols);

	protected:
		IMyS52Symbol* cspCLRLIN01(S52CspParam* pCspParam);
		IMyS52Symbol* cspDEPARE01(S52CspParam* pCspParam);
		IMyS52Symbol* cspDEPCNT02(S52CspParam* pCspParam);
		IMyS52Symbol* cspLEGLIN02(S52CspParam* pCspParam);
		IMyS52Symbol* cspLIGHTS05(S52CspParam* pCspParam);
		IMyS52Symbol* cspOBSTRN04(S52CspParam* pCspParam);
		IMyS52Symbol* cspPASTRK01(S52CspParam* pCspParam);
		IMyS52Symbol* cspQUAPOS01(S52CspParam* pCspParam);
		IMyS52Symbol* cspQUALIN01(S52CspParam* pCspParam);
		IMyS52Symbol* cspQUAPNT01(S52CspParam* pCspParam);
		IMyS52Symbol* cspSLCONS03(S52CspParam* pCspParam);
		IMyS52Symbol* cspRESARE02(S52CspParam* pCspParam);
		IMyS52Symbol* cspRESTRN01(S52CspParam* pCspParam);
		IMyS52Symbol* cspSOUNDG02(S52CspParam* pCspParam);
		IMyS52Symbol* cspTOPMAR01(S52CspParam* pCspParam);
		IMyS52Symbol* cspWRECKS02(S52CspParam* pCspParam);

		IMyS52Symbol* cspSEABED01(float DRVAL1, float DRVAL2,S52CspParam* pCspParam);
		IMyS52Symbol* cspRESCSP01(QString Restrn,S52CspParam* pCspParam);
		IMyS52Symbol* cspRESCSP01_ContinuationA(QString Restrn,S52CspParam* pCspParam);
		IMyS52Symbol* cspRESCSP01_ContinuationB(QString Restrn,S52CspParam* pCspParam);
		IMyS52Symbol* cspRESCSP01_ContinuationC(QString Restrn,S52CspParam* pCspParam);
		IMyS52Symbol* cspDEPCNT02_ContinuationA(QString valdco,S52CspParam* pCspParam);
		IMyS52Symbol* cspDEPCNT02_ContinuationB(S52CspParam* pCspParam);
		IMyS52Symbol* cspLEGLIN02_ContinuationA(S52CspParam* pCspParam);
		IMyS52Symbol* cspLIGHTS05_ContinuationA(S52CspParam* pCspParam);
		IMyS52Symbol* cspLITDSN01(S52CspParam* pCspParam);
		IMyS52Symbol* cspLIGHTS05_ContinuationB(QString sectr1, QString sectr2, QString colour,S52CspParam* pCspParam);
		float cspDEPVAL01(S52CspParam* pCspParam);//return a float value;
		bool cspSNDFRM02(float depth_value,std::vector<ComplexSymbol>& symbolList,S52CspParam* pCspParam);
		bool cspUDWHAZ03(float depth_value,S52CspParam* pCspParam);
		void cspOBSTRN04_ContinuationA(bool isDangerShow,S52CspParam* pCspParam);
		void cspOBSTRN04_ContinuationB(bool isDangerShow,S52CspParam* pCspParam);
		void cspOBSTRN04_ContinuationC(bool isDangerShow,S52CspParam* pCspParam);

		void cspSNDFRM02_ContinuationA(float depth_value, std::vector<ComplexSymbol>& listOfSymbol, QString symbol_prefix,S52CspParam* pCspParam);
		void cspSNDFRM02_ContinuationB(float depth_value, std::vector<ComplexSymbol>& listOfSymbol, QString symbol_prefix,S52CspParam* pCspParam);
		void cspSNDFRM02_ContinuationC(float depth_value, std::vector<ComplexSymbol>& listOfSymbol, QString symbol_prefix,S52CspParam* pCspParam);
		void cspSLCONS03_ContinuationA(S52CspParam* pCspParam);
		void cspRESARE02_ContinuationA(QString Restrn, QString Catrea,S52CspParam* pCspParam);
		void cspRESARE02_ContinuationB(QString Restrn, QString Catrea,S52CspParam* pCspParam);
		void cspRESARE02_ContinuationC(QString Restrn, QString Catrea,S52CspParam* pCspParam);
		void cspRESARE02_ContinuationD(QString Catrea,S52CspParam* pCspParam);
		void cspWRECKS02_ContinuationA(QString valsou, QString watlev, QString catwrk,S52CspParam* pCspParam);
		void cspWRECKS02_ContinuationB(QString valsou, QString watlev, std::vector<ComplexSymbol>& depthSymbolList,S52CspParam* pCspParam);

	protected:

		static CMyS52ConditionSymbolProgram* s_instance;
	
	};
}
#endif // MYCHARTS52CONDITIONPROGREAM_H
