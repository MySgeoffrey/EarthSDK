#include "geoobject/customscutcheonnode.h"

#if _USE_SCREEN_SCUTCHEON_ == 0
/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
* Copyright 2016 Pelican Mapping
* http://osgearth.org
*
* osgEarth is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <osgEarth/ShaderGenerator>
#include <osgEarth/ScreenSpaceLayout>
#include <osg/Depth>
#include <osgText/Text>
#include <osgEarth/GeoMath>
#include <osgUtil/SceneView>
#define LC "[CGeoScutcheonNode] "

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Symbology;

osg::Geometry*
	createImageGeometryExForTest(osg::Image*       image,
	const osg::Vec2s& pixelOffset,
	unsigned          textureUnit,
	double            heading,
	double            wScale,
	double            hScale,
	int			   zOffset = -2)
{
	if ( !image )
		return 0L;

	osg::Texture2D* texture = new osg::Texture2D();
	texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
	texture->setResizeNonPowerOfTwoHint(false);
	texture->setImage( image );

	// set up the decoration.
	osg::StateSet* dstate = new osg::StateSet;
	dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
	dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	dstate->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);

	// set up the geoset.
	osg::Geometry* geom = new osg::Geometry();
	geom->setUseVertexBufferObjects(true);
	geom->setStateSet(dstate);

	float s = wScale * image->s();
	float t = hScale * image->t();

	float x0 = (float)pixelOffset.x() - s/2.0;
	float y0 = (float)pixelOffset.y() - t/2.0;

	osg::Vec3Array* verts = new osg::Vec3Array(4);
#if 0
	(*verts)[0].set( x0,     y0,     zOffset );
	(*verts)[1].set( x0 + s, y0,     zOffset );
	(*verts)[2].set( x0 + s, y0 + t, zOffset );
	(*verts)[3].set( x0,     y0 + t, zOffset );
#else
	//zOffset += 100;
	(*verts)[0].set( 0,     0,     0 );
	(*verts)[1].set( s, 0,     0 );
	(*verts)[2].set( s, t, 0 );
	(*verts)[3].set( 0,   t, 0 );
	for (int i = 0; i < 4; ++i)
	{
		(*verts)[i] += osg::Vec3(x0,y0,zOffset);
	}
#endif

	if (heading != 0.0)
	{
		osg::Matrixd rot;
		rot.makeRotate( heading, 0.0, 0.0, 1.0);
		for (unsigned int i = 0; i < 4; i++)
		{
			(*verts)[i] = rot * (*verts)[i];
		}
	}
	geom->setVertexArray(verts);

	osg::Vec2Array* tcoords = new osg::Vec2Array(4);
	(*tcoords)[0].set(0, 0);
	(*tcoords)[1].set(1, 0);
	(*tcoords)[2].set(1, 1);
	(*tcoords)[3].set(0, 1);
	geom->setTexCoordArray(textureUnit,tcoords);

	osg::Vec4Array* colors = new osg::Vec4Array(1);
	(*colors)[0].set(1.0f,1.0f,1.0,1.0f);
	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	return geom;
}

osg::Vec3d worldToScreen(const osg::Vec3d& worldPoint,osg::Camera* pCamera)
{
	osg::Camera* p_camera = pCamera;
	osg::Matrixd VPW = p_camera->getViewMatrix() *
		p_camera->getProjectionMatrix() *
		p_camera->getViewport()->computeWindowMatrix();
	osg::Vec3d window = worldPoint * VPW;
	return window;
}

osg::Vec3d screenToWorld(const osg::Vec3d& screenPoint,osg::Camera* pCamera)
{
	// 屏幕-〉世界 
	osg::Camera* p_camera = pCamera;
	osg::Matrixd VPW = p_camera->getViewMatrix() *
		p_camera->getProjectionMatrix() *
		p_camera->getViewport()->computeWindowMatrix();
	osg::Matrixd inverseVPW;
	inverseVPW.invert(VPW);
	osg::Vec3d world = screenPoint * inverseVPW;
	return world;
}

//------------------------------------------------------------------------

CGeoScutcheonNode::CGeoScutcheonNode(MapNode*                    mapNode, 
	const GeoPoint&             position,
	osg::Image*                 image,
	const TrackNodeFieldSchema& fieldSchema,
	const osg::Vec2d& backImageScale,
	const osg::Vec2d& titleImageScale) :

_mapNode( mapNode),_geoPosition(position )
{
	if ( image )
	{
		IconSymbol* icon = _style.getOrCreate<IconSymbol>();
		icon->setImage( image );
	}

	init( fieldSchema,backImageScale,titleImageScale);
}

CGeoScutcheonNode::CGeoScutcheonNode(MapNode*                    mapNode, 
	const GeoPoint&             position,
	const Style&                style,
	const Style&			    titleStyle,
	const TrackNodeFieldSchema& fieldSchema,
	const osg::Vec2d& backImageScale,
	const osg::Vec2d& titleImageScale) :

_mapNode( mapNode),_geoPosition(position ),
	_style      ( style ),_titleStyle(titleStyle)
{
	init( fieldSchema,backImageScale,titleImageScale);
}

