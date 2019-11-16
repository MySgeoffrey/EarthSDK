#pragma once
#pragma execution_character_set("utf-8")
#define __USE_OSGRENDER__ 1

#if __USE_OSGRENDER__ == 1
#include "osg/Array"
#include <vector>
#include "osg/Geometry"
#include "osg/Geode"
#endif

#include<unordered_map>
#include"engine.h"
#include"ifcengine.h"
#include<tchar.h>

#include"stringFunction.h"
#include"generic.h"
#include"color.h"
#include"GenericTreeItem.h"
#include"unit.h"

using namespace std;

extern wchar_t *ifcFileName = 0, *ifcSchemaName_IFC2X3 = 0, *xmlSettings_IFC2x3 = 0;

extern STRUCT__SELECTABLE__TREEITEM	* baseTreeItem = 0;
extern STRUCT__IFC__ITEM *baseIfcItem = 0;
unordered_map<int, int> allIfcObjects;

bool firstItemWithGeometryPassed;

bool findIfcItemByGlobalID(wchar_t *wstr, const char* globalID)
{
	if (wstr == NULL)
		return false;

	int len = wcslen(wstr) + 1;
	//声明cstr的长度
	char *cstr = (char*)malloc(sizeof(char)*len);
	if (cstr == NULL)
	{
		//cout << "Fail!" << endl;
		return false;
	}
	//初始化化cstr为空
	memset(cstr, 0, len);
	int nlength = wcslen(wstr);
	//获取转换后的长度
	int nbytes = WideCharToMultiByte(0, 0, wstr, nlength, NULL, 0, NULL, NULL);
	if (nbytes > len)
		nbytes = len;
	//转换Unicode字符为ascii字符
	WideCharToMultiByte(0, 0, wstr, nlength, cstr, nbytes, NULL, NULL);

	if (strcmp(cstr, globalID) == 0)
	{
		free(cstr);
		return true;
	}
	else
	{
		free(cstr);
		return false;
	}
}

STRUCT__IFC__ITEM	* GetIfcItem(STRUCT__IFC__ITEM * ifcItem, int ifcID)
{
	return	(STRUCT__IFC__ITEM *)allIfcObjects[ifcID];
}

STRUCT__IFC__ITEM	* GetIveIfcItem(STRUCT__IFC__ITEM * ifcItem, STRUCT__IFC__ITEM * iveIfcItem, const char* globalID)
{
	while (ifcItem)
	{
		//如窗是由多个组件组成，IFCID为0，globalID为NULL
		if (ifcItem->globalId == NULL)
		{
			if (ifcItem->ifcID == 0)
			{
				if (findIfcItemByGlobalID(ifcItem->globalId, globalID))
					iveIfcItem = ifcItem->parent;
			}
			/*else
			{
			if (strcmp(Wchar2char(ifcItem->globalId), globalID) == 0)
			iveIfcItem = ifcItem;
			}*/
		}
		else
		{
			if (findIfcItemByGlobalID(ifcItem->globalId, globalID))
				iveIfcItem = ifcItem;
		}

		iveIfcItem = GetIveIfcItem(ifcItem->child, iveIfcItem, globalID);
		ifcItem = ifcItem->next;
	}

	return iveIfcItem;
}

void RetrieveObjects(int ifcModel, wchar_t * ObjectName, bool hide)
{
	int i, *ifcObjectUnstances, noIfcObjectInstances;
	ifcObjectUnstances = sdaiGetEntityExtentBN(ifcModel, (char*)ObjectName);
	noIfcObjectInstances = sdaiGetMemberCount(ifcObjectUnstances);
	if (noIfcObjectInstances)
	{
		STRUCT__IFC__ITEM	** ppLastIfcItem = &baseIfcItem;
		while ((*ppLastIfcItem)) {
			ppLastIfcItem = &(*ppLastIfcItem)->next;
		}
		(*ppLastIfcItem) = CreateIfcItem(ifcModel, 0, 0, 0, hide, ObjectName, 0, 0);

		for (i = 0; i < noIfcObjectInstances; ++i)
		{
			wchar_t* globalId = 0, *name = 0, *description = 0;
			int ifcObjectInstance = 0;
			engiGetAggrElement(ifcObjectUnstances, i, sdaiINSTANCE, &ifcObjectInstance);

			sdaiGetAttrBN(ifcObjectInstance, (char*)L"GlobalId", sdaiUNICODE, &globalId);
			sdaiGetAttrBN(ifcObjectInstance, (char*)L"Name", sdaiUNICODE, &name);
			sdaiGetAttrBN(ifcObjectInstance, (char*)L"Description", sdaiUNICODE, &description);

			STRUCT__IFC__ITEM *ifcItem = CreateIfcItem(ifcModel, (*ppLastIfcItem), ifcObjectInstance, ObjectName, hide, globalId, name, description);

			allIfcObjects[ifcObjectInstance] = (int)ifcItem;
		}
	}
}

