#ifndef _LANGUAGECONFIG_H
#define _LANGUAGECONFIG_H

#include "framework/vr_globeframework_global.h"
#include <map>

class VR_PIPEFRAMEWORK_EXPORT CLanguageConfig
{
public:
	CLanguageConfig();

	virtual ~CLanguageConfig();

	static void initial();

	static std::string getValue(const std::string& key);

	static void setValue(const std::string& key,const std::string& value);

public:

	//״̬����������
	static const std::string StateTextType_Longitude;
	static const std::string StateTextType_Lantitude;
	static const std::string StateTextType_TerrainAltitude;
	static const std::string StateTextType_ViewAltitude;

	//������������
	static const std::string DistanceTextType_KiloMetre;
	static const std::string DistanceTextType_Meter;
	static const std::string DistanceTextType_Total;
	static const std::string DistanceTextType_AzimuthLabel;
	static const std::string DistanceTextType_AzimuthUnit;

	static const std::string AreaTextType_Title;
	static const std::string AreaTextType_SqureKiloMetre;
	static const std::string AreaTextType_SqureMeter;
	 
	static const std::string AzimuthTextType_Title;
	static const std::string AzimuthTextType_Degree;

	//������Բ��
	static const std::string GeoRingGrid_DistanceTitle;
	static const std::string GeoRingGrid_DistanceMeter;
	static const std::string GeoRingGrid_AzimuthTitle;
	static const std::string GeoRingGrid_AzimuthTime;

	static const std::string GeoRectGrid_DistanceTitle;
	static const std::string GeoRectGrid_DistanceMeter;
	static const std::string GeoRectGrid_AzimuthTitle;
	static const std::string GeoRectGrid_AzimuthDegree;

	//ͼ�α༭������ʾ
	static const std::string GraphicEdit_InfiniteText;//���޵���
	static const std::string GraphicEdit_FiniteText;//���޵���

};

#endif
