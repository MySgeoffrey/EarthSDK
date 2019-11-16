#ifndef VR_DATAGENERATER_H
#define VR_DATAGENERATER_H

#include <QtCore/QThread>
#include "datagenerater/vr_datagenerater_global.h"
#include <string>
#include <map>
#include "dynamicobject/dynamicobject.h"

class VR_DATAGENERATER_EXPORT CDataCreator
{
public:

	static bool fromfile(const QString& path, std::vector<osg::Vec3d>& geometry);

	static bool tofile(const QString& path, std::vector<osg::Vec3d>& geometry);

};

class VR_DATAGENERATER_EXPORT CDataGenerater : public QThread
{
public:
	CDataGenerater();

	~CDataGenerater();

	static CDataGenerater* instance();

	virtual void run();

	void stopSendData();

	void startSendData();

	void readDataFromFile(QString id);

private:
	typedef std::map<std::string,DynamicObject::CDataSingal> DataSignals;
	DataSignals mSignals;
	static CDataGenerater* s_instance;
	bool mState;
	bool mIsOver;
	std::map<std::string,std::vector<osg::Vec3d>> mDatas;
	std::map<std::string,int> mDataSimIndexs;
};

#endif // VR_DATAGENERATER_H
