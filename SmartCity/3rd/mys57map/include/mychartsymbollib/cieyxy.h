//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : ccieyxy.h
//  @ Date : 2017/9/27
//  @ Author : sgeoffrey
//
//


#if !defined(_CCIEYXY_H)
#define _CCIEYXY_H
#include "mychartsymbollib/mychartsymbollib_global.h"


namespace MyChart
{
	class MYCHARTSYMBOLLIB_EXPORT CCIEYxy
	{
	public:

		CCIEYxy(double Y, double x, double y);

		virtual ~CCIEYxy();

		double Y();

		void setY(double value);

		double x();

		void setx(double value);

		double y();

		void sety(double value);

		//static bool operator ==(CCIEYxy& item1,CCIEYxy& item2);//����������

		//bool operator !=(CCIEYxy item);//����������

		void Operation1();

	protected:
		double _x;
		double _y;
		double _Y;
	};
}
#endif  //_CCIEYXY_H
