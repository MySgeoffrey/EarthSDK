#include "myclass.h"
#include "citybuilder/instancemodellayer.h"
#include "pipenet/pipeline.h"
#include "pipenet/pipepoint.h"
#include <QDir>


MyClass::MyClass(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->mpGroup = NULL;
	this->mpGlobeWidget = new VRGlobeWidget();
	//场景控件初始化，建立三维地图
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	std::string runPath_3d = runPath + "/VR-GlobeConfig3D.earth";
	this->mpGlobeWidget->initial(true, runPath_3d.c_str());
	this->ui.gridLayout->setMargin(0);
	this->ui.gridLayout->setSpacing(0);
	this->ui.gridLayout->setContentsMargins(0, 0, 0, 0);
	this->ui.gridLayout->addWidget(this->mpGlobeWidget);

	//加载谷歌影像数据
	Globe::CGlobeMap::getInstance()->initialise(this->mpGlobeWidget->getMapRef());
	Globe::CGlobeMap::getInstance()->addImageLayer("", "googleImage", Globe::MapType::WEB_GOOGLE_MT);

	//初始化
	this->init();
}

MyClass::~MyClass()
{

}

void MyClass::init()
{
	
	/*设置纹理与mesh的映射关系,以15米为单位重复贴一张完整的纹理*/
	MeshGenerator::MeshUtil::getInstance()->setTextureRepeatLength(15.f);
	/*设置接头在回退时是否采用箸距离回退方案*/
	MeshGenerator::MeshUtil::getInstance()->setSameOffset(true);
	MeshGenerator::MeshUtil::getInstance()->setBlendParam(0.7);
	MeshGenerator::MeshUtil::getInstance()->setHatParam(0.5, 0.5);
	/*设置接头建模时的固定回退偏移*/
	MeshGenerator::MeshUtil::getInstance()->setFixedOffset(0.8);

	

	/*初始化弯头以及管道要用的纹理路径*/
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	this->mLinkerTexturePath = runPath + "/resource/image/blue.bmp";
	this->mPipeTexturePath = runPath + "/resource/image/red.bmp";

	/*建模参考中心点,建模算法以此位置为参考来构建三维模型*/
	this->mRefGeoCenter = osgEarth::GeoPoint(this->mpGlobeWidget->getMapNodeRef()->getMapSRS(),
		osg::Vec3d(116, 39, 0));

	/*模型节点组，用于将生成的模型node挂接渲染到场景中*/
	this->mpGroup = new osgEarth::GeoTransform();
	this->mpGroup->setPosition(this->mRefGeoCenter);

	/*将参考中心线设置给建模算法工具对象，以便后续建模以此中心点为参考点*/
	MeshGenerator::MeshUtil::getInstance()->setReferenceCenter(this->mRefGeoCenter);
	/*将模型节点组挂接到场景中，便于后续建模成果的显示*/
	this->mpGlobeWidget->getMapNodeRef()->addChild(this->mpGroup);

	//临时测试管网
	//loadPipeData();
}

