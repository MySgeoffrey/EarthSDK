#include "modelbuilder/SectionCreator.h"
#include <algorithm>

namespace MeshGenerator
{
	bool SectionCircle::build(const SectionDesc& desc)
	{
		_radius = desc.params[0];
		int segment = 16;

		vertexs.clear();
		vertexs.resize(segment + 1);

		osg::Vec3 v(_radius, 0.f, 0.f);

		for (int i = 0; i <= segment;++i)
		{
			osg::Matrix mat;
			mat = osg::Matrix::rotate(osg::Quat((i * 2.f * osg::PI / segment), FRONT_DIR));
			vertexs[i].position = v * mat;
			vertexs[i].normal = vertexs[i].position;
			vertexs[i].normal.normalize();
			vertexs[i].uv = osg::Vec2(0.f, (float)i / (float)segment);
		}

		topVertexs.resize(1 + segment/2);
		for (int i = 0; i <= segment/2; ++i)
		{
			osg::Matrix mat;
			mat = osg::Matrix::rotate(osg::Quat((-i * 2.f * osg::PI / segment), FRONT_DIR));
			topVertexs[i].position = v * mat;
		}

		bodyVertexs.resize(1 + segment/2);
		for (int i = 0; i <= segment / 2; ++i)
		{
			osg::Matrix mat;
			mat = osg::Matrix::rotate(osg::Quat((i * 2.f * osg::PI / segment), FRONT_DIR));
			bodyVertexs[i].position = v * mat;
		}

		return true;
	}

	bool SectionRect::build(const SectionDesc& desc)
	{
		_width = desc.params[0];
		_height = desc.params[1];
		vertexs.clear();
		vertexs.resize(8);

		float grith = (_width + _height) * 2.f;

		vertexs[0].position = osg::Vec3(0.5f * _width, 0.f, 0.5f*_height);
		vertexs[0].normal = osg::Vec3(1.f,0.f,0.f);
		vertexs[0].uv = osg::Vec2(0.f, 0.f);

		vertexs[1].position = osg::Vec3(0.5f * _width, 0.f, -0.5f*_height);
		vertexs[1].normal = osg::Vec3(1.f, 0.f, 0.f);
		vertexs[1].uv = osg::Vec2(0.f, _height / grith);

		vertexs[2].position = osg::Vec3(0.5f * _width, 0.f, -0.5f*_height);
		vertexs[2].normal = osg::Vec3(0.f, 0.f, -1.f);
		vertexs[2].uv = osg::Vec2(0.f, _height / grith);

		vertexs[3].position = osg::Vec3(-0.5f * _width, 0.f, -0.5f*_height);
		vertexs[3].normal = osg::Vec3(0.f, 0.f, -1.f);
		vertexs[3].uv = osg::Vec2(0.f, (_width + _height) / grith);

		vertexs[4].position = osg::Vec3(-0.5f * _width, 0.f, -0.5f*_height);
		vertexs[4].normal = osg::Vec3(-1.f, 0.f, 0.f);
		vertexs[4].uv = osg::Vec2(0.f, (_width + _height) / grith);

		vertexs[5].position = osg::Vec3(-0.5f * _width, 0.f, 0.5f*_height);
		vertexs[5].normal = osg::Vec3(-1.f, 0.f, 0.f);
		vertexs[5].uv = osg::Vec2(0.f, (_width + 2.f * _height) / grith);

		vertexs[6].position = osg::Vec3(-0.5f * _width, 0.f, 0.5f*_height);
		vertexs[6].normal = osg::Vec3(0.f, 0.f, 1.f);
		vertexs[6].uv = osg::Vec2(0.f, (_width + 2.f * _height) / grith);

		vertexs[7].position = osg::Vec3(0.5f * _width, 0.f, 0.5f*_height);
		vertexs[7].normal = osg::Vec3(0.f, 0.f, 1.f);
		vertexs[7].uv = osg::Vec2(0.f, 1.f);

		topVertexs.resize(3);
		topVertexs[0].position = osg::Vec3(0.5f * _width, 0.f, 0.5f*_height);
		topVertexs[1].position = osg::Vec3(0.f, 0.f, 0.5f*_height);
		topVertexs[2].position = osg::Vec3(-0.5f * _width, 0.f, 0.5f*_height);

		bodyVertexs.resize(5);
		bodyVertexs[0].position = osg::Vec3(0.5f * _width, 0.f, 0.5f*_height);
		bodyVertexs[1].position = osg::Vec3(0.5f * _width, 0.f, -0.5f*_height);
		bodyVertexs[2].position = osg::Vec3(0.f, 0.f, -0.5f*_height);
		bodyVertexs[3].position = osg::Vec3(-0.5f * _width, 0.f, -0.5f*_height);
		bodyVertexs[4].position = osg::Vec3(-0.5f * _width, 0.f, 0.5f*_height);
	
		return true;
	}

