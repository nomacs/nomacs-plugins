/*******************************************************************************************************
 SbCompositePlugin.h

 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances

 Copyright (C) 2011-2015 Markus Diem <markus@nomacs.org>

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

#pragma once

#include "DkPluginInterface.h"
#include "DkImageStorage.h"
#include "SbChannelWidget.h"
#include <opencv2/opencv.hpp>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QStyle>


namespace nmc {

class SbCompositeDockWidget : public QDockWidget {
	Q_OBJECT
public:
	SbCompositeDockWidget(const QString& title, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : QDockWidget(title, parent, flags) {
		setObjectName("CompositeDockWidget");	// fixes saving...
	}
	~SbCompositeDockWidget() {}
protected:
	void closeEvent(QCloseEvent* event) {
		emit closed();
	}
signals:
	void closed();
};


class SbViewPort : public DkPluginViewPort {

public:
	SbViewPort(QWidget* parent = 0) : DkPluginViewPort(parent) {};

	void updateImageContainer(QSharedPointer<DkImageContainerT> imgC) override {

		if (!imgC)
			return;

		qDebug() << "aber hallo: " << imgC->fileName();
	}

};


class SbCompositePlugin : public QObject, DkViewPortInterface {
	Q_OBJECT
	Q_INTERFACES(nmc::DkViewPortInterface)
	Q_PLUGIN_METADATA(IID "com.nomacs.ImageLounge.SbCompositePlugin/0.1" FILE "SbCompositePlugin.json")

public:

	SbCompositePlugin(QObject* parent = 0);
	~SbCompositePlugin();

	QImage image() const override;
	QSharedPointer<nmc::DkImageContainer> runPlugin(const QString &runID = QString(), QSharedPointer<nmc::DkImageContainer> imgC = QSharedPointer<nmc::DkImageContainer>()) const override;

	//DkViewPortInterface
	// return false here if you have a simple viewport (no children)
	// and you want the user to be able to e.g. scroll thumbs while your plugin is active
	bool hideHUD() const override { return true; }
	bool createViewPort(QWidget* parent) override;
	DkPluginViewPort* getViewPort() override;

	virtual void setVisible(bool visible) override;
	virtual bool closesOnImageChange() { return false; }

protected:
	SbCompositeDockWidget* dockWidget = 0;
	QWidget* mainWidget = 0;
	QVector<SbChannelWidget*> channelWidgets;
	SbViewPort* viewport = 0;
	cv::Mat channels[3];
	bool apply = false;

	void buildUI();

public slots:
	void onImageChanged(int channel);
	void onDockWidgetClose();
	void onPushButtonApply();
	void onPushButtonCancel();

};

};