void MyClass::loadPipeData()
{
	/*建模参考中心点,建模算法以此位置为参考来构建三维模型*/
	bool initialCenter = false;
	osg::ref_ptr<osgEarth::GeoTransform> pGroup = new osgEarth::GeoTransform();
	this->mpGlobeWidget->getMapNodeRef()->addChild(pGroup);
	osgEarth::GeoPoint referCenter;
	MeshGenerator::CModelCreator modelCreator;
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	QString dirPath = QString::fromLocal8Bit(std::string(runPath + "/testPipeData").c_str());
	QDir dir(dirPath);
	if (dir.exists())
	{
		QStringList namefilters;
		namefilters << "*.shp";
		QStringList files = dir.entryList(namefilters, QDir::Files | QDir::Readable, QDir::Name);
		if (!files.empty())
		{
			for (int i = 0; i < files.size(); ++i)
			{
				QString lPath = files.at(i);
				QString pipePath = dirPath + "/" + files.at(i);
				PipeNet::CPipeLineDataSet* pPipeLineDataSet = NULL;
				PipeNet::CPipePointDataSet* pPipePointDataSet = NULL;
				CityBuilder::CPipeLayerDriver::loadEx(pipePath.toLocal8Bit().constData(), pPipeLineDataSet);
				CityBuilder::CPipeLayerDriver::loadEx(pipePath.toLocal8Bit().constData(), pPipePointDataSet);
				if (pPipeLineDataSet && pPipePointDataSet)
				{
					osg::Vec4 linkerColor(rand() % 10 / 10.0, rand() % 10 / 10.0, rand() % 10 / 10.0, 1.0);
					osg::Vec4 pipeColor(rand() % 10 / 10.0, rand() % 10 / 10.0, rand() % 10 / 10.0, 1.0);
					//@1:计算管网的拓扑
					for (int i = 0; i < pPipePointDataSet->getPipePoints().size(); ++i)
					{
						PipeNet::CPipePoint* pPipePoint = pPipePointDataSet->getPipePoints().at(i);
						std::vector<PipeNet::CPipeLine*> inout_pipeLines;
						if (pPipeLineDataSet->getPipeLines(pPipePoint, inout_pipeLines))
						{
							pPipePoint->setAdjcentLines(inout_pipeLines);
						}
						if (initialCenter == false)
						{
							referCenter = osgEarth::GeoPoint(this->mpGlobeWidget->getMapNodeRef()->getMapSRS(),
								pPipePoint->getGeoPosition());
							pGroup->setPosition(referCenter);
							MeshGenerator::MeshUtil::getInstance()->setReferenceCenter(referCenter);
							this->mpGlobeWidget->flyTo(pPipePoint->getGeoPosition().x(), pPipePoint->getGeoPosition().y(), pPipePoint->getGeoPosition().z());
							initialCenter = true;
						}
					}
					std::map<std::string, MeshGenerator::JointData> jointDatas;
					//@2:接头建模
					for (int i = 0; i < pPipePointDataSet->getPipePoints().size(); ++i)
					{
						PipeNet::CPipePoint* pPipePoint = pPipePointDataSet->getPipePoints().at(i);
						if (pPipePoint->getAdjcentLines().size() >= 2)
						{
							std::vector<osg::Vec3d> adjcentPoints;
							std::vector<double> radiusVecs;
							//弯头建模参数
							std::vector<PipeNet::CPipeLine*>& adjcentPipeLines = pPipePoint->getAdjcentLines();
							for (int j = 0; j < adjcentPipeLines.size(); ++j)
							{
								PipeNet::CPipeLine* pPipeLine = adjcentPipeLines.at(j);
								PipeNet::CPipePoint* pAdjencentPoint = NULL;
								if ((pPipeLine->getStartGeoPosition() - pPipePoint->getGeoPosition()).length() < 0.00000001)
								{
									pAdjencentPoint = dynamic_cast<PipeNet::CPipePoint*>(pPipePointDataSet->getPipePoint(pPipeLine->getEndGeoPosition()));
								}
								else
								{
									pAdjencentPoint = dynamic_cast<PipeNet::CPipePoint*>(pPipePointDataSet->getPipePoint(pPipeLine->getStartGeoPosition()));
								}
								if (pAdjencentPoint != NULL)
								{
									adjcentPoints.push_back(pAdjencentPoint->getGeoPosition());
									radiusVecs.push_back(pPipeLine->getRadius());
								}
							}

							if (!adjcentPoints.empty())
							{
								MeshGenerator::JointData jointData = modelCreator.createCircleJointData(
									pPipePoint->getGeoPosition(),
									adjcentPoints, radiusVecs);
								jointDatas[pPipePoint->getID()] = jointData;
								osg::ref_ptr<osg::Node> pLinkerNode = modelCreator.createLinkerModel(jointData, linkerColor, this->mLinkerTexturePath);
								osg::ref_ptr<osg::LOD> pLod = new osg::LOD();
								pLod->addChild(pLinkerNode, 0, 1000);
								pGroup->addChild(pLod);
							}
						}
					}
					//@3:管段建模
					for (int i = 0; i < pPipeLineDataSet->getPipeLines().size(); ++i)
					{
						PipeNet::CPipeLine* pipe = pPipeLineDataSet->getPipeLines().at(i);
						if (pipe)
						{
							PipeNet::CPipePoint* startPoint = pPipePointDataSet->getPipePoint(pipe->getStartGeoPosition());
							PipeNet::CPipePoint* endPoint = pPipePointDataSet->getPipePoint(pipe->getEndGeoPosition());

							MeshGenerator::JointData startJointData = jointDatas[startPoint->getID()];
							MeshGenerator::JointData endJointData = jointDatas[endPoint->getID()];
							osg::ref_ptr<osg::Node> pPipeNode = modelCreator.createPipeModel(startJointData, endJointData, pipeColor, this->mPipeTexturePath);
							osg::ref_ptr<osg::LOD> pLod = new osg::LOD();
							pLod->addChild(pPipeNode, 0, 3000);
							pGroup->addChild(pLod);
						}
					}
				}
			}
		}
	}
}

