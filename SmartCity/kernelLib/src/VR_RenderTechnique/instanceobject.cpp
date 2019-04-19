#include "rendertechnique/instanceobject.h"
#include <osg/Image>
#include <osg/TextureBuffer>
#include <osg/Geometry>
#include <osg/Geode>

#define _matrix_buffer_channel_ 5

namespace Render
{
	// assume x is positive
	static int 
		nextPowerOf2(int x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return x+1;
	}

	ConvertToDrawInstanced::ConvertToDrawInstanced(unsigned numInstances,
			const osg::BoundingBox& bbox,
			bool                    optimize ) :
		_numInstances    ( numInstances ),
			_bbox(bbox),
			_optimize        ( optimize )
		{
			setTraversalMode( TRAVERSE_ALL_CHILDREN );
			setNodeMaskOverride( ~0 );
		}


		void 
			ConvertToDrawInstanced::apply( osg::Geode& geode )
		{
			for( unsigned d=0; d<geode.getNumDrawables(); ++d )
			{
				osg::Geometry* geom = geode.getDrawable(d)->asGeometry();
				if ( geom )
				{
					if ( _optimize )
					{
						// activate VBOs
						geom->setUseDisplayList( false );
						geom->setUseVertexBufferObjects( true );
					}

					geom->setInitialBound(_bbox);

					// convert to use DrawInstanced
					for( unsigned p=0; p<geom->getNumPrimitiveSets(); ++p )
					{
						osg::PrimitiveSet* ps = geom->getPrimitiveSet(p);
						ps->setNumInstances( _numInstances );
						_primitiveSets.push_back( ps );
					}

#ifdef USE_INSTANCE_LODS
					geom->setDrawCallback( new LODCallback() );
#endif
				}
			}

			traverse(geode);
		}

	CInstanceObject::CInstanceObject(osg::Group* pSceneNode)
	{
		this->mpRefNode = NULL;
		this->mpRefSceneNode = pSceneNode;
	}

	CInstanceObject::~CInstanceObject()
	{

	}

	std::vector<InstanceMatrix>& CInstanceObject::getMatrixes()
	{
		return this->mMatrixes;
	}

	osg::Node* CInstanceObject::getNode()
	{
		return this->mpRefNode;
	}

	void CInstanceObject::setNode(osg::Node* pNode)
	{
		this->mpRefNode = pNode;
	}

	void CInstanceObject::update(const int& objectIndex,const osg::Matrix& matrix)
	{

	}

	void CInstanceObject::initial()
	{
		if (NULL == this->getNode())
		{
			return ;
		}

		osg::StateSet* pStateset = this->getNode()->getOrCreateStateSet();
		if (!this->getMatrixes().empty())
		{
			this->createInstanceStateset(pStateset);
			this->createInstanceGeometry(this->getNode(),this->getMatrixes().size());
		}
	}

	void CInstanceObject::render()
	{
		if (this->mpRefNode)
		{
			this->mpRefSceneNode->addChild(mpRefNode);
		}
	}

