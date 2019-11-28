#ifndef MODELBUILDER_h__
#define MODELBUILDER_h__

#include "modelbuilder/MeshUtil.h"

namespace MeshGenerator
{
	/**
	* 模型构建类
	*
	*	主要是根据用户输出的参数，通过参数化建模算法
	*	构建对应的三维模型节点，如：圆形截面的连接头，管道，方形截面的连接头、管道
	*/
	class MESH_GENERATOR_EXPORT CModelCreator
	{
	public:

		/**
		* 构建圆形连接头数据
		*
		*@param in_geoPosition		:	连接头的地理坐标
		*@param in_adjacentPoints	:	邻接点集合
		*@param in_radius			:	截面半径(单位：米)
		*@param return				:	连接头数据
		*/
		JointData createCircleJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const double& in_radius);

		JointData createCircleJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const std::vector<double>& in_radiusVecs);

		/**
		* 构建方形连接头数据
		*
		*@param in_geoPosition		:	连接头的地理坐标
		*@param in_adjacentPoints	:	邻接点集合
		*@param in_width			:	宽度(单位：米)
		*@param in_height			:	高度(单位：米)
		*@param return				:	连接头数据
		*/
		JointData createRectJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const double& in_width,
			const double& in_height);

		JointData createRectJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const std::vector<double>& in_widths,
			const std::vector<double>& in_heights);

		/**
		* 构建拱形连接头数据
		*
		*@param in_geoPosition		:	连接头的地理坐标
		*@param in_adjacentPoints	:	邻接点集合
		*@param in_width			:	宽度(单位：米)
		*@param in_height			:	高度(单位：米)
		*@param in_arcHeight		:	拱高(单位：米)
		*@param return				:	连接头数据
		*/
		JointData createArcJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const double& in_width,
			const double& in_height,
			const double& in_arcHeight);


		/**
		* 构建梯形连接头数据
		*
		*@param in_geoPosition		:	连接头的地理坐标
		*@param in_adjacentPoints	:	邻接点集合
		*@param in_upEdge			:	上底长(单位：米)
		*@param in_downEdge			:	下底长(单位：米)
		*@param in_arcHeight		:	高(单位：米)
		*@param return				:	连接头数据
		*/
		JointData createLadderJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const double& in_upEdge,
			const double& in_downEdge,
			const double& in_height);

		/**
		* 根据连接头数据构建模型节点
		*
		*@param in_joint			:	连接头数据
		*@param in_color			:	颜色
		*@param in_texturePath		:	纹理路径
		*@param return				:	模型节点
		*/
		osg::ref_ptr<osg::Node> createLinkerModel(
			const JointData& in_joint,
			const osg::Vec4& in_color,
			const std::string& in_texturePath);

		/**
		* 构建圆形截面的管道模型
		*
		*@param in_startGeoPosition	:	管道起始点坐标
		*@param in_endGeoPosition	:	管道终止点坐标
		*@param in_radius			:	截面半径(单位：米)
		*@param in_color			:	颜色
		*@param in_texturePath		:	纹理路径
		*@param return				:	模型节点
		*/
		osg::ref_ptr<osg::Node> createPipeModel(
			const JointData& in_startJoint,
			const JointData& in_endJoint,
			const osg::Vec4& in_color,
			const std::string& in_texturePath);

	};

	/**
	* 模型文件类型
	*
	*/
	enum class MESH_GENERATOR_EXPORT ModelFileType
	{
		OSGB = 0,
		OBJ = 1
	};

	/**
	* 模型节点导出类
	*
	*	主要提供将三维模型节点输出到本地
	*/
	class MESH_GENERATOR_EXPORT CModelExporter
	{
	public:

		static bool execute(
			osg::ref_ptr<osg::Node> in_node, 
			const ModelFileType& in_modelType,
			const std::string& in_dir,
			const std::string& in_fileName);
	};
}

#endif // MODELBUILDER_h__
