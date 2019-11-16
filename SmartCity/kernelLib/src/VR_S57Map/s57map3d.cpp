#include "s57map/s57map3d.h"
#include "mychartmaplib/imylayer.h"
#include "mycharts52symbollib/imys52layersymbol.h"
#include "mycharts52symbollib/imys52pointsymbol.h"
#include "mycharts52symbollib/imys52linesymbol.h"
#include "mycharts52symbollib/imys52areasymbol.h"
#include "mycharts57datalib/imys57layerdata.h"
#include "s57map/s57layer3d.h"
#include "mychartmaplib/imygeometry.h"
#include "mychartmaplib/imyshaperecord.h"
#include "mychartsymbollib/mychartsymbollib.h"
#include "mychartsymbollib/colortable.h"
#include "mychartsymbollib/colorrecord.h"
#include "mychartsymbollib/vectorsymbol.h"
#include "mychartsymbollib/vectorpattern.h"
#include "mychartsymbollib/vectorline.h"
#include "mychartsymbollib/bitmapsymbol.h"
#include "mychartsymbollib/bitmappattern.h"
#include "mychartmaplib/imyproperty.h"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/Array"

namespace MyChart
{

	IMyS57Map3D::IMyS57Map3D()
	{

	}


	IMyS57Map3D::IMyS57Map3D(const QString& name,IMyVectorMapData* pData)
		:IMyS57Map(name,pData),mpRefRoot(NULL)
	{

	}


	IMyS57Map3D::~IMyS57Map3D()
	{

	}

	void IMyS57Map3D::setRefRoot(osg::ref_ptr<osg::Group> in_pRefRoot)
	{
		this->mpRefRoot = in_pRefRoot;
	}

	bool IMyS57Map3D::constructDrawLayer()
	{
		bool r = false;
		if (NULL != this->mpS57Data)
		{
			std::vector<IMyLayer*>& pS57MapData = this->mpS57Data->getLayers();
			for (int i = 0 ; i < pS57MapData.size(); ++i)
			{
				IMyVectorLayerData* pS57LayerData = 
					dynamic_cast<IMyVectorLayerData*>(pS57MapData.at(i));
				IMyS57Layer3D* pS57DrawLayer = new IMyS57Layer3D(pS57LayerData->getName());
				pS57DrawLayer->setType(pS57LayerData->getType());
				pS57DrawLayer->setData(pS57LayerData);
				pS57DrawLayer->setOwnerDrawObject(this);
				if (pS57LayerData->getLayerType() == "A")
				{
					this->mPolygonLayers.push_back(pS57DrawLayer);
				}
				else if (pS57LayerData->getLayerType() == "L")
				{
					this->mPolylineLayers.push_back(pS57DrawLayer);
				}
				else if (pS57LayerData->getLayerType() == "P"
					|| pS57LayerData->getLayerType() == "MP")
				{
					this->mPointLayers.push_back(pS57DrawLayer);
				}

				this->getLayers().push_back(pS57DrawLayer);
				r = true;
			}
		}
		return r;
	}

	double g_Rx = 0,g_Ry = 0;

