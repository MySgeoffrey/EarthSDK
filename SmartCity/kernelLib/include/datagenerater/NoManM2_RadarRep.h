#pragma once
#ifndef NO_MANM2_INCLUDE_H
#define NO_MANM2_INCLUDE_H
#include <QMetaType>
#include <QBitArray>

#pragma pack(1)
/************************14���״﷢�����ݽӿ�**********************/
//�״����ݱ���ͷ
typedef struct RadarRepMSGHeader
{
	unsigned int RepFlag;	//0xddccbbaa
	unsigned int RepTime;	//1970.1.1 00:00:00�����ڵ�����
	unsigned int SrcID;		//��Դ 4�ֽڣ�
	unsigned int DataType;	//0x01-�״�״̬  0x02-�״�Ŀ��  0x03-�״���Ʊ� 0x04-��ʧ��
	unsigned int DataLen;	//�����򳤶� �״�״̬-0x20  �״�Ŀ��-0x20 *n n���20�� �� �״����-0x20
} RadarRepMSGHeader;
Q_DECLARE_METATYPE(RadarRepMSGHeader)

//�״�̽��Ŀ���ӽṹ
typedef struct RadarTargetRepSub
{
	unsigned char TargetSeq;		//Ŀ��������
	int SignalA;		//Ŀ���źŷ���
	int SignalType;		//�ź����ͣ�ң�⡢ͼ����������
	unsigned int SubA;	//��λ�� 0.1du
	unsigned int SubR;	// Ŀ�����
	unsigned int Carr;	// Ŀ����Ƶ
	unsigned char TargetSubClass;//���˻��ͺ�
} RadarTargetRepSub;
Q_DECLARE_METATYPE(RadarTargetRepSub)

//�״�̽��Ŀ������
typedef struct RadarTargetRep
{
	RadarRepMSGHeader head;
	unsigned int PH;			//����
	unsigned int A;				//��λ ����0.1�� 100=10.0��
	unsigned int E;				//���� ����0.1�� 100=10.0��
	unsigned int TrackCourse;	//���� ����0.1�� 100=10.0��
	unsigned int RCS;			//RCS  ����0.1�� 100=10.0ƽ����
	unsigned int R;				//���� ����0.1�� 100=10.0m
	unsigned int TrackSpeed;	//�ٶ� ����0.1�� 100=10.0m/s
	unsigned int Reserve;		//����

	double lon;                 //����
	double lat;                 //γ��
	double alt;                 //�߶� m

	//////////////////////////////////////////////////////add by jestin 20180720
	unsigned char TargetClass;//Ŀ������ 0-δ֪�� 1-���˻� 2-���� 3-��
	unsigned char Num;//���ߵ�����Ŀ�����������40
	RadarTargetRepSub RDTargetSub[40];
	//����
	/////////////////////////////////////////////////////
} RadarTargetRep;
Q_DECLARE_METATYPE(RadarTargetRep)

#pragma	pack()
#endif