#ifndef _MYLICENSE_H
#define _MYLICENSE_H


#ifdef MYLOCKER_EXPORTS
# define MYLOCKER_EXPORT _declspec(dllexport)
#else
# define MYLOCKER_EXPORT _declspec(dllimport)
#endif

class MYLOCKER_EXPORT CMylicense
{
	//���״̬
	enum LicenserState
	{
		//��Ч״̬
		LS_INVALID,

		//����״̬
		LS_NOMAL,

		//����״̬
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