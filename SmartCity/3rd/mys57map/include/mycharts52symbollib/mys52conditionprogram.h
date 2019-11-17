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
	/// ����S52��������ʱ���õ���ʱ������
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
			IsSymbolizedBoundraries = true;//�Ƿ��ô����ŵı߽�
			IsShowTextDescription = true;//��Mariner��ѡ�����
			TimeTags = 1.0f;
			DistanceTags = 10.0f;
			IsFullLength = false;
			Result = NULL;
		}

		QString NameOfCSP;///<������������
		IMyS57MapData* S57Map;///<��ǰ��ͼ����
		IMyS57LayerData* S57Layer;///<��ǰ��ͼͼ��
		IMyS57LayerData* DepareArea;///<Depareͼ��
		IMyS57LayerData* DrgareArea;///<Drgareͼ��
		int RecordIndex;///<��ǰ��¼������
		IMyS57ShapeRecord* S57ShapeRecord;///<��ǰ��¼
		std::vector<IMyS52Symbol*>* RecordSymbols;///�Ѿ�������ɵķ��ż���
		IMyS52Symbol* Result;///<S52���Ž������
		double SafeDepth;///<��ȫ���

		float ShallowContour		;///<Shallow������
		float SafetyContour			;///<��ȫ������
		float DeepContour			;///<��ȵ�����
		bool Two_shades				;
		bool ShallowPatten			;
		bool IsSymbolizedBoundraries;
		bool IsShowTextDescription	;
		float TimeTags				;
		float DistanceTags			;
		bool IsFullLength			;
	};

	/// <summary>
	/// S52�������ų��������
	/// </summary>
	class MYCHARTS52SYMBOLLIB_EXPORT CMyS52ConditionSymbolProgram
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CMyS52ConditionSymbolProgram();

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~CMyS52ConditionSymbolProgram();

		/// <summary>
		/// S52�������ų���ʵ��
		/// </summary>
		/// <returns>��������</returns>
		static CMyS52ConditionSymbolProgram* instance();

		/// <summary>
		/// ���������ų������S52���
		/// </summary>
		/// <param name="nameOfCSP">�������ų�������</param>
		/// <param name="pS57Map">��ͼ����</param>
		/// <param name="pS57Layer">��ͼͼ��</param>
		/// <param name="recordIndex">��ǰ��¼������</param>
		/// <param name="pS57ShapeRecord">��ǰ��¼</param>
		/// <param name="isSymbolizedBoundries">�Ƿ�Ϊ���Ż��߽�</param>
		/// <param name="safeCountour">��ȫ������</param>
		/// <param name="pRecordSymbols">�Ѿ������ķ��ż���</param>
		/// <returns>��ǰ�������ص�S52���</returns>
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
