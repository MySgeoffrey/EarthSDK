#define _USE_SCREEN_SCUTCHEON_ 1

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
#ifndef MY_ANNOTATION_SCUTCHEON_NODEEX_H
#define MY_ANNOTATION_SCUTCHEON_NODEEX_H 1

#include <osgEarthAnnotation/TrackNode>
#include "geoobject/geoobject_global.h"

namespace osgEarth
{ 
    class MapNode;
}
    
namespace osgEarth { namespace Annotation
{	
    using namespace osgEarth;
    using namespace osgEarth::Symbology;

    /** 
     * CGeoScutcheonNode is a movable, single-point entity represented by an icon,
     * optional placeable text labels, and optional localized geometry.
     *
     * NOTE. CGeoScutcheonNode does not automatically support terrain clamping. This is
     * because its intention is for use as a trackable entity marker, and 
     * presumably the entity itself will be responsible for its own absolute
     * positioning (and clamping, if applicable).
     */
    class VR_GEOOBJECT_EXPORT CGeoScutcheonNode : public osg::AutoTransform
    {
    public:
        /**
         * Constructs a new track node
         * @param mapNode     Map node under which this track will live
         * @param position    Initial position
         * @param image       Icon image to use
         * @param fieldSchema Schema for track label fields
         */
        CGeoScutcheonNode(
            MapNode*                    mapNode,
            const GeoPoint&             position,
            osg::Image*                 image,
            const TrackNodeFieldSchema& fieldSchema,
			const osg::Vec2d& backImageScale,
			const osg::Vec2d& titleImageScale);

        /**
         * Constructs a new track node
         * @param mapNode     Map node under which this track will live
         * @param position    Initial position
         * @param style       Style containing an IconSymbol for the image
         * @param fieldSchema Schema for track label fields
         */
        CGeoScutcheonNode(
            MapNode*                    mapNode,
            const GeoPoint&             position,
            const Style&                style,
			const Style&			    titleStyle,
            const TrackNodeFieldSchema& fieldSchema,
			const osg::Vec2d& backImageScale,
			const osg::Vec2d& titleImageScale);

        /** 
         * Sets the value of one of the field labels.
         * @param name  Field name as identified in the field schema.
         * @param value Value to which to set the field label.
         */
        void setFieldValue( const std::string& name, const std::string& value ) { setFieldValue(name, osgText::String(value)); }
        void setFieldValue( const std::string& name, const osgText::String& value );

        /**
         * Adds an arbitrary drawable to this track node. Useful for adding
         * user-defined graphics.
         * @param name     Name of the drawable (for later retrieval). The namespace
         *                 should not conflict with that of the field schema.
         * @param drawable Drawable to add.
         */
        void addDrawable( const std::string& name, osg::Drawable* drawable );

        /**
         * Gets a drawable (as previously added with addDrawable). 
         * Note: Make sure that if you are modifying a drawable, mark it with a 
         * DYNAMIC data variance so it will be thread-safe.
         */
        osg::Drawable* getDrawable( const std::string& name ) const;

		void updateLayoutData(const osg::Vec2s& screenOffset);

		void updateWorldPosition(const osg::Vec3d& worldPosition);

		bool contain(const int& x, const int& y ,osg::Camera* pCamera);

		osg::Vec3d getNearWorldPointToOrigin(osg::Camera* pCamera);

    public:

        virtual void setPriority(float value);

    protected:

        virtual ~CGeoScutcheonNode() { }

        Style             _style;
		Style			  _titleStyle;
        class osg::Geode* _imageGeode;
		osg::MatrixTransform* _localTransform;
		MapNode*              _mapNode;
        GeoPoint             _geoPosition;
		float _beginX,_beginY;
		float _width,_height;

        typedef fast_map<std::string, osg::Drawable*> NamedDrawables;
        NamedDrawables _namedDrawables;

        void init( const TrackNodeFieldSchema& schema,
			const osg::Vec2d& backImageScale,
			const osg::Vec2d& titleImageScale);

    private:
        // required by META_Node, but this object is not cloneable
        CGeoScutcheonNode() { }
        CGeoScutcheonNode(const CGeoScutcheonNode& rhs, const osg::CopyOp& op =osg::CopyOp::DEEP_COPY_ALL) { }

        void updateLayoutData();

    };

} } // namespace osgEarth::Annotation

#endif //MY_ANNOTATION_SCUTCHEON_NODE_H

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
#ifndef MY_ANNOTATION_SCUTCHEON_NODEEX_H
#define MY_ANNOTATION_SCUTCHEON_NODEEX_H 1

#include <osgEarthAnnotation/TrackNode>
#include "geoobject/geoobject_global.h"
#include <osg/AutoTransform>
#include <osgViewer/Viewer>

namespace osgEarth
{ 
    class MapNode;
}
    
