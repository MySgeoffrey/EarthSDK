#include "framework/common/languageconfig.h"

static std::map<std::string,std::string> LanguageDatas;

const std::string CLanguageConfig::StateTextType_Longitude = "StateTextType_Longitude";
const std::string CLanguageConfig::StateTextType_Lantitude = "StateTextType_Lantitude";
const std::string CLanguageConfig::StateTextType_TerrainAltitude = "StateTextType_TerrainAltitude";
const std::string CLanguageConfig::StateTextType_ViewAltitude = "StateTextType_ViewAltitude";
const std::string CLanguageConfig::DistanceTextType_KiloMetre = "DistanceTextType_KiloMetre";
const std::string CLanguageConfig::DistanceTextType_Meter = "DistanceTextType_Meter";
const std::string CLanguageConfig::DistanceTextType_Total = "DistanceTextType_Total";
const std::string CLanguageConfig::DistanceTextType_AzimuthLabel = "DistanceTextType_AzimuthLabel";
const std::string CLanguageConfig::DistanceTextType_AzimuthUnit = "DistanceTextType_AzimuthUnit";
const std::string CLanguageConfig::AreaTextType_Title = "AreaTextType_Title";
const std::string CLanguageConfig::AreaTextType_SqureKiloMetre = "AreaTextType_SqureKiloMetre";
const std::string CLanguageConfig::AreaTextType_SqureMeter = "AreaTextType_SqureMeter";
const std::string CLanguageConfig::AzimuthTextType_Title = "AzimuthTextType_Title";
const std::string CLanguageConfig::AzimuthTextType_Degree = "AzimuthTextType_Degree";
const std::string CLanguageConfig::GeoRingGrid_DistanceTitle = "GeoRingGrid_DistanceTitle";
const std::string CLanguageConfig::GeoRingGrid_DistanceMeter = "GeoRingGrid_DistanceMeter";
const std::string CLanguageConfig::GeoRingGrid_AzimuthTitle = "GeoRingGrid_AzimuthTitle";
const std::string CLanguageConfig::GeoRingGrid_AzimuthTime = "GeoRingGrid_AzimuthTime";
const std::string CLanguageConfig::GeoRectGrid_DistanceTitle = "GeoRectGrid_DistanceTitle";
const std::string CLanguageConfig::GeoRectGrid_DistanceMeter = "GeoRectGrid_DistanceMeter";
const std::string CLanguageConfig::GeoRectGrid_AzimuthTitle = "GeoRectGrid_AzimuthTitle";
const std::string CLanguageConfig::GeoRectGrid_AzimuthDegree = "GeoRectGrid_AzimuthDegree";
const std::string CLanguageConfig::GraphicEdit_InfiniteText = "GraphicEdit_InfiniteText";
const std::string CLanguageConfig::GraphicEdit_FiniteText = "GraphicEdit_FiniteText";

CLanguageConfig::CLanguageConfig()
{

}

CLanguageConfig::~CLanguageConfig()
{

}

void CLanguageConfig::initial()
{
	LanguageDatas[StateTextType_Longitude] = "经度:";
	LanguageDatas[StateTextType_Lantitude] = "纬度:";
	LanguageDatas[StateTextType_TerrainAltitude] = "高程:";
	LanguageDatas[StateTextType_ViewAltitude] = "视点高度:";

	LanguageDatas[DistanceTextType_KiloMetre] = "(公里)";
	LanguageDatas[DistanceTextType_Meter] = "(米)";
	LanguageDatas[DistanceTextType_Total] = "总长:";
	LanguageDatas[DistanceTextType_AzimuthLabel] = "方位角:";
	LanguageDatas[DistanceTextType_AzimuthUnit] = "(度)";

	LanguageDatas[AreaTextType_Title] = "面积:";
	LanguageDatas[AreaTextType_SqureKiloMetre] = "(平方公里)";
	LanguageDatas[AreaTextType_SqureMeter] = "(平方米)";

	LanguageDatas[AzimuthTextType_Title] = "方位角:";
	LanguageDatas[AzimuthTextType_Degree] = "(度)";
	
	LanguageDatas[GeoRingGrid_DistanceTitle] = "距离:";
	LanguageDatas[GeoRingGrid_DistanceMeter] = "(米)";
	LanguageDatas[GeoRingGrid_AzimuthTitle] = "方位:";
	LanguageDatas[GeoRingGrid_AzimuthTime] = "(点)";

	LanguageDatas[GeoRectGrid_DistanceTitle] = "距离:";
	LanguageDatas[GeoRectGrid_DistanceMeter] = "(米)";
	LanguageDatas[GeoRectGrid_AzimuthTitle] = "方位:";
	LanguageDatas[GeoRectGrid_AzimuthDegree] = "(度)";

	LanguageDatas[GraphicEdit_InfiniteText] = "鼠标选择若干点后绘制图形,右键结束";
	LanguageDatas[GraphicEdit_FiniteText] = "鼠标选择%s个点后绘制图形,右键结束";

}

std::string CLanguageConfig::getValue(const std::string& key)
{
	if (LanguageDatas.empty())
	{
		CLanguageConfig::initial();
	}
	return LanguageDatas[key];
}

void CLanguageConfig::setValue(const std::string& key,const std::string& value)
{
	if (LanguageDatas.empty())
	{
		CLanguageConfig::initial();
	}
	LanguageDatas[key] = value;
}