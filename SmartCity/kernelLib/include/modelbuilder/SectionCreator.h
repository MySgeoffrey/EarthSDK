#ifndef SectionCreator_h__
#define SectionCreator_h__

#include "MeshDefine.h"

namespace MeshGenerator
{
	class SectionBase
	{
	public:
		SectionBase(){};
		virtual ~SectionBase(){};

	public:
		const std::vector<MeshVertex>& getMeshVertexs() const{ return vertexs; }
		SectionType getSectionType() const{ return _type; }
	
		virtual bool build(const SectionDesc& desc) = 0;

		virtual float getWidth() { return 0.f; }

		virtual float getHeight() { return 0.f; }

		const std::vector<MeshVertex>& getTopMeshVertexs() const{ return topVertexs; }

		const std::vector<MeshVertex>& getBodyMeshVertexs() const{ return bodyVertexs; }

		//top点集中点附近多少个作为底部连接数据，不能大于总点数的一半
		virtual int getTopOffset() const = 0;
		//body点集中点附近多少个作为底部连接数据，不能大于总点数的一半
		virtual int getBottomOffset() const = 0;

	protected:
		SectionType _type = SectionType::UnKnown;
		std::vector<MeshVertex> vertexs;

		/*点的构建规则：
		总共分成顶盖和主体两部分，
		每部分由大于等于3的奇数个点组成，
		中间索引位置的点作为控制点，用于交叉口的底部和顶部封口
		点的顺序全部按从右到左排序*/

		//	|-------------------|
		//	|					| 顶
		// _|___________________|__
		//	|					|
		//	|---------O---------| 主体

		//top点集，点序全部从右到左
		std::vector<MeshVertex> topVertexs;
		//body点集，点序全部从右到左
		std::vector<MeshVertex> bodyVertexs;

	};

	class SectionCircle:public SectionBase
	{
	public:
		SectionCircle(){ _type = SectionType::Circle; }
		~SectionCircle(){};

	public:
		virtual bool build(const SectionDesc& desc) override;
		virtual float getWidth() override { return _radius * 2.f; }
		virtual float getHeight() override { return _radius * 2.f; }
		virtual int getTopOffset() const override  { return 0; }
		virtual int getBottomOffset() const override  { return 0; }
	private:
		float _radius;
	};

	class SectionRect :public SectionBase
	{
	public:
		SectionRect(){ _type = SectionType::Rect; }
		~SectionRect(){};

	public:
		virtual bool build(const SectionDesc& desc) override;
		virtual float getWidth() override { return _width; }
		virtual float getHeight() override { return _height; }
		virtual int getTopOffset() const override  { return 0; }
		virtual int getBottomOffset() const override  { return 1; }
	private:
		float _width;
		float _height;
	};

	class SectionLadder :public SectionBase
	{
	public:
		SectionLadder(){ _type = SectionType::Ladder; }
		~SectionLadder(){};

	public:
		virtual bool build(const SectionDesc& desc) override;;
		virtual float getWidth() override;
		virtual float getHeight() override { return _height; }
		virtual int getTopOffset() const override  { return 0; }
		virtual int getBottomOffset() const override  { return 1; }
	private:
		float _topWidth;
		float _bottomWidth;
		float _height;
		float _angle;
	};

	class SectionArc :public SectionBase
	{
	public:
		SectionArc(){ _type = SectionType::Arc; }
		~SectionArc(){};

	public:
		virtual bool build(const SectionDesc& desc) override;
		virtual float getWidth() override { return _width; }
		virtual float getHeight() override { return _height + _arcHeight; }
		virtual int getTopOffset() const override  { return 0; }
		virtual int getBottomOffset() const override  { return 1; }
	private:
		float _width;
		float _height;
		float _arcHeight;
	};

	class SectionCreator
	{
	public:
		SectionCreator() = delete;
		~SectionCreator() = delete;

	public:
		static SectionBase* createStandardSection(const SectionDesc& desc);

	private:
		struct SectionCache
		{
			SectionDesc desc;
			std::shared_ptr<SectionBase> value = nullptr;
		};

		static std::vector<SectionCache> _sectionCache;
	};
}

#endif // SectionCreator_h__