namespace osgEarth { namespace Annotation
{	
    using namespace osgEarth;
    using namespace osgEarth::Symbology;
	class CGeoScutcheonNodeUpdateCallBack;
    /** 
     * CGeoScutcheonNode is a movable, single-point entity represented by an icon,
     * optional placeable text labels, and optional localized geometry.
     *
     * NOTE. CGeoScutcheonNode does not automatically support terrain clamping. This is
     * because its intention is for use as a trackable entity marker, and 
     * presumably the entity itself will be responsible for its own absolute
     * positioning (and clamping, if applicable).
     */
	class VR_GEOOBJECT_EXPORT CGeoScutcheonNode : public osg::MatrixTransform
    {
    public:
        /**
         * Constructs a new track node
         * @param mapNode     Map node under which this track will live
         * @param position    Initial position
         * @param image       Icon image to use
         * @param fieldSchema Schema for track label fields
         */
        CGeoScutcheonNode(
            MapNode*                    mapNode,
            const GeoPoint&             position,
            osg::Image*                 image,
            const TrackNodeFieldSchema& fieldSchema,
			const osg::Vec2d& backImageScale,
			const osg::Vec2d& titleImageScale,
			osgViewer::Viewer* pRefViewer,
			osg::Camera* pScreenCamera );

        /**
         * Constructs a new track node
         * @param mapNode     Map node under which this track will live
         * @param position    Initial position
         * @param style       Style containing an IconSymbol for the image
         * @param fieldSchema Schema for track label fields
         */
        CGeoScutcheonNode(
            MapNode*                    mapNode,
            const GeoPoint&             position,
            const Style&                style,
			const Style&			    titleStyle,
            const TrackNodeFieldSchema& fieldSchema,
			const osg::Vec2d& backImageScale,
			const osg::Vec2d& titleImageScale,
			osgViewer::Viewer* pRefViewer,
			osg::Camera* pScreenCamera);

        /** 
         * Sets the value of one of the field labels.
         * @param name  Field name as identified in the field schema.
         * @param value Value to which to set the field label.
         */
        void setFieldValue( const std::string& name, const std::string& value ) { setFieldValue(name, osgText::String(value)); }
        void setFieldValue( const std::string& name, const osgText::String& value );

        /**
         * Adds an arbitrary drawable to this track node. Useful for adding
         * user-defined graphics.
         * @param name     Name of the drawable (for later retrieval). The namespace
         *                 should not conflict with that of the field schema.
         * @param drawable Drawable to add.
         */
        void addDrawable( const std::string& name, osg::Drawable* drawable );

        /**
         * Gets a drawable (as previously added with addDrawable). 
         * Note: Make sure that if you are modifying a drawable, mark it with a 
         * DYNAMIC data variance so it will be thread-safe.
         */
        osg::Drawable* getDrawable( const std::string& name ) const;

		void updateLayoutData(const osg::Vec2s& screenOffset);

		void updateWorldPosition(const osg::Vec3d& worldPosition);

		double contain(const int& x, const int& y ,osg::Camera* pCamera);

		osg::Vec3d getNearWorldPointToOrigin(osg::Camera* pCamera);

		osg::Vec3d getWorldPosition();

    public:

        virtual void setPriority(float value);

    protected:

        virtual ~CGeoScutcheonNode();

        Style             _style;
		Style			  _titleStyle;
        class osg::Geode* _imageGeode;
		MapNode*              _mapNode;
        osg::Vec3d             _worldPosition;
		osgViewer::Viewer* mpRefViewer;
		osg::ref_ptr<osg::MatrixTransform> _localTransform;
		float _beginX,_beginY;
		float _width,_height;
		CGeoScutcheonNodeUpdateCallBack* mUpdateCallBack;

        typedef fast_map<std::string, osg::Drawable*> NamedDrawables;
        NamedDrawables _namedDrawables;

		static osg::ref_ptr<osg::Camera> g_ScutcheonCamera;

        void init( const TrackNodeFieldSchema& schema,
			const osg::Vec2d& backImageScale,
			const osg::Vec2d& titleImageScale,
			osgViewer::Viewer* pRefViewer,
			osg::Camera* pScreenCamera);

    private:
        // required by META_Node, but this object is not cloneable
        CGeoScutcheonNode() { }
        CGeoScutcheonNode(const CGeoScutcheonNode& rhs, const osg::CopyOp& op =osg::CopyOp::DEEP_COPY_ALL) { }

        void updateLayoutData();

    };

	class VR_GEOOBJECT_EXPORT CGeoScutcheonNodeUpdateCallBack
		: public osg::NodeCallback
	{
	public:
		CGeoScutcheonNodeUpdateCallBack();

		~CGeoScutcheonNodeUpdateCallBack();
		
		void setSceneCamera(osg::Camera* pRefCamera);

		/** Callback method called by the NodeVisitor when visiting a node.*/
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

	private:
		osg::Camera* mpRefCamera;
	};

} } // namespace osgEarth::Annotation

#endif //MY_ANNOTATION_SCUTCHEON_NODE_H

#endif