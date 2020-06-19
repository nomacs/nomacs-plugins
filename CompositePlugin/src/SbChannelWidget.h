#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QFileInfo>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QElapsedTimer>
#include <QPushButton>
#include <QFileDialog>
#include <QSettings>
#include <QDir>

#include <opencv2/opencv.hpp>
#include "DkImageStorage.h"
#include "DkBasicLoader.h"

namespace nmc {

	class SbChannelWidget : public QWidget {
		Q_OBJECT

	public:
		enum Channel {
			R, G, B
		};


		SbChannelWidget(Channel c, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
		~SbChannelWidget();

		const Channel c;

		cv::Mat getImg();
		void setImg(cv::Mat _img = cv::Mat(), QString _name="");


	protected:
		static const int DISP_IMG_MAX_SIZE = 150;

		cv::Mat img;
		QPushButton* thumbnail;
		QLabel* filenameLabel;

		void loadImage(QString file = "");
		void buildUI();
		void setThumbnail(cv::Mat img);

		void dropEvent(QDropEvent* event) override;
		void dragEnterEvent(QDragEnterEvent* event) override;

	public slots:
		void onPushButtonInvert();
		void onClickThumbnail();

	signals:
		void imageChanged(int channel);
		void newAlpha(QImage alpha);
	};


};