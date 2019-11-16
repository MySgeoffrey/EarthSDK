#pragma execution_character_set("utf-8")

#include "ixservice_ifc/ifcparser.h"
////点选物体
//#include"PickIfcItem.h"
//IFCEngineDll
#include"IFCEngineInteract.h"
//#include "engine.h"
//#include "ifcengine.h"

CIfcParser::CIfcParser()
{

}

CIfcParser::~CIfcParser()
{

}

osg::Group* CIfcParser::parse(const std::string& in_ifcPath)
{
	wchar_t *strFilePath = L"D:\\gitReource\\EarthSDK\\SmartCity\\bin\\Release\\";
	wchar_t * ifcilePath = L"E:/dataService/bim/test.ifc";

	int i = wcslen(strFilePath);

	ifcSchemaName_IFC2X3 = new wchar_t[i + wcslen(L"IFC2X3_TC1.exp") + 1];
	memcpy(&ifcSchemaName_IFC2X3[0], strFilePath, (i + 1)*sizeof(wchar_t));
	memcpy(&ifcSchemaName_IFC2X3[i], L"IFC2X3_TC1.exp", (wcslen(L"IFC2X3_TC1.exp") + 1)*sizeof(wchar_t));

	ifcFileName = new wchar_t[512];
	memcpy(&ifcFileName[0], strFilePath, (i + 1)*sizeof(wchar_t));
	xmlSettings_IFC2x3 = new wchar_t[i + wcslen(L"IFC2X3-Settings.xml") + 1];
	memcpy(&xmlSettings_IFC2x3[0], strFilePath, (i + 1)*sizeof(wchar_t));
	memcpy(&xmlSettings_IFC2x3[i], L"IFC2X3-Settings.xml", (wcslen(L"IFC2X3-Settings.xml") + 1)*sizeof(wchar_t));

	setStringUnicode(true);
	int ifcModel = sdaiOpenModelBNUnicode(0, (char *)ifcilePath, (char *)ifcSchemaName_IFC2X3);
	if (ifcModel)
	{
		FILE *file = 0;
		wchar_t* fileSchema = 0;
		GetSPFFHeaderItem(ifcModel, 9, 0, sdaiUNICODE, (char **)&fileSchema);
		if (fileSchema == 0 
			|| contains(fileSchema, L"IFC2x3")
			|| contains(fileSchema, L"IFC2X3") 
			|| contains(fileSchema, L"IFC2x2") 
			|| contains(fileSchema, L"IFC2X2") 
			|| contains(fileSchema, L"IFC2x_")
			|| contains(fileSchema, L"IFC2X_") 
			|| contains(fileSchema, L"IFC20")) 
			_wfopen_s(&file, xmlSettings_IFC2x3, L"r");

		__int64 owlModel = 0;
		owlGetModel(ifcModel, &owlModel);
		initializeColor(owlModel);

		if (file)
		{
			bool hide;
			wchar_t buff[512], buffName[512];
			while (0 != fgetws(buff, 500, file))
			{
				if (charContains(buff, L"<object "))
				{
					charFill(buff, L"name", buffName);
					hide = charFind(buff, L"hide");
					RetrieveObjects(ifcModel, buffName, hide);
				}
			}
			fclose(file);
		}

		completeItemColorItems();
	}
	GenerateGeometry(ifcModel, baseIfcItem);
	GetRenderData(baseIfcItem);
	osg::Group* pGroup = 0;
	RenderFaces(baseIfcItem,0,&pGroup);
	int x = 0;
	return pGroup;
	/*if	(baseIfcItem) 
	{
		CreateTreeItem_ifcProject(ifcModel, &baseTreeItem);

		if	(baseTreeItem) 
		{
#ifdef	_UNICODE
			baseTreeItem->next = CreateTreeItem(0, L"Header Info", 0, L"", L"");
#else
			baseTreeItem->next = CreateTreeItem(0, "Header Info", 0, "", "");
#endif
			baseTreeItem->next->headerSet = GetHeaderInfo(ifcModel);
			baseTreeItem->next->select = ITEM_PROPERTY_SET;
			baseTreeItem->next->interfaceType = TYPE_IFCOBJECT_PROPERTYSET;
		}
	}*/
}