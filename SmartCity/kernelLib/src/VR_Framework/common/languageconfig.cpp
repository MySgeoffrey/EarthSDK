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
	LanguageDatas[StateTextType_Longitude] = "����:";
	LanguageDatas[StateTextType_Lantitude] = "γ��:";
	LanguageDatas[StateTextType_TerrainAltitude] = "�߳�:";
	LanguageDatas[StateTextType_ViewAltitude] = "�ӵ�߶�:";

	LanguageDatas[DistanceTextType_KiloMetre] = "(����)";
	LanguageDatas[DistanceTextType_Meter] = "(��)";
	LanguageDatas[DistanceTextType_Total] = "�ܳ�:";
	LanguageDatas[DistanceTextType_AzimuthLabel] = "��λ��:";
	LanguageDatas[DistanceTextType_AzimuthUnit] = "(��)";

	LanguageDatas[AreaTextType_Title] = "���:";
	LanguageDatas[AreaTextType_SqureKiloMetre] = "(ƽ������)";
	LanguageDatas[AreaTextType_SqureMeter] = "(ƽ����)";

	LanguageDatas[AzimuthTextType_Title] = "��λ��:";
	LanguageDatas[AzimuthTextType_Degree] = "(��)";
	
	LanguageDatas[GeoRingGrid_DistanceTitle] = "����:";
	LanguageDatas[GeoRingGrid_DistanceMeter] = "(��)";
	LanguageDatas[GeoRingGrid_AzimuthTitle] = "��λ:";
	LanguageDatas[GeoRingGrid_AzimuthTime] = "(��)";

	LanguageDatas[GeoRectGrid_DistanceTitle] = "����:";
	LanguageDatas[GeoRectGrid_DistanceMeter] = "(��)";
	LanguageDatas[GeoRectGrid_AzimuthTitle] = "��λ:";
	LanguageDatas[GeoRectGrid_AzimuthDegree] = "(��)";

	LanguageDatas[GraphicEdit_InfiniteText] = "���ѡ�����ɵ�����ͼ��,�Ҽ�����";
	LanguageDatas[GraphicEdit_FiniteText] = "���ѡ��%s��������ͼ��,�Ҽ�����";

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