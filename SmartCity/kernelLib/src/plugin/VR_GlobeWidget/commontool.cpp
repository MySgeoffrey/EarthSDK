#include "commontool.h"
#include <QtCore/qmath.h> 
#include <QImage>
#include <QFile>
#include <QPainter>
#include <QDir>
#include <osgEarth/SpatialReference>
#include "osgDB/FileUtils"

using namespace osgEarth;

osgText::Text* CUtility::createText(const osg::Vec3& pos, const std::string& content, float size, const std::string &font)
{
	osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile(font);
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setDataVariance(osg::Object::DYNAMIC);
	text->setFont(g_font.get());
	text->setCharacterSize(size);
	text->setAxisAlignment(osgText::TextBase::XY_PLANE);
	text->setPosition(pos);
	setUTF8Text(text.get(), content);
	return text.release();
}

osg::Node* CUtility::CreatScreenText(const float &x, const float &y, const float &width,
	const float &height, const float &size, const std::string &content, const osg::Vec4 &color, const std::string &font)
{
	osg::ref_ptr<osg::Camera> hudCamera = createHUDCamera(0, width, 0, height);
	osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
	hudCamera->addChild(textGeode);
	osg::ref_ptr<osgText::Text>  text = createText(osg::Vec3(x, y, 0), content, size, font);
	text->setColor(color);

	/// 设置信息提示节点的属性，并关闭光照和深度测试
	osg::StateSet* stateset = textGeode->getOrCreateStateSet();
	/*stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);*/

	/// 将文本添加到绘制列表中
	textGeode->addDrawable(text.get());
	return hudCamera.release();
}

void CUtility::setUTF8Text(osgText::Text* text, const std::string& content)
{
	std::string utf8str = GBKToUTF8(content);
	text->setText(utf8str, osgText::String::ENCODING_UTF8);
}

std::string CUtility::GBKToUTF8(const std::string &strGBK)
{
	std::string strOutUTF8 = "";
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	WCHAR * str1 = new WCHAR[n * 2];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	std::wstring wstr(str1);
	//strOutUTF8 = WStrToStr(wstr);
	char* str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;

	delete[] str1;
	delete[] str2;
	str1 = NULL;
	str2 = NULL;

	return strOutUTF8;
}

osg::Camera* CUtility::createHUDCamera(double left, double right, double bottom, double top)
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);
	camera->setProjectionMatrix(osg::Matrix::ortho2D(left, right, bottom, top));
	camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return camera.release();
}


class ITileImageData
{
public:

	char * ImageData;
	int DataLength;
};

CImageCreator::CImageCreator()
{

}

CImageCreator::~CImageCreator()
{

}

