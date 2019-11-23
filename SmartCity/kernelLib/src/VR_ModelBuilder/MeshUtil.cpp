#include "modelbuilder/MeshUtil.h"
#include "osg\Geode"
#include "osg\Geometry"
#include "osg\Texture2D"
#include "osgDB\ReadFile"
#include "osg\PrimitiveSet"
#include "modelbuilder/SectionCreator.h"
#include "osg\MatrixTransform"
#include "osgEarth\GeoTransform"
#include "osg\PolygonMode"
#include "osg\Material"

namespace MeshGenerator
{
	static osg::Matrix generateRotateMatrix(const osg::Vec3& dir, const osg::Vec3& up)
	{
		auto dirN = dir;
		dirN.normalize();

		auto newUp = up;
		auto t = dirN * up;
		if (t > 0.997f || t < -0.997f)
		{
			newUp = osg::X_AXIS;
		}

		auto right = dirN ^ newUp;
		right.normalize();

		auto realUp = right ^ dir;
		realUp.normalize();

		osg::Matrix mat;
		mat.makeRotate(osg::Y_AXIS, dirN);
		auto v = newUp * mat;
		v.normalize();

		osg::Matrix mat2;
		mat2.makeRotate(v, realUp);
		return mat * mat2;
	}

	static void TransformMeshVertex(const MeshVertex& vertex, const osg::Matrix& transfrom, MeshVertex& out)
	{
		out.position = vertex.position * transfrom;
		out.normal = osg::Matrix::transform3x3(vertex.normal, transfrom);
		out.normal.normalize();
		out.uv = vertex.uv;
	}

	static void generateIndices(int rows, int columns, std::vector<unsigned int>& indeces)
	{
		indeces.resize(rows * columns * 6u);
		for (int row = 0u; row < rows; row++)
		{
			for (int col = 0u; col < columns; col++)
			{
				indeces[(row * columns + col) * 6 + 0] = row * (columns + 1) + col;
				indeces[(row * columns + col) * 6 + 1] = (row + 1) * (columns + 1) + col;
				indeces[(row * columns + col) * 6 + 2] = row * (columns + 1) + 1 + col;

				indeces[(row * columns + col) * 6 + 3] = row * (columns + 1) + 1 + col;
				indeces[(row * columns + col) * 6 + 4] = (row + 1) * (columns + 1) + col;
				indeces[(row * columns + col) * 6 + 5] = (row + 1) * (columns + 1) + 1 + col;
			}
		}
	}

	static void calcuteNormals(MeshData* mesh, bool cw /* = true */)
	{
		if (!mesh)
		{
			return;
		}
		if (!mesh->vertexs.empty())
		{
			for (auto& v : mesh->vertexs)
			{
				v.normal = osg::Vec3(0.f, 0.f, 0.f);
			}

			for (size_t i = 0; i < mesh->subMeshs.size(); ++i)
			{
				uint32_t triCount = mesh->subMeshs[i].indices.size() / 3u;
				for (uint32_t j = 0u; j < triCount; ++j)
				{
					const auto& v1 = mesh->vertexs[mesh->subMeshs[i].indices[j * 3]].position;
					const auto& v2 = mesh->vertexs[mesh->subMeshs[i].indices[j * 3 + 1]].position;
					const auto& v3 = mesh->vertexs[mesh->subMeshs[i].indices[j * 3 + 2]].position;

					osg::Vec3 triNormal = (v2 - v1) ^ (v3 - v2);
					triNormal.normalize();
					if (!cw)
					{
						triNormal *= -1;
					}

					for (uint32_t k = 0u; k < 3u; ++k)
					{
						mesh->vertexs[mesh->subMeshs[i].indices[j * 3 + k]].normal += triNormal;
						mesh->vertexs[mesh->subMeshs[i].indices[j * 3 + k]].normal.normalize();
					}
				}
			}
		}
	}

	MeshUtil* MeshUtil::getInstance()
	{
		static MeshUtil g;
		return &g;
	}

	osg::Node* MeshUtil::createGeodeFromMeshData(const std::vector<MeshData*>& meshDatas,
		const osg::Vec4& color, const std::string& imgPath)
	{
		osg::ref_ptr<osg::Image> img = osgDB::readImageFile(imgPath);
		return createGeodeFromMeshData(meshDatas, color, img);
	}

	osg::Node* MeshUtil::createGeodeFromMeshData(const std::vector<MeshData*>& meshDatas, const osg::Vec4& color /*= osg::Vec4(1.f, 1.f, 1.f, 1.f)*/, osg::ref_ptr<osg::Image> image /*= nullptr*/)
	{
		osg::ref_ptr<osg::Geode> root = new osg::Geode;
		osg::ref_ptr<osg::Vec3Array> pts = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec2Array> uvs = new osg::Vec2Array;
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;

		for (auto mesh : meshDatas)
		{
			if (mesh)
			{
				for (const auto& v : mesh->vertexs)
				{
					pts->push_back(v.position);
					uvs->push_back(v.uv);
					normals->push_back(v.normal);
				}
			}
		}

		osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
		gm->setVertexArray(pts.get());
		gm->setNormalArray(normals.get());
		gm->setNormalBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);

		osg::Material *material = new osg::Material;
		if (image)
		{
			gm->setTexCoordArray(0, uvs.get());
			osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(image);
			texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
			texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
			texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			gm->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
		}
		else
		{
			material->setDiffuse(osg::Material::FRONT, color);
		}
		gm->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

		material->setShininess(osg::Material::FRONT, 90.0);
		gm->getOrCreateStateSet()->setAttribute(material);