void
	CGeoScutcheonNode::init( const TrackNodeFieldSchema& schema,
	const osg::Vec2d& backImageScale,
	const osg::Vec2d& titleImageScale)
{
	_imageGeode = new osg::Geode();
	//osg::Geode* titleGeode = new osg::Geode();

	IconSymbol* icon = _style.get<IconSymbol>();
	osg::Image* image = icon ? icon->getImage() : 0L;

	osg::Vec2s imageOffset(0,0);
	if ( icon && image )
	{
		double s = backImageScale.x() * image->s();
		double t = backImageScale.y() * image->t();
		// this offset anchors the image at the bottom
		osg::Vec2s offset;
		if ( !icon || !icon->alignment().isSet() )
		{	
			// default to bottom center
			offset.set(0.0, t / 2.0);
		}
		else
		{	// default to bottom center
			switch (icon->alignment().value())
			{
			case IconSymbol::ALIGN_LEFT_TOP:
				offset.set((s / 2.0), -(t / 2.0));
				break;
			case IconSymbol::ALIGN_LEFT_CENTER:
				offset.set((s / 2.0), 0.0);
				break;
			case IconSymbol::ALIGN_LEFT_BOTTOM:
				offset.set((s / 2.0), (t / 2.0));
				break;
			case IconSymbol::ALIGN_CENTER_TOP:
				offset.set(0.0, -(t / 2.0));
				break;
			case IconSymbol::ALIGN_CENTER_CENTER:
				offset.set(0.0, 0.0);
				break;
			case IconSymbol::ALIGN_CENTER_BOTTOM:
			default:
				offset.set(0.0, (t / 2.0));
				break;
			case IconSymbol::ALIGN_RIGHT_TOP:
				offset.set(-(s / 2.0), -(t / 2.0));
				break;
			case IconSymbol::ALIGN_RIGHT_CENTER:
				offset.set(-(s / 2.0), 0.0);
				break;
			case IconSymbol::ALIGN_RIGHT_BOTTOM:
				offset.set(-(s / 2.0), (t / 2.0));
				break;
			}
		}
		//offset = offset + screenOffset;
		imageOffset = offset;
		// apply the image icon.
		osg::Geometry* imageGeom = createImageGeometryExForTest( 
			image,                    // image
			offset,          // offset
			0,                        // tex image unit
			icon->heading()->eval(),
			backImageScale.x(),backImageScale.y(),-3);//-3

		if ( imageGeom )
		{
			float s = backImageScale.x() * image->s();
			float t = backImageScale.y() * image->t();

			float x0 = (float)offset.x() - s/2.0;
			float y0 = (float)offset.y() - t/2.0;

			_beginX = x0;
			_beginY = y0;
			_width = s;
			_height = t;
			imageGeom->setName("0");
			_imageGeode->addDrawable( imageGeom );
		}
	}

#if 1
	osgEarth::Symbology::TextSymbol* pTitleTextSymbol =
		this->_titleStyle.get<osgEarth::Symbology::TextSymbol>();
	if (pTitleTextSymbol)
	{
		IconSymbol* titleIcon = _titleStyle.get<IconSymbol>();
		osg::Image* titleImage = titleIcon ? titleIcon->getImage() : 0L;

		osg::Vec2s titleImageOffset(0,0);
		if ( titleIcon && titleImage )
		{
			// this offset anchors the image at the bottom
			osg::Vec2s offset(0,0);
			offset.y() = pTitleTextSymbol->pixelOffset().get().y();
			titleImageOffset = imageOffset + offset;

			/*titleImageOffset.x() -= titleImage->t() / 2;
			titleImageOffset.y() -= titleImage->s() / 2;*/
			// apply the image icon.
			osg::Geometry* imageGeom = createImageGeometryExForTest( 
				titleImage,                    // image
				titleImageOffset,          // offset
				0,                        // tex image unit
				titleIcon->heading()->eval(),
				titleImageScale.x(),titleImageScale.y(),-1.5);//-1.5

			if ( imageGeom )
			{
				imageGeom->setName("1");
				_imageGeode->addDrawable( imageGeom );
			}
		}
	}

#endif

	if ( !schema.empty() )
	{
		// turn the schema defs into text drawables and record a map so we can
		// set the field text later.
		for( TrackNodeFieldSchema::const_iterator i = schema.begin(); i != schema.end(); ++i )
		{
			const TrackNodeField& field = i->second;
			if ( field._symbol.valid() )
			{
				osg::Vec3 offset(
					field._symbol->pixelOffset()->x() + imageOffset.x(),
					field._symbol->pixelOffset()->y() + imageOffset.y(),
					0.0);

				osg::Drawable* drawable = AnnotationUtils::createTextDrawable( 
					field._symbol->content()->expr(),   // text
					field._symbol.get(),                // symbol
					offset );                           // offset

				if ( drawable )
				{
					// if the user intends to change the label later, make it dynamic
					// since osgText updates are not thread-safe
					if ( field._dynamic )
						drawable->setDataVariance( osg::Object::DYNAMIC );
					else
						drawable->setDataVariance( osg::Object::STATIC );
					drawable->setName("2");
					addDrawable( i->first, drawable );
					osgText::Text* pText = dynamic_cast<osgText::Text*>(drawable);
				}
			}
		}
	}

	osg::Vec3d worldPos(0,0,0);
	if (this->_geoPosition.vec3d() != osg::Vec3d(0,0,0))
		this->_geoPosition.toWorld(worldPos);

	this->_localTransform = new osg::MatrixTransform();
	this->_localTransform->addChild(_imageGeode);
	this->addChild(this->_localTransform);
	this->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
	this->setAutoScaleToScreen(true);
	this->setMinimumScale(0);
	this->setMaximumScale(FLT_MAX);//FLT_MAX
	this->setPosition(worldPos);

	osg::StateSet* stateSet = _imageGeode->getOrCreateStateSet();
#if 1
	stateSet->setAttributeAndModes( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1 );
	stateSet->setRenderBinDetails(15,"DepthSortedBin");
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON); //Alpha混合开启 
	//stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON); //取消深度测试 
	//stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF /*| osg::StateAttribute::PROTECTED*/);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
