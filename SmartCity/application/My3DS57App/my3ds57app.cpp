#include "my3ds57app.h"
#include "mychartsymbollib/mychartsymbollib.h"
#include "mychartdatadriverlib/mychartsdatalib.h"
#include "mychartdrawlib/imys57map.h"
#include "s57map/s57map3d.h"

My3DS57App::My3DS57App(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	this->mpScenViewer = new Engine::CSceneViewer(this);
	this->mpScenViewer->initial();
	this->ui.gridLayout->addWidget(this->mpScenViewer);
	MyChart::CS57Symbollib::instance()->initial();
	MyChart::CMyShpDataDriver::initial();
	this->loadS57Chart();
}

My3DS57App::~My3DS57App()
{

}

void My3DS57App::loadS57Chart()
{
	QString filePath = "D:/gitReource/S57Map/ChartMap2dApp/bin/Release/C1313100.000";
	MyChart::CMyChartS57DataDriver driver;
	MyChart::IMyVectorMapData* pS57Data = driver.readS57Map(filePath);
	if (pS57Data)
	{
		MyChart::IMyS57Map3D* pS57Map3D = new MyChart::IMyS57Map3D(pS57Data->getName(), pS57Data);
		pS57Map3D->setRefRoot(this->mpScenViewer->getRoot());
		pS57Map3D->draw(NULL);
	}
	/*osg::ref_ptr<osg::Geode> pNode = createNode(0,osg::Vec4());
	this->mpScenViewer->getRoot()->addChild(pNode);*/
}

osg::ref_ptr<osg::Geode> My3DS57App::createNode(osg::ref_ptr<osg::Vec3dArray> in_points,osg::Vec4 in_color)
{
	//Ҫô��Geometry��ʹ�÷�ʽ��

	osg::ref_ptr<osg::Geode> geode1 = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geom1 = new osg::Geometry();
	//������������
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(0,0,0));
	v->push_back(osg::Vec3(-50,50,0));
	v->push_back(osg::Vec3(0,100,0));
	v->push_back(osg::Vec3(50,100,0));
	v->push_back(osg::Vec3(100,100,0));
	geom1->setVertexArray(v.get());

	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	vc->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	vc->push_back(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
	vc->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	vc->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
	geom1->setColorArray(vc.get());
	geom1->setColorBinding(osg::Geometry::BIND_OVERALL);

	//���÷�������
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	nc->push_back(osg::Vec3(0,0,1));
	geom1->setNormalArray(nc.get());
	geom1->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//���ͼԪ����ͼ��ԪΪ�ı���
	geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,5));
	geode1->addDrawable(geom1.get());
	return geode1;
}