//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : ccolortable.h
//  @ Date : 2017/9/27
//  @ Author : sgeoffrey
//
//


#if !defined(_CCOLORTABLE_H)
#define _CCOLORTABLE_H

#include "mychartsymbollib/rleobject.h"
#include <map>
#include <QString>

namespace MyChart
{
	class CColorRecord;
	class MYCHARTSYMBOLLIB_EXPORT CColorTable : public CRleObject
	{
	public:
		CColorTable();

		CColorTable(QString& palettedata);

		virtual ~CColorTable();

		void p();

	public:
		QString Name;
		QString COLS;
		QString ObjectType;
		std::map<QString, CColorRecord*> Colors;
	};
}
#endif  //_CCOLORTABLE_H