//void	CreateTreeItem_ifcProject(int ifcModel, STRUCT__SELECTABLE__TREEITEM ** ppParent)
//{
//	int	i, * ifcProjectInstances, noIfcProjectInstances;
//	
//#ifdef	_UNICODE
//	ifcProjectInstances = sdaiGetEntityExtentBN(ifcModel, (char *) L"IFCPROJECT");
//#else
//	ifcProjectInstances = sdaiGetEntityExtentBN(ifcModel, "IFCPROJECT");
//#endif
//	noIfcProjectInstances = sdaiGetMemberCount(ifcProjectInstances);
//	for (i = 0; i < noIfcProjectInstances; ++i) {
//#ifdef	_UNICODE
//		WCHAR	* globalId = 0, * name = 0, * description = 0;
//#else
//		char	* globalId = 0, * name = 0, * description = 0;
//#endif
//		int		ifcProjectInstance = 0;
//		engiGetAggrElement(ifcProjectInstances, i, sdaiINSTANCE, &ifcProjectInstance);
//
//		STRUCT__SIUNIT	* units = GetUnits(ifcModel, ifcProjectInstance);
//
//#ifdef	_UNICODE
//		sdaiGetAttrBN(ifcProjectInstance, (char *) L"GlobalId", sdaiUNICODE, &globalId);
//		sdaiGetAttrBN(ifcProjectInstance, (char *) L"Name", sdaiUNICODE, &name);
//		sdaiGetAttrBN(ifcProjectInstance, (char *) L"Description", sdaiUNICODE, &description);
//#else
//		sdaiGetAttrBN(ifcProjectInstance, "GlobalId", sdaiSTRING, &globalId);
//		sdaiGetAttrBN(ifcProjectInstance, "Name", sdaiSTRING, &name);
//		sdaiGetAttrBN(ifcProjectInstance, "Description", sdaiSTRING, &description);
//#endif
//
//#ifdef	_UNICODE
//		(* ppParent) = CreateTreeItem(0, L"IfcProject", globalId, name,  description);
//#else
//		(* ppParent) = CreateTreeItem(0, "IfcProject", globalId, name, description);
//#endif
//		STRUCT__SELECTABLE__TREEITEM	** ppChild = &(* ppParent)->child;
//		int	* ifcRelDecomposesInstances = 0, ifcRelDecomposesInstancesCnt;
//#ifdef	_UNICODE
//		sdaiGetAttrBN(ifcProjectInstance, (char *) L"IsDecomposedBy", sdaiAGGR, &ifcRelDecomposesInstances);
//#else
//		sdaiGetAttrBN(ifcProjectInstance, "IsDecomposedBy", sdaiAGGR, &ifcRelDecomposesInstances);
//#endif
//		if	(ifcRelDecomposesInstances) {
//			ifcRelDecomposesInstancesCnt = sdaiGetMemberCount(ifcRelDecomposesInstances);
//			//ASSERT(ifcRelDecomposesInstancesCnt <= 1);
//			for  (int j = 0; j < ifcRelDecomposesInstancesCnt; ++j) {
//				int ifcRelDecomposesInstance = 0,
//#ifdef	_UNICODE
//					ifcRelAggregates_TYPE = sdaiGetEntity(ifcModel, (char *) L"IFCRELAGGREGATES");
//#else
//					ifcRelAggregates_TYPE = sdaiGetEntity(ifcModel, "IFCRELAGGREGATES");
//#endif
//				engiGetAggrElement(ifcRelDecomposesInstances, j, sdaiINSTANCE, &ifcRelDecomposesInstance);
//				if	(sdaiGetInstanceType(ifcRelDecomposesInstance) == ifcRelAggregates_TYPE) {
//					int	* ifcSiteInstances = 0, ifcSiteInstancesCnt;
//#ifdef	_UNICODE
//					sdaiGetAttrBN(ifcRelDecomposesInstance, (char *) L"RelatedObjects", sdaiAGGR, &ifcSiteInstances);
//#else
//					sdaiGetAttrBN(ifcRelDecomposesInstance, "RelatedObjects", sdaiAGGR, &ifcSiteInstances);
//#endif
//					ifcSiteInstancesCnt = sdaiGetMemberCount(ifcSiteInstances);
//					for  (int k = 0; k < ifcSiteInstancesCnt; ++k) {
//						int ifcSiteInstance = 0,
//#ifdef	_UNICODE
//							ifcSite_TYPE = sdaiGetEntity(ifcModel, (char *) L"IFCSITE");
//#else
//							ifcSite_TYPE = sdaiGetEntity(ifcModel, "IFCSITE");
//#endif
//						engiGetAggrElement(ifcSiteInstances, k, sdaiINSTANCE, &ifcSiteInstance);
//						if	(sdaiGetInstanceType(ifcSiteInstance) == ifcSite_TYPE) {
//							CreateTreeItem_ifcSite(ifcModel, baseTreeItem, ppChild, ifcSiteInstance, units);
//							ppChild = &(* ppChild)->next;
//						} else {
////							ASSERT(false);
//						}
//					}
//				}
//			}
//		}
//
//		ppChild = &(* ppChild)->next;
//	}
//}


