#ifndef _GPRMC_H
#define _GPRMC_H


#include "mymeaturelib/mychartmeaturelib_global.h"
#include <list>

namespace MyChart
{
	typedef unsigned char U8;
	//时间结构体：
	typedef struct{
		int year; 
		int month; 
		int day;
		int hour;
		int minute;
		int second;
	}DATE_TIME;

	//GPS数据结构体
	typedef struct GPS_DATA{

	public:

		GPS_DATA()
		{
			latitude = 0;			
			longitude = 0;			
			latitude_Degree= 0;    
			latitude_Cent = 0;		
			latitude_Second = 0;	
			longitude_Degree = 0;   
			longitude_Cent = 0;		
			longitude_Second = 0;	
			speed = 0;				
			direction = 0;			
			height = 0;				
			satellite = 0;
			depth = 0;				
		}

		GPS_DATA(const GPS_DATA& object)
		{
			latitude = object.latitude;			
			longitude = object.longitude;			
			latitude_Degree= object.latitude_Degree;    
			latitude_Cent = object.latitude_Cent;		
			latitude_Second = object.latitude_Second;	
			longitude_Degree = object.longitude_Degree;   
			longitude_Cent = object.longitude_Cent;		
			longitude_Second = object.longitude_Second;	
			speed = object.speed;				
			direction = object.direction;			
			height = object.height;				
			satellite = object.satellite;
			depth = object.depth;			
		}

		virtual ~GPS_DATA()
		{}

		double		latitude;			//经度
		double		longitude;			//纬度
		double		latitude_Degree;    //度
		double		latitude_Cent;		//分
		double		latitude_Second;	//秒
		double		longitude_Degree;   //度
		double		longitude_Cent;		//分
		double		longitude_Second;	//秒
		float		speed;				//速度
		float		direction;			//航向
		float		height;				//海拔高度
		int			satellite;
		double		depth;				//水深
		U8			NS;
		U8			EW;
		DATE_TIME	D;
	}GPS_DATA;

	class MYCHARTMEATURELIB_EXPORT CGPSParser
	{
	public:
		CGPSParser();

		virtual ~CGPSParser();

		static int gps_rmc_parse(char *line, GPS_DATA *GPS);

		static int gps_gga_parse(char *line, GPS_DATA *GPS);

		static int gps_dbt_parse(char *line, GPS_DATA *GPS);

		static int gps_parse(QString gpsDataPath,std::list<GPS_DATA>& gpsDatas);
	};

}

#endif //_GPRMC_H