#ifndef _INSTANCEMODELLAYER_H
#define _INSTANCEMODELLAYER_H

#include "citybuilder/citybuilderconfig.h"
#include "framework/core/ilayer.h"
#include "rendertechnique/instanceobject.h"
#include "pipenet/pipelinedataset.h"
#include "pipenet/pipepointdataset.h"

namespace MyChart
{
	class IMyLayer;
}

namespace CityBuilder
{
	class VR_CITYBUILDER_DLL CPipeLayerDriver
	{
	public:
		CPipeLayerDriver();

		virtual ~CPipeLayerDriver();

		static bool load(const std::string& filePath,PipeNet::CPipeLineDataSet*& inout_pDataSet);

		static bool loadEx(const std::string& filePath, PipeNet::CPipeLineDataSet*& inout_pDataSet);

		static bool load(const std::string& filePath,PipeNet::CPipePointDataSet*& inout_pDataSet);

		static bool loadEx(const std::string& filePath, PipeNet::CPipePointDataSet*& inout_pDataSet);
	};

	class VR_CITYBUILDER_DLL CInstanceModelLayer
		: public Core::ILayer
	{
	public:
		CInstanceModelLayer();

		CInstanceModelLayer(osgEarth::MapNode* pRefMapNode);

		virtual bool loadDataFromFile(const std::string& filePath);

		virtual bool loadToScene();

		virtual bool loadToScene(PipeNet::CPipeLineDataSet* pPipeLineDataSet);

		virtual bool renderToScene();

		virtual bool deRenderFromScene();

		virtual bool unLoadFromScene();

	protected:
		MyChart::IMyLayer* mpDataSet;
		Render::CInstanceObject* mpRenderObject;
	};
}


#endif