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
#include <QSlider>
#include <QStyleOptionSlider>
#include <QToolTip>

#include <opencv2/opencv.hpp>
#include "DkImageStorage.h"
#include "DkBasicLoader.h"

namespace nmc {

	// a slider that dynmically displays the current value in a tooltip.
	// https://stackoverflow.com/questions/18383885/qslider-show-min-max-and-current-value
	class SbIntensitySlider : public QSlider {
		Q_OBJECT
	public:
		SbIntensitySlider(QWidget* parent = 0) : QSlider(parent) {}
		SbIntensitySlider(Qt::Orientation orientation, QWidget* parent = 0) : QSlider(orientation, parent) {}


	protected:
		virtual void sliderChange(SliderChange change)
		{
			QSlider::sliderChange(change);

			if (change == QAbstractSlider::SliderValueChange)
			{
				QStyleOptionSlider opt;
				initStyleOption(&opt);

				QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
				QPoint bottomRightCorner = sr.bottomLeft();

				QToolTip::showText(mapToGlobal(QPoint(bottomRightCorner.x(), bottomRightCorner.y())), QString("%1%").arg(value()), this);
			}
		}
	};




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
		static const int INT_SLIDER_MIN = 0;
		static const int INT_SLIDER_MAX = 200;
		static const int INT_SLIDER_INIT = 100;

		cv::Mat img;
		QPushButton* thumbnail;
		QLabel* filenameLabel;
		SbIntensitySlider* intSlider;

		void loadImage(QString file = "");
		void buildUI();
		void updateThumbnail();

		void dropEvent(QDropEvent* event) override;
		void dragEnterEvent(QDragEnterEvent* event) override;

	public slots:
		void onPushButtonInvert();
		void onClickThumbnail();
		void onIntensityChange();

	signals:
		void imageChanged(int channel);
		void newAlpha(QImage alpha);
	};


};