	bool SectionArc::build(const SectionDesc& desc)
	{
		_width = desc.params[0];
		_height = desc.params[1];
		_arcHeight = desc.params[2];

		int arcSegment = 8;

		vertexs.clear();
		vertexs.resize(6 + arcSegment + 1);

		float radius = ((_width*0.5f)*(_width*0.5f) + _arcHeight*_arcHeight) / (2.f * _arcHeight);
		float angle = 2.f * acos((radius - _arcHeight) / radius);

		float grith = _width + _height * 2.f + angle * radius;

		vertexs[0].position = osg::Vec3(0.5f * _width, 0.f, 0.5f*_height);
		vertexs[0].normal = osg::Vec3(1.f, 0.f, 0.f);
		vertexs[0].uv = osg::Vec2(0.f, 0.f);

		vertexs[1].position = osg::Vec3(0.5f * _width, 0.f, -0.5f*_height);
		vertexs[1].normal = osg::Vec3(1.f, 0.f, 0.f);
		vertexs[1].uv = osg::Vec2(0.f, _height / grith);

		vertexs[2].position = osg::Vec3(0.5f * _width, 0.f, -0.5f*_height);
		vertexs[2].normal = osg::Vec3(0.f, 0.f, -1.f);
		vertexs[2].uv = osg::Vec2(0.f, _height / grith);

		vertexs[3].position = osg::Vec3(-0.5f * _width, 0.f, -0.5f*_height);
		vertexs[3].normal = osg::Vec3(0.f, 0.f, -1.f);
		vertexs[3].uv = osg::Vec2(0.f, (_width + _height) / grith);

		vertexs[4].position = osg::Vec3(-0.5f * _width, 0.f, -0.5f*_height);
		vertexs[4].normal = osg::Vec3(-1.f, 0.f, 0.f);
		vertexs[4].uv = osg::Vec2(0.f, (_width + _height) / grith);

		vertexs[5].position = osg::Vec3(-0.5f * _width, 0.f, 0.5f*_height);
		vertexs[5].normal = osg::Vec3(-1.f, 0.f, 0.f);
		vertexs[5].uv = osg::Vec2(0.f, (_width + 2.f * _height) / grith);

		osg::Vec3 origin(0.f, 0.f, _arcHeight + _height * 0.5f - radius);
		osg::Vec3 v = osg::Vec3(-_width*0.5f, 0.f, 0.5f * _height) - origin;
		for (int i = 0; i < arcSegment; ++i)
		{
			auto a = i * angle / arcSegment;
			osg::Matrix mat = osg::Matrix::rotate(osg::Quat(a, osg::Y_AXIS));

			vertexs[6 + i].position = v * mat + origin;
			vertexs[6 + i].normal = vertexs[6 + i].position - origin;
			vertexs[6 + i].normal.normalize();
			vertexs[6 + i].uv = osg::Vec2(0.f, (_width + 2.f * _height + a * radius) / grith);
		}
		vertexs[6].normal = osg::Vec3(-1.f, 0.f, 0.f);

		vertexs[6 + arcSegment].position = osg::Vec3(0.5f * _width, 0.f, 0.5f*_height);
		vertexs[6 + arcSegment].normal = osg::Vec3(1.f, 0.f, 0.f);
		vertexs[6 + arcSegment].normal.normalize();
		vertexs[6 + arcSegment].uv = osg::Vec2(0.f, 1.f);

		topVertexs.resize(arcSegment + 1);
		v = osg::Vec3(_width*0.5f, 0.f, 0.5f * _height) - origin;
		for (int i = 0; i < arcSegment; ++i)
		{
			auto a = -i * angle / arcSegment;
			osg::Matrix mat = osg::Matrix::rotate(osg::Quat(a, osg::Y_AXIS));
			topVertexs[i].position = v * mat + origin;
		}
		topVertexs[arcSegment].position = osg::Vec3(-0.5f * _width, 0.f, 0.5f*_height);

		bodyVertexs.resize(5);
		bodyVertexs[0].position = osg::Vec3(0.5f * _width, 0.f, 0.5f*_height);
		bodyVertexs[1].position = osg::Vec3(0.5f * _width, 0.f, -0.5f*_height);
		bodyVertexs[2].position = osg::Vec3(0.f, 0.f, -0.5f*_height);
		bodyVertexs[3].position = osg::Vec3(-0.5f * _width, 0.f, -0.5f*_height);
		bodyVertexs[4].position = osg::Vec3(-0.5f * _width, 0.f, 0.5f*_height);

		return true;
	}

