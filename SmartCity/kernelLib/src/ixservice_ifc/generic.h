#if _MSC_VER > 1000
#pragma once
#pragma execution_character_set("utf-8")
#endif // _MSC_VER > 1000

//#include<CommCtrl.h>

#ifdef _MSC_VER
//
//	Need to surpress warning due to a known issue in Visual Studio 2010
//
//#pragma warning (push)
//#pragma warning (disable : 4005)
//#include	<stdint.h>
//#pragma warning (pop)
//#else
//#include	<stdint.h>
#endif


#ifdef WIN64
	#define int __int64
#endif



#define		STRUCT_TYPE_ITEM_COLOR				200
#define		STRUCT_TYPE_OBJECT_COLOR			201
#define		STRUCT_TYPE_HEADER_SET				202
#define		STRUCT_TYPE_PROPERTY				203
#define		STRUCT_TYPE_PROPERTY_SET			204
#define		STRUCT_TYPE_IFC_ITEM				205
#define		STRUCT_TYPE_SELECTABLE_TREEITEM		206



struct STRUCT__COLOR;							//		color
struct STRUCT__HEADER__SET;						//		headerInfo
struct STRUCT__SIUNIT;


struct STRUCT__ITEM__COLOR;
struct STRUCT__OBJECT__COLOR;


typedef struct VECTOR3 {
	double							x;
	double							y;
	double							z;
}	VECTOR3;

typedef struct STRUCT__BASE {
	int								structType;
}	STRUCT_BASE;

typedef struct STRUCT__PROPERTY {
	int								structType;
	int								ifcID;

#ifdef	_UNICODE
	wchar_t							* name;
	wchar_t							* description;
#else
	char							* name;
	char							* description;
#endif

#ifdef	_UNICODE
	wchar_t							* nominalValue;
	wchar_t							* lengthValue;
	wchar_t							* areaValue;
	wchar_t							* volumeValue;
	wchar_t							* countValue;
	wchar_t							* weigthValue;
	wchar_t							* timeValue;
	wchar_t							* unit;
#else
	char							* nominalValue;
	char							* lengthValue;
	char							* areaValue;
	char							* volumeValue;
	char							* countValue;
	char							* weigthValue;
	char							* timeValue;
	char							* unit;
#endif

#ifdef	_UNICODE
	wchar_t							* nameBuffer;
#else
	char							* nameBuffer;
#endif

	STRUCT__PROPERTY				* next;
}	STRUCT__PROPERTY;

typedef struct STRUCT__PROPERTY__SET {
	int								structType;
	int								ifcID;

#ifdef	_UNICODE
	wchar_t							* name;
	wchar_t							* description;
#else
	char							* name;
	char							* description;
#endif
	
	STRUCT__PROPERTY				* properties;

#ifdef	_UNICODE
	wchar_t							* nameBuffer;
#else
	char							* nameBuffer;
#endif

	STRUCT__PROPERTY__SET			* next;
}	STRUCT__PROPERTY__SET;

#if 0
typedef struct STRUCT__IFC__ITEM {
	int								structType;
	int								ifcID;

#ifdef	_UNICODE
	wchar_t							* globalId;
	wchar_t							* ifcType;
	wchar_t							* name;
#else
	char							* globalId;
	char							* ifcType;
	char							* name;
	char							* description;
#endif

	STRUCT__PROPERTY__SET			* propertySets;
	bool							propertySetsAvailableHasLoadedYet;

	STRUCT__IFC__ITEM				* parent;
	STRUCT__IFC__ITEM				* child;
	STRUCT__IFC__ITEM				* next;

	STRUCT__ITEM__COLOR				* itemColor;
	STRUCT__OBJECT__COLOR			* objectColor;

	STRUCT__SIUNIT					* units;

}	STRUCT__IFC__ITEM;
#else
struct _TREEITEM;
typedef struct _TREEITEM *HTREEITEM;
struct STRUCT__IFC__ITEM;
typedef struct STRUCT__SELECTABLE__TREEITEM {
	int								structType;

#ifdef	_UNICODE
	wchar_t							* globalId;
	wchar_t							* ifcType;
	wchar_t							* name;
	wchar_t							* description;
#else
	char							* globalId;
	char							* ifcType;
	char							* name;
	char							* description;
#endif

	STRUCT__SELECTABLE__TREEITEM	* parent;
	STRUCT__SELECTABLE__TREEITEM	* child;
	STRUCT__SELECTABLE__TREEITEM	* next;

	int								interfaceType;

	HTREEITEM						hTreeItem;
#ifdef	_UNICODE
	wchar_t							* nameBuffer;
#else
	char							* nameBuffer;
#endif
	__int32							select;
	
	STRUCT__HEADER__SET				* headerSet;
	STRUCT__IFC__ITEM				* ifcItem;

	bool							showIfcObjectWireFrame;
	bool							showIfcObjectSolid;
	bool							showPlane;

	int								vertexOffset;
	int								indexOffset;
}	STRUCT__SELECTABLE__TREEITEM;

typedef struct STRUCT__IFC__ITEM {
	int								structType;
	int								ifcID;

#ifdef	_UNICODE
	wchar_t							* globalId;
	wchar_t							* ifcType;
	wchar_t							* name;
	wchar_t							* description;
#else
	char							* globalId;
	char							* ifcType;
	char							* name;
	char							* description;
#endif

	STRUCT__SELECTABLE__TREEITEM	* treeItem;
	STRUCT__PROPERTY__SET			* propertySets;
	bool							propertySetsAvailableButNotLoadedYet;

	STRUCT__IFC__ITEM				* parent;
	STRUCT__IFC__ITEM				* child;
	STRUCT__IFC__ITEM				* next;
#if 0
	STRUCT__COLOR					* color;
#else
	STRUCT__COLOR					* color;
	STRUCT__ITEM__COLOR				* itemColor;
	STRUCT__OBJECT__COLOR			* objectColor;
#endif
	STRUCT__SIUNIT					* units;

	VECTOR3							vecMin;
	VECTOR3							vecMax;

	int								noVerticesForFaces;
	int								noPrimitivesForFaces;
	float							* verticesForFaces;
	int								* indicesForFaces;
	int								vertexOffsetForFaces;
	int								indexOffsetForFaces;

	int								noVerticesForWireFrame;
	int								noPrimitivesForWireFrame;
//	double							* verticesForWireFrame;
	float							* verticesForWireFrame;
	int								* indicesForWireFrame;
	int								* indicesForWireFrameLineParts;
	int								vertexOffsetForWireFrame;
	int								indexOffsetForWireFrame;
}	STRUCT__IFC__ITEM;
#endif

#ifdef	_UNICODE
bool	Equals(wchar_t * txtI, wchar_t * txtII);
#else
bool	Equals(char * txtI, char * txtII);
#endif

bool	EqualsC(char * txtI, char * txtII);

#ifdef	_UNICODE
wchar_t	* __copy__(wchar_t * txt);
#else
char	* __copy__(char * txt);
#endif

double	__abs__(double value);



#undef int