#else
	//让这些点总是画在最上面
	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(0,0);   
	stateSet->setAttributeAndModes(depth.get(),osg::StateAttribute::ON );
	stateSet->setRenderBinDetails(15,"DepthSortedBin");
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON); //Alpha混合开启 
	//stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON); //取消深度测试 
	//stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF /*| osg::StateAttribute::PROTECTED*/);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
#endif
}

void
	CGeoScutcheonNode::setPriority(float value)
{
	/*  GeoPositionNode::setPriority( value );
	updateLayoutData();*/
}

void CGeoScutcheonNode::updateWorldPosition(const osg::Vec3d& worldPosition)
{
	this->setPosition(worldPosition);
}

osg::Vec3d CGeoScutcheonNode::getNearWorldPointToOrigin(osg::Camera* pCamera)
{
	osg::Vec3d result(0,0,0);
	if (this->getPosition() == osg::Vec3d(0,0,0))
		return result;
	if (this->_localTransform && pCamera)
	{
		osg::Vec3 originPoint = worldToScreen(this->getPosition(),pCamera);
		double minDis = DBL_MAX;
		osg::Vec3d leftUp(_beginX,_beginY,0);
		osg::Vec3d rightUp(_beginX + _width,_beginY,0);
		osg::Vec3d rightDown(_beginX + _width,_beginY + _height,0);
		osg::Vec3d leftDown(_beginX,_beginY + _height,0);
		std::vector<osg::Vec3d> points;
		points.push_back(leftUp);
		points.push_back(rightUp);
		points.push_back(rightDown);
		points.push_back(leftDown);
		for (int i = 0 ; i < 4; ++i)
		{
			osg::Vec3d point = points.at(i);

			osg::Matrix mat = this->_localTransform->getMatrix();
			point = point * mat + originPoint;

			double dis = (point - originPoint).length();
			if (dis < minDis)
			{
				minDis = dis;
				result = point;
			}
		}
		result = screenToWorld(result,pCamera);
	}

	return result;
}

bool CGeoScutcheonNode::contain(const int& x, const int& y ,osg::Camera* pCamera)
{
	bool r = false;
	osg::Vec3 originPoint = worldToScreen(this->getPosition(),pCamera);

	osg::Vec3d beginPoint(_beginX,_beginY,0);
	osg::Vec3d endPoint(_beginX + _width,_beginY + _height,0);
	if (this->_localTransform)
	{
		osg::Matrix mat = this->_localTransform->getMatrix();
		beginPoint = beginPoint * mat + originPoint;
		endPoint = endPoint * mat  + originPoint;
		if (x >= beginPoint.x() && x <= endPoint.x()
			&& y >= beginPoint.y() && y <= endPoint.y())
		{
			r = true;
		}
	}
	return r;
}

void 
	CGeoScutcheonNode::updateLayoutData(const osg::Vec2s& screenOffset)
{
	if (this->_localTransform)
	{
		this->_localTransform->setMatrix(osg::Matrix::translate(
			screenOffset.x(),screenOffset.y(),0));
	}
}

void
	CGeoScutcheonNode::updateLayoutData()
{

}

void
	CGeoScutcheonNode::setFieldValue( const std::string& name, const osgText::String& value )
{
	NamedDrawables::const_iterator i = _namedDrawables.find(name);

	if ( i != _namedDrawables.end() )
	{
		osgText::Text* drawable = dynamic_cast<osgText::Text*>( i->second );
		if ( drawable )
		{
			// only permit updates if the field was declared dynamic, OR
			// this node is not connected yet
			if (drawable->getDataVariance() == osg::Object::DYNAMIC || this->getNumParents() == 0)
			{
				// btw, setText checks for assigning an equal value, so we don't have to
				drawable->setText( value );
			}
			else
			{
				OE_WARN << LC 
					<< "Illegal: attempt to modify a CGeoScutcheonNode field value that is not marked as dynamic"
					<< std::endl;
			}
		}
	}
}

void
	CGeoScutcheonNode::addDrawable( const std::string& name, osg::Drawable* drawable )
{
	_namedDrawables[name] = drawable;
	_imageGeode->addDrawable( drawable );
	updateLayoutData();
}

osg::Drawable*
	CGeoScutcheonNode::getDrawable( const std::string& name ) const
{
	NamedDrawables::const_iterator i = _namedDrawables.find(name);
	return i != _namedDrawables.end() ? i->second : 0L;
}

#else
/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
* Copyright 2016 Pelican Mapping
* http://osgearth.org
*
* osgEarth is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <osgEarth/ShaderGenerator>
#include <osgEarth/ScreenSpaceLayout>
#include <osg/Depth>
#include <osgText/Text>
#include "geoobject/customscutcheonnode.h"
#include <osgEarth/GeoMath>
#include <osgUtil/SceneView>
#define LC "[CGeoScutcheonNode] "

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Symbology;

osg::Geometry*
	createImageGeometryExForTest(osg::Image*       image,
	const osg::Vec2s& pixelOffset,
	unsigned          textureUnit,
	double            heading,
	double            wScale,
	double            hScale,
	int			   zOffset = -2)
{
	if ( !image )
		return 0L;

	osg::Texture2D* texture = new osg::Texture2D();
	/*texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
	texture->setResizeNonPowerOfTwoHint(false);*/
	texture->setImage( image );

	// set up the decoration.
	osg::StateSet* dstate = new osg::StateSet;
	/*dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);*/
	dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	dstate->setMode(GL_BLEND,osg::StateAttribute::ON);
	dstate->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);
	dstate->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	//dstate->setAttributeAndModes( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1 );
	//dstate->setRenderBinDetails(-1,"DepthSortedBin");

	// set up the geoset.
	osg::Geometry* geom = new osg::Geometry();
	geom->setUseVertexBufferObjects(true);
	geom->setStateSet(dstate);

	float s = wScale * image->s();
	float t = hScale * image->t();

	float x0 = (float)pixelOffset.x() - s/2.0;
	float y0 = (float)pixelOffset.y() - t/2.0;

	osg::Vec3Array* verts = new osg::Vec3Array(4);
