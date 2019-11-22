#ifndef _VR_CITYBUILDER_CONGIF_H
#define _VR_CITYBUILDER_CONGIF_H

#ifdef VR_CITYBUILDEREX_EXPORTS
#define VR_CITYBUILDER_DLL _declspec(dllexport)
#else
#define VR_CITYBUILDER_DLL  _declspec(dllimport)
#endif
#endif