MeshGenerator::JointData MyClass::createCircleJoint_2_h()
{
	//水平两通圆形接头
	osg::Vec3d offset(0.0, 0.001, 0);
	//@1：弯头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;

	//@4：弯头的半径（与对应管道的半径保持一致）
	double radius = 1.0;
	//@5：模型构建对象，根据用户输入构建相应的jointData
	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createCircleJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2 },
		radius);
		return jointData;
}

MeshGenerator::JointData MyClass::createCircleJoint_3_h()
{
	//水平三通圆形接头
	osg::Vec3d offset(0.0, 0.002, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@3：第三个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 38.9999, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	//@4：弯头的半径（与对应管道的半径保持一致）
	double radius = 1.0;
	//@5：模型构建对象，根据用户输入构建相应的jointData
	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createCircleJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3 },
		radius);
		return jointData;
}

MeshGenerator::JointData MyClass::createCircleJoint_3_v()
{
	//竖直三通圆形接头
	osg::Vec3d offset(0.0, 0.003, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0, 38.999, 3);
	//@4：第三个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point3(116.0, 39.0, 6);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	double radius = 1.0;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createCircleJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3 },
		radius);
		return jointData;
}

MeshGenerator::JointData MyClass::createCircleJoint_4_h()
{
	osg::Vec3d offset(0.0, 0.004, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4：第三个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 2.9);
	//@5：第四个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point4(116.0, 38.999, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;
	jointAdjacentGeoPosition_Point4 += offset;

	double radius = 1.0;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createCircleJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3,
		jointAdjacentGeoPosition_Point4 },
		radius);
		return jointData;
}

MeshGenerator::JointData MyClass::createRectJoint_2_h()
{
	//两通圆形接头
	osg::Vec3d offset(0.0, 0.005, 0);
	//@1：建模
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;

	double width = 3;
	double height = 2;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createRectJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2},
		width, height);
		return jointData;
}

MeshGenerator::JointData MyClass::createRectJoint_3_h()
{
	//水平三通圆形接头
	osg::Vec3d offset(0.0, 0.006, 0);
	//@1：建模
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 38.9999, 3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	double width = 3;
	double height = 2;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createRectJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3 },
		width, height);
		return jointData;
}

MeshGenerator::JointData MyClass::createRectJoint_3_v()
{
	//竖直三通圆形接头
	osg::Vec3d offset(0.0, 0.007, 0);
	//@1：建模
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.001, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0, 38.999, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point3(116.0, 39.0, 6);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	double width = 3;
	double height = 2;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createRectJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3 },
		width, height);
		return jointData;
}

MeshGenerator::JointData MyClass::createRectJoint_4_h()
{
	osg::Vec3d offset(0.0, 0.008, 0);
	//@1：建模
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 3);
	osg::Vec3d jointAdjacentGeoPosition_Point4(116.0, 38.999, 3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;
	jointAdjacentGeoPosition_Point4 += offset;

	double width = 3;
	double height = 2;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createRectJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3,
		jointAdjacentGeoPosition_Point4 },
		width, height);
		return jointData;
}

MeshGenerator::JointData MyClass::createArcJoint_2_h()
{
	//水平两通圆形接头
	osg::Vec3d offset(0.0, 0.009, 0);
	//@1：弯头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;

	//@4：
	double height = 3.0;
	double width = 2.5;
	double arcHeight = 1.0;
	//@5：模型构建对象，根据用户输入构建相应的jointData
	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createArcJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2 },
		height, width, arcHeight);
		return jointData;
}