#if 0
	(*verts)[0].set( x0,     y0,     zOffset );
	(*verts)[1].set( x0 + s, y0,     zOffset );
	(*verts)[2].set( x0 + s, y0 + t, zOffset );
	(*verts)[3].set( x0,     y0 + t, zOffset );
#else
	//zOffset += 100;
	(*verts)[0].set( 0,     0,     0 );
	(*verts)[1].set( s, 0,     0 );
	(*verts)[2].set( s, t, 0 );
	(*verts)[3].set( 0,   t, 0 );
	for (int i = 0; i < 4; ++i)
	{
		(*verts)[i] += osg::Vec3(x0,y0,zOffset);
	}
#endif

	if (heading != 0.0)
	{
		osg::Matrixd rot;
		rot.makeRotate( heading, 0.0, 0.0, 1.0);
		for (unsigned int i = 0; i < 4; i++)
		{
			(*verts)[i] = rot * (*verts)[i];
		}
	}
	geom->setVertexArray(verts);

	osg::Vec2Array* tcoords = new osg::Vec2Array(4);
	(*tcoords)[0].set(0, 0);
	(*tcoords)[1].set(1, 0);
	(*tcoords)[2].set(1, 1);
	(*tcoords)[3].set(0, 1);
	geom->setTexCoordArray(textureUnit,tcoords);

	osg::Vec4Array* colors = new osg::Vec4Array(1);
	(*colors)[0].set(1.0f,1.0f,1.0,1.0f);
	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	return geom;
}

osg::Camera* CreateTextHUDEX(osg::Vec2s position)
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	//二维相机，设置观察矩阵始终是一个单位矩阵
	camera->setViewMatrix(osg::Matrix::identity());
	//最后渲染
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	//在其他相机的基础上渲染，清楚缓存
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	////不接受事件
	//camera->setAllowEventFocus(false);
	//始终显示，不受其他节点影响
	//设置参考帧
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//设置视口
	//camera->setProjectionMatrixAsOrtho2D(0, 1920, 0, 1280);
	camera->setProjectionMatrixAsOrtho2D(0,1280,0,1024);
	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
		);

	//添加纹理
	osg::ref_ptr<osg::Geode> gnode2 = new osg::Geode;
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry();

	gnode2->addDrawable(gm);

	double width = 200;
	double height = 100;
	double depth = -0.00001;
	//压入顶点
	osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array();
	vertex->push_back(osg::Vec3(0.0, 0.0, depth));
	vertex->push_back(osg::Vec3(width, 0.0, depth));
	vertex->push_back(osg::Vec3(width, height, depth));
	vertex->push_back(osg::Vec3(0.0, height, depth));
	for (int i = 0; i < 4; ++i)
	{
		(*vertex)[i] += osg::Vec3(position.x(),position.y(),0);
	}

	gm->setVertexArray(vertex);

	//法线
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
	gm->setNormalArray(normal);

	//设置绑定方式
	gm->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> coord = new osg::Vec2Array();
	coord->push_back(osg::Vec2(0.0, 0.0));
	coord->push_back(osg::Vec2(1.0, 0.0));
	coord->push_back(osg::Vec2(1.0, 1.0));
	coord->push_back(osg::Vec2(0.0, 1.0));
	gm->setTexCoordArray(0, coord);

	//设置关联
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	//贴纹理
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("systemImages/resources/image/pannelbackground.png");
	osg::ref_ptr<osg::Texture2D> t2d = new osg::Texture2D();
	t2d->setImage(0, image);

	gm->getOrCreateStateSet()->setTextureAttributeAndModes(0, t2d, osg::StateAttribute::ON);
	gm->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	gm->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
	camera->addChild(gnode2);

	//设置字体
	osg::ref_ptr<osg::Geode> gnode = new osg::Geode;
	osg::ref_ptr<osgText::Text> text = new osgText::Text;

	//关闭字体光照
	gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	gnode->addDrawable(text);
	//微软雅黑
	text->setFont("Fonts/msyh.ttc");    
	text->setCharacterSize(60.0);
	text->setFontResolution(60.0, 60.0);
	text->setText("abc");
	text->setPosition(osg::Vec3(position.x() + 50,position.y() + 30,0.0));
	camera->addChild(gnode);
	return camera.release();
}

osg::Camera* CreateHUDCamera()
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	//二维相机，设置观察矩阵始终是一个单位矩阵
	camera->setViewMatrix(osg::Matrix::identity());
	//最后渲染
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	//在其他相机的基础上渲染，清楚缓存
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	//不接受事件
	camera->setAllowEventFocus(false);
	//始终显示，不受其他节点影响
	//设置参考帧
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//设置视口
	//camera->setProjectionMatrixAsOrtho2D(0, 1920, 0, 1280);
	camera->setProjectionMatrixAsOrtho2D(0,1280,0,1024);
	return camera.release();

	/*osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
		);
	const static int width = 1280, height = 1024;
	osg::Matrix m = osg::Matrix::ortho2D(0.0f, width, 0.0f, height);

	camera->setProjectionMatrix(m);
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT); 
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setViewport(0, 0, width, height);
	return camera;*/
}