	bool SectionLadder::build(const SectionDesc& desc)
	{
		_topWidth = desc.params[0];
		_bottomWidth = desc.params[1];
		_height = desc.params[2];
		//_angle = desc.params[3];

		vertexs.clear();
		vertexs.resize(8);

		/*float offset = 0.f;
		if (_angle - 90.f > 0.0001f && _angle - 90.f < -0.0001f)
		{
		offset = 0.f;
		}
		else
		{
		offset = _height / tan(osg::PI * _angle / 180.f);
		}

		osg::Vec3 v1 = osg::Vec3(-0.5 * _bottomWidth + offset + _topWidth, 0.f, _height * 0.5f);
		osg::Vec3 v2 = osg::Vec3(0.5 * _bottomWidth, 0.f, -_height * 0.5f);
		osg::Vec3 v3 = osg::Vec3(-0.5 * _bottomWidth, 0.f, -_height * 0.5f);
		osg::Vec3 v4 = osg::Vec3(-0.5 * _bottomWidth + offset, 0.f, _height * 0.5f);*/
		osg::Vec3 v1 = osg::Vec3(_topWidth* 0.5f, 0.f, 0.5 *_height);
		osg::Vec3 v2 = osg::Vec3(0.5 * _bottomWidth, 0.f, -_height * 0.5f);
		osg::Vec3 v3 = osg::Vec3(-0.5 * _bottomWidth, 0.f, -_height * 0.5f);
		osg::Vec3 v4 = osg::Vec3(-_topWidth* 0.5f, 0.f, 0.5 *_height);

		float grith = _bottomWidth + _topWidth + (v2 - v1).length() + (v4 - v3).length();

		vertexs[0].position = v1;
		vertexs[0].normal = (v2 - v1) ^ osg::Y_AXIS;
		vertexs[0].normal.normalize();
		vertexs[0].uv = osg::Vec2(0.f, 0.f);

		vertexs[1].position = v2;
		vertexs[1].normal = vertexs[0].normal;
		vertexs[1].uv = osg::Vec2(0.f, (v2 - v1).length() / grith);

		vertexs[2].position = v2;
		vertexs[2].normal = osg::Vec3(0.f, 0.f, -1.f);
		vertexs[2].uv = vertexs[1].uv;

		vertexs[3].position = v3;
		vertexs[3].normal = osg::Vec3(0.f, 0.f, -1.f);
		vertexs[3].uv = osg::Vec2(0.f, ((v2 - v1).length() + _bottomWidth) / grith);

		vertexs[4].position = v3;
		vertexs[4].normal = (v4 - v3) ^ osg::Y_AXIS;
		vertexs[4].normal.normalize();
		vertexs[4].uv = vertexs[3].uv;

		vertexs[5].position = v4;
		vertexs[5].normal = vertexs[4].normal;
		vertexs[5].uv = osg::Vec2(0.f, ((v2 - v1).length() + _bottomWidth + (v4 - v3).length()) / grith);

		vertexs[6].position = v4;
		vertexs[6].normal = osg::Vec3(0.f, 0.f, 1.f);
		vertexs[6].uv = vertexs[5].uv;

		vertexs[7].position = v1;
		vertexs[7].normal = osg::Vec3(0.f, 0.f, 1.f);
		vertexs[7].uv = osg::Vec2(0.f, 1.f);

		topVertexs.resize(3);
		topVertexs[0].position = v1;
		topVertexs[1].position = (v1 + v4) * 0.5;
		topVertexs[2].position = v4;

		bodyVertexs.resize(5);
		bodyVertexs[0].position = v1;
		bodyVertexs[1].position = v2;
		bodyVertexs[2].position = (v2 + v3) * 0.5f;
		bodyVertexs[3].position = v3;
		bodyVertexs[4].position = v4;

		return true;
	}

	float SectionLadder::getWidth()
	{
		return std::max(_topWidth, _bottomWidth);
	}

	SectionBase* SectionCreator::createStandardSection(const SectionDesc& desc)
	{
		auto itr = std::find_if(_sectionCache.begin(), _sectionCache.end(), [&desc](const SectionCache& c)->bool{
			return desc.type == c.desc.type && desc.params == c.desc.params;
		});

		if (itr == _sectionCache.end())
		{
			std::shared_ptr<SectionBase> section  = nullptr;
			switch (desc.type)
			{
			case SectionType::Circle:
				section = std::make_shared<SectionCircle>();
				break;
			case SectionType::Rect:
				section = std::make_shared<SectionRect>();
				break;
			case SectionType::Arc:
				section = std::make_shared<SectionArc>();
				break;
			case SectionType::Ladder:
				section = std::make_shared<SectionLadder>();
				break;
			default:
				break;
			}

			if (section)
			{
				section->build(desc);
				_sectionCache.emplace_back(SectionCache());
				_sectionCache.back().desc = desc;
				_sectionCache.back().value = section;
			}
			
			return section.get();
		}
		else{
			return (*itr).value.get();
		}
	}

	std::vector<SectionCreator::SectionCache> SectionCreator::_sectionCache;

}