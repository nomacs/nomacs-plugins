/*******************************************************************************************************
 DkFilter.cpp

 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances

 Copyright (C) 2015 #YOUR_NAME

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************************************/

#include "DkFilter.h"

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QAction>
#pragma warning(pop)		// no warnings from includes - end

 /*******************************************************************************************************
  * DkFilter	- enter the plugin class name (e.g. DkPageExtractionPlugin)
  * #YOUR_NAME			- your name/pseudonym whatever
  * #DATE				- today...
  * #DESCRIPTION		- describe your plugin in one sentence
  * #MENU_NAME			- a user friendly name (e.g. Flip Image)
  * #MENU_STATUS_TIPP	- status tip of your plugin
  * #RUN_ID_1			- generate an ID using: GUID without hyphens generated at http://www.guidgenerator.com/
  * ID_GOTHAM			- your action name (e.g. id_flip_horizontally)
  * #ACTION_NAME1		- your action name (e.g. Flip Horizotally - user friendly!)
  * #ACTION_TIPP1		- your action status tip (e.g. Flips an image horizontally - user friendly!)
  *******************************************************************************************************/

namespace nmc {

/**
*	Constructor
**/
DkFilter::DkFilter(QObject* parent) : QObject(parent) {

	// create run IDs
	QVector<QString> runIds;
	runIds.resize(id_end);

	runIds[ID_GOTHAM] = "b35412d307a847218a1ddc1ff6ccb84b";
	runIds[ID_LOMO] = "920a6644a1914dc683cd5b10f51c61eb";
	runIds[ID_KELVIN] = "d9d794b65fae4874846bdc2937e1a81b";
	runIds[ID_SEPIA] = "4863aa921e9d4c52a0f60ded4b952cf3";
	mRunIDs = runIds.toList();

	// create menu actions
	QVector<QString> menuNames;
	menuNames.resize(id_end);

	menuNames[ID_GOTHAM] = tr("Gotham");
	menuNames[ID_LOMO] = tr("Lomo(without vignette)");
	menuNames[ID_KELVIN] = tr("Kelvin");
	menuNames[ID_SEPIA] = tr("Sepia");

	mMenuNames = menuNames.toList();

	// create menu status tips
	QVector<QString> statusTips;
	statusTips.resize(id_end);

	statusTips[ID_GOTHAM] = tr("Applies Instagram Gotham Filter to image");
	statusTips[ID_LOMO] = tr("Applies Instagram Lomo(without vignette) Filter to image,");
	statusTips[ID_KELVIN] = tr("Applies Instagram Kelvin Filter to image");
	statusTips[ID_SEPIA] = tr("Applies Sepia Filter to image");
	mMenuStatusTips = statusTips.toList();
}
/**
*	Destructor
**/
DkFilter::~DkFilter() {
}


/**
* Returns unique ID for the generated dll
**/
QString DkFilter::id() const {

	return PLUGIN_ID;
};

/**
* Returns descriptive iamge for every ID
* @param plugin ID
**/
QImage DkFilter::image() const {

	return QImage(":/FilterPlugin/img/instagram.png");
};

/**
* Returns plugin version for every ID
* @param plugin ID
**/
QString DkFilter::version() const {

	return PLUGIN_VERSION;
};

QList<QAction*> DkFilter::createActions(QWidget* parent) {

	if (mActions.empty()) {

		for (int idx = 0; idx < id_end; idx++) {
			QAction* ca = new QAction(mMenuNames[idx], parent);
			ca->setObjectName(mMenuNames[idx]);
			ca->setStatusTip(mMenuStatusTips[idx]);
			ca->setData(mRunIDs[idx]);	// runID needed for calling function runPlugin()
			mActions.append(ca);
		}
	}

	return mActions;
}

QList<QAction*> DkFilter::pluginActions() const {

	return mActions;
}

/**
* Main function: runs plugin based on its ID
* @param plugin ID
* @param image to be processed
**/
QSharedPointer<nmc::DkImageContainer> DkFilter::runPlugin(const QString &runID, QSharedPointer<nmc::DkImageContainer> imgC) const {

	if (!imgC)
		return imgC;
	#ifdef WITH_OPENCV
		if(runID == mRunIDs[ID_GOTHAM]) {
			QImage return_img(imgC->image());
			return_img = DkFilter::applyGothamFilter(imgC->image()) ;
			imgC->setImage(return_img, tr("Gotham"));

		}
		else if (runID == mRunIDs[ID_LOMO]){
			QImage return_img(imgC->image());
			return_img = DkFilter::applyLomoFilter(imgC->image()) ;
			imgC->setImage(return_img, tr("Lomo"));
		}
		else if (runID == mRunIDs[ID_KELVIN]){
			QImage return_img(imgC->image());
			return_img = DkFilter::applyKelvinFilter(imgC->image()) ;
			imgC->setImage(return_img, tr("Kelvin"));
		}
		else if (runID == mRunIDs[ID_SEPIA]){
			QImage return_img(imgC->image());
			return_img = DkFilter::applySepiaFilter(imgC->image()) ;
			imgC->setImage(return_img, tr("Sepia"));
		}
		else
			qWarning() << "Illegal run ID...";
		return imgC;
	#else
		return imgC;
	#endif
};

QImage DkFilter::applyGothamFilter(const QImage inImg){
	cv::Mat matInImg=nmc::DkImage::qImage2Mat(inImg);

	cv::Mat matResImg;
	//increasing contrast by 20%  
	matInImg.convertTo(matResImg,-1,1.2,0); 
	
	//Desaturate to 10% by changing to HSV and decresaing S channel to 10%
	cv::cvtColor(matResImg,matResImg,cv::COLOR_BGR2HSV);

	int nChnls = matResImg.channels();
	int indexS = 1;
    int nRows = matResImg.rows;
    int nCols = matResImg.cols;
    uchar* p;
    for(int i=0;i< nRows;i++){
    	p = matResImg.ptr<uchar>(i);
    	for(int j = 0; j < nCols; j++){
    		p[nChnls*j+indexS] = 0.1*p[nChnls*j+indexS];
    	}
    }
	// HSV back to BGR
	cv::cvtColor(matResImg, matResImg, cv::COLOR_HSV2BGR);

	//give light purple shade
	for (int i=0; i < nRows ; i++)
	{
	  p = matResImg.ptr<uchar>(i);
      for(int j=0; j < nCols; j++)
      {
      		
      		p[nChnls*j+2]= 0.13*p[nChnls*j+2]+0.17*p[nChnls*j+1]+0.43*p[nChnls*j];
      		p[nChnls*j+1]= 0.12*p[nChnls*j+2]+0.16*p[nChnls*j+1]+0.42*p[nChnls*j];
      		p[nChnls*j]= 0.11*p[nChnls*j+2]+0.18*p[nChnls*j+1]+0.42*p[nChnls*j];

      }
	}
	
	matResImg.convertTo(matResImg,-1,1.2,0); 
	QImage resImg = nmc::DkImage::mat2QImage(matResImg);
	return resImg;
};

QImage DkFilter::applyLomoFilter(const QImage inImg){
	cv::Mat matResImg=nmc::DkImage::qImage2Mat(inImg);

	std::vector<cv::Mat> channels(3);
	cv::split(matResImg, channels);
	
	channels[2].convertTo(channels[2],-1,1.33,0);
	channels[1].convertTo(channels[1],-1,1.33,0);
	// cv::normalize(channels[2],channels[2],85,255,cv::NORM_MINMAX);
	// cv::normalize(channels[1],channels[1],85,255,cv::NORM_MINMAX);
	cv::merge(channels,matResImg);
	QImage resImg = nmc::DkImage::mat2QImage(matResImg);
	return resImg;
	
};

QImage DkFilter::applyKelvinFilter(const QImage inImg){
	cv::Mat matResImg=nmc::DkImage::qImage2Mat(inImg);

	//enhance contrast by 20%
	matResImg.convertTo(matResImg,-1,1.2,0);

	std::vector<cv::Mat> channels(3);
	
	//Overlay a Peach color image
	cv::Mat blendImg; 
	blendImg.create(matResImg.size(),matResImg.type());
	blendImg.setTo(cv::Scalar(0,153,255));
	cv::addWeighted(matResImg, 0.6, blendImg, 0.4 , 0.0, matResImg); 
	
	QImage resImg = nmc::DkImage::mat2QImage(matResImg);
	return resImg;

};

QImage DkFilter::applySepiaFilter(const QImage inImg){
	cv::Mat matResImg=nmc::DkImage::qImage2Mat(inImg);


	int nRows = matResImg.rows;
    int nCols = matResImg.cols;
    int nChnls = matResImg.channels();
    uchar* p;
	for (int i=0; i < nRows ; i++)
	{
	  p = matResImg.ptr<uchar>(i);
      for(int j=0; j < nCols; j++)
      {
    
      		p[nChnls*j+2]= std::min(0.393*p[nChnls*j+2]+0.769*p[nChnls*j+1]+0.189*p[nChnls*j],255.0);
      		p[nChnls*j+1]= std::min(0.349*p[nChnls*j+2]+0.686*p[nChnls*j+1]+0.168*p[nChnls*j],255.0);
      		p[nChnls*j]= std::min(0.272*p[nChnls*j+2]+0.534*p[nChnls*j+1]+0.131*p[nChnls*j],255.0);

      }
	}
	
	QImage resImg = nmc::DkImage::mat2QImage(matResImg);
	return resImg;
};

};