void textInfo(osgText::Text* text)
{
	const osgText::Text::TextureGlyphQuadMap& tgqm = text->getTextureGlyphQuadMap();

	const osgText::Text::TextureGlyphQuadMap::const_iterator tgqmi = tgqm.begin();

	const osgText::Text::GlyphQuads& gq = tgqmi->second;

	osgText::String& s = text->getText();

	for (unsigned int i = 0; i < s.size(); i++)
	{
		osg::Vec2 ul = (*gq.getCoords())[0 + (i * 4)]; // upperLeft
		osg::Vec2 ll = (*gq.getCoords())[1 + (i * 4)]; // lowerLeft
		osg::Vec2 lr = (*gq.getCoords())[2 + (i * 4)]; // lowerRight
		osg::Vec2 ur = (*gq.getCoords())[3 + (i * 4)]; // upperRight

	}
}

osgText::Text* createLabel(const std::string& l, const char* f, unsigned int size)
{
	static osg::Vec3 pos(100.0f, 150.0f, 0.0f);
	//static osg::Vec3 pos(8000, 100, 0.0f);

	osgText::Text* label = new osgText::Text();
	osgText::Font* font = osgText::readFontFile("fonts/simhei.ttf");

	label->setFont(font);
	label->setCharacterSize(size);
	label->setFontResolution(size, size);
	label->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	label->setPosition(pos);
	label->setAlignment(osgText::Text::LEFT_BOTTOM);

	// It seems to be important we do this last to get best results?
	label->setText(l, osgText::String::ENCODING_UTF8);

	textInfo(label);

	pos.y() += size + 10.0f;
	//g_label = label;
	return label;
}


osg::Vec3d worldToScreen(const osg::Vec3d& worldPoint,osg::Camera* pCamera)
{
	osg::Camera* p_camera = pCamera;
	osg::Matrixd VPW = p_camera->getViewMatrix() *
		p_camera->getProjectionMatrix() *
		p_camera->getViewport()->computeWindowMatrix();
	osg::Vec3d window = worldPoint * VPW;
	return window;
}

osg::Vec3d screenToWorld(const osg::Vec3d& screenPoint,osg::Camera* pCamera)
{
	// 屏幕-〉世界 
	osg::Camera* p_camera = pCamera;
	osg::Matrixd VPW = p_camera->getViewMatrix() *
		p_camera->getProjectionMatrix() *
		p_camera->getViewport()->computeWindowMatrix();
	osg::Matrixd inverseVPW;
	inverseVPW.invert(VPW);
	osg::Vec3d world = screenPoint * inverseVPW;
	return world;
}

//------------------------------------------------------------------------

CGeoScutcheonNode::CGeoScutcheonNode(MapNode*                    mapNode, 
	const GeoPoint&             position,
	osg::Image*                 image,
	const TrackNodeFieldSchema& fieldSchema,
	const osg::Vec2d& backImageScale,
	const osg::Vec2d& titleImageScale,
	osgViewer::Viewer* pRefViewer,
	osg::Camera* pScreenCamera) :

_mapNode( mapNode),mpRefViewer(pRefViewer)
{
	if ( image )
	{
		IconSymbol* icon = _style.getOrCreate<IconSymbol>();
		icon->setImage( image );
	}
	this->_worldPosition = osg::Vec3d(0,0,0);
	if (position.vec3d() != osg::Vec3d(0,0,0))
		position.toWorld(this->_worldPosition);
	init( fieldSchema,backImageScale,titleImageScale,pRefViewer,pScreenCamera);
	mUpdateCallBack = new 
		CGeoScutcheonNodeUpdateCallBack();
	mUpdateCallBack->setSceneCamera(pRefViewer->getCamera());
	this->addUpdateCallback(mUpdateCallBack);
}

CGeoScutcheonNode::CGeoScutcheonNode(MapNode*                    mapNode, 
	const GeoPoint&             position,
	const Style&                style,
	const Style&			    titleStyle,
	const TrackNodeFieldSchema& fieldSchema,
	const osg::Vec2d& backImageScale,
	const osg::Vec2d& titleImageScale,
	osgViewer::Viewer* pRefViewer,
	osg::Camera* pScreenCamera) :

_mapNode( mapNode),mpRefViewer(pRefViewer),
	_style      ( style ),_titleStyle(titleStyle)
{
	this->_worldPosition = osg::Vec3d(0,0,0);
	if (position.vec3d() != osg::Vec3d(0,0,0))
		position.toWorld(this->_worldPosition);
	
	init( fieldSchema,backImageScale,titleImageScale,pRefViewer,pScreenCamera);

	mUpdateCallBack = new 
		CGeoScutcheonNodeUpdateCallBack();
	mUpdateCallBack->setSceneCamera(pRefViewer->getCamera());
	this->addUpdateCallback(mUpdateCallBack);
}

CGeoScutcheonNode::~CGeoScutcheonNode()
{
	if (this->mUpdateCallBack)
	{
		this->removeUpdateCallback(this->mUpdateCallBack);
		this->mUpdateCallBack = NULL;
	}
}


