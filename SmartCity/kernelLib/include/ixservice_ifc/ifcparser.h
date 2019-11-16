#ifndef _IFC_PARSER_CONGIF_H
#define _IFC_PARSER_CONGIF_H

#include "ixservice_ifc/ixifcconfig.h"
#include "osg/group"

class IXSERVICE_IFC_DLL CIfcParser
{
public:
	CIfcParser();

	virtual ~CIfcParser();

	osg::Group* parse(const std::string& in_ifcPath);
};

#endif