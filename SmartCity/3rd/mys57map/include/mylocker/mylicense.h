#ifndef _MYLICENSE_H
#define _MYLICENSE_H


#ifdef MYLOCKER_EXPORTS
# define MYLOCKER_EXPORT _declspec(dllexport)
#else
# define MYLOCKER_EXPORT _declspec(dllimport)
#endif

class MYLOCKER_EXPORT CMylicense
{
	//许可状态
	enum LicenserState
	{
		//无效状态
		LS_INVALID,

		//正常状态
		LS_NOMAL,

		//过期状态
		LS_EXPIRE

	};
public:
	CMylicense();

	~CMylicense();

	char* createUserInfo(int terminalDayNum);

	char* generateLicenseKey(const char* pUserInfo);

	bool generatefile(const char* pUserInfo, char* pLicensePath);

	LicenserState fromfile(char* pLicensePath);
};

#endif