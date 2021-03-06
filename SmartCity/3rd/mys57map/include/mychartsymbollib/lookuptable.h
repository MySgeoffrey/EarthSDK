//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : clookuptable.h
//  @ Date : 2017/9/27
//  @ Author : sgeoffrey
//
//


#if !defined(_CLOOKUPTABLE_H)
#define _CLOOKUPTABLE_H

#include "mychartsymbollib/rleobject.h"

namespace MyChart
{
	class CLookupTable : public CRleObject
	{
	public:

		CLookupTable();

		CLookupTable(QString& objecttext);

		virtual ~CLookupTable();

		void fillObjectFromText(QString& objecttext);

		void p();

	public:
		QString LUPT;
		QString ATTC;
		QString INST;
		QString DISC;
		QString LUCM;
		QString Code;
		QString Symbolization;
		QString AttributeCombination;
		QString SymbolizationInstructions;
		QString DisplayPriority;
		QString Radar;
		QString ImoDisplayCategory;
		QString ViewingGroup;
	};
}
#endif  //_CLOOKUPTABLE_H