void
	CGeoScutcheonNode::init( const TrackNodeFieldSchema& schema,
	const osg::Vec2d& backImageScale,
	const osg::Vec2d& titleImageScale,
	osgViewer::Viewer* pRefViewer,
	osg::Camera* pScreenCamera)
{
#if _USE_SCREEN_SCUTCHEON_ == 0
	_imageGeode = new osg::Geode();
	//osg::Geode* titleGeode = new osg::Geode();

	IconSymbol* icon = _style.get<IconSymbol>();
	osg::Image* image = icon ? icon->getImage() : 0L;

	osg::Vec2s imageOffset(0,0);
	if ( icon && image )
	{
		double s = backImageScale.x() * image->s();
		double t = backImageScale.y() * image->t();
		// this offset anchors the image at the bottom
		osg::Vec2s offset;
		if ( !icon || !icon->alignment().isSet() )
		{	
			// default to bottom center
			offset.set(0.0, t / 2.0);
		}
		else
		{	// default to bottom center
			switch (icon->alignment().value())
			{
			case IconSymbol::ALIGN_LEFT_TOP:
				offset.set((s / 2.0), -(t / 2.0));
				break;
			case IconSymbol::ALIGN_LEFT_CENTER:
				offset.set((s / 2.0), 0.0);
				break;
			case IconSymbol::ALIGN_LEFT_BOTTOM:
				offset.set((s / 2.0), (t / 2.0));
				break;
			case IconSymbol::ALIGN_CENTER_TOP:
				offset.set(0.0, -(t / 2.0));
				break;
			case IconSymbol::ALIGN_CENTER_CENTER:
				offset.set(0.0, 0.0);
				break;
			case IconSymbol::ALIGN_CENTER_BOTTOM:
			default:
				offset.set(0.0, (t / 2.0));
				break;
			case IconSymbol::ALIGN_RIGHT_TOP:
				offset.set(-(s / 2.0), -(t / 2.0));
				break;
			case IconSymbol::ALIGN_RIGHT_CENTER:
				offset.set(-(s / 2.0), 0.0);
				break;
			case IconSymbol::ALIGN_RIGHT_BOTTOM:
				offset.set(-(s / 2.0), (t / 2.0));
				break;
			}
		}
		//offset = offset + screenOffset;
		imageOffset = offset;
		// apply the image icon.
		osg::Geometry* imageGeom = createImageGeometryExForTest( 
			image,                    // image
			offset,          // offset
			0,                        // tex image unit
			icon->heading()->eval(),
			backImageScale.x(),backImageScale.y(),-3);//-3

		if ( imageGeom )
		{
			float s = backImageScale.x() * image->s();
			float t = backImageScale.y() * image->t();

			float x0 = (float)offset.x() - s/2.0;
			float y0 = (float)offset.y() - t/2.0;

			_beginX = x0;
			_beginY = y0;
			_width = s;
			_height = t;
			imageGeom->setName("0");
			_imageGeode->addDrawable( imageGeom );
		}
	}

#if 1
	osgEarth::Symbology::TextSymbol* pTitleTextSymbol =
		this->_titleStyle.get<osgEarth::Symbology::TextSymbol>();
	if (pTitleTextSymbol)
	{
		IconSymbol* titleIcon = _titleStyle.get<IconSymbol>();
		osg::Image* titleImage = titleIcon ? titleIcon->getImage() : 0L;

		osg::Vec2s titleImageOffset(0,0);
		if ( titleIcon && titleImage )
		{
			// this offset anchors the image at the bottom
			osg::Vec2s offset(0,0);
			offset.y() = pTitleTextSymbol->pixelOffset().get().y();
			titleImageOffset = imageOffset + offset;

			/*titleImageOffset.x() -= titleImage->t() / 2;
			titleImageOffset.y() -= titleImage->s() / 2;*/
			// apply the image icon.
			osg::Geometry* imageGeom = createImageGeometryExForTest( 
				titleImage,                    // image
				titleImageOffset,          // offset
				0,                        // tex image unit
				titleIcon->heading()->eval(),
				titleImageScale.x(),titleImageScale.y(),-1.5);//-1.5

			if ( imageGeom )
			{
				imageGeom->setName("1");
				_imageGeode->addDrawable( imageGeom );
			}
		}
	}

#endif

	if ( !schema.empty() )
	{
		// turn the schema defs into text drawables and record a map so we can
		// set the field text later.
		for( TrackNodeFieldSchema::const_iterator i = schema.begin(); i != schema.end(); ++i )
		{
			const TrackNodeField& field = i->second;
			if ( field._symbol.valid() )
			{
				osg::Vec3 offset(
					field._symbol->pixelOffset()->x() + imageOffset.x(),
					field._symbol->pixelOffset()->y() + imageOffset.y(),
					0.0);

				osg::Drawable* drawable = AnnotationUtils::createTextDrawable( 
					field._symbol->content()->expr(),   // text
					field._symbol.get(),                // symbol
					offset );                           // offset

				if ( drawable )
				{
					// if the user intends to change the label later, make it dynamic
					// since osgText updates are not thread-safe
					if ( field._dynamic )
						drawable->setDataVariance( osg::Object::DYNAMIC );
					else
						drawable->setDataVariance( osg::Object::STATIC );
					drawable->setName("2");
					addDrawable( i->first, drawable );
					osgText::Text* pText = dynamic_cast<osgText::Text*>(drawable);
				}
			}
		}
	}

	osg::Vec3d worldPos(0,0,0);
	if (this->_geoPosition.vec3d() != osg::Vec3d(0,0,0))
		this->_geoPosition.toWorld(worldPos);

	this->_localTransform = new osg::MatrixTransform();
	this->_localTransform->addChild(_imageGeode);
	this->addChild(this->_localTransform);
	this->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
	this->setAutoScaleToScreen(true);
	this->setMinimumScale(0);
	this->setMaximumScale(FLT_MAX);//FLT_MAX
	this->setPosition(worldPos);

	osg::StateSet* stateSet = _imageGeode->getOrCreateStateSet();
#if 1
	stateSet->setAttributeAndModes( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1 );
	stateSet->setRenderBinDetails(15,"DepthSortedBin");
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON); //Alpha混合开启 
	//stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON); //取消深度测试 
	//stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF /*| osg::StateAttribute::PROTECTED*/);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
#else
	//让这些点总是画在最上面
	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(0,0);   
	stateSet->setAttributeAndModes(depth.get(),osg::StateAttribute::ON );
	stateSet->setRenderBinDetails(15,"DepthSortedBin");
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON); //Alpha混合开启 
	//stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON); //取消深度测试 
	//stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF /*| osg::StateAttribute::PROTECTED*/);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
