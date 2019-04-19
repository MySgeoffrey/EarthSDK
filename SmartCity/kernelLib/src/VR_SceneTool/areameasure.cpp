#include "scenetool/areameasure.h"
#include "geographic/geographictype.h"
#include "osgEarthSymbology/geometry"
#include "framework/common/languageconfig.h"

using namespace GeometryEditor;


          

class DistanceArea
{
public:
	DistanceArea();
 
    ~DistanceArea();
 
	/**
	* @brief SetEllipsePara 设置计算面积的参数
	* @param[in] double a 长半轴
	* @param[in] double b 短半轴
	* @return void
	* @author zxg
	* @date 2015年5月15日
	* @note 
	*/
	void SetEllipsePara(double a,double b);
 
	/**
	* @brief ComputePolygonArea 计算地球椭球面上多边形的面积
	* @param[in]  const double *padX X坐标数组
	* @param[in] const double* padY Y坐标数组
	* @param[in] int nCount 点的个数
	* @return double 返回值是面积
	* @author zxg
	* @date 2015年5月15日
	* @note 
	*/
	double ComputePolygonArea( const double *padX,const double* padY,int nCount );
 
private:
 
    double mSemiMajor, mSemiMinor, mInvFlattening;
 
    double GetQ( double x );
    double GetQbar( double x );
 
	void ComputeAreaInit();
 
    // 面积计算临时变量
 
    double m_QA, m_QB, m_QC;
    double m_QbarA, m_QbarB, m_QbarC, m_QbarD;
    double m_AE;  /* a^2(1-e^2) */
    double m_Qp;  
    double m_E;  
    double m_TwoPI;
 
};
 
#define M_PI 3.1415926
#define DEG2RAD(x)    ((x)*M_PI/180)
 
 
DistanceArea::DistanceArea()
{
	//
}
 
 
DistanceArea::~DistanceArea()
{
}
 
void DistanceArea::SetEllipsePara(double a,double b)
{
	mSemiMajor = a;
	mSemiMinor = b;
	//mInvFlattening = mSemiMajor
 
	ComputeAreaInit();
}
 
double DistanceArea::GetQ( double x )
{
	double sinx, sinx2;
 
	sinx = sin( x );
	sinx2 = sinx * sinx;
 
	return sinx *( 1 + sinx2 *( m_QA + sinx2 *( m_QB + sinx2 * m_QC ) ) );
}
 
 
double DistanceArea::GetQbar( double x )
{
	double cosx, cosx2;
 
	cosx = cos( x );
	cosx2 = cosx * cosx;
 
	return cosx *( m_QbarA + cosx2 *( m_QbarB + cosx2 *( m_QbarC + cosx2 * m_QbarD ) ) );
}
 
 
void DistanceArea::ComputeAreaInit()
{
	double a2 = ( mSemiMajor * mSemiMajor );
	double e2 = 1 - ( a2 / ( mSemiMinor * mSemiMinor ) );
	double e4, e6;
 
	m_TwoPI = M_PI + M_PI;
 
	e4 = e2 * e2;
	e6 = e4 * e2;
 
	m_AE = a2 * ( 1 - e2 );
 
	m_QA = ( 2.0 / 3.0 ) * e2;
	m_QB = ( 3.0 / 5.0 ) * e4;
	m_QC = ( 4.0 / 7.0 ) * e6;
 
	m_QbarA = -1.0 - ( 2.0 / 3.0 ) * e2 - ( 3.0 / 5.0 ) * e4  - ( 4.0 / 7.0 ) * e6;
	m_QbarB = ( 2.0 / 9.0 ) * e2 + ( 2.0 / 5.0 ) * e4  + ( 4.0 / 7.0 ) * e6;
	m_QbarC =                     - ( 3.0 / 25.0 ) * e4 - ( 12.0 / 35.0 ) * e6;
	m_QbarD = ( 4.0 / 49.0 ) * e6;
 
	m_Qp = GetQ( M_PI / 2 );
	m_E  = 4 * M_PI * m_Qp * m_AE;
	if ( m_E < 0.0 )
	m_E = -m_E;
}
 
 
double DistanceArea::ComputePolygonArea( const double *padX,const double* padY,int nCount )
{
	double x1, y1, dx, dy;
	double Qbar1, Qbar2;
 
	if (NULL == padX || NULL == padY)
	{
		return 0;
	}
	
 
	if (nCount < 3)
	{
		return 0;
	}
	
	
 
	double x2 = DEG2RAD( padX[nCount-1] );
	double y2 = DEG2RAD( padY[nCount-1] );
	Qbar2 = GetQbar( y2 );
 
	double area = 0.0;
 
	for ( int i = 0; i < nCount; i++ )
	{
		x1 = x2;
		y1 = y2;
		Qbar1 = Qbar2;
 
		x2 = DEG2RAD( padX[i] );
		y2 = DEG2RAD( padY[i] );
		Qbar2 = GetQbar( y2 );
 
		if ( x1 > x2 )
		  while ( x1 - x2 > M_PI )
			x2 += m_TwoPI;
		else if ( x2 > x1 )
		  while ( x2 - x1 > M_PI )
			x1 += m_TwoPI;
 
		dx = x2 - x1;
		area += dx * ( m_Qp - GetQ( y2 ) );
 
		if (( dy = y2 - y1 ) != 0.0 )
		  area += dx * GetQ( y2 ) - ( dx / dy ) * ( Qbar2 - Qbar1 );
	}
	if (( area *= m_AE ) < 0.0 )
		area = -area;
 
	if ( area > m_E )
		area = m_E;
	if ( area > m_E / 2 )
		area = m_E - area;
 
	return area;
}


