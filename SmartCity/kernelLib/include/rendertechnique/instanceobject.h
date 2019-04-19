#ifndef _CINSTANCE_OBJECT_H
#define _CINSTANCE_OBJECT_H

#include <osg/Matrix>
#include <osg/Node>
#include <osg/Group>
#include <osg/Vec4>
#include "rendertechnique/rendertechniqueconfig.h"

namespace Render
{
	struct InstanceMatrix
	{
		InstanceMatrix()
		{
			Matrix = osg::Matrix::identity();
			Color = osg::Vec4(1.0,1.0,1.0,1.0);
		}
		osg::Matrix Matrix;
		osg::Vec4 Color;
		int ObjectIndex;
	};

	class VR_RENDERTECHNIQUE_DLL ConvertToDrawInstanced : public osg::NodeVisitor
	{
	public:
		/**
		* Create the visitor that will convert primitive sets to draw
		* <num> instances.
		*/
		ConvertToDrawInstanced(
			unsigned                numInstances,
			const osg::BoundingBox& bbox,
			bool                    optimize );

		void apply(osg::Geode&);
		

	protected:
		unsigned _numInstances;
		osg::BoundingBox _bbox;
		bool _optimize;
		std::list<osg::PrimitiveSet*> _primitiveSets;
	};

	class VR_RENDERTECHNIQUE_DLL CInstanceObject
	{
	public:
		CInstanceObject(osg::Group* pSceneNode);

		~CInstanceObject();

		std::vector<InstanceMatrix>& getMatrixes();

		osg::Node* getNode();

		void setNode(osg::Node* pNode);

		void update(const int& objectIndex,const osg::Matrix& matrix);

		void initial();

		void render();

	protected:
		void createInstanceStateset(osg::StateSet* pStateset = new osg::StateSet);
		void createInstanceGeometry(osg::Node* pNode,const int& instanceNum);
		void assignInstaceDraw(osg::Geometry*pGeoemtry , const int& instanceCount = 1);
	protected:
		osg::Node* mpRefNode;
		osg::Group* mpRefSceneNode;
		std::vector<InstanceMatrix> mMatrixes;

	};
}

#endif