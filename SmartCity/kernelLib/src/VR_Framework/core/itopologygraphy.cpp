//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : itopologygraphy.cpp
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#include "framework/core/itopologygraphy.h"
#include "framework/core/itopologyarc.h"
#include "framework/core/itopologypoint.h"

namespace Core
{
	ITopologyGraphy::ITopologyGraphy()
	{
	
	}

	ITopologyGraphy::ITopologyGraphy(osgEarth::MapNode* pRefMapNode)
		: Core::ILayer(pRefMapNode)
	{
	
	}

	ITopologyGraphy::~ITopologyGraphy()
	{
		
	}

	Core::IObjectContainer* ITopologyGraphy::getArcsByStartPoint(const std::string& id)
    {
		Core::IObjectContainer* pArcs = NULL;
		std::vector<std::string> keys;
		if (this->getKeys(keys))
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string arcID = keys.at(i);
				IObject* pObject = this->getObject(arcID);
				if (Core::ITopologyArc* pTopologyArc =
					dynamic_cast<Core::ITopologyArc*>(pObject))
				{
					Core::ITopologyPoint* pStartTopologyPoint = pTopologyArc->getStartPoint();
					Core::ITopologyPoint* pEndTopologyPoint = pTopologyArc->getEndPoint();
					if (pStartTopologyPoint && pEndTopologyPoint)
					{
						if (pStartTopologyPoint->getID() == id)
						{
							if (pArcs == NULL)
							{
								pArcs = new Core::IObjectContainer();
							}
							pArcs->addObject(pTopologyArc);
						}
					}
					else
					{
						throw std::exception("it's a invalid TopologyArc at topologyGraphy getArcs !");
					}

				}
			}
		}
		return pArcs;
    }
    
	Core::IObjectContainer* ITopologyGraphy::getArcsByEndPoint(const std::string& id)
    {
		Core::IObjectContainer* pArcs = NULL;
		std::vector<std::string> keys;
		if (this->getKeys(keys))
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string arcID = keys.at(i);
				IObject* pObject = this->getObject(arcID);
				if (Core::ITopologyArc* pTopologyArc =
					dynamic_cast<Core::ITopologyArc*>(pObject))
				{
					Core::ITopologyPoint* pStartTopologyPoint = pTopologyArc->getStartPoint();
					Core::ITopologyPoint* pEndTopologyPoint = pTopologyArc->getEndPoint();
					if (pStartTopologyPoint && pEndTopologyPoint)
					{
						if (pEndTopologyPoint->getID() == id)
						{
							if (pArcs == NULL)
							{
								pArcs = new Core::IObjectContainer();
							}
							pArcs->addObject(pTopologyArc);
						}
					}
					else
					{
						throw std::exception("it's a invalid TopologyArc at topologyGraphy getArcs !");
					}

				}
			}
		}
		return pArcs;
    }
    
	Core::IObjectContainer* ITopologyGraphy::getArcs(const std::string& id)
    {
		Core::IObjectContainer* pArcs = NULL;
		std::vector<std::string> keys;
		if (this->getKeys(keys))
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string arcID = keys.at(i);
				IObject* pObject = this->getObject(arcID);
				if (Core::ITopologyArc* pTopologyArc =
					dynamic_cast<Core::ITopologyArc*>(pObject))
				{
					Core::ITopologyPoint* pStartTopologyPoint = pTopologyArc->getStartPoint();
					Core::ITopologyPoint* pEndTopologyPoint = pTopologyArc->getEndPoint();
					if (pStartTopologyPoint && pEndTopologyPoint)
					{
						if (pStartTopologyPoint->getID() == id
							|| pEndTopologyPoint->getID() == id)
						{
							if (pArcs == NULL)
							{
								pArcs = new Core::IObjectContainer();
							}
							pArcs->addObject(pTopologyArc);
						}
					}
					else
					{
						throw std::exception("it's a invalid TopologyArc at topologyGraphy getArcs !");
					}

				}
			}
		}
		return pArcs;
    }

	Core::ITopologyPoint* ITopologyGraphy::getTopologyPoint(const double& lon, const double& lat, const double& alt)
	{
		Core::ITopologyPoint* pResult = NULL;
		std::vector<std::string> keys;
		if (this->mTopologyPoints.getKeys(keys))
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string id = keys.at(i);
				IObject* pObject = this->mTopologyPoints.getObject(id);
				if (Core::ITopologyPoint* pTopologyPoint =
					dynamic_cast<Core::ITopologyPoint*>(pObject))
				{
					osg::Vec3d& position = pTopologyPoint->getGeoPosition();
					if (std::abs(position.x() - lon) < std::numeric_limits<double>::min()
						&& std::abs(position.y() - lat) < std::numeric_limits<double>::min()
						&& std::abs(position.z() - alt) < std::numeric_limits<double>::min())
					{
						pResult = pTopologyPoint;
						break;
					}
						
				}
			}
		}

		return pResult;
	}
    
	Core::ITopologyArc* ITopologyGraphy::getTopologyArc(const std::string& id1, const std::string& id2)
	{
		Core::ITopologyArc* pArc = NULL;
		std::vector<std::string> keys;
		if (this->getKeys(keys))
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string id = keys.at(i);
				IObject* pObject = this->getObject(id);
				if (Core::ITopologyArc* pTopologyArc =
					dynamic_cast<Core::ITopologyArc*>(pObject))
				{
					Core::ITopologyPoint* pStartTopologyPoint = pTopologyArc->getStartPoint();
					Core::ITopologyPoint* pEndTopologyPoint = pTopologyArc->getEndPoint();
					if (pStartTopologyPoint && pEndTopologyPoint)
					{
						bool r1 = pStartTopologyPoint->getID() == id1
							&& pEndTopologyPoint->getID() == id2;

						bool r2 = pStartTopologyPoint->getID() == id2
							&& pEndTopologyPoint->getID() == id1;
						if (r1 || r2)
						{
							pArc = pTopologyArc;
							break;
						}
					}
					else
					{
						throw std::exception("it's a invalid TopologyArc at topologyGraphy getArcs !");
					}

				}
			}
		}

		return pArc;
	}

	bool ITopologyGraphy::loadToScene()
	{
		return false;
	}

	bool ITopologyGraphy::renderToScene()
	{
		return false;
	}
}