namespace SceneTool
{
	static osgEarth::Symbology::Style mTextStyle;
	static osgEarth::Symbology::Style g_AreaGeometryStyle;
	CAreaMeasure::CAreaMeasure(
		osgEarth::MapNode* pRefMapNode, MeasureType measureType) :IPolygonEditor(pRefMapNode)
	{
		mpAreaContent = NULL;
		mMeasureType = measureType;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color::Red;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width() = 5.0;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->tessellation() = 20.0;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
			osgEarth::Symbology::Color(osgEarth::Symbology::Color::Blue, 0.6);
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;// TECHNIQUE_DRAPE;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->verticalOffset() = 0.1;
	}

	CAreaMeasure::~CAreaMeasure()
	{
		if (mpAreaContent)
		{
			mpAreaContent->unLoadFromScene();
			delete mpAreaContent;
		}
		mpAreaContent = NULL;
	}

	double calcArea(std::vector<osg::Vec3d> points)
	{
		double dTotalArea = 0.0;
		int iCount = points.size();
		if (iCount > 2)
		{
			double dLowX = 0.0;
			double dLowY = 0.0;
			double dMiddleX = 0.0;
			double dMiddleY = 0.0;
			double dHighX = 0.0;
			double dHighY = 0.0;
			double AM = 0.0;
			double BM = 0.0;
			double CM = 0.0;
			double AL = 0.0;
			double BL = 0.0;
			double CL = 0.0;
			double AH = 0.0;
			double BH = 0.0;
			double CH = 0.0;
			//Coefficient系数
			double dCoefficientL = 0.0;
			double dCoefficientH = 0.0;
			//tangent切线
			double dALtangent = 0.0;
			double dBLtangent = 0.0;
			double dCLtangent = 0.0;
			double dAHtangent = 0.0;
			double dBHtangent = 0.0;
			double dCHtangent = 0.0;
			//NormalLine法线     
			double dANormalLine = 0.0;
			double dBNormalLine = 0.0;
			double dCNormalLine = 0.0;
			//Orientation  Value方向值  
			double dOrientationValue = 0.0;
			//余弦角
			double dAngleCos = 0.0;

			double dSum1 = 0.0;
			double dSum2 = 0.0;
			int iCount2 = 0;
			int iCount1 = 0;

			double dSum = 0.0;

			for (int i = 0; i < iCount; i++)
			{
				if (i == 0)
				{
					//换算成弧度;
					dLowX = osg::DegreesToRadians(points.at(iCount - 1).x());
					dLowY = osg::DegreesToRadians(points.at(iCount - 1).y());
					dMiddleX = osg::DegreesToRadians(points.at(0).x());
					dMiddleY = osg::DegreesToRadians(points.at(0).y());
					dHighX = osg::DegreesToRadians(points.at(1).x());
					dHighY = osg::DegreesToRadians(points.at(1).y());
				}
				else if (i == iCount - 1)
				{
					dLowX = osg::DegreesToRadians(points.at(iCount - 2).x());
					dLowY = osg::DegreesToRadians(points.at(iCount - 2).y());
					dMiddleX = osg::DegreesToRadians(points.at(iCount - 1).x());
					dMiddleY = osg::DegreesToRadians(points.at(iCount - 1).y());
					dHighX = osg::DegreesToRadians(points.at(0).x());
					dHighY = osg::DegreesToRadians(points.at(0).y());
				}
				else
				{
					dLowX = osg::DegreesToRadians(points.at(i - 1).x());
					dLowY = osg::DegreesToRadians(points.at(i - 1).y());
					dMiddleX = osg::DegreesToRadians(points.at(i).x());
					dMiddleY = osg::DegreesToRadians(points.at(i).y());
					dHighX = osg::DegreesToRadians(points.at(i + 1).x());
					dHighY = osg::DegreesToRadians(points.at(i + 1).y());
				}
				AM = cos(dMiddleY) * cos(dMiddleX);
				BM = cos(dMiddleY) * sin(dMiddleX);
				CM = sin(dMiddleY);
				AL = cos(dLowY) * cos(dLowX);
				BL = cos(dLowY) * sin(dLowX);
				CL = sin(dLowY);
				AH = cos(dHighY) * cos(dHighX);
				BH = cos(dHighY) * sin(dHighX);
				CH = sin(dHighY);

				dCoefficientL = (AM*AM + BM*BM + CM*CM) / (AM*AL + BM*BL + CM*CL);
				dCoefficientH = (AM*AM + BM*BM + CM*CM) / (AM*AH + BM*BH + CM*CH);

				dALtangent = dCoefficientL * AL - AM;
				dBLtangent = dCoefficientL * BL - BM;
				dCLtangent = dCoefficientL * CL - CM;
				dAHtangent = dCoefficientH * AH - AM;
				dBHtangent = dCoefficientH * BH - BM;
				dCHtangent = dCoefficientH * CH - CM;

				dAngleCos = (dAHtangent * dALtangent + dBHtangent * dBLtangent + dCHtangent * dCLtangent) /
					(sqrt(dAHtangent * dAHtangent + dBHtangent * dBHtangent + dCHtangent * dCHtangent) *
					sqrt(dALtangent * dALtangent + dBLtangent * dBLtangent + dCLtangent * dCLtangent));

				dAngleCos = acos(dAngleCos);

				dANormalLine = dBHtangent * dCLtangent - dCHtangent * dBLtangent;
				dBNormalLine = 0 - (dAHtangent * dCLtangent - dCHtangent * dALtangent);
				dCNormalLine = dAHtangent * dBLtangent - dBHtangent * dALtangent;

				if (AM != 0)
				{
					dOrientationValue = dANormalLine / AM;
				}
				else if (BM != 0)
				{
					dOrientationValue = dBNormalLine / BM;
				}
				else
				{
					dOrientationValue = dCNormalLine / CM;
				}
				if (dOrientationValue > 0)
				{
					dSum1 += dAngleCos;
					iCount1++;
				}
				else
				{
					dSum2 += dAngleCos;
					iCount2++;
				}
			}
			if (dSum1 > dSum2)
			{
				dSum = dSum1 + (2 * osg::PI*iCount2 - dSum2);
			}
			else
			{
				dSum = (2 * osg::PI*iCount1 - dSum1) + dSum2;
			}

			dTotalArea = (dSum - (iCount - 2)*osg::PI)* osg::WGS_84_RADIUS_EQUATOR *
				osg::WGS_84_RADIUS_EQUATOR;
		}
		return std::abs(dTotalArea);
	}
	osgEarth::SpatialReference* g_SRS = NULL;
	double calcArea2(std::vector<osg::Vec3d> points,osgEarth::MapNode* pRefMapNode)
	{
		if(points.size() < 3)
			return 0.0;
		if (g_SRS == NULL)
			g_SRS = osgEarth::SpatialReference::get("spherical-mercator");
		std::vector<osg::Vec3d> polyline;
		std::vector<double> vecX;
		std::vector<double> vecY;
		for(int index = 0; index < points.size(); ++index)
		{
			vecX.push_back(points.at(index).x());
			vecY.push_back(points.at(index).y());
			/*osgEarth::GeoPoint prjCoor;
			osgEarth::GeoPoint(pRefMapNode->getMapSRS(), 
				points.at(index).x(), 
				points.at(index).y()).transform(
				g_SRS,
				prjCoor);
			polyline.push_back(prjCoor.vec3d());*/
		}
		
		/*int i, j;
		double area = 0;
		for (i = 0; i < polyline.size(); i++) 
		{
			j = (i + 1) % polyline.size();
			area += polyline[i].x() * polyline[j].y();
			area -= polyline[i].y() * polyline[j].x();
		}
		area /= 2;
		area = std::abs(area);*/
		
		
		/*int point_num = polyline.size();
		double s = polyline[0].y() * (polyline[point_num-1].x() - polyline[1].x());
		for(int i = 1; i < point_num; ++i)
			s += polyline[i].y() * (polyline[i-1].x() - polyline[(i+1)%point_num].x());
		return fabs(s/2.0);*/

		/*double sum = 0.0;

		for( unsigned i=0; i<polyline.size(); ++i )
		{
			const osg::Vec3d& p0 = polyline[0];
			const osg::Vec3d& p1 = i+1 < polyline.size() ? polyline[i+1] : polyline[0];
			sum += p0.x()*p1.y() - p1.x()*p0.y();
		}
		return .5*sum;*/

		//return area;
		
		DistanceArea area;
		area.SetEllipsePara(6378140.0,6356755.0);
		double aaa = area.ComputePolygonArea(&vecX[0],&vecY[0],vecY.size());
		return aaa;
	}

