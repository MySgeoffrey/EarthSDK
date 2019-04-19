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
#include "geoobject/scutcheonnode.h"
#include <osgEarth/GeoMath>
#define LC "[CScutcheonNode] "

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Symbology;

osg::Geometry*
createImageGeometryEx(osg::Image*       image,
                                     const osg::Vec2s& pixelOffset,
                                     unsigned          textureUnit,
                                     double            heading,
									 double            wScale,
                                     double            hScale,
									 int			   zDepthOffset = -20)
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
    (*verts)[0].set( x0,     y0,     zDepthOffset );
    (*verts)[1].set( x0 + s, y0,     zDepthOffset );
    (*verts)[2].set( x0 + s, y0 + t, zDepthOffset );
    (*verts)[3].set( x0,     y0 + t, zDepthOffset );

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

//------------------------------------------------------------------------

CScutcheonNode::CScutcheonNode(MapNode*                    mapNode, 
                     const GeoPoint&             position,
                     osg::Image*                 image,
                     const TrackNodeFieldSchema& fieldSchema,
					 const osg::Vec2d& backImageScale,
					 const osg::Vec2d& titleImageScale) :

GeoPositionNode   ( mapNode, position )
{
    if ( image )
    {
        IconSymbol* icon = _style.getOrCreate<IconSymbol>();
        icon->setImage( image );
    }

    init( fieldSchema,backImageScale,titleImageScale);
}

CScutcheonNode::CScutcheonNode(MapNode*                    mapNode, 
                     const GeoPoint&             position,
                     const Style&                style,
					 const Style&			    titleStyle,
                     const TrackNodeFieldSchema& fieldSchema,
					 const osg::Vec2d& backImageScale,
					 const osg::Vec2d& titleImageScale) :

GeoPositionNode   ( mapNode, position ),
	_style      ( style ),_titleStyle(titleStyle)
{
    init( fieldSchema,backImageScale,titleImageScale);
}

void
CScutcheonNode::init( const TrackNodeFieldSchema& schema,
	const osg::Vec2d& backImageScale,
	const osg::Vec2d& titleImageScale)
{
    // CScutcheonNodes draw in screen space at their geoposition.
    ScreenSpaceLayout::activate( this->getOrCreateStateSet() );

    osgEarth::clearChildren( getPositionAttitudeTransform() );

    _imageGeode = new osg::Geode();
	//_textGeode = new osg::Geode();

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
        osg::Geometry* imageGeom = createImageGeometryEx( 
            image,                    // image
            offset,          // offset
            0,                        // tex image unit
            icon->heading()->eval(),
            backImageScale.x(),backImageScale.y(),-50);

        if ( imageGeom )
        {
			imageGeom->setName("0");
            _imageGeode->addDrawable( imageGeom );

            ScreenSpaceLayoutData* layout = new ScreenSpaceLayoutData();
            layout->setPriority(getPriority());
            imageGeom->setUserData(layout);
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
			// apply the image icon.
			osg::Geometry* imageGeom = createImageGeometryEx( 
				titleImage,                    // image
				titleImageOffset,          // offset
				0,                        // tex image unit
				titleIcon->heading()->eval(),
				titleImageScale.x(),titleImageScale.y());

			if ( imageGeom )
			{
				imageGeom->setName("1");
				_imageGeode->addDrawable( imageGeom );

				ScreenSpaceLayoutData* layout = new ScreenSpaceLayoutData();
				layout->setPriority(getPriority());
				imageGeom->setUserData(layout);
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

					/* osgText::Text* t = dynamic_cast< osgText::Text*>(drawable);
					 t->setEnableDepthWrites(false);*/
				}
            }
        }
    }

    // ensure depth testing always passes, and disable depth buffer writes.
    osg::StateSet* stateSet = _imageGeode->getOrCreateStateSet();
	stateSet->setAttributeAndModes( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1 );
	//stateSet->setRenderBinDetails(3,"DepthSortedBin");
	/*osg::StateSet* stateTextSet = _textGeode->getOrCreateStateSet();
    stateTextSet->setAttributeAndModes( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1 );*/
	//stateTextSet->setRenderBinDetails(5,"DepthSortedBin");
	applyStyle( _style );

    setLightingIfNotSet( false );

    getPositionAttitudeTransform()->addChild( _imageGeode );
	//getPositionAttitudeTransform()->addChild( _textGeode );
    
    // generate shaders:
    Registry::shaderGenerator().run(
        this,
        "osgEarth.TrackNode",
        Registry::stateSetCache() );
}