		int vertexOffset = 0;
		for (auto mesh : meshDatas)
		{
			if (mesh)
			{
				for (const auto& subMesh : mesh->subMeshs)
				{
					osg::ref_ptr<osg::DrawElementsUInt> elements =
						new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, subMesh.indices.size());

					for (size_t i = 0; i < subMesh.indices.size(); ++i)
					{
						(*elements)[i] = vertexOffset + subMesh.indices[i];
					}

					gm->addPrimitiveSet(elements.get());
				}
				vertexOffset += mesh->vertexs.size();
			}
		}

		root->addDrawable(gm);

		return root.release();
	}

	MeshData* MeshUtil::createLoft(const osg::Vec3& start, const osg::Vec3& end, const SectionDesc& desc)
	{
		MeshData* mesh = nullptr;
		auto section = SectionCreator::createStandardSection(desc);
		if (section)
		{
			auto dir = end - start;
			auto rotate = generateRotateMatrix(dir, osg::Z_AXIS);
			auto length = dir.length();
			mesh = new MeshData;
			const auto& pts = section->getMeshVertexs();
			mesh->vertexs.reserve(pts.size() * 2.f);
			for (const auto& pt : pts)
			{
				mesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(pt, rotate * osg::Matrix::translate(start), mesh->vertexs.back());
			}

			for (const auto& pt : pts)
			{
				mesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(pt, rotate * osg::Matrix::translate(end), mesh->vertexs.back());
				mesh->vertexs.back().uv.x() = length / _textureRepeatLength;
			}

			mesh->subMeshs.emplace_back(SubMeshData());
			generateIndices(1, pts.size() - 1, mesh->subMeshs.back().indices);
		}
		return mesh;
	}

	MeshData* MeshUtil::createLoft(const std::vector<osg::Vec3>& pts, const SectionDesc& desc, bool isColsed)
	{
		if (pts.size() < 2)
		{
			return nullptr;
		}

		if (isColsed && pts.size() < 3)
		{
			return nullptr;
		}

		if (isColsed)
		{
			if ((pts[pts.size() - 1] - pts[0]).length() > 0.00001f)
			{
				std::vector<osg::Vec3> newPts;
				newPts.reserve(pts.size() + 1);
				newPts.assign(pts.begin(), pts.end());
				newPts.emplace_back(pts[0]);

				auto v1 = newPts[1] - newPts[0];
				v1.normalize();

				osg::Vec3 v2 = newPts[0] - newPts[newPts.size() - 2];
				v2.normalize();

				auto front = v1 + v2;
				return createLoft(newPts, desc, front, front);
			}
			else
			{
				auto v1 = pts[1] - pts[0];
				v1.normalize();

				osg::Vec3 v2 = pts[0] - pts[pts.size() - 2];
				v2.normalize();

				auto front = v1 + v2;
				return createLoft(pts, desc, front, front);
			}
		}
		else
		{
			auto startFront = pts[1] - pts[0];
			auto endFront = pts[pts.size() - 1] - pts[pts.size() - 2];
			return createLoft(pts, desc, startFront, endFront);
		}
	}

	MeshData* MeshUtil::createLoft(const std::vector<osg::Vec3>& pts, const SectionDesc& desc,
		const osg::Vec3& startFront, const osg::Vec3& endFront)
	{
		if (pts.size() < 2)
		{
			return nullptr;
		}

		MeshData* mesh = nullptr;
		auto section = SectionCreator::createStandardSection(desc);
		if (section)
		{
			mesh = new MeshData;
			const auto& vertexs = section->getMeshVertexs();

			int count = pts.size();
			mesh->vertexs.reserve(vertexs.size() * count);

			float totalLength = 0.f;
			for (int i = 0; i < count; ++i)
			{
				if (i == 0)
				{
					auto rotate = generateRotateMatrix(startFront, osg::Z_AXIS);
					const auto& m = rotate * osg::Matrix::translate(pts[0]);
					for (const auto& v : vertexs)
					{
						mesh->vertexs.emplace_back(MeshVertex());
						TransformMeshVertex(v, m, mesh->vertexs.back());
					}
				}
				else if (i == count - 1)
				{
					totalLength += (pts[i] - pts[i - 1]).length();
					auto rotate = generateRotateMatrix(endFront, osg::Z_AXIS);

					const auto& m = rotate  * osg::Matrix::translate(pts[i]);
					for (const auto& v : vertexs)
					{
						mesh->vertexs.emplace_back(MeshVertex());
						TransformMeshVertex(v, m, mesh->vertexs.back());
						mesh->vertexs.back().uv.x() = totalLength / _textureRepeatLength;
					}
				}
				else
				{
					osg::Vec3 v1 = pts[i + 1] - pts[i];
					v1.normalize();

					osg::Vec3 v2 = pts[i] - pts[i - 1];
					totalLength += v2.length();
					v2.normalize();

					osg::Vec3 midDir = v1 + v2;
					midDir.normalize();

					auto rotate = generateRotateMatrix(midDir, osg::Z_AXIS);

					const auto& m = rotate * osg::Matrix::translate(pts[i]);
					for (const auto& v : vertexs)
					{
						mesh->vertexs.emplace_back(MeshVertex());
						TransformMeshVertex(v, m, mesh->vertexs.back());
						mesh->vertexs.back().uv.x() = totalLength / _textureRepeatLength;
					}
				}
			}

			SubMeshData subMesh;
			generateIndices(count - 1, vertexs.size() - 1, subMesh.indices);
			mesh->subMeshs.emplace_back(subMesh);
		}
		return mesh;
	}

	MeshData* MeshUtil::createLoft(const std::vector<osg::Vec3>& pts, const std::vector<osg::Vec3>& outline, const osg::Vec3& frontDir /*= osg::Y_AXIS*/, bool isColsed /*= false*/)
	{
		// to do

		return nullptr;
	}

	MeshData* MeshUtil::createJointCircle(const JointData& joint, bool withHat /* = true */)
	{
		auto count = joint.adjacents.size();
		if (count < 2)
		{
			return nullptr;
		}

		if (count == 2)
		{
			//B(t) = (1 - t) ^ 2 * p0 + 2 * (1 - t) * t * p1 + t ^ 2 * p2
			std::vector<osg::Vec3> pts;
			const auto& p0 = joint.adjacents[0].pos;
			const auto& p1 = joint.pos;
			const auto& p2 = joint.adjacents[1].pos;

			auto dir = p1 - p0;
			dir.normalize();
			auto dir2 = p2 - p1;
			dir2.normalize();

			float angle = acos(dir * dir2);
			float per = (1.f * _blendParam + 10.f * (1.f - _blendParam)) * osg::PI / 180.f;
			int count = angle / per;
			count = std::max(2, count);
			float countI = 1.f / count;
			for (int i = 0; i <= count; ++i)
			{
				float t = i * countI;
				pts.emplace_back(osg::Vec3());
				pts.back() = p0 * (1.f - t) * (1.f - t) + p1 * 2.f * (1.f - t) * t + p2 * t * t;
			}

			std::vector<MeshData*> tempMesh;

			auto mesh = createLoft(pts, joint.adjacents[0].sectionDesc, dir, dir2);
			tempMesh.emplace_back(mesh);

			if (withHat)
			{
				auto outlineMesh = createJointHat(joint.adjacents[0].sectionDesc, joint.adjacents[0].pos, joint.adjacents[0].pos - joint.pos);
				tempMesh.emplace_back(outlineMesh);

				auto outlineMesh2 = createJointHat(joint.adjacents[0].sectionDesc, joint.adjacents[1].pos, joint.adjacents[1].pos - joint.pos);
				tempMesh.emplace_back(outlineMesh2);
			}

			return mergeMeshDatas(tempMesh);
		}
		else
		{
			std::vector<MeshData*> tempMesh;

			for (size_t i = 0; i < joint.adjacents.size(); ++i)
			{
				auto m = createLoft(joint.adjacents[i].pos, joint.pos, joint.adjacents[i].sectionDesc);
				tempMesh.emplace_back(m);
				auto outline = createJointHat(joint.adjacents[i].sectionDesc, joint.adjacents[i].pos, joint.adjacents[i].pos - joint.pos);
				tempMesh.emplace_back(outline);
			}

			return mergeMeshDatas(tempMesh);
		}
	}

	MeshData* MeshUtil::createJointGeneral(const JointData& joint, bool withHat /* = true */)
	{
		JointData newJoint;
		newJoint.pos = joint.pos;
		std::vector<JointAdjacent> upAdjacents;
		for (size_t i = 0; i < joint.adjacents.size(); ++i)
		{
			auto v = joint.adjacents[i].pos - joint.pos;
			v.normalize();
			float angle = acos(v * osg::Z_AXIS);
			if (angle < osg::PI / 6.f || angle > osg::PI * 5.f / 6.f)
			{
				upAdjacents.emplace_back(joint.adjacents[i]);
			}
			else
			{
				newJoint.adjacents.emplace_back(joint.adjacents[i]);
			}
			
		}

		std::vector<MeshData*> tempMeshs;
		std::vector<osg::Matrix> transform;
		std::vector<SectionBase*> sections;

		for (size_t i = 0; i < newJoint.adjacents.size(); ++i)
		{
			auto section = SectionCreator::createStandardSection(newJoint.adjacents[i].sectionDesc);
			sections.emplace_back(section);
		}

		for (size_t i = 0; i < upAdjacents.size(); ++i)
		{
			auto v = upAdjacents[i].pos - newJoint.pos;

			auto mesh = createLoft(newJoint.pos, upAdjacents[i].pos, upAdjacents[i].sectionDesc);
			auto rotate = generateRotateMatrix(v, osg::Z_AXIS);
			tempMeshs.emplace_back(mesh);

			if (withHat)
			{
				auto hat = createJointHat(upAdjacents[i].sectionDesc, upAdjacents[i].pos, upAdjacents[i].pos - newJoint.pos);
				tempMeshs.emplace_back(hat);
			}
		}

		for (size_t i = 0; i < newJoint.adjacents.size(); ++i)
		{
			auto preIndex = (i == 0 ? newJoint.adjacents.size() - 1 : i - 1);
			auto nextIndex = (i == newJoint.adjacents.size() - 1 ? 0 : i + 1);

			auto v = newJoint.adjacents[i].pos - newJoint.pos;
			auto preV = newJoint.adjacents[preIndex].pos - newJoint.pos;
			auto nextV = newJoint.adjacents[nextIndex].pos - newJoint.pos;

			v.normalize();
			preV.normalize();
			nextV.normalize();

			float offset1 = calcuteOffset(v, preV, newJoint.adjacents[i].sectionDesc.getWidth(), newJoint.adjacents[preIndex].sectionDesc.getWidth());
			float offset2 = calcuteOffset(v, nextV, newJoint.adjacents[i].sectionDesc.getWidth(), newJoint.adjacents[nextIndex].sectionDesc.getWidth());
			float offset = std::max(offset1, offset2);

			auto mesh = createLoft(v * offset + newJoint.pos, newJoint.adjacents[i].pos, newJoint.adjacents[i].sectionDesc);
			auto rotate = generateRotateMatrix(v, osg::Z_AXIS);
			transform.emplace_back(rotate * osg::Matrix::translate(v* offset + newJoint.pos));

			tempMeshs.emplace_back(mesh);

			if (withHat)
			{
				auto hat = createJointHat(newJoint.adjacents[i].sectionDesc, newJoint.adjacents[i].pos, newJoint.adjacents[i].pos - newJoint.pos);
				tempMeshs.emplace_back(hat);
			}
		}

		tempMeshs.emplace_back(createJointGeneralCrossBottom(newJoint, sections, transform));
		tempMeshs.emplace_back(createJointGeneralCrossTop(newJoint, sections, transform));
		tempMeshs.emplace_back(createJointGeneralCrossBodySide(newJoint, sections, transform));
		tempMeshs.emplace_back(createJointGeneralCrossTopSide(newJoint, sections, transform));

		return mergeMeshDatas(tempMeshs);
	}

	MeshData* MeshUtil::createJoint(const JointData& joint, bool withHat)
	{
		auto count = joint.adjacents.size();
		if (count < 2)
		{
			return nullptr;
		}
		auto newJoint = calcuteJointData(joint);

		if (count == 2 && joint.adjacents[0].sectionDesc == joint.adjacents[1].sectionDesc)
		{
			return createJointOnlyTwo(newJoint,withHat);
		}

	    // all sections are circle type
		if (_circleSpecialProcess)
		{
			bool flag = false;
			for (size_t i = 0; i < newJoint.adjacents.size(); ++i)
			{
				if (newJoint.adjacents[i].sectionDesc.type == SectionType::Circle)
				{
					flag = true;
					break;
				}
			}

			if (flag)
			{
				return createJointCircle(newJoint, withHat);
			}
			else
			{
				return createJointGeneral(newJoint,withHat);
			}
		}
		else{
			return createJointGeneral(newJoint);
		}		
	}

	MeshData* MeshUtil::mergeMeshDatas(const std::vector<MeshData*>& meshDatas, bool clearOldData /* = true */)
	{
		if (meshDatas.empty())
		{
			return nullptr;
		}

		auto result = new MeshData;
		for (auto mesh : meshDatas)
		{
			if (mesh)
			{
				for (const auto& v : mesh->vertexs)
				{
					result->vertexs.emplace_back(v);
				}
			}
		}

		int vertexOffset = 0;
		for (auto mesh : meshDatas)
		{
			if (mesh)
			{
				for (const auto& subMesh : mesh->subMeshs)
				{
					result->subMeshs.emplace_back(SubMeshData());
					for (size_t i = 0; i < subMesh.indices.size(); ++i)
					{
						result->subMeshs.back().indices.emplace_back(vertexOffset + subMesh.indices[i]);
					}
				}
				vertexOffset += mesh->vertexs.size();
			}
		}

		if (clearOldData)
		{
			for (auto m : meshDatas)
			{
				if (m)
				{
					delete m;
					m = nullptr;
				}
			}
		}
		return result;
	}

	MeshData* MeshUtil::createJointHat(const SectionDesc& desc, const osg::Vec3& pos, const osg::Vec3& dir)
	{
		auto mesh = new MeshData;
		auto section = SectionCreator::createStandardSection(desc.extent(-_hatThickness * 0.5f));
		auto sectionExtend = SectionCreator::createStandardSection(desc.extent(_hatThickness * 0.5f));
		float width = _hatLength;
		if (section && sectionExtend)
		{
			float dis = (sectionExtend->getMeshVertexs()[0].position - section->getMeshVertexs()[0].position).length();
			float totalLength = 0.f;
			osg::Vec3 dir2 = dir;
			dir2.normalize();
			auto rotate = generateRotateMatrix(dir2, osg::Z_AXIS);

			auto count = section->getMeshVertexs().size();

			for (auto& v : section->getMeshVertexs())
			{
				mesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(v, rotate * osg::Matrix::translate(pos), mesh->vertexs.back());
			}

			totalLength += dis;
			for (auto& v : sectionExtend->getMeshVertexs())
			{
				mesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(v, rotate * osg::Matrix::translate(pos), mesh->vertexs.back());
				mesh->vertexs.back().uv.x() = totalLength / _textureRepeatLength;
			}

			int index = mesh->vertexs.size() - count;
			for (int i = 0; i < count; ++i)
			{
				mesh->vertexs.emplace_back(MeshVertex(mesh->vertexs[i + index]));
			}

			totalLength += width;
			for (auto& v : sectionExtend->getMeshVertexs())
			{
				mesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(v, rotate * osg::Matrix::translate(pos + dir2 *width), mesh->vertexs.back());
				mesh->vertexs.back().uv.x() = totalLength / _textureRepeatLength;
			}

			index = mesh->vertexs.size() - count;
			for (int i = 0; i < count; ++i)
			{
				mesh->vertexs.emplace_back(MeshVertex(mesh->vertexs[i + index]));
			}

			totalLength += dis;
			for (auto& v : section->getMeshVertexs())
			{
				mesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(v, rotate * osg::Matrix::translate(pos + dir2 *width), mesh->vertexs.back());
				mesh->vertexs.back().uv.x() = totalLength / _textureRepeatLength;
			}

			index = mesh->vertexs.size() - count;
			for (int i = 0; i < count; ++i)
			{
				mesh->vertexs.emplace_back(MeshVertex(mesh->vertexs[i + index]));
			}

			totalLength += width;
			for (auto& v : section->getMeshVertexs())
			{
				mesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(v, rotate * osg::Matrix::translate(pos), mesh->vertexs.back());
				mesh->vertexs.back().uv.x() = totalLength / _textureRepeatLength;
			}

			mesh->subMeshs.emplace_back(SubMeshData());
			generateIndices(7, section->getMeshVertexs().size() - 1, mesh->subMeshs.back().indices);
			calcuteNormals(mesh, false);
			return mesh;
		}
		return nullptr;
	}

	MeshData* MeshUtil::createLoftLonLat(const osg::Vec3d& start, const osg::Vec3d& end, const SectionDesc& desc)
	{
		osg::Vec3d localStart, localEnd;
		convertToLocalFromLonLat(start, localStart);
		convertToLocalFromLonLat(end, localEnd);
		return createLoft(localStart, localEnd, desc);
	}

	bool MeshUtil::convertToLocalFromLonLat(const osg::Vec3d& lla, osg::Vec3d& local)
	{
		osgEarth::GeoPoint geoLLa(_referencePos.getSRS(),
			lla.x(), lla.y(), lla.z(), osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);

		osg::Vec3d w;
		bool result = geoLLa.toWorld(w);
		if (result)
		{
			local = w * _w2l;
		}

		return result;
	}

	void MeshUtil::setReferenceCenter(const osgEarth::GeoPoint& center)
	{
		_referencePos = center;
		_referencePos.createWorldToLocal(_w2l);
	}

	MeshData* MeshUtil::createJointLonLat(const JointData& joint, bool withHat /* = true */)
	{
		JointData newJoint = joint;
		convertToLocalFromLonLat(joint.pos, newJoint.pos);
		for (size_t i = 0; i < joint.adjacents.size(); ++i)
		{
			convertToLocalFromLonLat(joint.adjacents[i].pos, newJoint.adjacents[i].pos);
		}
		return createJoint(newJoint,withHat);
	}

	MeshData* MeshUtil::createJointGeneralCrossBottom(const JointData& joint, const std::vector<SectionBase*>& sections, const std::vector<osg::Matrix>& transform)
	{
		MeshData* bottomMesh = new MeshData;

		for (size_t i = 0; i < joint.adjacents.size(); ++i)
		{
			auto section = sections[i];
			int ptOffset = section->getBottomOffset();
			for (int j = 0; j < (2 * ptOffset + 1); ++j)
			{
				bottomMesh->vertexs.emplace_back(MeshVertex());
				TransformMeshVertex(section->getBodyMeshVertexs()[ptOffset - j + section->getBodyMeshVertexs().size() / 2], transform[i], bottomMesh->vertexs.back());
			}
		}

		osg::Vec3 center(0.f, 0.f, 0.f);
		for (auto v : bottomMesh->vertexs)
		{
			center += v.position;
		}
		center /= bottomMesh->vertexs.size();
		
		bottomMesh->vertexs.emplace_back(MeshVertex());
		bottomMesh->vertexs.back().position = center;

		for (auto& v : bottomMesh->vertexs)
		{
			v.uv.x() = (v.position - joint.pos).x() / _textureRepeatLength;
			v.uv.y() = (v.position - joint.pos).y() / _textureRepeatLength;
		}

		bottomMesh->subMeshs.emplace_back(SubMeshData());
		for (int i = 0; i < bottomMesh->vertexs.size() - 1; ++i)
		{
			if (i == bottomMesh->vertexs.size() - 2)
			{
				bottomMesh->subMeshs.back().indices.emplace_back(bottomMesh->vertexs.size() - 1);
				bottomMesh->subMeshs.back().indices.emplace_back(0);
				bottomMesh->subMeshs.back().indices.emplace_back(bottomMesh->vertexs.size() - 2);
			}
			else
			{
				bottomMesh->subMeshs.back().indices.emplace_back(bottomMesh->vertexs.size() - 1);
				bottomMesh->subMeshs.back().indices.emplace_back(i + 1);
				bottomMesh->subMeshs.back().indices.emplace_back(i);
			}
		}
		calcuteNormals(bottomMesh, true);
		return bottomMesh;
	}

	MeshData* MeshUtil::createJointGeneralCrossTop(const JointData& joint, const std::vector<SectionBase*>& sections, const std::vector<osg::Matrix>& transform)
	{
		MeshData* topMesh = new MeshData;
		for (size_t i = 0; i < joint.adjacents.size(); ++i)
		{
			auto section = sections[i];
			topMesh->vertexs.emplace_back(MeshVertex());
			TransformMeshVertex(section->getTopMeshVertexs()[section->getTopMeshVertexs().size() / 2],
				transform[i], topMesh->vertexs.back());
		}
		osg::Vec3 center(0.f, 0.f, 0.f);
		for (auto v : topMesh->vertexs)
		{
			center += v.position;
		}
		center /= topMesh->vertexs.size();
		topMesh->vertexs.emplace_back(MeshVertex());
		topMesh->vertexs.back().position = center;

		for (auto& v : topMesh->vertexs)
		{
			v.uv.x() = (v.position - joint.pos).x() / _textureRepeatLength;
			v.uv.y() = (v.position - joint.pos).y() / _textureRepeatLength;
		}

		topMesh->subMeshs.emplace_back(SubMeshData());
		for (int i = 0; i < topMesh->vertexs.size() - 1; ++i)
		{
			if (i == topMesh->vertexs.size() - 2)
			{
				topMesh->subMeshs.back().indices.emplace_back(topMesh->vertexs.size() - 1);
				topMesh->subMeshs.back().indices.emplace_back(0);
				topMesh->subMeshs.back().indices.emplace_back(topMesh->vertexs.size() - 2);
			}
			else
			{
				topMesh->subMeshs.back().indices.emplace_back(topMesh->vertexs.size() - 1);
				topMesh->subMeshs.back().indices.emplace_back(i + 1);
				topMesh->subMeshs.back().indices.emplace_back(i);
			}

		}
		calcuteNormals(topMesh, true);
		return topMesh;
	}

	void destinyPts(std::vector<MeshVertex>& pts, int des_pt_count)
	{
		auto count = pts.size();
		if (count <= 1 || count >= des_pt_count)
		{
			return;
		}

		int per = (des_pt_count - pts.size()) / (pts.size() - 1);  // 每段平均添加的点数
		int remainder = (des_pt_count - pts.size()) % (pts.size() - 1);  // 不能平均分配的余数，算在第一段插入

		std::vector<MeshVertex> ptDestiny;
		ptDestiny.assign(pts.begin(), pts.end());
		pts.clear();

		pts.emplace_back(ptDestiny[0]);

		for (int i = 1; i < ptDestiny.size(); ++i)
		{
			int nSegment = per;
			if (i == 1)
			{
				nSegment += remainder;
			}

			osg::Vec3& v0 = ptDestiny[i - 1].position;
			osg::Vec3& v1 = ptDestiny[i].position;

			osg::Vec3 vNormal = v1 - v0;
			float totoalLength = vNormal.length();
			vNormal.normalize();

			for (int j = 1; j <= nSegment; ++j)
			{
				pts.emplace_back(MeshVertex());
				pts.back().position = v0 + vNormal*(j * totoalLength / (nSegment + 1));
			}
			pts.emplace_back(MeshVertex());
			pts.back().position = v1;
		}
	}

	MeshData* MeshUtil::createJointGeneralCrossBodySide(const JointData& joint, const std::vector<SectionBase*>& sections, const std::vector<osg::Matrix>& transform)
	{
		std::vector<MeshData*> tempMeshs;
		for (size_t i = 0; i < joint.adjacents.size(); ++i)
		{
			MeshData* bodySideMesh = new MeshData;
			//相邻两个侧边的 一侧顶点的最大个数
			int nBodyVerticesMax = 0;

			auto section = sections[i];
			std::vector<MeshVertex> vertexs;
			vertexs.insert(vertexs.end(), section->getBodyMeshVertexs().begin(),
				section->getBodyMeshVertexs().begin() + section->getBodyMeshVertexs().size() / 2 + 1 - section->getBottomOffset());

			for (auto& v : vertexs)
			{
				TransformMeshVertex(v, transform[i], v);
			}

			auto section2 = (i == joint.adjacents.size() - 1 ? sections[0] : sections[i + 1]);
			std::vector<MeshVertex> vertexs2;
			vertexs2.insert(vertexs2.end(),
				section2->getBodyMeshVertexs().begin() + section2->getBodyMeshVertexs().size() / 2 + section2->getBottomOffset(),
				section2->getBodyMeshVertexs().end());

			for (auto& v : vertexs2)
			{
				TransformMeshVertex(v, transform[(i == joint.adjacents.size() - 1 ? 0 : i + 1)], v);
			}

			nBodyVerticesMax = std::max(vertexs.size(), vertexs2.size());

			std::vector<float> destinyTex;
			if (nBodyVerticesMax > vertexs.size())
			{
				destinyPts(vertexs, nBodyVerticesMax);
			}

			if (nBodyVerticesMax > vertexs2.size())
			{
				destinyPts(vertexs2, nBodyVerticesMax);
			}

			bodySideMesh->vertexs.insert(bodySideMesh->vertexs.end(), vertexs.begin(), vertexs.end());
			std::reverse(vertexs2.begin(), vertexs2.end());
			bodySideMesh->vertexs.insert(bodySideMesh->vertexs.end(), vertexs2.begin(), vertexs2.end());

			bodySideMesh->vertexs[0].uv.x() = 0.f;
			bodySideMesh->vertexs[0].uv.y() = 0.f;

			auto count = bodySideMesh->vertexs.size() / 2;
			bodySideMesh->vertexs[count].uv.x() = 0.f;
			bodySideMesh->vertexs[count].uv.y() = (bodySideMesh->vertexs[count].position -
				bodySideMesh->vertexs[0].position).length() / _textureRepeatLength;

			for (size_t i = 1; i < count; ++i)
			{
				bodySideMesh->vertexs[i].uv.x() = (bodySideMesh->vertexs[i].position - bodySideMesh->vertexs[i - 1].position).length()/_textureRepeatLength;
				bodySideMesh->vertexs[i].uv.y() = 0.f;

				bodySideMesh->vertexs[i + count].uv.x() = bodySideMesh->vertexs[i].uv.x();
				bodySideMesh->vertexs[i + count].uv.y() = (bodySideMesh->vertexs[i + count].position - bodySideMesh->vertexs[i].position).length() / _textureRepeatLength;
			}

			bodySideMesh->subMeshs.emplace_back(SubMeshData());
			generateIndices(1, vertexs.size() - 1, bodySideMesh->subMeshs.back().indices);
			tempMeshs.emplace_back(bodySideMesh);
		}
		auto mesh = mergeMeshDatas(tempMeshs);
		calcuteNormals(mesh, true);
		return mesh;
	}

	MeshData* MeshUtil::createJointGeneralCrossTopSide(const JointData& joint, const std::vector<SectionBase*>& sections, const std::vector<osg::Matrix>& transform)
	{
		std::vector<MeshData*> tempMeshs;
		for (size_t i = 0; i < joint.adjacents.size(); ++i)
		{
			MeshData* topSideMesh = new MeshData;
			//相邻两个侧边的 一侧顶点的最大个数
			int nTopVerticesMax = 0;

			auto section = sections[i];
			std::vector<MeshVertex> vertexs;
			vertexs.insert(vertexs.end(), section->getTopMeshVertexs().begin(),
				section->getTopMeshVertexs().begin() + section->getTopMeshVertexs().size() / 2 + 1);

			for (auto& v : vertexs)
			{
				TransformMeshVertex(v, transform[i], v);
			}

			auto section2 = (i == joint.adjacents.size() - 1 ? sections[0] : sections[i + 1]);
			std::vector<MeshVertex> vertexs2;
			vertexs2.insert(vertexs2.end(),
				section2->getTopMeshVertexs().begin() + section2->getTopMeshVertexs().size() / 2,
				section2->getTopMeshVertexs().end());

			for (auto& v : vertexs2)
			{
				TransformMeshVertex(v, transform[(i == joint.adjacents.size() - 1 ? 0 : i + 1)], v);
			}

			nTopVerticesMax = std::max(vertexs.size(), vertexs2.size());

			std::vector<float> destinyTex;
			if (nTopVerticesMax > vertexs.size())
			{
				destinyPts(vertexs, nTopVerticesMax);
			}

			if (nTopVerticesMax > vertexs2.size())
			{
				destinyPts(vertexs2, nTopVerticesMax);
			}

			topSideMesh->vertexs.insert(topSideMesh->vertexs.end(), vertexs.begin(), vertexs.end());
			std::reverse(vertexs2.begin(), vertexs2.end());
			topSideMesh->vertexs.insert(topSideMesh->vertexs.end(), vertexs2.begin(), vertexs2.end());

			for (auto& v : topSideMesh->vertexs)
			{
				v.uv.x() = (v.position - joint.pos).x() / _textureRepeatLength;
				v.uv.y() = (v.position - joint.pos).y() / _textureRepeatLength;
			}

			topSideMesh->subMeshs.emplace_back(SubMeshData());
			generateIndices(1, vertexs.size() - 1, topSideMesh->subMeshs.back().indices);

			tempMeshs.emplace_back(topSideMesh);
		}
		auto mesh = mergeMeshDatas(tempMeshs);
		calcuteNormals(mesh, false);
		return mesh;
	}

	float MeshUtil::calcuteOffset(const osg::Vec3& v, const osg::Vec3& refV, float widthV, float widthRefV)
	{
		float offset = 0.f;
		float angle = acos(v * refV);
		float gap = 0.001f;
		if ((angle - osg::PI) > -gap && (angle - osg::PI) < gap)
		{
			offset = 0.5f * widthV;
		}
		if ((angle - osg::PI_2) > -gap && (angle - osg::PI_2) < gap)
		{
			offset = 0.5f * widthRefV;
		}
		if ((angle - osg::PI_2) < -gap)
		{
			//offset = ((0.5f * widthV / sin(osg::PI_2 - angle)) + 0.5f * widthRefV) / sin(angle);
			offset = ((0.5f * widthRefV / sin(osg::PI_2 - angle)) + 0.5f * widthV) / tan(angle);
		}
		if ((angle - osg::PI_2) > gap)
		{
			//offset = 2.f * ((0.5f * widthRefV / sin(angle - osg::PI_2)) - 0.5f * widthV) * tan(angle - osg::PI_2);
			offset = ((0.5f * widthRefV / sin(angle - osg::PI_2)) - 0.5f * widthV) / tan(osg::PI - angle);
		}
		return offset;
	}

	MeshGenerator::JointData MeshUtil::calcuteJointData(const JointData& joint)
	{
		JointData newJoint = joint;
		if (joint.adjacents.size() < 2)
		{
			return newJoint;
		}

		clockWiseSortJoint(newJoint);

		if (newJoint.adjacents.size() == 2)
		{
			auto v1 = newJoint.adjacents[0].pos - newJoint.pos;
			float l1 = v1.length();
			v1.normalize();

			auto v2 = newJoint.adjacents[1].pos - newJoint.pos;
			float l2 = v2.length();
			v2.normalize();

			float offset1 = calcuteOffset(v1, v2, newJoint.adjacents[0].sectionDesc.getWidth(),
				newJoint.adjacents[1].sectionDesc.getWidth());
			float offset2 = calcuteOffset(v2, v1, newJoint.adjacents[1].sectionDesc.getWidth(),
				newJoint.adjacents[0].sectionDesc.getWidth());

			if (_sameOffset)
			{
				auto v = std::max(offset1, offset2);
				offset1 = v;
				offset2 = v;
			}

			newJoint.adjacents[0].pos = joint.pos + v1 * std::min((offset1 + _fixedOffset), l1);
			newJoint.adjacents[1].pos = joint.pos + v2 * std::min((offset2 + _fixedOffset), l2);
		}
		else
		{
			std::vector<osg::Vec3> newPos;
			float maxOffset = 0.f;
			for (size_t i = 0; i < newJoint.adjacents.size(); ++i)
			{
				osg::Vec3 preV, v, nextV;
				int preIndex = (i == 0 ? (newJoint.adjacents.size() - 1) : i - 1);
				preV = newJoint.adjacents[preIndex].pos - newJoint.pos;
				preV.normalize();

				v = newJoint.adjacents[i].pos - newJoint.pos;
				float l = v.length();
				v.normalize();

				int nextIndex = (i == newJoint.adjacents.size() - 1 ? 0 : i + 1);
				nextV = newJoint.adjacents[nextIndex].pos - newJoint.pos;
				nextV.normalize();

				float offset1 = calcuteOffset(v, preV, newJoint.adjacents[i].sectionDesc.getWidth(),
					newJoint.adjacents[preIndex].sectionDesc.getWidth());
				float offset2 = calcuteOffset(v, nextV, newJoint.adjacents[i].sectionDesc.getWidth(),
					newJoint.adjacents[nextIndex].sectionDesc.getWidth());

				maxOffset = std::max(maxOffset,std::max(offset1, offset2));

				if (!_sameOffset)
				{
					newPos.emplace_back(joint.pos + v * std::min(std::max(offset1 + _fixedOffset, offset2 + _fixedOffset), l));
				}			
			}

			if (_sameOffset)
			{
				for (size_t i = 0; i < newJoint.adjacents.size(); ++i)
				{
					auto v = newJoint.adjacents[i].pos - newJoint.pos;
					v.normalize();
					newPos.emplace_back(joint.pos + v * (maxOffset + _fixedOffset));
				}
			}

			for (size_t i = 0; i < joint.adjacents.size(); ++i)
			{
				newJoint.adjacents[i].pos = newPos[i];
			}
		}
		return newJoint;
	}

	void MeshUtil::clockWiseSortJoint(JointData& joint)
	{
		std::map<float, JointAdjacent, std::less<float>> temp;
		const auto& center = joint.pos;
		for (auto adj : joint.adjacents)
		{
			osg::Vec3 v = adj.pos - joint.pos;
			v.normalize();

			float angle = acos(osg::Y_AXIS * v);
			if ((osg::Y_AXIS^v)[2] > 0)
			{
				angle = 2.f * osg::PI - angle;
			}

			temp[angle] = adj;
		}

		joint.adjacents.clear();
		for (auto ite = temp.begin(); ite != temp.end(); ++ite)
		{
			joint.adjacents.emplace_back(ite->second);
		}
	}

	MeshData* MeshUtil::createPipeSegmentLonLat(const JointData& first, const JointData& second)
	{
		JointData newJoint1 = first;
		convertToLocalFromLonLat(first.pos, newJoint1.pos);
		for (size_t i = 0; i < first.adjacents.size(); ++i)
		{
			convertToLocalFromLonLat(first.adjacents[i].pos, newJoint1.adjacents[i].pos);
		}

		JointData newJoint2 = second;
		convertToLocalFromLonLat(second.pos, newJoint2.pos);
		for (size_t i = 0; i < second.adjacents.size(); ++i)
		{
			convertToLocalFromLonLat(second.adjacents[i].pos, newJoint2.adjacents[i].pos);
		}

		return createPipeSegment(newJoint1, newJoint2);
	}

	MeshData* MeshUtil::createPipeSegment(const JointData& first, const JointData& second)
	{
		if (first.adjacents.empty() || second.adjacents.empty())
		{
			return nullptr;
		}

		osg::Vec3 posStart;
		SectionDesc desc;
		if (first.adjacents.size() == 1)
		{
			posStart = first.pos;
			desc = first.adjacents[0].sectionDesc;
		}
		else
		{
			auto dir = second.pos - first.pos;
			dir.normalize();
			auto j1 = calcuteJointData(first);
			bool flag = false;
			for (size_t i = 0; i < j1.adjacents.size(); ++i)
			{
				auto v = j1.adjacents[i].pos - first.pos;
				v.normalize();
				auto angle = v * dir;
				if (angle - 1.0 > -0.0001f && angle - 1.0 < 0.0001f)
				{
					posStart = j1.adjacents[i].pos;
					flag = true;
					desc = j1.adjacents[i].sectionDesc;
					break;
				}
			}

			if (!flag)
			{
				return nullptr;
			}

		}

		osg::Vec3 posEnd;
		if (second.adjacents.size() <= 1)
		{
			posEnd = second.pos;
		}
		else
		{
			auto dir = first.pos - second.pos;
			dir.normalize();
			auto j2 = calcuteJointData(second);
			bool flag = false;
			for (size_t i = 0; i < j2.adjacents.size(); ++i)
			{
				auto v = j2.adjacents[i].pos - second.pos;
				v.normalize();
				auto angle = v * dir;
				if (angle - 1.0 > -0.0001f && angle - 1.0 < 0.0001f)
				{
					posEnd = j2.adjacents[i].pos;
					flag = true;
					break;
				}
			}

			if (!flag)
			{
				return nullptr;
			}
		}

		return createLoft(posStart, posEnd, desc);
	}

	MeshData* MeshUtil::createJointOnlyTwo(const JointData& joint, bool withHat /* = true */)
	{
		if (joint.adjacents.size() != 2)
		{
			return nullptr;
		}

		//B(t) = (1 - t) ^ 2 * p0 + 2 * (1 - t) * t * p1 + t ^ 2 * p2
		std::vector<osg::Vec3> pts;
		const auto& p0 = joint.adjacents[0].pos;
		const auto& p1 = joint.pos;
		const auto& p2 = joint.adjacents[1].pos;

		auto dir = p1 - p0;
		dir.normalize();
		auto dir2 = p2 - p1;
		dir2.normalize();

		float angle = acos(dir * dir2);
		float per = (1.f * _blendParam + 10.f * (1.f - _blendParam)) * osg::PI / 180.f;
		int count = angle / per;
		count = std::max(2, count);
		float countI = 1.f / count;
		for (int i = 0; i <= count; ++i)
		{
			float t = i * countI;
			pts.emplace_back(osg::Vec3());
			pts.back() = p0 * (1.f - t) * (1.f - t) + p1 * 2.f * (1.f - t) * t + p2 * t * t;
		}

		std::vector<MeshData*> tempMesh;

		auto mesh = createLoft(pts, joint.adjacents[0].sectionDesc, dir, dir2);
		tempMesh.emplace_back(mesh);

		if (withHat)
		{
			auto outlineMesh = createJointHat(joint.adjacents[0].sectionDesc, joint.adjacents[0].pos, joint.adjacents[0].pos - joint.pos);
			tempMesh.emplace_back(outlineMesh);

			auto outlineMesh2 = createJointHat(joint.adjacents[0].sectionDesc, joint.adjacents[1].pos, joint.adjacents[1].pos - joint.pos);
			tempMesh.emplace_back(outlineMesh2);
		}
		
		return mergeMeshDatas(tempMesh);
	}

	void MeshUtil::setTextureRepeatLength(float l)
	{
		_textureRepeatLength = std::max(0.f, l);
	}

	void MeshUtil::setBlendParam(float p)
	{
		_blendParam = std::max(0.f, std::min(1.f, p));
	}

	void MeshUtil::setHatParam(float length, float thickness)
	{
		_hatLength = length;
		_hatThickness = thickness;
	}

	void MeshUtil::setReferenceCenterXYZ(const osg::Vec3& xyz)
	{
		_referenceXYZ = xyz;
	}

	MeshData* MeshUtil::createJointXYZ(const JointData& joint, bool withHat /*= true*/)
	{
		JointData newJoint = joint;
		convertToLocalFromXYZ(joint.pos, newJoint.pos);
		for (size_t i = 0; i < joint.adjacents.size(); ++i)
		{
			convertToLocalFromXYZ(joint.adjacents[i].pos, newJoint.adjacents[i].pos);
		}
		return createJoint(newJoint, withHat);
	}

	MeshData* MeshUtil::createPipeSegmentXYZ(const JointData& first, const JointData& second)
	{
		JointData newJoint1 = first;
		convertToLocalFromXYZ(first.pos, newJoint1.pos);
		for (size_t i = 0; i < first.adjacents.size(); ++i)
		{
			convertToLocalFromXYZ(first.adjacents[i].pos, newJoint1.adjacents[i].pos);
		}

		JointData newJoint2 = second;
		convertToLocalFromXYZ(second.pos, newJoint2.pos);
		for (size_t i = 0; i < second.adjacents.size(); ++i)
		{
			convertToLocalFromXYZ(second.adjacents[i].pos, newJoint2.adjacents[i].pos);
		}

		return createPipeSegment(newJoint1, newJoint2);
	}

	bool MeshUtil::convertToLocalFromXYZ(const osg::Vec3d& xyz, osg::Vec3d& local)
	{
		local = xyz - _referenceXYZ;
		return true;
	}

	bool MeshUtil::calcutePipeSegmentPosLonLat(const JointData& first, const JointData& second, osg::Vec3d& start, osg::Vec3d& end)
	{
		return calcutePipeSegmentPosImpl(first, second, start, end, true);
	}

	bool MeshUtil::convertToLonlatFromLocal(const osg::Vec3d& local, osg::Vec3d& lla)
	{
		const auto& m = osg::Matrix::inverse(_w2l);
		const auto& world = local * m;

		osgEarth::GeoPoint geoLLa;
		bool result = geoLLa.fromWorld(_referencePos.getSRS(), world);
		if (result)
		{
			lla = geoLLa.vec3d();
		}
		
		return result;
	}

	bool MeshUtil::convertToXYZFromLocal(const osg::Vec3d& local, osg::Vec3d& xyz)
	{
		xyz = local + _referenceXYZ;
		return true;
	}

	bool MeshUtil::calcutePipeSegmentPosXYZ(const JointData& first, const JointData& second, osg::Vec3d& start, osg::Vec3d& end)
	{
		return calcutePipeSegmentPosImpl(first, second, start, end, false);
	}

	bool MeshUtil::calcutePipeSegmentPosImpl(const JointData& first, const JointData& second, osg::Vec3d& start, osg::Vec3d& end, bool islla)
	{
		if (first.adjacents.empty() || second.adjacents.empty())
		{
			return false;
		}

		JointData newJoint1 = first;
		JointData newJoint2 = second;
		if (islla)
		{
			convertToLocalFromLonLat(first.pos, newJoint1.pos);
			for (size_t i = 0; i < first.adjacents.size(); ++i)
			{
				convertToLocalFromLonLat(first.adjacents[i].pos, newJoint1.adjacents[i].pos);
			}

			convertToLocalFromLonLat(second.pos, newJoint2.pos);
			for (size_t i = 0; i < second.adjacents.size(); ++i)
			{
				convertToLocalFromLonLat(second.adjacents[i].pos, newJoint2.adjacents[i].pos);
			}
		}
		else{
			convertToLocalFromXYZ(first.pos, newJoint1.pos);
			for (size_t i = 0; i < first.adjacents.size(); ++i)
			{
				convertToLocalFromXYZ(first.adjacents[i].pos, newJoint1.adjacents[i].pos);
			}

			convertToLocalFromXYZ(second.pos, newJoint2.pos);
			for (size_t i = 0; i < second.adjacents.size(); ++i)
			{
				convertToLocalFromXYZ(second.adjacents[i].pos, newJoint2.adjacents[i].pos);
			}
		}
		

		osg::Vec3 posStart;
		SectionDesc desc;
		if (first.adjacents.size() == 1)
		{
			posStart = first.pos;
			desc = first.adjacents[0].sectionDesc;
		}
		else
		{
			auto dir = second.pos - first.pos;
			dir.normalize();
			auto j1 = calcuteJointData(first);
			bool flag = false;
			for (size_t i = 0; i < j1.adjacents.size(); ++i)
			{
				auto v = j1.adjacents[i].pos - first.pos;
				v.normalize();
				auto angle = v * dir;
				if (angle - 1.0 > -0.0001f && angle - 1.0 < 0.0001f)
				{
					posStart = j1.adjacents[i].pos;
					flag = true;
					desc = j1.adjacents[i].sectionDesc;
					break;
				}
			}

			if (!flag)
			{
				return false;
			}

		}

		osg::Vec3 posEnd;
		if (second.adjacents.size() <= 1)
		{
			posEnd = second.pos;
		}
		else
		{
			auto dir = first.pos - second.pos;
			dir.normalize();
			auto j2 = calcuteJointData(second);
			bool flag = false;
			for (size_t i = 0; i < j2.adjacents.size(); ++i)
			{
				auto v = j2.adjacents[i].pos - second.pos;
				v.normalize();
				auto angle = v * dir;
				if (angle - 1.0 > -0.0001f && angle - 1.0 < 0.0001f)
				{
					posEnd = j2.adjacents[i].pos;
					flag = true;
					break;
				}
			}

			if (!flag)
			{
				return false;
			}
		}

		if (islla)
		{
			return convertToLonlatFromLocal(posStart, start) && convertToLonlatFromLocal(posEnd, end);
		}
		else{
			return convertToXYZFromLocal(posStart, start) && convertToXYZFromLocal(posEnd, end);
		}		
	}

	MeshData* MeshUtil::createRadius(osg::Vec3& pos, float radius)
	{
		return nullptr;
		//int kRow , kCol = 16;
		//for (int i = 0; i < nSegment; ++i)
		//{		
		//	float radius = radius - (nSegment / 2 - i)
		//	for (int j = 0; j < nSegment; ++j)
		//	{
		//		osg::Matrix mat;
		//		mat = osg::Matrix::rotate(osg::Quat((i * 2.f * osg::PI / nSegment), osg::Z_AXIS));
		//	}
		//}

		//float step_z = osg::PI / kRow;
		//float step_xy = 2 * osg::PI / kCol; 

		//float angle_z = 0; //起始角度
		//float angle_xy = 0;
		//int i = 0, j = 0;

		//for (i = 0; i < kRow; i++)
		//{
		//	angle_z = i * step_z;

		//	for (j = 0; j < kCol; j++)
		//	{
		//		angle_xy = j * step_xy;
		//		x[0] = radius * sin(angle_z) * cos(angle_xy);
		//		y[0] = radius * sin(angle_z) * sin(angle_xy);
		//		z[0] = radius * cos(angle_z);
		//	}
		//}
	}

}