//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : helper.h
//  @ Date : 2017/9/27
//  @ Author : sgeoffrey
//
//


#if !defined(_HEPER_H)
#define _HEPER_H

namespace MyChart
{
	static QString trim(QString str , char c)
	{
#if 0
		int pos = str.indexOf(c);
		QString r = str.mid(pos + 1);
		r = str.mid(0,r.indexOf(c) - 1);
#else
		/*int pos = str.indexOf(c);
		QString r = str.mid(pos);
		int index = str.indexOf(r);
		r = str.mid(0,index);*/
		QString r = str.replace(c,"");
#endif
		return r;
	}
}
#endif  //_HEPER_H