#ifndef MODELBUILDER_h__
#define MODELBUILDER_h__

#include "modelbuilder/MeshUtil.h"

namespace MeshGenerator
{
	/**
	* ģ�͹�����
	*
	*	��Ҫ�Ǹ����û�����Ĳ�����ͨ����������ģ�㷨
	*	������Ӧ����άģ�ͽڵ㣬�磺Բ�ν��������ͷ���ܵ������ν��������ͷ���ܵ�
	*/
	class MESH_GENERATOR_EXPORT CModelCreator
	{
	public:

		/**
		* ����Բ������ͷ����
		*
		*@param in_geoPosition		:	����ͷ�ĵ�������
		*@param in_adjacentPoints	:	�ڽӵ㼯��
		*@param in_radius			:	����뾶(��λ����)
		*@param return				:	����ͷ����
		*/
		JointData createCircleJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const double& in_radius);

		/**
		* ������������ͷ����
		*
		*@param in_geoPosition		:	����ͷ�ĵ�������
		*@param in_adjacentPoints	:	�ڽӵ㼯��
		*@param in_width			:	���(��λ����)
		*@param in_height			:	�߶�(��λ����)
		*@param return				:	����ͷ����
		*/
		JointData createRectJointData(
			const osg::Vec3d& in_geoPosition,
			const std::vector<osg::Vec3d>& in_adjacentPoints,
			const double& in_width,
			const double& in_height);

		/**
		* ��������ͷ���ݹ���ģ�ͽڵ�
		*
		*@param in_joint			:	����ͷ����
		*@param in_color			:	��ɫ
		*@param in_texturePath		:	����·��
		*@param return				:	ģ�ͽڵ�
		*/
		osg::ref_ptr<osg::Node> createLinkerModel(
			const JointData& in_joint,
			const osg::Vec4& in_color,
			const std::string& in_texturePath);

		/**
		* ����Բ�ν���Ĺܵ�ģ��
		*
		*@param in_startGeoPosition	:	�ܵ���ʼ������
		*@param in_endGeoPosition	:	�ܵ���ֹ������
		*@param in_radius			:	����뾶(��λ����)
		*@param in_color			:	��ɫ
		*@param in_texturePath		:	����·��
		*@param return				:	ģ�ͽڵ�
		*/
		osg::ref_ptr<osg::Node> createPipeModel(
			const JointData& in_startJoint,
			const JointData& in_endJoint,
			const osg::Vec4& in_color,
			const std::string& in_texturePath);

		///**
		//* �������ν��������ͷģ��
		//*
		//*@param in_geoPosition		:	����ͷ�ĵ�������
		//*@param in_adjacentPoints	:	�ڽӵ㼯��
		//*@param in_width			:	������(��λ����)
		//*@param in_height			:	����߶�(��λ����)
		//*@param in_color			:	��ɫ
		//*@param in_texturePath		:	����·��
		//*@param return				:	ģ�ͽڵ�
		//*/
		//osg::ref_ptr<osg::Node> createLinkerModelByRectSection(
		//	const osg::Vec3d& in_geoPosition,
		//	const std::vector<osg::Vec3d>& in_adjacentPoints,
		//	const double& in_width,
		//	const double& in_height,
		//	const osg::Vec4& in_color,
		//	const std::string& in_texturePath);

		///**
		//* �������ν���Ĺܵ�ģ��
		//*
		//*@param in_startGeoPosition	:	�ܵ���ʼ������
		//*@param in_endGeoPosition	:	�ܵ���ֹ������
		//*@param in_width			:	������(��λ����)
		//*@param in_height			:	����߶�(��λ����)
		//*@param in_color			:	��ɫ
		//*@param in_texturePath		:	����·��
		//*@param return				:	ģ�ͽڵ�
		//*/
		//osg::ref_ptr<osg::Node> createPipeModelByRectSection(
		//	const osg::Vec3d& in_startGeoPosition,
		//	const osg::Vec3d& in_endGeoPosition,
		//	const double& in_width,
		//	const double& in_height,
		//	const osg::Vec4& in_color,
		//	const std::string& in_texturePath);
	};

	/**
	* ģ���ļ�����
	*
	*/
	enum class MESH_GENERATOR_EXPORT ModelFileType
	{
		OSGB = 0,
		OBJ = 1
	};

	/**
	* ģ�ͽڵ㵼����
	*
	*	��Ҫ�ṩ����άģ�ͽڵ����������
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