#endif
#else
	osg::Vec3d worldPos = _worldPosition;
	
	_imageGeode = new osg::Geode();
	osg::Geode* _backImageGeode = new osg::Geode();
	osg::Vec2s imageScreenPosition(0,600);
	osg::Vec3d screenPoint = worldToScreen(worldPos,pRefViewer->getCamera());
	imageScreenPosition = osg::Vec2s(screenPoint.x(),screenPoint.y());
	IconSymbol* icon = _style.get<IconSymbol>();
	osg::Image* image = icon ? icon->getImage() : 0L;

	osg::Vec2s imageOffset(0,0);
	if ( icon && image )
	{
		double s = backImageScale.x() * image->s();
		double t = backImageScale.y() * image->t();
		// this offset anchors the image at the bottom
		osg::Vec2s offset;
		if ( !icon || !icon->alignment().isSet() )
		{	
			// default to bottom center
			offset.set(0.0, t / 2.0);
		}
		else
		{	// default to bottom center
			switch (icon->alignment().value())
			{
			case IconSymbol::ALIGN_LEFT_TOP:
				offset.set((s / 2.0), -(t / 2.0));
				break;
			case IconSymbol::ALIGN_LEFT_CENTER:
				offset.set((s / 2.0), 0.0);
				break;
			case IconSymbol::ALIGN_LEFT_BOTTOM:
				offset.set((s / 2.0), (t / 2.0));
				break;
			case IconSymbol::ALIGN_CENTER_TOP:
				offset.set(0.0, -(t / 2.0));
				break;
			case IconSymbol::ALIGN_CENTER_CENTER:
				offset.set(0.0, 0.0);
				break;
			case IconSymbol::ALIGN_CENTER_BOTTOM:
			default:
				offset.set(0.0, (t / 2.0));
				break;
			case IconSymbol::ALIGN_RIGHT_TOP:
				offset.set(-(s / 2.0), -(t / 2.0));
				break;
			case IconSymbol::ALIGN_RIGHT_CENTER:
				offset.set(-(s / 2.0), 0.0);
				break;
			case IconSymbol::ALIGN_RIGHT_BOTTOM:
				offset.set(-(s / 2.0), (t / 2.0));
				break;
			}
		}
		//offset = offset + screenOffset;
		imageOffset = offset;
		// apply the image icon.
		osg::Geometry* imageGeom = createImageGeometryExForTest( 
			image,                    // image
			offset,          // offset
			0,                        // tex image unit
			icon->heading()->eval(),
			backImageScale.x(),backImageScale.y(),0);//-3

		if ( imageGeom )
		{
			float s = backImageScale.x() * image->s();
			float t = backImageScale.y() * image->t();

			float x0 = (float)offset.x() - s/2.0;
			float y0 = (float)offset.y() - t/2.0;

			_beginX = x0;
			_beginY = y0;
			_width = s;
			_height = t;
			imageGeom->setName("0");
			_backImageGeode->addDrawable( imageGeom );
		}
	}

#if 1
	osgEarth::Symbology::TextSymbol* pTitleTextSymbol =
		this->_titleStyle.get<osgEarth::Symbology::TextSymbol>();
	if (pTitleTextSymbol)
	{
		IconSymbol* titleIcon = _titleStyle.get<IconSymbol>();
		osg::Image* titleImage = titleIcon ? titleIcon->getImage() : 0L;

		osg::Vec2s titleImageOffset(0,0);
		if ( titleIcon && titleImage )
		{
			// this offset anchors the image at the bottom
			osg::Vec2s offset(0,0);
			offset.y() = pTitleTextSymbol->pixelOffset().get().y();
			titleImageOffset = imageOffset + offset;

			osg::Geometry* imageGeom = createImageGeometryExForTest( 
				titleImage,                    // image
				titleImageOffset,          // offset
				0,                        // tex image unit
				titleIcon->heading()->eval(),
				titleImageScale.x(),titleImageScale.y(),-100);//-1.5

			if ( imageGeom )
			{
				imageGeom->setName("1");
				_imageGeode->addDrawable( imageGeom );
			}
		}
	}

#endif

	if ( !schema.empty() )
	{
		// turn the schema defs into text drawables and record a map so we can
		// set the field text later.
		for( TrackNodeFieldSchema::const_iterator i = schema.begin(); i != schema.end(); ++i )
		{
			const TrackNodeField& field = i->second;
			if ( field._symbol.valid() )
			{
				osg::Vec3 offset(
					field._symbol->pixelOffset()->x() + imageOffset.x(),
					field._symbol->pixelOffset()->y() + imageOffset.y(),
					0.0);

				osg::Drawable* drawable = AnnotationUtils::createTextDrawable( 
					field._symbol->content()->expr(),   // text
					field._symbol.get(),                // symbol
					offset);                           // offset

				if ( drawable )
				{
					// if the user intends to change the label later, make it dynamic
					// since osgText updates are not thread-safe
					if ( field._dynamic )
						drawable->setDataVariance( osg::Object::DYNAMIC );
					else
						drawable->setDataVariance( osg::Object::STATIC );
					drawable->setName("2");
					addDrawable( i->first, drawable );
					osgText::Text* pText = dynamic_cast<osgText::Text*>(drawable);
					osg::StateSet* dstate = new osg::StateSet;
					/*dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);*/
					dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
					dstate->setMode(GL_BLEND,osg::StateAttribute::ON);
					dstate->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
					pText->setStateSet(dstate);
				}

			}
		}
	}


	_localTransform = new osg::MatrixTransform();
	_localTransform->addChild(_backImageGeode);
	_localTransform->addChild(_imageGeode);
	this->updateWorldPosition(this->_worldPosition);
	this->addChild(_localTransform);
