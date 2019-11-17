#ifndef MeshDefine_h__
#define MeshDefine_h__

#include "Macro.h"

namespace MeshGenerator
{
	struct MESH_GENERATOR_EXPORT MeshVertex
	{
		osg::Vec3 position;
		osg::Vec2 uv;
		osg::Vec3 normal;
	};

	struct MESH_GENERATOR_EXPORT SubMeshData
	{
		std::vector<unsigned int> indices;
	};

	struct MESH_GENERATOR_EXPORT MeshData
	{
		std::vector<MeshVertex> vertexs;
		std::vector<SubMeshData> subMeshs;
	};

	enum class MESH_GENERATOR_EXPORT SectionType
	{
		UnKnown = -1,
		Circle = 0,
		Rect = 1,
		Arc = 2,
		Ladder = 3,

		Count
	};

	struct MESH_GENERATOR_EXPORT SectionDesc
	{
		SectionType type;

		// 截面参数预留四个float
		// Circle : 0 radius
		// Rect: 0 width 1 height
		// Arc: 0 width 1 height 2 arcHeight
		// Ladder: 0 upEdge 1 downEdge 3 height
		float params[4];

		SectionDesc(){
			params[0] = params[1] = params[2] = params[3] = 1.f;
		}

		friend bool operator==(const SectionDesc& l, const SectionDesc& r)
		{
			return l.type == r.type && abs(l.params[0] - r.params[0]) < 0.000001f && 
				abs(l.params[1] - r.params[1]) < 0.000001f &&
				abs(l.params[2] - r.params[2]) < 0.000001f &&
				abs(l.params[3] - r.params[3]) < 0.000001f;
		}

		float getWidth() const 
		{
			float result = 0.f;
			switch (type)
			{
			case SectionType::Circle:
				return params[0] * 2.f;
			case SectionType::Rect:
			case SectionType::Arc:
			case SectionType::Ladder:
				result = std::max<float>(params[0],params[1]);
				break;
			default:
				break;
			}
			return result;
		}

		SectionDesc extent(float dis) const
		{
			SectionDesc r = *this;
			switch (type)
			{
			case SectionType::Circle:
			{
										r.params[0] += dis;
										break;
			}

			case SectionType::Rect:
			{
									  r.params[0] += dis;
									  r.params[1] += dis;
									  break;
			}
			case SectionType::Arc:
			case SectionType::Ladder:
			{
									  r.params[0] += dis;
									  r.params[1] += dis;
									  r.params[2] += dis;
									  break;
			}
			default:
				break;
			}
			return r;
		}	
	};

	struct MESH_GENERATOR_EXPORT JointAdjacent
	{
		osg::Vec3d pos;
		SectionDesc sectionDesc;
	};

	struct MESH_GENERATOR_EXPORT JointData
	{
		osg::Vec3d pos;
		std::vector<JointAdjacent> adjacents;
	};

}


#endif // MeshDefine_h__