void	GenerateFacesGeometry(int ifcModel, STRUCT__IFC__ITEM * ifcItem, int64_t setting, int64_t mask)
{
	if	(ifcItem  &&  ifcItem->ifcID) {
		int	noVertices = 0, noIndices = 0;
		initializeModellingInstance(ifcModel, &noVertices, &noIndices, 0, ifcItem->ifcID);

		if	(noVertices  &&  noIndices) {
			//
			//	<Added for Clipping>
			//
/*			int64_t		owlModel = 0, owlInstance = 0;
			owlGetModel(ifcModel, &owlModel);
			owlGetInstance(ifcModel, ifcItem->ifcID, &owlInstance);

			int64_t		classClipping = GetClassByName(owlModel, "Clipping"),
						classPlane = GetClassByName(owlModel, "Plane"),
						propertyA = GetPropertyByName(owlModel, "A"),
						propertyB = GetPropertyByName(owlModel, "B"),
						propertyC = GetPropertyByName(owlModel, "C"),
						propertyD = GetPropertyByName(owlModel, "D"),
						propertyObject = GetPropertyByName(owlModel, "object"),
						propertyPlane = GetPropertyByName(owlModel, "plane");

			int64_t		owlInstanceClipping = CreateInstance(classClipping, 0),
						owlInstancePlane = CreateInstance(classPlane, 0);

			double		A = 0,
						B = 0,
						C = 1,
						D = -1000;

			SetDataTypeProperty(owlInstancePlane, propertyA, &A, 1);
			SetDataTypeProperty(owlInstancePlane, propertyB, &B, 1);
			SetDataTypeProperty(owlInstancePlane, propertyC, &C, 1);
			SetDataTypeProperty(owlInstancePlane, propertyD, &D, 1);

			SetObjectTypeProperty(owlInstanceClipping, propertyObject, &owlInstance, 1);
			SetObjectTypeProperty(owlInstanceClipping, propertyPlane, &owlInstancePlane, 1);

			SetFormat(owlModel, setting, mask);

			int64_t		vertexBufferSize = 0, indexBufferSize = 0;
			CalculateInstance(owlInstanceClipping, &vertexBufferSize, &indexBufferSize, 0);
			
			if	(vertexBufferSize  &&  indexBufferSize) {
				ifcItem->noVerticesForFaces = vertexBufferSize;
				ifcItem->noPrimitivesForFaces = indexBufferSize/3;
				ifcItem->verticesForFaces = new float[(unsigned int) vertexBufferSize * 6];    //    <x, y, z, nx, ny, nz>
				ifcItem->indicesForFaces = new int[(unsigned int) indexBufferSize];

				UpdateInstanceVertexBuffer(owlInstanceClipping, ifcItem->verticesForFaces);
				UpdateInstanceIndexBuffer(owlInstanceClipping, ifcItem->indicesForFaces);
			} else {
				ifcItem->noVerticesForFaces = 0;
				ifcItem->noPrimitivesForFaces = 0;
				ifcItem->verticesForFaces = 0;								    //    <x, y, z, nx, ny, nz>
				ifcItem->indicesForFaces = 0;
			}
			//
			//	</Added for Clipping>		*/
			//
			ifcItem->noVerticesForFaces = noVertices;
			ifcItem->noPrimitivesForFaces = noIndices/3;
			ifcItem->verticesForFaces = new float[noVertices * 6];    //    <x, y, z, nx, ny, nz>
			ifcItem->indicesForFaces = new int[noIndices];
			finalizeModelling(ifcModel, (float *) ifcItem->verticesForFaces, ifcItem->indicesForFaces, 0);
		} 
		else 
		{
			//ASSERT(ifcItem->noVerticesForFaces == 0);
		}
	}
}


