#ifndef _OBJECTCONTAINEREX_H
#define _OBJECTCONTAINEREX_H

#include "dynamicobject/dynamicobjectconfig.h"
#include "framework/common/iobjectcontainer.h"
#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>
#include <math.h>

class VR_DYNAMICOBJECT_EXPORT CSafeObjectContainer
	: public Core::IObjectContainer
{
public:
	CSafeObjectContainer();

	virtual ~CSafeObjectContainer();

	virtual bool addObject(Common::IObject* pObject);

	/// <summary>
	/// 获取对象
	/// </summary>
	/// <param name="id">对象id</param>
	virtual Common::IObject* getObject(const std::string& id);

	/// <summary>
	/// 获取对象个数据
	/// </summary>
	virtual int getNumOfObjects();

	/// <summary>
	/// 获取对象key
	/// </summary>
	/// <param name="keys">对象keys返回值</param>
	virtual bool getKeys(std::vector<std::string>& keys);

	/// <summary>
	/// 移除对象
	/// </summary>
	/// <param name="id">对象id</param>
	virtual Common::IObject* removeObject(const std::string& id);

	/// <summary>
	/// 获取所有对象
	/// </summary>
	virtual bool removeAllObjects();

	/// <summary>
	/// 获取并释放所有对象
	/// </summary>
	virtual bool removeAndDestroyAllObjects();

protected:
	OpenThreads::Mutex mObjectMutex;
};
#endif