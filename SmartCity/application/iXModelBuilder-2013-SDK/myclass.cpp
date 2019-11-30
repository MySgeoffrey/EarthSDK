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
	//�����ؼ���ʼ����������ά��ͼ
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	std::string runPath_3d = runPath + "/VR-GlobeConfig3D.earth";
	this->mpGlobeWidget->initial(true, runPath_3d.c_str());
	this->ui.gridLayout->setMargin(0);
	this->ui.gridLayout->setSpacing(0);
	this->ui.gridLayout->setContentsMargins(0, 0, 0, 0);
	this->ui.gridLayout->addWidget(this->mpGlobeWidget);

	//���عȸ�Ӱ������
	Globe::CGlobeMap::getInstance()->initialise(this->mpGlobeWidget->getMapRef());
	Globe::CGlobeMap::getInstance()->addImageLayer("", "googleImage", Globe::MapType::WEB_GOOGLE_MT);

	//��ʼ��
	this->init();
}

MyClass::~MyClass()
{

}

void MyClass::init()
{
	
	/*����������mesh��ӳ���ϵ,��15��Ϊ��λ�ظ���һ������������*/
	MeshGenerator::MeshUtil::getInstance()->setTextureRepeatLength(15.f);
	/*���ý�ͷ�ڻ���ʱ�Ƿ�����������˷���*/
	MeshGenerator::MeshUtil::getInstance()->setSameOffset(true);
	MeshGenerator::MeshUtil::getInstance()->setBlendParam(0.7);
	MeshGenerator::MeshUtil::getInstance()->setHatParam(0.5, 0.5);
	/*���ý�ͷ��ģʱ�Ĺ̶�����ƫ��*/
	MeshGenerator::MeshUtil::getInstance()->setFixedOffset(0.8);

	

	/*��ʼ����ͷ�Լ��ܵ�Ҫ�õ�����·��*/
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	this->mLinkerTexturePath = runPath + "/resource/image/blue.bmp";
	this->mPipeTexturePath = runPath + "/resource/image/red.bmp";

	/*��ģ�ο����ĵ�,��ģ�㷨�Դ�λ��Ϊ�ο���������άģ��*/
	this->mRefGeoCenter = osgEarth::GeoPoint(this->mpGlobeWidget->getMapNodeRef()->getMapSRS(),
		osg::Vec3d(116, 39, 0));

	/*ģ�ͽڵ��飬���ڽ����ɵ�ģ��node�ҽ���Ⱦ��������*/
	this->mpGroup = new osgEarth::GeoTransform();
	this->mpGroup->setPosition(this->mRefGeoCenter);

	/*���ο����������ø���ģ�㷨���߶����Ա������ģ�Դ����ĵ�Ϊ�ο���*/
	MeshGenerator::MeshUtil::getInstance()->setReferenceCenter(this->mRefGeoCenter);
	/*��ģ�ͽڵ���ҽӵ������У����ں�����ģ�ɹ�����ʾ*/
	this->mpGlobeWidget->getMapNodeRef()->addChild(this->mpGroup);

	//��ʱ���Թ���
	//loadPipeData();
}

void MyClass::loadPipeData()
{
	/*��ģ�ο����ĵ�,��ģ�㷨�Դ�λ��Ϊ�ο���������άģ��*/
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
					//@1:�������������
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
					//@2:��ͷ��ģ
					for (int i = 0; i < pPipePointDataSet->getPipePoints().size(); ++i)
					{
						PipeNet::CPipePoint* pPipePoint = pPipePointDataSet->getPipePoints().at(i);
						if (pPipePoint->getAdjcentLines().size() >= 2)
						{
							std::vector<osg::Vec3d> adjcentPoints;
							std::vector<double> radiusVecs;
							//��ͷ��ģ����
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
					//@3:�ܶν�ģ
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
	//ˮƽ��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.001, 0);
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;

	//@4����ͷ�İ뾶�����Ӧ�ܵ��İ뾶����һ�£�
	double radius = 1.0;
	//@5��ģ�͹������󣬸����û����빹����Ӧ��jointData
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
	//ˮƽ��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.002, 0);
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@3�����������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 38.9999, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	//@4����ͷ�İ뾶�����Ӧ�ܵ��İ뾶����һ�£�
	double radius = 1.0;
	//@5��ģ�͹������󣬸����û����빹����Ӧ��jointData
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
	//��ֱ��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.003, 0);
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0, 38.999, 3);
	//@4�����������ڵĵ�λ��
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
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4�����������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 2.9);
	//@5�����ĸ����ڵĵ�λ��
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
	//��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.005, 0);
	//@1����ģ
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
	//ˮƽ��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.006, 0);
	//@1����ģ
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
	//��ֱ��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.007, 0);
	//@1����ģ
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
	//@1����ģ
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
	//ˮƽ��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.009, 0);
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;

	//@4��
	double height = 3.0;
	double width = 2.5;
	double arcHeight = 1.0;
	//@5��ģ�͹������󣬸����û����빹����Ӧ��jointData
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
	//ˮƽ��ͨԲ�ν�ͷ
	osg::Vec3d offset(0.0, 0.01, 0);
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@3�����������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 38.9999, 2.9);
	jointAdjacentGeoPosition_Point1 += offset;
	jointAdjacentGeoPosition_Point2 += offset;
	jointAdjacentGeoPosition_Point3 += offset;

	//@4����ͷ�İ뾶�����Ӧ�ܵ��İ뾶����һ�£�
	double height = 3.0;
	double width = 2.5;
	double arcHeight = 1.0;
	//@5��ģ�͹������󣬸����û����빹����Ӧ��jointData
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
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4�����������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 2.9);
	//@5�����ĸ����ڵĵ�λ��
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
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
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
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4�����������ڵĵ�λ��
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
	//@1����ͷ��λ��
	osg::Vec3d jointGeoPosition(116.0, 39.0, 3);
	jointGeoPosition += offset;
	//@2����һ�����ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point1(116.0, 39.0001, 3);
	//@3���ڶ������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point2(116.0001, 39.0000, 3.3);
	//@4�����������ڵĵ�λ��
	osg::Vec3d jointAdjacentGeoPosition_Point3(115.9999, 39.0000, 2.9);
	//@5�����ĸ����ڵĵ�λ��
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
	//ˮƽ��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createCircleJoint_3_h();
	//@2��ģ����ɫ
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "CircleJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotCirclePipe3()
{
	//��ֱ��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createCircleJoint_3_v();
	//@2��ģ����ɫ
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "CircleJoint_3_v", this->mLinkerTexturePath);
}