	osg::ref_ptr<osg::Geode> createNode(osg::ref_ptr<osg::Vec3dArray> in_points,osg::Vec4 in_color)
	{
		//要么用Geometry，使用方式：

		osg::ref_ptr<osg::Geode> geode1 = new osg::Geode();
		osg::ref_ptr<osg::Geometry> geom1 = new osg::Geometry();
		//创建顶点数组
		osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
		for (int i = 0; i < in_points->size(); ++i)
		{
			v->push_back(in_points->at(i));
		}
		
		geom1->setVertexArray(v.get());

		//设置颜色数组
		osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
		vc->push_back(in_color);
		geom1->setColorArray(vc.get());
		geom1->setColorBinding(osg::Geometry::BIND_OVERALL);

		//设置法线数组
		osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
		nc->push_back(osg::Vec3(0,0,1));
		geom1->setNormalArray(nc.get());
		geom1->setNormalBinding(osg::Geometry::BIND_OVERALL);

		//添加图元，绘图基元为四边形
		geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,v->size()));
		geode1->addDrawable(geom1.get());
		return geode1;
	}

	bool IMyS57Map3D::drawPolygonRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs)
	{
		QPainterPath polylinePath;
		std::vector<QPolygonF> polygons;
		IMyPolygon* pGeometry = 
			dynamic_cast<IMyPolygon*>(pRecord->Geometry);

		for (int n = 0; n < pGeometry->numOfParts();++n)
		{
			int startPointIndex = pGeometry->Parts[n];
			int endPointIndex = (n < (pGeometry->numOfParts() - 1))
				? pGeometry->Parts[n + 1] : pGeometry->numOfPoints();
			QPolygonF segmnetPath;
			for (int i = startPointIndex; i < endPointIndex;++i)
			{
				IVector3& point = pGeometry->TempPoints.at(i);

				if (g_Rx == 0)
				{
					g_Rx = point.X;
					g_Ry = point.Y;
				}
				point.X -= g_Rx;
				point.Y -= g_Ry;
				segmnetPath.append(QPointF(point.X,point.Y));
			}
			polylinePath.addPolygon(segmnetPath);
			polygons.push_back(segmnetPath);
		}


		if (polylinePath.elementCount() > 0)
		{
			IMyS52AreaSymbol* pS52AreaSymbol = 
				dynamic_cast<IMyS52AreaSymbol*>(pSymbol);
			//"DAY_BRIGHT";//
			QString colorMode = "DAY_BRIGHT";
			std::map<QString, CColorTable*>& colorTables = CS57Symbollib::instance()->ColorTables;
			CColorTable* pColorTable = CS57Symbollib::instance()->ColorTables[colorMode];
			if (pS52AreaSymbol->RecordPara.FillColor != "")
			{
				osg::Vec4 color(1,0,0,1);				
				CColorRecord* pColorRecord = pColorTable->Colors[pS52AreaSymbol->RecordPara.FillColor];
				if (pColorRecord)
				{
					color = 
						osg::Vec4(
						pColorRecord->color().red() / 255.0,
						pColorRecord->color().green() / 255.0,
						pColorRecord->color().blue() / 255.0,
						pColorRecord->color().alpha() / 255.0);
				}
				for (int i = 0; i < polygons.size(); ++i)
				{
					QPolygonF& p = polygons.at(i);
					
					osg::ref_ptr<osg::Vec3dArray> points = new osg::Vec3dArray();
					for (int j = 0; j < p.size(); ++j)
					{
						points->push_back(osg::Vec3d(
							p.at(j).x(),
							p.at(j).y(),
							0
							));
					}
					osg::ref_ptr<osg::Geode> pNode = createNode(points,color);
					if (pNode)
					{
						this->mpRefRoot->addChild(pNode);
					}
				}
			}
		}
		return false;
	}

	bool IMyS57Map3D::draw(IMyS57Layer* pLayer, IMyDrawArgs* pArgs)
	{
		bool r = false;
		if (pLayer)
		{
			IMyS52LayerSymbol* pSymbol = (IMyS52LayerSymbol*)(pLayer->getSymbol());
			std::vector<IMyShapeRecord*>& records = pLayer->getData()->getRecords();
			for (int i = 0; i < records.size();++i)
			{
				IMyShapeRecord* pRecord = records.at(i);
				IMyS52Symbol* pRecordSymbol = pSymbol != NULL ? pSymbol->RecordSymbols.at(i) : NULL;
				this->drawPolygonRecord(pRecord,pRecordSymbol,pArgs);
			}
		}
		return r;
	}

	bool IMyS57Map3D::draw(IMyDrawArgs* pArgs)
	{
		bool r = false;
		if (!this->getLayers().empty())
		{
			for (int i = 0; i < this->mPolygonLayers.size(); ++i)
			{
				IMyS57Layer* pLayer = 
					dynamic_cast<IMyS57Layer*>(this->mPolygonLayers.at(i));
				if (pLayer->getName() == "DEPARE")
				{
					//深度区域
					this->draw(pLayer,NULL);
				}
				//else 
				//if (pLayer->getName() == "LNDARE")
				//{
				//	//陆地区域
				//	this->draw(pLayer,NULL);
				//}
			}
		}
		return r;
	}
}