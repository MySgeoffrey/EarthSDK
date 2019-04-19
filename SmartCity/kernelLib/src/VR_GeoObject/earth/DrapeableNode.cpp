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
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "DrapeableNode.h"
#include "DrapingCullSet.h"
#include <osgEarth/Registry>
#include <osgEarth/CullingUtils>

#define LC "[DrapeableNodeEX] "

using namespace osgEarth;


DrapeableNodeEX::DrapeableNodeEX() :
_drapingEnabled( true )
{
    // Unfortunetly, there's no way to return a correct bounding sphere for
    // the node since the draping will move it to the ground. The bounds
    // check has to be done by the Draping Camera at cull time. Therefore we
    // have to ensure that this node makes it into the draping cull set so it
    // can be frustum-culled at the proper time.
    setCullingActive( !_drapingEnabled );
}

DrapeableNodeEX::DrapeableNodeEX(const DrapeableNodeEX& rhs, const osg::CopyOp& copy) :
osg::Group(rhs, copy)
{
    _drapingEnabled = rhs._drapingEnabled;
}

void
DrapeableNodeEX::setDrapingEnabled(bool value)
{
    if ( value != _drapingEnabled )
    {
        _drapingEnabled = value;
        setCullingActive( !_drapingEnabled );
    }
}

void
DrapeableNodeEX::traverse(osg::NodeVisitor& nv)
{
    if ( _drapingEnabled && nv.getVisitorType() == nv.CULL_VISITOR )
    {
        // access the cull visitor:
        osgUtil::CullVisitor* cv = Culling::asCullVisitor(nv);

        // find the cull set for this camera:
        DrapingCullSetEX& cullSet = DrapingCullSetEX::get( cv->getCurrentCamera() );
        cullSet.push( this, cv->getNodePath(), nv.getFrameStamp() );
		cullSet.accept(nv);
    }
    else
    {
        osg::Group::traverse( nv );
    }
}

//...........................................................................

#undef  LC
#define LC "[DrapeableNodeEX Serializer] "

#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

namespace
{
    REGISTER_OBJECT_WRAPPER(
        DrapeableNodeEX,
        new osgEarth::DrapeableNodeEX,
        osgEarth::DrapeableNodeEX,
        "osg::Object osg::Node osg::Group osgEarth::DrapeableNodeEX")
    {
        ADD_BOOL_SERIALIZER(DrapingEnabled, true);
    }
}
