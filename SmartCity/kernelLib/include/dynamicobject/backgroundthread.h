#ifndef _BACKGROUNDTHREAD_H
#define _BACKGROUNDTHREAD_H

#include "dynamicobject/dynamicobjectconfig.h"
#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>
#include <math.h>
#include "dynamicobject/dynamicobject.h"
#include "dynamicobject/objectcontainerex.h"

class VR_DYNAMICOBJECT_EXPORT CBackgroundProcessor
	: public CSafeObjectContainer , public OpenThreads::Thread
{
public:
	CBackgroundProcessor();

	virtual ~CBackgroundProcessor();

	virtual void setState(const bool& state);

	/*
	* 后台线程函数
	*/
	virtual void run();

protected:
	bool mState;
	std::vector<DynamicObject::CDynamicObject*> mTempSceneObjectsNeedToRender;
};
#endif