MeshGenerator::JointData MyClass::createArcJoint_3_h()
{
	//水平三通圆形接头
	osg::Vec3d offset(0.0, 0.01, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@3：第三个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 38.9999, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	//@4：弯头的半径（与对应管道的半径保持一致）
	double height = 3.0;
	double width = 2.5;
	double arcHeight = 1.0;
	//@5：模型构建对象，根据用户输入构建相应的jointData
	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createArcJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3 },
		height, width, arcHeight);
		return jointData;
}

MeshGenerator::JointData MyClass::createArcJoint_4_h()
{
	osg::Vec3d offset(0.0, 0.011, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4：第三个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 2.9);
	//@5：第四个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point4(116.0, 38.999, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;
	jointAdjacentGeoPosition_Point4 += offset;

	double height = 3.0;
	double width = 2.5;
	double arcHeight = 1.0;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createArcJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3,
		jointAdjacentGeoPosition_Point4 },
		height, width, arcHeight);
		return jointData;
}

MeshGenerator::JointData MyClass::createLadderJoint_2_h()
{
	osg::Vec3d offset(0.0, 0.012, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;

	double upwidth = 3.0;
	double bottomWidth = 5;
	double height = 2.0;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createLadderJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2 },
		upwidth, bottomWidth, height);
		return jointData;
}

MeshGenerator::JointData MyClass::createLadderJoint_3_h()
{
	osg::Vec3d offset(0.0, 0.013, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4：第三个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	double upwidth = 3.0;
	double bottomWidth = 5;
	double height = 2.0;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createLadderJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3},
		upwidth, bottomWidth, height);
		return jointData;
}

MeshGenerator::JointData MyClass::createLadderJoint_4_h()
{
	osg::Vec3d offset(0.0, 0.014, 0);
	//@1：接头的位置
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2：第一个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3：第二个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4：第三个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 2.9);
	//@5：第四个相邻的点位置
	osg::Vec3d jointAdjacentGeoPosition_Point4(116.0, 38.999, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;
	jointAdjacentGeoPosition_Point4 += offset;

	double upwidth = 3.0;
	double bottomWidth = 5;
	double height = 2.0;

	MeshGenerator::CModelCreator modelCreator;
	MeshGenerator::JointData jointData = modelCreator.createLadderJointData(
		jointGeoPosition,
		{
		jointAdjacentGeoPosition_Point1,
		jointAdjacentGeoPosition_Point2,
		jointAdjacentGeoPosition_Point3,
		jointAdjacentGeoPosition_Point4 },
		upwidth, bottomWidth, height);
		return jointData;
}

void MyClass::slotCirclePipe1()
{
	MeshGenerator::JointData jointData = createCircleJoint_2_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "CircleJoint_2_h", this->mLinkerTexturePath);
}

