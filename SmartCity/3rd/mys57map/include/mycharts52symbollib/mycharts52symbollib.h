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
	/// S52�����ʱ��������
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
	/// S52���Ž�����
	/// </summary>
	class MYCHARTS52SYMBOLLIB_EXPORT CMyChartS52SymbolLib
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CMyChartS52SymbolLib();

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~CMyChartS52SymbolLib();

		/// <summary>
		/// ��������
		/// </summary>
		/// <returns>��������</returns>
		static CMyChartS52SymbolLib* instance();

		/// <summary>
		/// ����S52���
		/// </summary>
		/// <param name="safeCountour">��ȫ������</param>
		/// <param name="areaModeOfSymbol">��״��������</param>
		/// <param name="pointModeOfSymbol">��״��������</param>
		/// <param name="in_pS57Layer">��ǰS57ͼ��</param>
		/// <param name="inout_pS52Symbol">���ص�S52���</param>
		/// <returns>�Ƿ�����ɹ�</returns>
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