QImage CImageCreator::createImage(
	osgEarth::GeoPoint& leftBottomGeoPoint,
	osgEarth::GeoPoint& rightTopGeoPoint,
	const ImageSourceType& imageSourceType)
{
	QImage result;
	const osgEarth::Profile* profile = osgEarth::Profile::create(
		osgEarth::SpatialReference::get("spherical-mercator"),
		MERC_MINX, MERC_MINY, MERC_MAXX, MERC_MAXY,
		1, 1);
	
	osgEarth::GeoPoint leftBottomPoint;
	osgEarth::GeoPoint rightTopPoint;

	leftBottomGeoPoint.transform(profile->getSRS(), leftBottomPoint);
	rightTopGeoPoint.transform(profile->getSRS(), rightTopPoint);
	{
		int firstLevel = 0;
		int dataLevel = 17;
		double firstLevelTileSpan = MERC_WIDTH;
		int rowDirection = 0;
		int currentTileLevel = dataLevel;
		osgEarth::GeoPoint tileOriginPosition(profile->getSRS(), MERC_MINX, MERC_MAXX);
		double currentTileSpan = firstLevelTileSpan / qPow(2, currentTileLevel - firstLevel);
		if (rowDirection == 0)
		{
			int rowCount = (int)(std::ceil(std::abs(MERC_WIDTH) / currentTileSpan));
			int colCount = (int)(std::ceil((std::abs(MERC_WIDTH) / currentTileSpan)));

			int relative_StartRow = (int)(std::abs(tileOriginPosition.y() - leftBottomPoint.y()) / currentTileSpan);
			int relative_StartCol = (int)(std::abs(tileOriginPosition.x() - leftBottomPoint.x()) / currentTileSpan);

			int relative_EndRow = (int)(std::abs(tileOriginPosition.y() - rightTopPoint.y()) / currentTileSpan);
			int relative_EndCol = (int)(std::abs(tileOriginPosition.x() - rightTopPoint.x()) / currentTileSpan);

			relative_StartRow = relative_StartRow >= rowCount ? relative_StartRow - 1 : relative_StartRow;
			relative_EndRow = relative_EndRow >= rowCount ? relative_EndRow - 1 : relative_EndRow;

			relative_StartCol = relative_StartCol >= colCount ? relative_StartCol - 1 : relative_StartCol;
			relative_EndCol = relative_EndCol >= colCount ? relative_EndCol - 1 : relative_EndCol;

			int realStartRow = relative_StartRow <= relative_EndRow ? relative_StartRow : relative_EndRow;
			int realStartCol = relative_StartCol <= relative_EndCol ? relative_StartCol : relative_EndCol;

			int realEndRow = relative_StartRow >= relative_EndRow ? relative_StartRow : relative_EndRow;
			int realEndCol = relative_StartCol >= relative_EndCol ? relative_StartCol : relative_EndCol;

			double startLongitude = tileOriginPosition.x() + realStartCol * currentTileSpan;
			double startLatitude = tileOriginPosition.y() - realStartRow * currentTileSpan;

			osgEarth::GeoPoint leftTopPoint(profile->getSRS(), leftBottomPoint.x(), rightTopPoint.y());
			osgEarth::GeoPoint rightBottomPoint(profile->getSRS(), rightTopPoint.x(), leftBottomPoint.y());
			
			int rowPanCount = (realEndRow - realStartRow + 1);
			int colPanCount = (realEndCol - realStartCol + 1);

			double rowPanSize = rowPanCount * currentTileSpan;
			double colPanSize = colPanCount * currentTileSpan;

			double colStartRate = std::abs(leftTopPoint.x() - startLongitude) / colPanSize;
			double rowStartRate = std::abs(leftTopPoint.y() - startLatitude) / rowPanSize;

			double colEndRate = std::abs(rightBottomPoint.x() - startLongitude) / colPanSize;
			double rowEndRate = std::abs(rightBottomPoint.y() - startLatitude) / rowPanSize;

			int imageTileSeriveStartRow = 0, imageTileSeriveStartCol = 0;
			//this->getStartRowCol(imageTileSeriveStartRow, imageTileSeriveStartCol, currentTileLevel);
			realStartRow += imageTileSeriveStartRow;
			realEndRow += imageTileSeriveStartRow;

			realStartCol += imageTileSeriveStartCol;
			realEndCol += imageTileSeriveStartCol;

			/*inout_relativeServiceTileInfos.setLevel(currentTileLevel);
			inout_relativeServiceTileInfos.setRowCount(rowPanCount);
			inout_relativeServiceTileInfos.setColCount(colPanCount);
			inout_relativeServiceTileInfos.setStartRowRatio(rowStartRate);
			inout_relativeServiceTileInfos.setStartColRatio(colStartRate);
			inout_relativeServiceTileInfos.setRowRatio(rowEndRate - rowStartRate);
			inout_relativeServiceTileInfos.setColRatio(colEndRate - colStartRate);
			inout_relativeServiceTileInfos.setStartRow(realStartRow);
			inout_relativeServiceTileInfos.setStartCol(realStartCol);*/

#if 1
			std::string cacheDir = osgDB::getCurrentWorkingDirectory() + "/tempCache";
			if (!osgDB::fileExists(cacheDir))
			{
				osgDB::makeDirectory(cacheDir);
			}
			std::map<std::string, ITileImageData*> tileImageDatas;
			for (int rowIndex = realStartRow;
				rowIndex <= realStartRow + rowPanCount - 1; ++rowIndex)
			{
				for (int colIndex = realStartCol;
					colIndex <= realStartCol + colPanCount - 1; ++colIndex)
				{
					std::string key = 
						QString::number(rowIndex).toStdString() + "_" 
						+ QString::number(colIndex).toStdString();

					unsigned int tile_x = colIndex, tile_y = rowIndex, lod = dataLevel;
					char str[200];

					sprintf(str, "http://mt2.google.cn/vt/lyrs=s@258000000&hl=zh-CN&gl=CN&src=app&x=%d&y=%d&z=%d&s=Ga\0", tile_x, tile_y, lod);
					osg::Image* pImage = osgEarth::URI(str).getImage();
					if (NULL != pImage)
					{
						std::string cachePath = osgDB::getCurrentWorkingDirectory() + "/tempCache/" + key + ".png";
						bool successful = osgDB::writeImageFile(*pImage, cachePath);
						//ITileImageData * pImageData = new ITileImageData();
						///*pImageData->ImageData = pData;
						//pImageData->DataLength = dataLength;*/
						//tileImageDatas[key] = pImageData;
					}
					else
					{
						return result;
					}
				}
			}

			QImage::Format format = QImage::Format_RGB32;
			{
				format = QImage::Format_ARGB32;
			}
			int tilePixelSize = 256;
			QImage mergedImage(tilePixelSize * colPanCount, tilePixelSize * rowPanCount, format);
			mergedImage.fill(QColor(0, 0, 0, 0));
			QPainter painter(&mergedImage);
			QBrush brush;
			brush.setColor(QColor(0, 0, 0, 0));
			painter.setBrush(brush);

			for (int rowIndex = realStartRow;
				rowIndex <= realStartRow + rowPanCount - 1; ++rowIndex)
			{
				for (int colIndex = realStartCol;
					colIndex <= realStartCol + colPanCount - 1; ++colIndex)
				{
					std::string key =
						QString::number(rowIndex).toStdString() + "_"
						+ QString::number(colIndex).toStdString(); 
					{
						std::string cachePath = osgDB::getCurrentWorkingDirectory() + "/tempCache/" + key + ".png";

						QImage tileImage(QString(cachePath.c_str()));
						int x = (colIndex - realStartCol) * tilePixelSize;
						int y = (rowIndex - realStartRow) * tilePixelSize;
						painter.drawImage(x, y, tileImage);
						QFile fileTemp(cachePath.c_str());
						fileTemp.remove();
					}
				}
			}

			int scale = 1;
			QImage scaledMergedImage = mergedImage.scaled(mergedImage.width() * scale, mergedImage.height() * scale);
			int rowStartPixelPosition = rowStartRate * scaledMergedImage.height();
			int colStartPixelPosition = colStartRate * scaledMergedImage.width();

			int rowEndPixelPosition = (rowStartRate + (rowEndRate - rowStartRate)) * scaledMergedImage.height();
			int colEndPixelPosition = (colStartRate + (colEndRate - colStartRate)) * scaledMergedImage.width();

			QImage requestTileImage = scaledMergedImage.copy(colStartPixelPosition, rowStartPixelPosition,
				colEndPixelPosition - colStartPixelPosition, rowEndPixelPosition - rowStartPixelPosition);

			return requestTileImage;
#endif
		}
	}
	return result;
}