void MyClass::slotCirclePipe2()
{
	//水平三通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createCircleJoint_3_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "CircleJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotCirclePipe3()
{
	//竖直三通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createCircleJoint_3_v();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "CircleJoint_3_v", this->mLinkerTexturePath);
}

void MyClass::slotCirclePipe4()
{
	//水平四通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createCircleJoint_4_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "CircleJoint_4_h", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe1()
{
	//两通方形接头
	//@1：建模
	MeshGenerator::JointData jointData = createRectJoint_2_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "RectJoint_2_h", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe2()
{
	//三通方形接头
	//@1：建模
	MeshGenerator::JointData jointData = createRectJoint_3_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "RectJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe3()
{
	//三通方形接头
	//@1：建模
	MeshGenerator::JointData jointData = createRectJoint_3_v();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "RectJoint_3_v", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe4()
{
	//四通方形接头
	//@1：建模
	MeshGenerator::JointData jointData = createRectJoint_4_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "RectJoint_4_h", this->mLinkerTexturePath);
}

void MyClass::slotCirclePipeLine()
{
	MeshGenerator::JointData jointData1 = createCircleJoint_3_v();
	MeshGenerator::JointData jointData2 = createCircleJoint_4_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createSegmentModelAndExport(jointData1, jointData2, color, "CirclePipeLine", this->mPipeTexturePath);
}

void MyClass::slotRectPipeLine()
{
	MeshGenerator::JointData jointData1 = createRectJoint_3_v();
	MeshGenerator::JointData jointData2 = createRectJoint_4_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createSegmentModelAndExport(jointData1, jointData2, color, "CirclePipeLine", this->mPipeTexturePath);
}

void MyClass::slotArcPipe_2()
{
	//两通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createArcJoint_2_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "ArcJoint_2_h", this->mLinkerTexturePath);
}

void MyClass::slotArcPipe_3_h()
{
	//两通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createArcJoint_3_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "ArcJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotArcPipe_4_h()
{
	//两通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createArcJoint_4_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "ArcJoint_4_h", this->mLinkerTexturePath);
}

void MyClass::slotArcPipeLine()
{
	MeshGenerator::JointData jointData1 = createArcJoint_3_h();
	MeshGenerator::JointData jointData2 = createArcJoint_4_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createSegmentModelAndExport(jointData1, jointData2, color, "ArcPipeLine", this->mPipeTexturePath);
}

void MyClass::slotLadderPipe_2()
{
	//两通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createLadderJoint_2_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "LadderJoint_2_h", this->mLinkerTexturePath);
}

void MyClass::slotLadderPipe_3_h()
{
	//两通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createLadderJoint_3_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "LadderJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotLadderPipe_4_h()
{
	//两通圆形接头
	//@1：构建JointData
	MeshGenerator::JointData jointData = createLadderJoint_4_h();
	//@2：模型颜色
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "LadderJoint_4_h", this->mLinkerTexturePath);
}

void MyClass::slotLadderPipeLine()
{
	MeshGenerator::JointData jointData1 = createLadderJoint_3_h();
	MeshGenerator::JointData jointData2 = createLadderJoint_4_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createSegmentModelAndExport(jointData1, jointData2, color, "LadderPipeLine", this->mPipeTexturePath);
}

void MyClass::createJointModelAndExport(const MeshGenerator::JointData& in_jointData,
	const osg::Vec4& in_color, const std::string& in_name, const std::string& in_texturePath)
{
	//@2：模型颜色
	//@3：模型构建对象，根据用户输入的jointData生成模型数据节点
	//当用户输入纹理时，以纹理显示，若纹理为空"",则以颜色显示
	MeshGenerator::CModelCreator modelCreator;
	osg::ref_ptr<osg::Node> pJointNode = modelCreator.createLinkerModel(
		in_jointData, in_color, in_texturePath);

	//@4：模型导出
	if (pJointNode)
	{
		std::string runPath = osgDB::getCurrentWorkingDirectory();
		MeshGenerator::CModelExporter::execute(
			pJointNode, MeshGenerator::ModelFileType::OBJ, runPath, in_name);
	}

	//@5：可视化
	if (pJointNode)
	{
		this->mpGroup->addChild(pJointNode);
		this->mpGlobeWidget->flyTo(in_jointData.pos.x(), in_jointData.pos.y(), 20);
	}
}

void MyClass::createSegmentModelAndExport(const MeshGenerator::JointData& in_jointData1,
	const MeshGenerator::JointData& in_jointData2,
	const osg::Vec4& in_color, const std::string& in_name, const std::string& in_texturePath)
{
	MeshGenerator::CModelCreator modelCreator;
	osg::ref_ptr<osg::Node> pJointNode = modelCreator.createPipeModel(
		in_jointData1, in_jointData2, in_color, in_texturePath);

	//@2：导出
	if (pJointNode)
	{
		std::string runPath = osgDB::getCurrentWorkingDirectory();
		MeshGenerator::CModelExporter::execute(
			pJointNode, MeshGenerator::ModelFileType::OBJ, runPath, in_name);
	}

	//@3：可视化
	if (pJointNode)
	{
		this->mpGroup->addChild(pJointNode);
		this->mpGlobeWidget->flyTo(in_jointData1.pos.x(), in_jointData1.pos.y(), 20);
	}
}