void MyClass::slotCirclePipe4()
{
	//ˮƽ��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createCircleJoint_4_h();
	//@2��ģ����ɫ
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "CircleJoint_4_h", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe1()
{
	//��ͨ���ν�ͷ
	//@1����ģ
	MeshGenerator::JointData jointData = createRectJoint_2_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "RectJoint_2_h", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe2()
{
	//��ͨ���ν�ͷ
	//@1����ģ
	MeshGenerator::JointData jointData = createRectJoint_3_h();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "RectJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe3()
{
	//��ͨ���ν�ͷ
	//@1����ģ
	MeshGenerator::JointData jointData = createRectJoint_3_v();
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "RectJoint_3_v", this->mLinkerTexturePath);
}

void MyClass::slotRectPipe4()
{
	//��ͨ���ν�ͷ
	//@1����ģ
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
	//��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createArcJoint_2_h();
	//@2��ģ����ɫ
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "ArcJoint_2_h", this->mLinkerTexturePath);
}

void MyClass::slotArcPipe_3_h()
{
	//��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createArcJoint_3_h();
	//@2��ģ����ɫ
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "ArcJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotArcPipe_4_h()
{
	//��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createArcJoint_4_h();
	//@2��ģ����ɫ
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
	//��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createLadderJoint_2_h();
	//@2��ģ����ɫ
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "LadderJoint_2_h", this->mLinkerTexturePath);
}

void MyClass::slotLadderPipe_3_h()
{
	//��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createLadderJoint_3_h();
	//@2��ģ����ɫ
	osg::Vec4 color(1.0, 0.0, 0.0, 1.0);
	this->createJointModelAndExport(jointData, color, "LadderJoint_3_h", this->mLinkerTexturePath);
}

void MyClass::slotLadderPipe_4_h()
{
	//��ͨԲ�ν�ͷ
	//@1������JointData
	MeshGenerator::JointData jointData = createLadderJoint_4_h();
	//@2��ģ����ɫ
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
	//@2��ģ����ɫ
	//@3��ģ�͹������󣬸����û������jointData����ģ�����ݽڵ�
	//���û���������ʱ����������ʾ��������Ϊ��"",������ɫ��ʾ
	MeshGenerator::CModelCreator modelCreator;
	osg::ref_ptr<osg::Node> pJointNode = modelCreator.createLinkerModel(
		in_jointData, in_color, in_texturePath);

	//@4��ģ�͵���
	if (pJointNode)
	{
		std::string runPath = osgDB::getCurrentWorkingDirectory();
		MeshGenerator::CModelExporter::execute(
			pJointNode, MeshGenerator::ModelFileType::OBJ, runPath, in_name);
	}

	//@5�����ӻ�
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

	//@2������
	if (pJointNode)
	{
		std::string runPath = osgDB::getCurrentWorkingDirectory();
		MeshGenerator::CModelExporter::execute(
			pJointNode, MeshGenerator::ModelFileType::OBJ, runPath, in_name);
	}

	//@3�����ӻ�
	if (pJointNode)
	{
		this->mpGroup->addChild(pJointNode);
		this->mpGlobeWidget->flyTo(in_jointData1.pos.x(), in_jointData1.pos.y(), 20);
	}
}
