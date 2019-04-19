#include <objbase.h>  
#include <stdio.h>  
#include "framework/vr_globeframework_global.h"

#define GUID_LEN 64  
using namespace std;
namespace Common
{
	/// <summary>
	/// Œﬁ–ß¿‡
	/// </summary>
	class VR_PIPEFRAMEWORK_EXPORT IGenguid
	{
	public:		
		~IGenguid();
		static IGenguid* instance();
		static void releaseInstance();
		string createGuid();
	private:
		IGenguid();


	};

}

