//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : cvectorpattern.h
//  @ Date : 2017/9/27
//  @ Author : sgeoffrey
//
//


#if !defined(_CVECTORPATTERN_H)
#define _CVECTORPATTERN_H

#include "mychartsymbollib/symbol.h"

namespace MyChart
{
	class CColorTable;
	class CVectorPattern : public CSymbol
	{
	public:

		CVectorPattern();

		CVectorPattern(QString& bitmapdata, CColorTable* colortable);

		virtual ~CVectorPattern();

		void Operation1();

		bool isValid();

	public:
		QString PATT;
		QString PATD;
		QString PXPO;
		QString PCRF;

		QString Value2;
		int Value3;
		int Value4;

		bool _isvalid;
	};
}
#endif  //_CVECTORPATTERN_H