void	GenerateWireFrameGeometry(int ifcModel, STRUCT__IFC__ITEM * ifcItem)
{
	if	(ifcItem  &&  ifcItem->ifcID) {
		int	noVertices = 0, noIndices = 0;
		initializeModellingInstance(ifcModel, &noVertices, &noIndices, 0, ifcItem->ifcID);

		if	(noVertices  &&  noIndices) {
			ifcItem->noVerticesForWireFrame = noVertices;
			ifcItem->verticesForWireFrame = new float[noVertices * 6];    //    <x, y, z>
			ifcItem->indicesForWireFrame = new int[noIndices];
			finalizeModelling(ifcModel, (float *) ifcItem->verticesForWireFrame, ifcItem->indicesForWireFrame, 0);

			ifcItem->noPrimitivesForWireFrame = 0;
			ifcItem->indicesForWireFrameLineParts = new int[2*noIndices];

			ifcItem->noVerticesForFaces = noVertices;
			ifcItem->verticesForFaces = ifcItem->verticesForWireFrame;
			ifcItem->indicesForFaces = new int[noIndices];

#if 0
			ifcItem->noPrimitivesForFaces = noIndices / 3;
#endif

            int    faceCnt = getConceptualFaceCnt(ifcItem->ifcID);
            for  (int j = 0; j < faceCnt; j++) {
                int    startIndexTriangles = 0, noIndicesTrangles = 0, startIndexFacesPolygons = 0, noIndicesFacesPolygons = 0;
                getConceptualFaceEx(ifcItem->ifcID, j, &startIndexTriangles, &noIndicesTrangles, 0, 0, 0, 0, &startIndexFacesPolygons, &noIndicesFacesPolygons, 0, 0);
                    
				int	i = 0;
				while  (i < noIndicesTrangles) {
					ifcItem->indicesForFaces[ifcItem->noPrimitivesForFaces * 3 + i] = ifcItem->indicesForWireFrame[startIndexTriangles + i];
					i++;
				}
				ifcItem->noPrimitivesForFaces += noIndicesTrangles/3;

				i = 0;
				int	lastItem = -1;
				while  (i < noIndicesFacesPolygons) {
					if	(lastItem >= 0  &&  ifcItem->indicesForWireFrame[startIndexFacesPolygons+i] >= 0) {
						ifcItem->indicesForWireFrameLineParts[2*ifcItem->noPrimitivesForWireFrame + 0] = lastItem;
						ifcItem->indicesForWireFrameLineParts[2*ifcItem->noPrimitivesForWireFrame + 1] = ifcItem->indicesForWireFrame[startIndexFacesPolygons+i];
						ifcItem->noPrimitivesForWireFrame++;
					}
					lastItem = ifcItem->indicesForWireFrame[startIndexFacesPolygons+i];
					i++;
				}
            }
		} 
		else 
		{
			//ASSERT(ifcItem->noVerticesForWireFrame == 0);
		}
	}
}

void	GenerateGeometry(int ifcModel, STRUCT__IFC__ITEM * ifcItem)
{
	while  (ifcItem) {
		//
		//	Get Geometry
		//
		int	setting = 0, mask = 0;
		mask += flagbit2;        //    PRECISION (32/64 bit)
		mask += flagbit3;        //	   INDEX ARRAY (32/64 bit)
		mask += flagbit5;        //    NORMALS
		mask += flagbit8;        //    TRIANGLES
		mask += flagbit12;       //    WIREFRAME

//		setting += flagbit2;     //    DOUBLE PRECISION (double)
		setting += 0;		     //    SINGLE PRECISION (float)
#ifndef	WIN64
		setting += 0;            //    32 BIT INDEX ARRAY (Int32)
#else
		setting += flagbit3;     //    64 BIT INDEX ARRAY (Int64)
#endif
//		setting += 0;            //    NORMALS OFF
		setting += flagbit5;     //    NORMALS ON
//		setting += 0;			 //    TRIANGLES OFF
		setting += flagbit8;     //    TRIANGLES ON
		setting += flagbit12;    //    WIREFRAME ON
		setFormat(ifcModel, setting, mask);

		GenerateWireFrameGeometry(ifcModel, ifcItem);

//		setting = 0;
//		setting += 0;		     //    SINGLE PRECISION (float)
//#ifndef	WIN64
//		setting += 0;            //    32 BIT INDEX ARRAY (Int32)
//#else
//		setting += flagbit3;     //    64 BIT INDEX ARRAY (Int64)
//#endif
//		setting += flagbit5;     //    NORMALS ON
//		setting += flagbit8;     //    TRIANGLES ON
//		setting += 0;			 //    WIREFRAME OFF
//		setFormat(ifcModel, setting, mask);
//
//		GenerateFacesGeometry(ifcModel, ifcItem, setting, mask);
//
//		cleanMemory(ifcModel, 1);

		GenerateGeometry(ifcModel, ifcItem->child);
		ifcItem = ifcItem->next;
	}
}