	osgEarth::Symbology::Style& CAreaMeasure::getTextStyle()
	{
		if (mTextStyle.get<osgEarth::Symbology::TextSymbol>() == NULL)
		{
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//指定中文字体路径
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() =  osgText::Text::BackdropImplementation::STENCIL_BUFFER;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() =osgEarth::Symbology::Color(0,0,0,1);
			mTextStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral("");//设置点图标显示的图片
		}
		return mTextStyle;
	}

	osgEarth::Symbology::Style& CAreaMeasure::getGeometryStyle()
	{
		if (g_AreaGeometryStyle.get<osgEarth::Symbology::LineSymbol>() == NULL)
		{
			g_AreaGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
				->stroke()->color() = osgEarth::Symbology::Color::Red;
			g_AreaGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
				->stroke()->width() = 5.0;
			g_AreaGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
				->tessellation() = 20.0;
			g_AreaGeometryStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
				osgEarth::Symbology::Color(osgEarth::Symbology::Color::Blue, 0.6);
			g_AreaGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
				->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
			g_AreaGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
				->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;// TECHNIQUE_DRAPE;
			g_AreaGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
				->verticalOffset() = 0.1;
		}
		return g_AreaGeometryStyle;
	}

	void CAreaMeasure::onEvent(Common::IObject* pResult)
	{
		GeometryEditor::IEPointSet* pPointSet = dynamic_cast<GeometryEditor::IEPointSet*>(pResult);
		if (NULL == pPointSet)
			return;
		std::vector<GeometryEditor::IEditPoint*> editorPointVector = pPointSet->Data;
		if (editorPointVector.size() < 3)
			return;
		double area = 0.0;
		std::vector<osg::Vec3d> points;
		osg::Vec3d contentPos = osg::Vec3d(0.0, 0.0, 0.0);
		for (int index = 0; index < editorPointVector.size(); ++index)
		{
			GeometryEditor::IEditPoint* pEditorPoint = editorPointVector.at(index);
			if (NULL == pEditorPoint)
				continue;
			points.push_back(pEditorPoint->getGeoPosition());
			contentPos += pEditorPoint->getGeoPosition();
		}

		area = calcArea2(points,mpRefMapNode);//,mpRefMapNode
		if (mMeasureType == SPHERE_MT)
			area *= 1.1;

		contentPos = contentPos / editorPointVector.size();
		if (NULL == mpAreaContent)
		{
			mpAreaContent = new Graphic::CGeoPoint(mpRefMapNode);
			osgEarth::Symbology::Style& style = mpAreaContent->getStyle();
			style = getTextStyle();

		}
		else
		{
			mpAreaContent->unLoadFromScene();
		}
		char str[200];
		double area2 = area / (1000.0 * 1000.0);
		if (area2 > 1)
		{
			sprintf(str, "面积:%.1f(平方公里)", area2);
		}
		else
		{
			sprintf(str, "面积:%.0f(平方米)", area);
		}
		mpAreaContent->setName(str);
		mpAreaContent->setGeoPosition(contentPos);
		mpAreaContent->loadToScene();
		mpAreaContent->renderToScene();
	}

