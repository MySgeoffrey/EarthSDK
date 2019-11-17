//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : imys52symbol.h
//  @ Date : 2017/11/3
//  @ Author : sgeoffrey
//
//


#if !defined(_IMYS52SYMBOL_H)
#define _IMYS52SYMBOL_H

#include "mycharts52symbollib/mycharts52symbollib_global.h"
#include "mychartmaplib/imysymbol.h"

namespace MyChart
{
	
	/// <summary>
    /// 复杂符号参数
    /// </summary>
    struct ComplexSymbol
    {
	public:

		/// <summary>
		/// 构造函数
		/// </summary>
		ComplexSymbol()
		{
			CodeOfSymbol = "";
			AngleOfSymbol = 0;
		}

		/// <summary>
		/// 析构函数
		/// </summary>
		~ComplexSymbol()
		{
		
		}
        /// <summary>
        /// 符号的代码
        /// </summary>
        QString CodeOfSymbol;

        /// <summary>
        /// 符号的旋转角度角度
        /// </summary>
        float AngleOfSymbol;
    };

	/// <summary>
	/// 基础参数类
	/// </summary>
	struct BaseParameter
    {
        /// <summary>
        /// 最大显示比例
        /// </summary>
        QString MaxScale;
        /// <summary>
        /// 最大显示比例
        /// </summary>
        QString MinScale;
        /// <summary>
        /// 渲染优先级
        /// </summary>
        QString DisplayPriority;
        /// <summary>
        /// 雷达覆盖
        /// </summary>
        QString Radar;
        /// <summary>
        /// 显示目录
        /// </summary>
        QString ImoDisplayCategory;
        /// <summary>
        /// 显示组
        /// </summary>
        QString ViewingGroup;
    };

	/// <summary>
	/// 灯塔参数
	/// </summary>
	struct ParameterOfLight
    {
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		ParameterOfLight()
		{
			LegStyle = "";
			LegLength = 0;
			ArcSingleStyle = "";
			ArcLowStyle = "";
			ArcStyle = "";
			QString Color;
			Width = 0;
			Length = 0;
			Sector1 = 0;
			Sector2 = 0;
		}
        /// <summary>
        /// 边线线型
        /// </summary>
        QString LegStyle;
        /// <summary>
        /// 扇区边线长度
        /// </summary>
        float LegLength;
        /// <summary>
        /// 弧断单一线型
        /// </summary>
        QString ArcSingleStyle;
        /// <summary>
        /// 弧段底下线型
        /// </summary>
        QString ArcLowStyle;
        /// <summary>
        /// 弧段上层线型
        /// </summary>
        QString ArcStyle;
        /// <summary>
        /// 弧段颜色
        /// </summary>
        QString Color;
        /// <summary>
        /// 弧段宽度
        /// </summary>
        int Width;
        /// <summary>
        /// 扇区半径
        /// </summary>
        float Length;
        /// <summary>
        /// 边线范围1
        /// </summary>
        float Sector1;
        /// <summary>
        /// 边线范围2
        /// </summary>
        float Sector2;
    };
	/// <summary>
	/// 符号绘制参数
	/// </summary>
	class CParaOfDrawSymbol
	{
	public:

		/// <summary>
		/// 构造函数
		/// </summary>
		CParaOfDrawSymbol(){}

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CParaOfDrawSymbol(){}
	};

	/// <summary>
	/// 画笔参数
	/// </summary>
	class CParaOfPen : public CParaOfDrawSymbol
    {
	public:
        QString Name;///<参数名称
        QString StyleOfSingleLine;///<单线样式
        float WidthOfSingleLine;///<单线宽度
        QString ColorOfSigleLine;///<单线颜色代码
    };
	/// <summary>
	/// 字符参数类
	/// </summary>
	struct Chars
    {
        QString strFormat;///<字符串的格式
        
        QString Style;///<样式
        QString Weight;///<比重
        QString Width;///<宽度
        QString Bodysize;///<大小
    };

	/// <summary>
	/// 字符串参数
	/// </summary>
	struct ParaOfTextString
    {
        /// <summary>
        /// 要显示的英文字符串
        /// </summary>
        QString TextString;
        /// <summary>
        /// 要显示的中文字符串
        /// </summary>
        QString TextStringOfChinese;
        /// <summary>
        /// 水平位置
        /// </summary>
        QString Hjust;
		/// <summary>
        /// 垂直位置
        /// </summary>
        QString Vjust;
		/// <summary>
        /// 空格大小
        /// </summary>
        QString Space;
        Chars Chars;
		/// <summary>
        /// X偏移
        /// </summary>
        QString Xoffs;
		/// <summary>
        /// Y偏移
        /// </summary>
        QString Yoffs;
		/// <summary>
        /// 颜色代码
        /// </summary>
        QString Colour;
        QString Display;
    };

	/// <summary>
	/// S52风格基类
	/// </summary>
	class MYCHARTS52SYMBOLLIB_EXPORT IMyS52Symbol 
		: public IMySymbol
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyS52Symbol();

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyS52Symbol();

		/// <summary>
		/// 将S52风格转换为绘制参数
		/// </summary>
		/// <returns>绘制参数</returns>
		virtual CParaOfDrawSymbol* toDrawSymbol();
	};
}


#endif  //_IMYS52SYMBOL_H
