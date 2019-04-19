#include "dynamicobject/backgroundthread.h"
#include "dynamicobject/wirelessobject.h"

CBackgroundProcessor::CBackgroundProcessor()
	: mState(true)
{

}

CBackgroundProcessor::~CBackgroundProcessor()
{

}

void CBackgroundProcessor::setState(const bool& state)
{
	this->mState = state;
}

void CBackgroundProcessor::run()
{
	while (this->mState)
	{
#if 0
		std::vector<std::string> keys;
		this->getKeys(keys);
		if (!keys.empty())
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string id = keys.at(i);
				DynamicObject::CDynamicObject * pDyObject =
					dynamic_cast<DynamicObject::CDynamicObject*>(this->getObject(id));
				if (pDyObject)
				{
					pDyObject->aSynLoad();
				}

				DynamicObject::CWirelessObject* pWirelessObject = 
					dynamic_cast<DynamicObject::CWirelessObject*>(this->getObject(id));
				if(pWirelessObject)
					pWirelessObject->aSynLoad();
			}
			this->removeAllObjects();
		}
#else
		std::vector<std::string> keys;
		this->getKeys(keys);
		if (!keys.empty())
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string id = keys.at(i);
				if (DynamicObject::CDynamicObject * pDyObject =
					dynamic_cast<DynamicObject::CDynamicObject*>(this->getObject(id)))
				{
					this->removeObject(id);
					pDyObject->aSynLoad();
				}
				else if(DynamicObject::CWirelessObject* pWirelessObject = 
					dynamic_cast<DynamicObject::CWirelessObject*>(this->getObject(id)))
				{
					this->removeObject(id);
					pWirelessObject->aSynLoad();
				}
				break;
			}
		}
#endif
		OpenThreads::Thread::microSleep(1);
	}
}