	void CInstanceObject::createInstanceStateset(osg::StateSet* pStateset )
	{
		std::string shaderPath = "instanceRender.vert";
		osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
		vertexShader->setType( osg::Shader::VERTEX );
		vertexShader->loadShaderSourceFromFile(shaderPath);

		osg::ref_ptr< osg::Program > program = new osg::Program();
		program->addShader( vertexShader.get() );

		pStateset->setAttribute( program.get(),
			osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );

#if 1
		// This is the maximum size of the tbo 
		int maxTBOSize = 10000000;//Registry::capabilities().getMaxTextureBufferSize();
		// This is the total number of instances it can store
		// We will iterate below. If the number of instances is larger than the buffer can store
		// we make more tbos
		int maxTBOInstancesSize = 10000;//maxTBOSize/4;// 4 vec4s per matrix.

		unsigned tboSize = 0;
		unsigned numInstancesToStore = 0;

		if (this->getMatrixes().size()< maxTBOInstancesSize)
		{
			tboSize = nextPowerOf2(this->getMatrixes().size());
			numInstancesToStore = this->getMatrixes().size();
		}

		// sampler that will hold the instance matrices:
		osg::Image* pImage = new osg::Image();
		pImage->setName("osgearth.drawinstanced.postex");
		pImage->allocateImage( tboSize*4, 1, 1, GL_RGBA, GL_FLOAT );

		// could use PixelWriter but we know the format.
		// Note: we are building a transposed matrix because it makes the decoding easier in the shader.
		GLfloat* ptr = reinterpret_cast<GLfloat*>( pImage->data() );
		for(unsigned m=0; m < numInstancesToStore; ++m)
		{
			unsigned objectID = m;
			osg::Matrix& mat = this->getMatrixes().at(m).Matrix;

			// copy the first 3 columns:
			for(int col=0; col<3; ++col)
			{
				for(int row=0; row<4; ++row)
				{
					*ptr++ = mat(row,col);
				}
			}

			// encode the ObjectID in the last column, which is always (0,0,0,1)
			// in a standard scale/rot/trans matrix. We will reinstate it in the 
			// shader after extracting the object ID.
#if 0
			*ptr++ = (float)((objectID      ) & 0xff);
			*ptr++ = (float)((objectID >>  8) & 0xff);
			*ptr++ = (float)((objectID >> 16) & 0xff);
			*ptr++ = (float)((objectID >> 24) & 0xff);
#else
			osg::Vec4& color = this->getMatrixes().at(m).Color;
			*ptr++ = (float)(color.r());
			*ptr++ = (float)(color.g());
			*ptr++ = (float)(color.b());
			*ptr++ = (float)(color.a());
#endif
		}

		osg::TextureBuffer* pMatrixBuffer = new osg::TextureBuffer;
		pMatrixBuffer->setImage(pImage);
		pMatrixBuffer->setInternalFormat( GL_RGBA32F_ARB );
		pMatrixBuffer->setUnRefImageDataAfterApply( true );

		// so the TBO will serialize properly.
		pImage->setWriteHint(osg::Image::STORE_INLINE);

		//// Tell the SG to skip the positioning texture.
		//ShaderGenerator::setIgnoreHint(posTBO, true);

		pStateset->setTextureAttribute(_matrix_buffer_channel_, pMatrixBuffer);
		osg::ref_ptr< osg::Uniform > matrixBufferUniform =
			new osg::Uniform( "osgMatrixBuffer", _matrix_buffer_channel_ );
		pStateset->addUniform( matrixBufferUniform.get() );
#endif
	}

	void CInstanceObject::createInstanceGeometry(osg::Node* pNode,const int& instanceNum)
	{
		if (NULL == pNode)
			return ;
#if 0
		osg::BoundingBox bbox;
		bbox.expandBy(pNode->getInitialBound());
		ConvertToDrawInstanced nodeVisitor(instanceNum,bbox,true);
		pNode->accept(nodeVisitor);
#else
		osg::Group* pGroup = dynamic_cast<osg::Group*>(pNode);
		if (NULL == pGroup)
			return ;
		for (int i = 0; i < pGroup->getNumChildren(); ++i)
		{
			osg::Node* pSubNode = pGroup->getChild(i)->asNode();
			if (pSubNode)
			{
				osg::Group* pSubGroup = pSubNode->asGroup();
				if (pSubGroup == NULL)
					return ;
				osg::Geode* pGeode = pSubGroup->getChild(0)->asGeode();
				if (pGeode == NULL)
					return ;
				pGeode->setStateSet(pGroup->getStateSet());
				for (int n = 0; n < pGeode->getNumDrawables(); ++n)
				{
					osg::Drawable* pDrawable = pGeode->getDrawable(n);
					if (NULL != pDrawable)
					{
						osg::Geometry* pGeometry = pDrawable->asGeometry();
						if (pGeometry)
						{
							assignInstaceDraw(pGeometry,instanceNum);
							pGeometry->setUseDisplayList( false );
							pGeometry->setUseVertexBufferObjects( true );
						}
					}
				}
			}
		}
#endif
	}

	void CInstanceObject::assignInstaceDraw(osg::Geometry*pGeoemtry , const int& instanceCount )
	{
		for (int i = 0; i < pGeoemtry->getNumPrimitiveSets(); ++i)
		{
			osg::PrimitiveSet* pPrimitiveSet = pGeoemtry->getPrimitiveSet(i);
			if (pPrimitiveSet)
			{
				pPrimitiveSet->setNumInstances(instanceCount);
			}
		}
	}
}