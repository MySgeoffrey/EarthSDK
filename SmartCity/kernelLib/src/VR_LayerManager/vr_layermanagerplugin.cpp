#include "vr_layermanager.h"

#include <QtCore/QtPlugin>
#include "vr_layermanagerplugin.h"


VR_LayerManagerPlugin::VR_LayerManagerPlugin(QObject *parent)
	: QObject(parent)
{
	initialized = false;
}

void VR_LayerManagerPlugin::initialize(QDesignerFormEditorInterface */*core*/)
{
	if (initialized)
		return;

	initialized = true;
}

bool VR_LayerManagerPlugin::isInitialized() const
{
	return initialized;
}

QWidget *VR_LayerManagerPlugin::createWidget(QWidget *parent)
{
	return new VR_LayerManager(parent);
}

QString VR_LayerManagerPlugin::name() const
{
	return "VR_LayerManager";
}

QString VR_LayerManagerPlugin::group() const
{
	return "My Plugins";
}

QIcon VR_LayerManagerPlugin::icon() const
{
	return QIcon();
}

QString VR_LayerManagerPlugin::toolTip() const
{
	return QString();
}

QString VR_LayerManagerPlugin::whatsThis() const
{
	return QString();
}

bool VR_LayerManagerPlugin::isContainer() const
{
	return false;
}

QString VR_LayerManagerPlugin::domXml() const
{
	return "<widget class=\"VR_LayerManager\" name=\"vR_LayerManager\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString VR_LayerManagerPlugin::includeFile() const
{
	return "vr_layermanager.h";
}

Q_PLUGIN_METADATA(vr_layermanager)