void
CScutcheonNode::setPriority(float value)
{
    GeoPositionNode::setPriority( value );
    updateLayoutData();
}

void 
CScutcheonNode::updateLayoutData(const osg::Vec2s& screenOffset)
{
#if 1
	osg::ref_ptr<ScreenSpaceLayoutData> data = new ScreenSpaceLayoutData();
    data->setPriority(getPriority());
	data->setPixelOffset(screenOffset);
	for (unsigned i = 0; i<_imageGeode->getNumDrawables(); ++i)
	{
		_imageGeode->getDrawable(i)->setUserData(data.get());
	}

	/*for (unsigned i = 0; i<_textGeode->getNumDrawables(); ++i)
	{
		_textGeode->getDrawable(i)->setUserData(data.get());
	}*/
#else
	
	for (unsigned i = 0; i<_imageGeode->getNumDrawables(); ++i)
	{
		float priority = getPriority();
		std::string name = _imageGeode->getDrawable(i)->getName();
		if (name == "0")
		{
			priority -= 500;
		}
		else if (name == "1")
		{
			priority -= 100;
		}
		osg::ref_ptr<ScreenSpaceLayoutData> data = new ScreenSpaceLayoutData();
		data->setPriority(priority);
		data->setPixelOffset(screenOffset);
		_imageGeode->getDrawable(i)->setUserData(data.get());
	}
#endif
}

void
CScutcheonNode::updateLayoutData()
{
#if 1
    osg::ref_ptr<ScreenSpaceLayoutData> data = new ScreenSpaceLayoutData();
    data->setPriority(getPriority());
    // re-apply annotation drawable-level stuff as neccesary.
    for (unsigned i = 0; i<_imageGeode->getNumDrawables(); ++i)
    {
        _imageGeode->getDrawable(i)->setUserData(data.get());
    }

	/*for (unsigned i = 0; i<_textGeode->getNumDrawables(); ++i)
	{
		_textGeode->getDrawable(i)->setUserData(data.get());
	}*/
#else
	for (unsigned i = 0; i<_imageGeode->getNumDrawables(); ++i)
	{
		float priority = getPriority();
		std::string name = _imageGeode->getDrawable(i)->getName();
		if (name == "0")
		{
			priority -= 500;
		}
		else if (name == "1")
		{
			priority -= 100;
		}
		osg::ref_ptr<ScreenSpaceLayoutData> data = new ScreenSpaceLayoutData();
		data->setPriority(priority);
		_imageGeode->getDrawable(i)->setUserData(data.get());
	}
#endif
}

void
CScutcheonNode::setFieldValue( const std::string& name, const osgText::String& value )
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
                    << "Illegal: attempt to modify a CScutcheonNode field value that is not marked as dynamic"
                    << std::endl;
            }
        }
    }
}

void
CScutcheonNode::addDrawable( const std::string& name, osg::Drawable* drawable )
{
    _namedDrawables[name] = drawable;
    _imageGeode->addDrawable( drawable );
    updateLayoutData();
}

osg::Drawable*
CScutcheonNode::getDrawable( const std::string& name ) const
{
    NamedDrawables::const_iterator i = _namedDrawables.find(name);
    return i != _namedDrawables.end() ? i->second : 0L;
}
