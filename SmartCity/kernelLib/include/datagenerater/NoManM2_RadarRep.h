#pragma once
#ifndef NO_MANM2_INCLUDE_H
#define NO_MANM2_INCLUDE_H
#include <QMetaType>
#include <QBitArray>

#pragma pack(1)
/************************14所雷达发送数据接口**********************/
//雷达数据报文头
typedef struct RadarRepMSGHeader
{
	unsigned int RepFlag;	//0xddccbbaa
	unsigned int RepTime;	//1970.1.1 00:00:00距现在的秒数
	unsigned int SrcID;		//信源 4字节？
	unsigned int DataType;	//0x01-雷达状态  0x02-雷达目标  0x03-雷达控制报 0x04-消失报
	unsigned int DataLen;	//数据域长度 雷达状态-0x20  雷达目标-0x20 *n n最多20个 ， 雷达控制-0x20
} RadarRepMSGHeader;
Q_DECLARE_METATYPE(RadarRepMSGHeader)

//雷达探测目标子结构
typedef struct RadarTargetRepSub
{
	unsigned char TargetSeq;		//目标个数序号
	int SignalA;		//目标信号幅度
	int SignalType;		//信号类型（遥测、图传、其他）
	unsigned int SubA;	//方位角 0.1du
	unsigned int SubR;	// 目标距离
	unsigned int Carr;	// 目标载频
	unsigned char TargetSubClass;//无人机型号
} RadarTargetRepSub;
Q_DECLARE_METATYPE(RadarTargetRepSub)

//雷达探测目标正文
typedef struct RadarTargetRep
{
	RadarRepMSGHeader head;
	unsigned int PH;			//批号
	unsigned int A;				//方位 精度0.1， 100=10.0度
	unsigned int E;				//仰角 精度0.1， 100=10.0度
	unsigned int TrackCourse;	//航向 精度0.1， 100=10.0度
	unsigned int RCS;			//RCS  精度0.1， 100=10.0平方米
	unsigned int R;				//距离 精度0.1， 100=10.0m
	unsigned int TrackSpeed;	//速度 精度0.1， 100=10.0m/s
	unsigned int Reserve;		//保留

	double lon;                 //经度
	double lat;                 //纬度
	double alt;                 //高度 m

	//////////////////////////////////////////////////////add by jestin 20180720
	unsigned char TargetClass;//目标类型 0-未知， 1-无人机 2-风筝 3-民航
	unsigned char Num;//无线电侦测的目标个数不超过40
	RadarTargetRepSub RDTargetSub[40];
	//待续
	/////////////////////////////////////////////////////
} RadarTargetRep;
Q_DECLARE_METATYPE(RadarTargetRep)

#pragma	pack()
#endif