double	fabs__(double x)
{
	if (x < 0) {
		return	-x;
	}
	else {
		return	x;
	}
}

void	GetBufferSizes_ifcFaces(STRUCT__IFC__ITEM * ifcItem, int * pVBuffSize, int * pIBuffSize)
{
	while  (ifcItem) {
		if	(ifcItem->ifcID  &&  ifcItem->noVerticesForFaces  &&  ifcItem->noPrimitivesForFaces) {
			ifcItem->vertexOffsetForFaces = (int) (*pVBuffSize);
			ifcItem->indexOffsetForFaces = (int) (*pIBuffSize);

			(*pVBuffSize) += ifcItem->noVerticesForFaces;
			(*pIBuffSize) += 3 * ifcItem->noPrimitivesForFaces;
		}
		GetBufferSizes_ifcFaces(ifcItem->child, pVBuffSize, pIBuffSize);
		ifcItem = ifcItem->next;
	}
}

void	GetBufferSizes_ifcWireFrame(STRUCT__IFC__ITEM * ifcItem, int * pVBuffSize, int * pIBuffSize)
{
	while  (ifcItem) {
		if	(ifcItem->ifcID  &&  ifcItem->noVerticesForWireFrame  &&  ifcItem->noPrimitivesForWireFrame) {
			ifcItem->vertexOffsetForWireFrame = (int) (*pVBuffSize);
			ifcItem->indexOffsetForWireFrame = (int) (*pIBuffSize);

			(*pVBuffSize) += ifcItem->noVerticesForWireFrame;
			(*pIBuffSize) += 2 * ifcItem->noPrimitivesForWireFrame;
		}
		GetBufferSizes_ifcWireFrame(ifcItem->child, pVBuffSize, pIBuffSize);
		ifcItem = ifcItem->next;
	}
}

void	AdjustMinMax(STRUCT__IFC__ITEM * ifcItem, VECTOR3 * center, double size)
{
	while  (ifcItem) {
		if	(ifcItem->ifcID  &&  ifcItem->noVerticesForFaces  &&  ifcItem->noPrimitivesForFaces) {
			ifcItem->vecMin.x = (ifcItem->vecMin.x - center->x) / size;
			ifcItem->vecMin.y = (ifcItem->vecMin.y - center->y) / size;
			ifcItem->vecMin.z = (ifcItem->vecMin.z - center->z) / size;

			ifcItem->vecMax.x = (ifcItem->vecMax.x - center->x) / size;
			ifcItem->vecMax.y = (ifcItem->vecMax.y - center->y) / size;
			ifcItem->vecMax.z = (ifcItem->vecMax.z - center->z) / size;
		}
		AdjustMinMax(ifcItem->child, center, size);
		ifcItem = ifcItem->next;
	}
}

