#include "datagenerater/vr_datagenerater.h"
#include <QtGlobal>
#include <QSettings>
#include <QStringList>
#include "dynamicobject/objectmanager.h"

CDataGenerater* CDataGenerater::s_instance = NULL;

bool CDataCreator::fromfile(const QString& path, std::vector<osg::Vec3d>& geometry)
{
	bool r = false;
	QSettings *configIniWrite = new QSettings(path + ".ini", QSettings::IniFormat);
	//向ini文件中写入内容,setValue函数的两个参数是键值对
	//向ini文件的第一个节写入内容,ip节下的第一个参数
	QString data = configIniWrite->value("/path").toString();
	QStringList ps = data.split('*');
	for (int i = 0; i < ps.size(); ++i)
	{
		QStringList geoPos = ps.at(i).split(" ");
		if (geoPos.size() < 3)
			continue;
		double x = geoPos.at(0).toDouble();
		double y = geoPos.at(1).toDouble();
		double z = geoPos.at(2).toDouble();
		geometry.push_back(osg::Vec3d(x,y,z));
		r = true;
	}
	//写入完成后删除指针
	delete configIniWrite;
	return r;
}

bool CDataCreator::tofile(const QString& path, std::vector<osg::Vec3d>& geometry)
{
	bool r = false;
	QSettings *configIniWrite = new QSettings(path, QSettings::IniFormat);
	//向ini文件中写入内容,setValue函数的两个参数是键值对
	//向ini文件的第一个节写入内容,ip节下的第一个参数
	QString content = "";
	for (int i = 0; i < geometry.size(); ++i)
	{
		osg::Vec3d p = geometry.at(i);
		content += QString::number(p.x(),'f',10) + " ";
		content += QString::number(p.y(),'f',10) + " ";
		content += QString::number(p.z(),'f',10) + " ";
		content += "*";
		r = true;
	}
	configIniWrite->setValue("/path", content);
	//写入完成后删除指针
	delete configIniWrite;
	return r;
}

CDataGenerater::CDataGenerater()
{
	this->mState = true;
	this->mIsOver = false;
}

CDataGenerater::~CDataGenerater()
{

}

CDataGenerater* CDataGenerater::instance()
{
	if (NULL == s_instance)
		s_instance = new CDataGenerater();
	return s_instance;
}

void CDataGenerater::stopSendData()
{
	this->mState = false;
}

void CDataGenerater::startSendData()
{
	this->mState = true;
}

void CDataGenerater::readDataFromFile(QString id)
{
	std::vector<osg::Vec3d> geometry;
	CDataCreator::fromfile(id,geometry);
	double intervalDis = 10 / 111000.0;
	if (geometry.size() > 1)
	{
		for (int i = 0 ; i < geometry.size() - 1; ++i)
		{
			osg::Vec3d startPoint = geometry.at(i);
			osg::Vec3d endPoint = geometry.at(i+1);
			startPoint.z() = 10;
			endPoint.z() = 10;
			osg::Vec3d dir = (endPoint - startPoint);
			double dis = dir.length();
			dir.normalize();
			int n = dis / intervalDis;
			for (int k = 0; k < n ; ++k)
			{
				osg::Vec3d curPoint = startPoint + dir * k * intervalDis;
				this->mDatas[id.toStdString()].push_back(curPoint);
			}
			this->mDatas[id.toStdString()].push_back(endPoint);
		}
	}
	mDataSimIndexs[id.toStdString()] = 0;
}

void CDataGenerater::run()
{
	double baseLongitude = 116.301612;
	double baseLatitude = 39.906085;
	double baseAltitude = 10.0;
	while (!this->mIsOver)
	{
		if (this->mState)
		{
			int id = rand() % 7;
			std::string objectID = QString::number(id).toStdString();//产生0到200之间的随机数作为对象的Id
			DataSignals::iterator itr = this->mSignals.find(objectID);
			{
				if (this->mDatas.find(objectID) == this->mDatas.end())
				{
					this->readDataFromFile(objectID.c_str());
				}
				osg::Vec3d geoPosition;
				//雷达设置探测目标
				DynamicObject::CDataSingal dataSignal;
				dataSignal.id = objectID;
				dataSignal.deviceID = "11111";
				dataSignal.type = DynamicObject::CDynamicObjectType::UAV;
				dataSignal.group = DynamicObject::CDynamicObjectGroupType::RADARTARGET;
				if (itr == this->mSignals.end())
				{
					geoPosition = this->mDatas[objectID].at(0);
					this->mDataSimIndexs[objectID]++;

					dataSignal.longitude = geoPosition.x();
					dataSignal.latitude = geoPosition.y();
					dataSignal.altitude = geoPosition.z();
				}
				else
				{
					dataSignal = itr->second;

					if (this->mDataSimIndexs[objectID] < this->mDatas[objectID].size())
					{
						//dataSignal.longitude = dataSignal.longitude + 1 / 111000.0;
						osg::Vec3d geoPosition = this->mDatas[objectID].at(this->mDataSimIndexs[objectID]);
						dataSignal.longitude = geoPosition.x();
						dataSignal.latitude = geoPosition.y();
						dataSignal.altitude = geoPosition.z();
						this->mDataSimIndexs[objectID]++;
					}
				}
				dataSignal.iconImageName = "uav_normal.png";
				if (id == 0)
				{
					dataSignal.name = "消防车01";
					dataSignal.iconImageName = "fire-engine.png";
				}
				else if (id == 1)
				{
					dataSignal.name = "消防车02";
					dataSignal.iconImageName = "FireTruck_Right.png";
				}
				else if (id == 2)
				{
					dataSignal.name = "消防员01";
					dataSignal.iconImageName = "firefighter_male.png";
				}
				else
				{
					dataSignal.name = "消防车" + QString::number(id).toStdString();
					dataSignal.iconImageName = "fire-engine.png";
				}
				dataSignal.iconscale = 0.6;
				dataSignal.fields.clear();
				dataSignal.fields.push_back(Geo::Field("编号","20180804"));
				dataSignal.fields.push_back(Geo::Field("名称",dataSignal.name));
				dataSignal.fields.push_back(Geo::Field("单位","某消防局"));
				dataSignal.fields.push_back(Geo::Field("性质","防火监督"));
				dataSignal.fields.push_back(Geo::Field("经度",QString::number(dataSignal.longitude,'f',7).toStdString()));
				dataSignal.fields.push_back(Geo::Field("纬度",QString::number(dataSignal.latitude,'f',7).toStdString()));
				this->mSignals[objectID] = dataSignal;
				//接收信号
				CObjectManager::instance()->aSynReveiveSignal(&dataSignal);
			}
			//等待1毫秒
			QThread::msleep(1);
		}

	}
}
