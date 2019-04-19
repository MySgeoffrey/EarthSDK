#include "igenguid.h"


namespace Common
{
	static IGenguid* s_guid = NULL;
	IGenguid::IGenguid()
	{
		
	}


	IGenguid::~IGenguid()
	{
		releaseInstance();
		CoUninitialize();
	}

	IGenguid* IGenguid::instance()
	{
		if (NULL == s_guid)
		{
			CoInitialize(NULL);
			s_guid = new IGenguid();
		}
		return s_guid;
	}

	void IGenguid::releaseInstance()
	{
		if (NULL != s_guid)
		{
			delete s_guid;
			s_guid = NULL;
		}
	}

	string IGenguid::createGuid()
	{
		char buffer[GUID_LEN] = { 0 };
		GUID guid;

		if (S_OK != CoCreateGuid(&guid))
		{
			fprintf(stderr, "create guid error\n");
			return "";
		}
		_snprintf_s(buffer, sizeof(buffer),
			"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2],
			guid.Data4[3], guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
		printf("guid: %s\n", buffer);		
		return buffer;
	}

}