void	FillBuffers_ifcFaces(STRUCT__IFC__ITEM * ifcItem, int * pVBuffSize, int * pIBuffSize, float * pVertices, __int32 * pIndices, VECTOR3 * center, double size, STRUCT__COLOR * color)
{
	while  (ifcItem) {
		if	(ifcItem->ifcID  &&  ifcItem->noVerticesForFaces  &&  ifcItem->noPrimitivesForFaces  /*&&  ifcItem->color == color*/) {
			int i = 0;
			while  (i < ifcItem->noVerticesForFaces) {
				pVertices[6 * ((*pVBuffSize) + i) + 0] = (float) ((ifcItem->verticesForFaces[6 * i + 0] - center->x) / size);
				pVertices[6 * ((*pVBuffSize) + i) + 1] = (float) ((ifcItem->verticesForFaces[6 * i + 1] - center->y) / size);
				pVertices[6 * ((*pVBuffSize) + i) + 2] = (float) ((ifcItem->verticesForFaces[6 * i + 2] - center->z) / size);
				pVertices[6 * ((*pVBuffSize) + i) + 3] = ifcItem->verticesForFaces[6 * i + 3];
				pVertices[6 * ((*pVBuffSize) + i) + 4] = ifcItem->verticesForFaces[6 * i + 4];
				pVertices[6 * ((*pVBuffSize) + i) + 5] = ifcItem->verticesForFaces[6 * i + 5];
				i++;
			}
	
			i = 0;
			while  (i < ifcItem->noPrimitivesForFaces) {
				pIndices[(*pIBuffSize) + 3 * i + 0] = (__int32) (ifcItem->indicesForFaces[3*i + 0] + (*pVBuffSize));
				pIndices[(*pIBuffSize) + 3 * i + 1] = (__int32) (ifcItem->indicesForFaces[3*i + 1] + (*pVBuffSize));
				pIndices[(*pIBuffSize) + 3 * i + 2] = (__int32) (ifcItem->indicesForFaces[3*i + 2] + (*pVBuffSize));
				i++;
			}

			ifcItem->vertexOffsetForFaces = (int) (*pVBuffSize);
			ifcItem->indexOffsetForFaces = (int) (*pIBuffSize);
			//ASSERT(ifcItem->vertexOffsetForFaces == (*pVBuffSize));
			//ASSERT(ifcItem->indexOffsetForFaces == (*pIBuffSize));

			(*pVBuffSize) += ifcItem->noVerticesForFaces;
			(*pIBuffSize) += 3 * ifcItem->noPrimitivesForFaces;
		}
		FillBuffers_ifcFaces(ifcItem->child, pVBuffSize, pIBuffSize, pVertices, pIndices, center, size, color);
		ifcItem = ifcItem->next;
	}
}

void	FillBuffers_ifcWireFrame(STRUCT__IFC__ITEM * ifcItem, int * pVBuffSize, int * pIBuffSize, float * pVertices, __int32 * pIndices, VECTOR3 * center, double size)
{
	while  (ifcItem) {
		if	(ifcItem->ifcID  &&  ifcItem->noVerticesForFaces  &&  ifcItem->noVerticesForWireFrame  &&  ifcItem->noPrimitivesForFaces  &&  ifcItem->noPrimitivesForWireFrame) {
			int i = 0;
			while  (i < ifcItem->noVerticesForWireFrame) {
//				pVertices[6 * ((*pVBuffSize) + i) + 0] = ((ifcItem->verticesForWireFrame[6 * i + 0] - center->x) / size);
//				pVertices[6 * ((*pVBuffSize) + i) + 1] = ((ifcItem->verticesForWireFrame[6 * i + 1] - center->y) / size);
//				pVertices[6 * ((*pVBuffSize) + i) + 2] = ((ifcItem->verticesForWireFrame[6 * i + 2] - center->z) / size);
//				pVertices[6 * ((*pVBuffSize) + i) + 3] = ifcItem->verticesForWireFrame[6 * i + 3];
//				pVertices[6 * ((*pVBuffSize) + i) + 4] = ifcItem->verticesForWireFrame[6 * i + 4];
//				pVertices[6 * ((*pVBuffSize) + i) + 5] = ifcItem->verticesForWireFrame[6 * i + 5];
//				pVertices[6 * ((*pVBuffSize) + i) + 0] = (float) ((ifcItem->verticesForWireFrame[3 * i + 0] - center->x) / size);
//				pVertices[6 * ((*pVBuffSize) + i) + 1] = (float) ((ifcItem->verticesForWireFrame[3 * i + 1] - center->y) / size);
//				pVertices[6 * ((*pVBuffSize) + i) + 2] = (float) ((ifcItem->verticesForWireFrame[3 * i + 2] - center->z) / size);
//				pVertices[6 * ((*pVBuffSize) + i) + 3] = 0;
//				pVertices[6 * ((*pVBuffSize) + i) + 4] = 0;
//				pVertices[6 * ((*pVBuffSize) + i) + 5] = 1;
				i++;
			}
			(*pVBuffSize) = ifcItem->vertexOffsetForFaces;

			i = 0;
			while  (i < ifcItem->noPrimitivesForWireFrame) {
				pIndices[(*pIBuffSize) + 2 * i + 0] = (__int32) (ifcItem->indicesForWireFrameLineParts[2 * i + 0] + (*pVBuffSize));
				pIndices[(*pIBuffSize) + 2 * i + 1] = (__int32) (ifcItem->indicesForWireFrameLineParts[2 * i + 1] + (*pVBuffSize));
				i++;
			}

			ifcItem->vertexOffsetForWireFrame = (int) (*pVBuffSize);
			ifcItem->indexOffsetForWireFrame = (int) (*pIBuffSize);
			//ASSERT(ifcItem->vertexOffsetForWireFrame == (*pVBuffSize));
			//ASSERT(ifcItem->indexOffsetForWireFrame == (*pIBuffSize));

			(*pVBuffSize) += ifcItem->noVerticesForWireFrame;
			//ASSERT(ifcItem->vertexOffsetForWireFrame == ifcItem->vertexOffsetForFaces);
			(*pIBuffSize) += 2 * ifcItem->noPrimitivesForWireFrame;
		}
		FillBuffers_ifcWireFrame(ifcItem->child, pVBuffSize, pIBuffSize, pVertices, pIndices, center, size);
		ifcItem = ifcItem->next;
	}
}


