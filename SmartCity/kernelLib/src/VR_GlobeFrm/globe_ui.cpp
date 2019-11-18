#include "globe_ui.h"
#include "GeneratedFiles/ui_ImageTransparentUI.h"
#include "GeneratedFiles/ui_modelTransformUI.h"
#include "geoservice/modelreductionservice.h"
#include "citybuilder/instancemodellayer.h"
#include "citybuilder/instancemodellayer.h"
#include "pipenet/pipepoint.h"
#include "pipenet/pipeline.h"
#include <QFileDialog>

ImageTransparentDlg::ImageTransparentDlg(QWidget *parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
	: QDialog(parent, f)
{
	this->ui = new Ui::ImageTransparentUI();
	ui->setupUi(this);
}

ImageTransparentDlg::~ImageTransparentDlg()
{

}

void ImageTransparentDlg::setLayer(osgEarth::ImageLayer* webImageLayer)
{
	this->WebImageLayer = webImageLayer;
	if (this->WebImageLayer)
	{
		float transparent = (1 - this->WebImageLayer->getOpacity()) * 100.0;
		QString str = QString::number((int)transparent) + " %";
		this->ui->label_2->setText(str);
		this->ui->transparentValue->setValue((int)transparent);
	}
}

void ImageTransparentDlg::slotTransparentValueChanged(int value)
{
	if (this->WebImageLayer)
	{
		float transparent = 1 - value / 100.0;
		QString str = QString::number(value) + " %";
		this->ui->label_2->setText(str);
		this->WebImageLayer->setOpacity(transparent);
	}
}


ModelTransformDlg::ModelTransformDlg(QWidget *parent , Qt::WindowFlags f )
	: QDialog(parent, f)
{
	this->ui = new Ui::ModelTransformUI();
	ui->setupUi(this);

	
}

ModelTransformDlg::~ModelTransformDlg()
{
	
}

void ModelTransformDlg::accept()
{
	Service::CModelReductionService serviceTool;
	serviceTool.run();
}

void ModelTransformDlg::slotSelectModelDestPath()
{
	QString modelPath = QFileDialog::getSaveFileName(this, tr("save model file"), " ",  tr("tif(*.*)")); 
	QFileInfo fi;
	fi = QFileInfo(modelPath);
	QString fileName = fi.fileName();
	//ui->domNameGdalBtn->setText(fileName);
	ui->modelDestPath->setText(modelPath);
}

void ModelTransformDlg::slotSelectModelSrcPath()
{
	QString modelPath = QFileDialog::getOpenFileName(this, tr("open model file"), " ",  tr("tif(*.*)")); 
	QFileInfo fi;
	fi = QFileInfo(modelPath);
	QString fileName = fi.fileName();
	ui->modelSrcPath->setText(modelPath);

	if (fileName != "")
	{
		QString linePath = "";
		PipeNet::CPipeLineDataSet* pLineDataSet = NULL;
		if (CityBuilder::CPipeLayerDriver::load(modelPath.toLocal8Bit().constData(),pLineDataSet))
		{

		}
		QString pointPath = "";
		PipeNet::CPipePointDataSet* pPointDataSet = NULL;
		if (CityBuilder::CPipeLayerDriver::load(modelPath.toLocal8Bit().constData(), pPointDataSet))
		{

		}

		if (pLineDataSet && pPointDataSet)
		{
			for (int i = 0; i < pPointDataSet->getPipePoints().size(); ++i)
			{
				PipeNet::CPipePoint* pPipePoint = pPointDataSet->getPipePoints().at(i);
				std::vector<PipeNet::CPipeLine*> inout_pipeLines;
				if (pLineDataSet->getPipeLines(pPipePoint->getID(), inout_pipeLines))
				{
					pPipePoint->setAdjcentLines(inout_pipeLines);
				}
			}
		}
	}
}