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
	/// ��ȡ����
	/// </summary>
	/// <param name="id">����id</param>
	virtual Common::IObject* getObject(const std::string& id);

	/// <summary>
	/// ��ȡ���������
	/// </summary>
	virtual int getNumOfObjects();

	/// <summary>
	/// ��ȡ����key
	/// </summary>
	/// <param name="keys">����keys����ֵ</param>
	virtual bool getKeys(std::vector<std::string>& keys);

	/// <summary>
	/// �Ƴ�����
	/// </summary>
	/// <param name="id">����id</param>
	virtual Common::IObject* removeObject(const std::string& id);

	/// <summary>
	/// ��ȡ���ж���
	/// </summary>
	virtual bool removeAllObjects();

	/// <summary>
	/// ��ȡ���ͷ����ж���
	/// </summary>
	virtual bool removeAndDestroyAllObjects();

protected:
	OpenThreads::Mutex mObjectMutex;
};
#endif