void	GetDimensions(STRUCT__IFC__ITEM * ifcItem, VECTOR3 * min, VECTOR3 * max, bool * pInitMinMax)
{
	while  (ifcItem) {
		if	(ifcItem->noVerticesForFaces) {
			ifcItem->vecMin.x = ifcItem->verticesForFaces[0];
			ifcItem->vecMax.x = ifcItem->vecMin.x;
			ifcItem->vecMin.y = ifcItem->verticesForFaces[1];
			ifcItem->vecMax.y = ifcItem->vecMin.y;
			ifcItem->vecMin.z = ifcItem->verticesForFaces[2];
			ifcItem->vecMax.z = ifcItem->vecMin.z;
			int	i = 0;
			while  (i < ifcItem->noVerticesForFaces) {
				if	(ifcItem->vecMin.x > ifcItem->verticesForFaces[6*i + 0]) { ifcItem->vecMin.x = ifcItem->verticesForFaces[6*i + 0]; }
				if	(ifcItem->vecMax.x < ifcItem->verticesForFaces[6*i + 0]) { ifcItem->vecMax.x = ifcItem->verticesForFaces[6*i + 0]; }
				if	(ifcItem->vecMin.y > ifcItem->verticesForFaces[6*i + 1]) { ifcItem->vecMin.y = ifcItem->verticesForFaces[6*i + 1]; }
				if	(ifcItem->vecMax.y < ifcItem->verticesForFaces[6*i + 1]) { ifcItem->vecMax.y = ifcItem->verticesForFaces[6*i + 1]; }
				if	(ifcItem->vecMin.z > ifcItem->verticesForFaces[6*i + 2]) { ifcItem->vecMin.z = ifcItem->verticesForFaces[6*i + 2]; }
				if	(ifcItem->vecMax.z < ifcItem->verticesForFaces[6*i + 2]) { ifcItem->vecMax.z = ifcItem->verticesForFaces[6*i + 2]; }
				i++;
			}

			if	((*pInitMinMax) == false) {
				min->x = ifcItem->vecMin.x;
				max->x = ifcItem->vecMax.x;
				min->y = ifcItem->vecMin.y;
				max->y = ifcItem->vecMax.y;
				min->z = ifcItem->vecMin.z;
				max->z = ifcItem->vecMax.z;
				(*pInitMinMax) = true;
			} else {
				if	(min->x > ifcItem->vecMin.x) { min->x = ifcItem->vecMin.x; }
				if	(max->x < ifcItem->vecMax.x) { max->x = ifcItem->vecMax.x; }
				if	(min->y > ifcItem->vecMin.y) { min->y = ifcItem->vecMin.y; }
				if	(max->y < ifcItem->vecMax.y) { max->y = ifcItem->vecMax.y; }
				if	(min->z > ifcItem->vecMin.z) { min->z = ifcItem->vecMin.z; }
				if	(max->z < ifcItem->vecMax.z) { max->z = ifcItem->vecMax.z; }
			}
		}
		GetDimensions(ifcItem->child, min, max, pInitMinMax);
		ifcItem = ifcItem->next;
	}
}

float		* pVerticesDeviceBuffer;
__int32		* pIndicesDeviceBuffer;