#endif
}

void
	CGeoScutcheonNode::setPriority(float value)
{
	/*  GeoPositionNode::setPriority( value );
	updateLayoutData();*/
}

void CGeoScutcheonNode::updateWorldPosition(const osg::Vec3d& worldPosition)
{
	this->_worldPosition = worldPosition;
	if (this->_worldPosition == osg::Vec3d(0,0,0))
	{
		this->setNodeMask(0);
	}
	else
	{
		this->setNodeMask(1);
	}
	osg::Vec3d imageScreenPosition = worldToScreen(this->_worldPosition,this->mpRefViewer->getCamera());
	this->setMatrix(osg::Matrix::translate(
		imageScreenPosition.x(),imageScreenPosition.y(),0));
}

osg::Vec3d CGeoScutcheonNode::getWorldPosition()
{
	return this->_worldPosition;
}

osg::Vec3d CGeoScutcheonNode::getNearWorldPointToOrigin(osg::Camera* pCamera)
{
	osg::Vec3d result(0,0,0);
	if (this->_worldPosition == osg::Vec3d(0,0,0))
		return result;
	if (this->_localTransform && pCamera)
	{
		osg::Vec3 originPoint = worldToScreen(this->_worldPosition,pCamera);
		double minDis = DBL_MAX;
		osg::Vec3d leftUp(_beginX,_beginY,0);
		osg::Vec3d rightUp(_beginX + _width,_beginY,0);
		osg::Vec3d rightDown(_beginX + _width,_beginY + _height,0);
		osg::Vec3d leftDown(_beginX,_beginY + _height,0);
		std::vector<osg::Vec3d> points;
		points.push_back(leftUp);
		points.push_back(rightUp);
		points.push_back(rightDown);
		points.push_back(leftDown);
		for (int i = 0 ; i < 4; ++i)
		{
			osg::Vec3d point = points.at(i);

			osg::Matrix mat = this->_localTransform->getMatrix();
			point = point * mat + originPoint;

			double dis = (point - originPoint).length();
			if (dis < minDis)
			{
				minDis = dis;
				result = point;
			}
		}
		result = screenToWorld(result,pCamera);
	}

	return result;
}

double CGeoScutcheonNode::contain(const int& x, const int& y ,osg::Camera* pCamera)
{
	double r = 0;
	osg::Vec3 originPoint = worldToScreen(this->_worldPosition,pCamera);

	osg::Vec3d beginPoint(_beginX,_beginY,0);
	osg::Vec3d endPoint(_beginX + _width,_beginY + _height,0);
	if (this->_localTransform)
	{
		osg::Matrix mat = this->_localTransform->getMatrix();
		beginPoint = beginPoint * mat + originPoint;
		endPoint = endPoint * mat  + originPoint;
		if (x >= beginPoint.x() && x <= endPoint.x()
			&& y >= beginPoint.y() && y <= endPoint.y())
		{
			r = originPoint.z();
		}
	}
	return r;
}

void 
	CGeoScutcheonNode::updateLayoutData(const osg::Vec2s& screenOffset)
{
	if (this->_localTransform)
	{
		this->_localTransform->setMatrix(osg::Matrix::translate(
		screenOffset.x(),screenOffset.y(),0));
	}
	
}

void
	CGeoScutcheonNode::updateLayoutData()
{

}

void
	CGeoScutcheonNode::setFieldValue( const std::string& name, const osgText::String& value )
{
	NamedDrawables::const_iterator i = _namedDrawables.find(name);

	if ( i != _namedDrawables.end() )
	{
		osgText::Text* drawable = dynamic_cast<osgText::Text*>( i->second );
		if ( drawable )
		{
			// only permit updates if the field was declared dynamic, OR
			// this node is not connected yet
			if (drawable->getDataVariance() == osg::Object::DYNAMIC || this->getNumParents() == 0)
			{
				// btw, setText checks for assigning an equal value, so we don't have to
				drawable->setText( value );
			}
			else
			{
				OE_WARN << LC 
					<< "Illegal: attempt to modify a CGeoScutcheonNode field value that is not marked as dynamic"
					<< std::endl;
			}
		}
	}
}

void
	CGeoScutcheonNode::addDrawable( const std::string& name, osg::Drawable* drawable )
{
	_namedDrawables[name] = drawable;
	_imageGeode->addDrawable( drawable );
	updateLayoutData();
}

osg::Drawable*
	CGeoScutcheonNode::getDrawable( const std::string& name ) const
{
	NamedDrawables::const_iterator i = _namedDrawables.find(name);
	return i != _namedDrawables.end() ? i->second : 0L;
}

CGeoScutcheonNodeUpdateCallBack::CGeoScutcheonNodeUpdateCallBack()
{
	this->mpRefCamera = NULL;
}

CGeoScutcheonNodeUpdateCallBack::~CGeoScutcheonNodeUpdateCallBack()
{

}

void CGeoScutcheonNodeUpdateCallBack::setSceneCamera(osg::Camera* pRefCamera)
{
	this->mpRefCamera = pRefCamera;
}

void CGeoScutcheonNodeUpdateCallBack::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if (this->mpRefCamera)
	{
		 CGeoScutcheonNode* pRealNode = dynamic_cast<CGeoScutcheonNode*>(node);
		 if (pRealNode)
		 {
			 osg::Vec3d worldPosition = pRealNode->getWorldPosition();
			 if (worldPosition != osg::Vec3d(0,0,0))
				 pRealNode->updateWorldPosition(worldPosition);
		 }
	}
}

#endif
