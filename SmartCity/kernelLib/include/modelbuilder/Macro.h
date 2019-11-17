#ifndef MeshGeneratorDefine_h__
#define MeshGeneratorDefine_h__


#ifdef VR_MESH_GENERATOR_EXPORTS
# define MESH_GENERATOR_EXPORT _declspec(dllexport)
#else
# define MESH_GENERATOR_EXPORT _declspec(dllimport)
#endif


#include "osg/Vec2"
#include "osg/Vec3"
#include "osg/Vec4"
#include "osg/Matrix"
#include <vector>
#include <map>
#include <string>
#include <set>
#include <memory>
#include <algorithm>


const osg::Vec3 FRONT_DIR = osg::Y_AXIS;
const osg::Vec3 UP_DIR = osg::Z_AXIS;

#endif // MeshGeneratorDefine_h__