void	RenderFaces(STRUCT__IFC__ITEM * ifcItem, STRUCT__COLOR * color,osg::Group** pGroup)
{
	while	(ifcItem) 
	{
		//if	(ifcItem->treeItem  &&  ifcItem->treeItem->select == ITEM_CHECKED  &&  ifcItem->noPrimitivesForFaces) 
		{
			long long	vertexOffsetForFaces,
				noVerticesForFaces,
				indexOffsetForFaces,
				noPrimitivesForFaces;
			vertexOffsetForFaces = ifcItem->vertexOffsetForFaces;
			noVerticesForFaces = ifcItem->noVerticesForFaces;
			indexOffsetForFaces = ifcItem->indexOffsetForFaces;
			noPrimitivesForFaces = ifcItem->noPrimitivesForFaces;
#if __USE_OSGRENDER__ == 1
			osg::ref_ptr<osg::Geode> geodeNode = new osg::Geode;
			geodeNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
			geodeNode->addDrawable(geometry.get());

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
			 osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

			for (int i = vertexOffsetForFaces; i < vertexOffsetForFaces + noVerticesForFaces * 6; i += 6)
			{
				float x = pVerticesDeviceBuffer[i + 0];
				float y = pVerticesDeviceBuffer[i + 1];
				float z = pVerticesDeviceBuffer[i + 2];
				vertices->push_back(osg::Vec3(x,y,z));

				float nx = pVerticesDeviceBuffer[i + 3];
				float ny = pVerticesDeviceBuffer[i + 4];
				float nz = pVerticesDeviceBuffer[i + 5];
				normals->push_back(osg::Vec3(nx,ny,nz));

				STRUCT__ITEM__COLOR* pColor = 
					ifcItem->itemColor ? ifcItem->itemColor : ifcItem->objectColor->itemColor;

				if (pColor)
				{
					colors->push_back(osg::Vec4(
						pColor->diffuse.R,
						pColor->diffuse.G,
						pColor->diffuse.B,
						pColor->diffuse.A
						));
				}
				else
				{
					colors->push_back(osg::Vec4(
						1.0,
						1.0,
						1.0,
						1.0
						));
				}
			}
			if (vertices->size() > 0)
			{
				int indiceCount = ifcItem->noPrimitivesForFaces;// * 3;
				unsigned short* pIndices = new unsigned short[indiceCount];
				for (int i = 0; i < indiceCount;++i)
				{
					pIndices[i] = ifcItem->indicesForFaces[i] ;//+ ifcItem->indexOffsetForFaces;
				}
				geometry->setVertexArray( vertices.get() );
				geometry->setColorArray(colors);
				geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
				geometry->setNormalArray( normals.get() );
				geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
				/*geometry->addPrimitiveSet( new osg::DrawElementsUShort(osg::DrawArrays::TRIANGLES, indiceCount, pIndices) );*/
				geometry->addPrimitiveSet( new osg::DrawArrays(osg::DrawArrays::POINTS, 0, vertices->size()) );
				if (NULL == *pGroup)
				{
					*pGroup = new osg::Group();
				}
				(*pGroup)->addChild(geodeNode);
			}
#endif
			//g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,  (__int32) ifcItem->vertexOffsetForFaces,  (__int32) ifcItem->noVerticesForFaces,  (__int32) ifcItem->indexOffsetForFaces,  (__int32) ifcItem->noPrimitivesForFaces);
		}

		RenderFaces(ifcItem->child, color,pGroup);
		ifcItem = ifcItem->next;
	}
}


void GetRenderData(STRUCT__IFC__ITEM *baseIfcItem)
{
	VECTOR3	min, max;
	bool			initSizes = false;
	GetDimensions(baseIfcItem, &min, &max, &initSizes);
	if	(initSizes) 
	{
		VECTOR3	center;
		center.x = (max.x + min.x)/2;
		center.y = (max.y + min.y)/2;
		center.z = (max.z + min.z)/2;
		double size = max.x - min.x;
		if	(size < max.y - min.y) { size = max.y - min.y; }
		if	(size < max.z - min.z) { size = max.z - min.z; }

		int	vBuffSize = 0, iBuffSize = 0, tmpVBuffSize = 0;
		GetBufferSizes_ifcFaces(baseIfcItem, &vBuffSize, &iBuffSize);
		tmpVBuffSize = vBuffSize;
		vBuffSize = 0;
		GetBufferSizes_ifcWireFrame(baseIfcItem, &vBuffSize, &iBuffSize);

		int vertexCnt = 0, indexCnt = 0;

		AdjustMinMax(baseIfcItem, &center, size);
		pVerticesDeviceBuffer = new float[vBuffSize*6];
		pIndicesDeviceBuffer = new int[iBuffSize];
		FillBuffers_ifcFaces(baseIfcItem, &vertexCnt, &indexCnt, pVerticesDeviceBuffer, pIndicesDeviceBuffer, &center, size, 0);
		/*STRUCT__COLOR	* colorLoop = 0;
		while  (colorLoop)
		{
			FillBuffers_ifcFaces(baseIfcItem, &vertexCnt, &indexCnt, pVerticesDeviceBuffer, pIndicesDeviceBuffer, &center, size, colorLoop);
			colorLoop = colorLoop->next;
		}*/

		vertexCnt = 0;
		FillBuffers_ifcWireFrame(baseIfcItem, &vertexCnt, &indexCnt, pVerticesDeviceBuffer, pIndicesDeviceBuffer, &center, size);

	}
}