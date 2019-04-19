#include "framework/common/globesetting.h"

CGlobeSetting* g_Settings(NULL);

CGlobeSetting::CGlobeSetting()
{
	this->mDataServiceUrl = "";
}

CGlobeSetting::~CGlobeSetting()
{}

CGlobeSetting* CGlobeSetting::instance()
{
	if (NULL == g_Settings)
	{
		g_Settings = new CGlobeSetting(); 
	}
	return g_Settings;
}

std::string CGlobeSetting::getDataServiceUrl()
{
	return this->mDataServiceUrl;
}

void CGlobeSetting::setDataServiceUrl(const std::string& dataServiceUrl)
{
	this->mDataServiceUrl = dataServiceUrl;
}

std::string CGlobeSetting::getAxFullPath()
{
	// 使用Windows API 获取到ActiveX控件在磁盘上的绝对路径
	CHAR ch[MAX_PATH];
	CHAR longPath[MAX_PATH];
#if QT_NO_DEBUG
	GetModuleFileNameA(::GetModuleHandleA("VR_GlobeWidget.dll"), ch, MAX_PATH);
#else
	GetModuleFileNameA(::GetModuleHandleA("VR_GlobeWidgetd.dll"), ch, MAX_PATH);
#endif
	GetLongPathNameA(ch, longPath, MAX_PATH);

	return osgDB::getFilePath(longPath);
}

void CGlobeSetting::unicodeToUTF8(const wstring &src, string& result)
{
	int n = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0);
	result.resize(n);
	::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0);
}

void CGlobeSetting::gb2312ToUnicode(const string& src, wstring& result)
{
	int n = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	result.resize(n);
	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
}

void CGlobeSetting::gb2312ToUtf8(const string& src, string& result)
{
	wstring strWideChar;
	gb2312ToUnicode(src, strWideChar);
	unicodeToUTF8(strWideChar, result);
}