	bool CAreaMeasure::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		bool result = true;
		bool needUpdate = false;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
			{
				if (this->mIsEditState == false)
				{
					if (mpAreaContent)
					{
						mpAreaContent->unLoadFromScene();
						delete mpAreaContent;
					}
					mpAreaContent = NULL;
					this->endEdit();
					this->beginEdit();
				}
				break;
			}
		case osgGA::GUIEventAdapter::DRAG:
		case osgGA::GUIEventAdapter::RELEASE:
			{
				needUpdate = true;
				break;
			}
		}
		result = IPolylineEditor::handle(ea, aa);
		if (needUpdate)
		{
			this->forceListenerEvents(mpCurrentEditingGeometry);
		}

		return result;
	}

	CAreaMeasureTool::CAreaMeasureTool(osgEarth::MapNode* pRefMapNode,MeasureType measureType)
		: IGeoGraphicEditor(pRefMapNode),mMeasureType(measureType)
	{
		this->mNeedResetResult = false;
		this->mMouseDownAppendPointMode = true;
		this->mNeedMoveGraphic = false;
		this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->size() = 0;
	}

	CAreaMeasureTool::~CAreaMeasureTool()
	{

	}

	void CAreaMeasureTool::beginEdit(const std::string& graphicType,
		Common::ISceneObject* pGraphicObject,
		const bool& hideOriginGraphicObject)
	{
		this->getGeometryStyle() = CAreaMeasure::getGeometryStyle();
		this->mHighLightColor = CAreaMeasure::getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color();
		this->mHighLightWidth = CAreaMeasure::getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width().get();
		IGeoGraphicEditor::beginEdit(
			Graphic::GT_POLYGON,NULL);
		if (this->mpEditContent)
		{
			this->mpEditContent->deRenderFromScene();
			this->mpEditContent->unLoadFromScene();
			int x = 0;
		}
	}

	bool CAreaMeasureTool::endEdit(
		const bool& clearCurrentGeometry,
		const bool& needSaveEditingResult,
		const bool& hideOriginGraphicObject)
	{
		bool r = IGeoGraphicEditor::endEdit(
			clearCurrentGeometry,needSaveEditingResult,false);
		if (this->mpEditingObject)
		{
			this->mpEditingObject->deRenderFromScene();
			this->mpEditingObject->unLoadFromScene();
			delete this->mpEditingObject;
			this->mpEditingObject = NULL;
		}
		return r;
	}

	void CAreaMeasureTool::performPicked(osg::Vec3d pos)
	{
		if (this->mNeedResetResult)
		{
			if (this->mpPointsEdit != NULL)
			{
				int count = this->mpPointsEdit->getNumChildren();
				this->mpPointsEdit->removeChildren(0, count);
				this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
				this->mpPointsEdit = NULL;
			}
			osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
			pGeometry->clear();
			this->mpEditingObject->getGeometry().clear();
			this->mNeedResetResult = false;
			this->updateResult();
		}
		IGeoGraphicEditor::performPicked(pos);
		this->updateResult();
	}

	void CAreaMeasureTool::performMoving(osg::Vec3d pos)
	{
		IGeoGraphicEditor::performMoving(pos);
		this->updateResult();
	}

	void CAreaMeasureTool::performMovingXYZ(osg::Vec3d pos)
	{
		if (!this->mNeedResetResult)
		{
			IGeoGraphicEditor::performMovingXYZ(pos);
			this->updateResult();
		}
	}

	void CAreaMeasureTool::performRightPicked()
	{
		if (this->mNeedResetResult)
		{
			if (this->mpPointsEdit != NULL)
			{
				int count = this->mpPointsEdit->getNumChildren();
				this->mpPointsEdit->removeChildren(0, count);
				this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
				this->mpPointsEdit = NULL;
			}
			osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
			pGeometry->clear();
			this->mpGeometryNode->init();
			this->mpEditingObject->getGeometry().clear();
			this->mpEditingObject->deRenderFromScene();
			this->mpEditingObject->unLoadFromScene();
			this->updateResult();
		}
		else
		{
			IGeoGraphicEditor::performRightPicked();
			this->updateResult();
			this->mIsEditState = true;
			this->mNeedResetResult = true;
		}
	}

	void CAreaMeasureTool::createEditorCallBack()
	{
		
	}

	void CAreaMeasureTool::updateResult()
	{
		if (this->mpEditingObject->getGeometry().size() >= 3)
		{
			double area = 0.0;
			std::vector<osg::Vec3d> points;
			osg::Vec3d contentPos = osg::Vec3d(0.0, 0.0, 0.0);
			for (int index = 0;
				index < this->mpEditingObject->getGeometry().size(); ++index)
			{
				osg::Vec3d position = this->mpEditingObject->getGeometry().at(index);
				points.push_back(position);
				contentPos += position;
			}

			area = calcArea2(points,mpRefMapNode);//mpRefMapNode
			if (mMeasureType == SPHERE_MT)
				area *= 1.1;

			contentPos = contentPos / this->mpEditingObject->getGeometry().size();
			this->mpEditContent->getStyle() = CAreaMeasure::getTextStyle();
			this->mpEditContent->deRenderFromScene();
			this->mpEditContent->unLoadFromScene();
			
			char str[200];
			double area2 = area / (1000.0 * 1000.0);
			if (area2 > 1)
			{
				std::string areaTitle = CLanguageConfig::getValue(CLanguageConfig::AreaTextType_Title);
				std::string squreKM = CLanguageConfig::getValue(CLanguageConfig::AreaTextType_SqureKiloMetre);
				sprintf(str, "%s%.1f%s",areaTitle.c_str(), area2,squreKM.c_str());
			}
			else
			{
				std::string areaTitle = CLanguageConfig::getValue(CLanguageConfig::AreaTextType_Title);
				std::string squreM = CLanguageConfig::getValue(CLanguageConfig::AreaTextType_SqureMeter);
				sprintf(str, "%s%.0f%s", areaTitle.c_str(),area,squreM.c_str());
			}
			this->mpEditContent->setName(str);
			this->mpEditContent->setGeoPosition(contentPos);
			this->mpEditContent->loadToScene();
			this->mpEditContent->renderToScene();
		}
		else
		{
			this->mpEditContent->deRenderFromScene();
			this->mpEditContent->unLoadFromScene();
		}
	}
}