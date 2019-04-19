#include "geometryeditor/polygoneditor.h"
#include <math.h>

namespace GeometryEditor
{

	IPolygonEditor::IPolygonEditor(osgEarth::MapNode* pRefMapNode)
		: IPolylineEditor(pRefMapNode)
	{
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
			osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.6);
		//this->mGeometryStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 5.0;

		/*this->mGeometryStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->wallStyleName() = "building-wall";
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->roofStyleName() = "building-wall";*/

		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width() = 5.0;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 1);
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->tessellation() = 20;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->verticalOffset() = 0.1;
		/*this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
		*/
	}


	IPolygonEditor::~IPolygonEditor()
	{
	}

	void IPolygonEditor::initial()
	{
		if (this->getGeometry().empty())
			return;

		if (this->mpGeometryNode == NULL)
		{
			osgEarth::Features::Feature* pFeature = new osgEarth::Features::Feature(
				new osgEarth::Symbology::Polygon,
				this->mpRefMapNode->getMapSRS());

			this->mpGeometryNode = new osgEarth::Annotation::FeatureNode(
				this->mpRefMapNode, pFeature);
			this->mpGeometryNode->getFeature()->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;



			this->mpRefMapNode->addChild(this->mpGeometryNode);
		}

		osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
		pGeometry->clear();
		for (int i = 0; i < this->getGeometry().size(); ++i)
		{
			IEditPoint* pPoint = this->getGeometry().at(i);
			pGeometry->push_back(pPoint->getGeoPosition());
		}

		this->mpGeometryNode->setStyle(this->mGeometryStyle);

#if 0
		// 为墙的纹理创建风格
		osgEarth::Symbology::Style wallStyle;
		wallStyle.setName("building-wall");
		osgEarth::Symbology::SkinSymbol* wallSkin = wallStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();//得到设置墙体风格的指针
		//wallSkin->libraryName() = "us_resources";
		wallSkin->library() = "us_resources";
		wallSkin->addTag("building");
		wallSkin->randomSeed() = 1;

		// 为挤出的屋顶设置纹理风格
		osgEarth::Symbology::Style roofStyle;
		roofStyle.setName("building-roof");
		osgEarth::Symbology::SkinSymbol* roofSkin = roofStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();
		roofSkin->library() = "us_resources";
		roofSkin->addTag("rooftop");
		roofSkin->randomSeed() = 1;
		roofSkin->isTiled() = true;

		//将所有的墙体风格集合到一个建筑物上面，形成虚构的建筑物
		osgEarth::Symbology::StyleSheet* styleSheet = new osgEarth::Symbology::StyleSheet();
		//styleSheet->addStyle(buildingStyle);//建筑物风格
		styleSheet->addStyle(wallStyle);//墙体纹理的风格
		styleSheet->addStyle(roofStyle);//屋顶纹理的风格

		// 获取纹理图片所在文件夹的索引文件
		osgEarth::Symbology::ResourceLibrary* reslib = new osgEarth::Symbology::ResourceLibrary("us_resources", "../data/resources/textures_us/catalog.xml");
		styleSheet->addResourceLibrary(reslib);
		this->mpGeometryNode->setStyleSheet(styleSheet);
#endif
		
		this->mpGeometryNode->init();

		if (this->mpPointsEdit != NULL)
		{
			int count = this->mpPointsEdit->getNumChildren();
			this->mpPointsEdit->removeChildren(0, count);
			this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
			this->mpPointsEdit = NULL;
		}
		this->mpPointsEdit = new osgEarth::Annotation::FeatureEditor(this->mpGeometryNode);
		this->mpPointsEdit->setSize(
			this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->size().get());
		this->mpPointsEdit->setColor(
			this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->fill()->color());
		this->mpRefMapNode->addChild(this->mpPointsEdit);

		this->createEditorCallBack();
	}

	Common::ISceneObject* IPolygonEditor::generateSceneObject()
	{
		return NULL;
	}

	void IPolygonEditor::performPicked(osg::Vec3d pos)
	{
		IEditPoint* pEditPoint = new IEditPoint();
		pEditPoint->setGeoPosition(pos);
		pEditPoint->setIndex(this->getGeometry().size());
		this->getGeometry().push_back(pEditPoint);

		if (this->mpGeometryNode == NULL)
		{
			osgEarth::Features::Feature* pFeature = new osgEarth::Features::Feature(
				new osgEarth::Symbology::Polygon,
				this->mpRefMapNode->getMapSRS());

			this->mpGeometryNode = new osgEarth::Annotation::FeatureNode(
				this->mpRefMapNode, pFeature);
			this->mpGeometryNode->getFeature()->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

			this->mpRefMapNode->addChild(this->mpGeometryNode);
		}

		osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
		pGeometry->push_back(pos);
		
#if 0
		// 为墙的纹理创建风格
		osgEarth::Symbology::Style wallStyle;
		wallStyle.setName("building-wall");
		osgEarth::Symbology::SkinSymbol* wallSkin = wallStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();//得到设置墙体风格的指针
		//wallSkin->libraryName() = "us_resources";
		wallSkin->library() = "us_resources";
		wallSkin->addTag("building");
		wallSkin->randomSeed() = 1;

		// 为挤出的屋顶设置纹理风格
		osgEarth::Symbology::Style roofStyle;
		roofStyle.setName("building-roof");
		osgEarth::Symbology::SkinSymbol* roofSkin = roofStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();
		roofSkin->library() = "us_resources";
		roofSkin->addTag("rooftop");
		roofSkin->randomSeed() = 1;
		roofSkin->isTiled() = true;

		//将所有的墙体风格集合到一个建筑物上面，形成虚构的建筑物
		osgEarth::Symbology::StyleSheet* styleSheet = new osgEarth::Symbology::StyleSheet();
		//styleSheet->addStyle(buildingStyle);//建筑物风格
		styleSheet->addStyle(wallStyle);//墙体纹理的风格
		styleSheet->addStyle(roofStyle);//屋顶纹理的风格

		// 获取纹理图片所在文件夹的索引文件
		osgEarth::Symbology::ResourceLibrary* reslib = new osgEarth::Symbology::ResourceLibrary("us_resources", "../data/resources/textures_us/catalog.xml");
		styleSheet->addResourceLibrary(reslib);
		this->mpGeometryNode->setStyleSheet(styleSheet);
#endif
		
		this->mpGeometryNode->setStyle(this->mGeometryStyle);
		this->mpGeometryNode->init();

		if (this->mpPointsEdit != NULL)
		{
			int count = this->mpPointsEdit->getNumChildren();
			this->mpPointsEdit->removeChildren(0, count);
			this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
			this->mpPointsEdit = NULL;
		}
		this->mpPointsEdit = new osgEarth::Annotation::FeatureEditor(this->mpGeometryNode);
		this->mpPointsEdit->setSize(
			this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->size().get());
		this->mpPointsEdit->setColor(this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->fill()->color());
		this->mpRefMapNode->addChild(this->mpPointsEdit);

		this->createEditorCallBack();
	}

	void IPolygonEditor::performMoving(osg::Vec3d pos)
	{

	}

	void IPolygonEditor::performPickedXYZ(osg::Vec3d pos)
	{

	}

	void IPolygonEditor::performMovingXYZ(osg::Vec3d pos)
	{

	}

	void IPolygonEditor::performRightPicked()
	{
		this->mIsEditState = false;
		IGeometryEditor::performRightPicked();
	}

	void IPolygonEditor::performDeleteDrawing()
	{
		IPolylineEditor::performDeleteDrawing();
	}
}