//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : geographiccreator.h
//  @ Date : 2018/7/28
//  @ Author : sgeoffrey
//  @platform:���Ͽռ���Ϣƽ̨
//


#if !defined(_CGEOGRAPHICCREATOR_H)
#define _CGEOGRAPHICCREATOR_H

#include "framework/core/igeopoint.h"
#include "geographic/geographicconfig.h"
#include <osgEarth/MapNode>

namespace Graphic
{
	class VR_GEOGRAPHIC_EXPORT CGeoGraphicCreator
	{
	public:

		static Common::ISceneObject* createGraphicObject(
			const std::string& graphicType,osgEarth::MapNode* pRefMapNode);
	};

}

#endif  //_CGEOARITHMETIC_H