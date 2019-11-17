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
	/// ��ͼ�������ſ������
	/// </summary>
	class MYCHARTSYMBOLLIB_EXPORT CS57Symbollib
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CS57Symbollib();

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~CS57Symbollib();

		/// <summary>
		/// ����s57��ͼ���ſ��
		/// </summary>
		/// <returns>�Ƿ�ɹ������������ſ�/returns>
		bool initial();

		/// <summary>
		/// ж��s57��ͼ���ſ��
		/// </summary>
		void uninitial();

		/************************************************************************/
		/*�б���ſ��еķ�������                                                 */
		/************************************************************************/

		/// <summary>
		/// �ϲ�Bitmaps������Ϣ
		/// </summary>
		/// <param name="bitmapText">�����ַ�������</param>
		void mergeBitmaps(QString bitmapText);

		/// <summary>
		/// �ϲ�Vectors������Ϣ
		/// </summary>
		/// <param name="vectorText">�����ַ�������</param>
		void mergeVectors(QString vectorText);

		/// <summary>
		/// �ж��Ƿ�ΪBitmap���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isBitmap(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪBitmapSymbol���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isBitmapSymbol(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪBitmapPattern���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isBitmapPattern(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪVector���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isVector(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪVectorSymbol���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isVectorSymbol(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪVectorPattern���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isVectorPattern(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪVectorLine���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isVectorLine(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪColorTable����
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isColorTable(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪLines���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isLines(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪPaperChart���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isPaperChart(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪSimplified���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isSimplified(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪPlainBoundaries���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isPlainBoundaries(QString component);

		/// <summary>
		/// �ж��Ƿ�ΪSymbolizedBoundaries���ͷ���
		/// </summary>
		/// <param name="component">�ж�����</param>
		/// <returns>true���ǣ�false:��</returns>
		bool isSymbolizedBoundaries(QString component);

		/// <summary>
		/// �������Ž�����ʵ��
		/// </summary>
		/// <returns>��������</returns>
		static CS57Symbollib* instance();

		/// <summary>
		/// �ͷŻ������Ž�����ʵ��
		/// </summary>
		static void releaseInstance();

		static QString stripLenFromString(QString original);

		static QString trim(QString string , char c);

	private:

		static CS57Symbollib* s_s57symbilLib;

		/************************************************************************/
		/* ���ſ���Ϣ                                                            */
		/************************************************************************/
		//���ַ�������
	public: 
		static const QString PAPER_CHART;///<ֽ�ʷ���
		static const QString SIMPLIFIED;///<�򵥷���
		static const QString LINES;///<�߷���
		static const QString PLAIN_BOUNDARIES;///<�򵥱߽����
		static const QString SYMBOLIZED_BOUNDARIES;///<���Ż��߽����

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
