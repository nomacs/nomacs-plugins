#include "SbChannelWidget.h"

#include "DkTimer.h"

namespace nmc {
	SbChannelWidget::SbChannelWidget(Channel c, QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), c(c)
	{
		setAcceptDrops(true); 
		buildUI();
		setImg();
	}

	SbChannelWidget::~SbChannelWidget()
	{
	}

	cv::Mat SbChannelWidget::getImg()
	{
		return img;
	}

	void SbChannelWidget::setImg(cv::Mat _img, QString _name)
	{
		//reset channel to empty
		img = _img;
		if (img.empty())
			setThumbnail(cv::Mat::ones(cv::Size(DISP_IMG_MAX_SIZE, DISP_IMG_MAX_SIZE), CV_8UC1) * 255);
		else
			setThumbnail(img);
		filenameLabel->setText(_name);
	}


	void SbChannelWidget::loadImage(QString file) {

		//load image content for channel
		DkBasicLoader bl;
		bool couldLoad = bl.loadGeneral(file);

		if (couldLoad) {
			// much faster but not that stable:
			//img = cv::imread(file.toStdString(), cv::IMREAD_GRAYSCALE);
			
			auto gImg = DkImage::grayscaleImage(bl.image());

			img = DkImage::qImage2Mat(gImg);
			cv::cvtColor(img, img, CV_RGB2GRAY);

			setThumbnail(img);
			QFileInfo fi(file);
			filenameLabel->setText(fi.fileName());
			
			emit imageChanged(c);
		}
		else {
			qDebug() << "could not load:" << file;
		}

	}

	void SbChannelWidget::buildUI()
	{
		QVBoxLayout* outerLayout = new QVBoxLayout(this);
		outerLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

		thumbnail = new QPushButton();
		thumbnail->setFlat(true);
		thumbnail->setIconSize(QSize(150, 150));
		connect(thumbnail, SIGNAL(released()), this, SLOT(onClickThumbnail()));
		filenameLabel = new QLabel();
		QPushButton* invertButton = new QPushButton("invert");
		connect(invertButton, SIGNAL(released()), this, SLOT(onPushButtonInvert()));

		outerLayout->addWidget(thumbnail);
		outerLayout->addWidget(filenameLabel);
		outerLayout->addWidget(invertButton);

		//setLayout(outerLayout);
	}
	void SbChannelWidget::setThumbnail(cv::Mat img)
	{
		int s = std::max(img.rows, img.cols);
		double f = (double)DISP_IMG_MAX_SIZE / (double)s;
		cv::Mat imgScaled;
		cv::resize(img, imgScaled, cv::Size(), f, f);
		
		cv::Mat black = cv::Mat::zeros(cv::Size(imgScaled.cols, imgScaled.rows), imgScaled.type());
		cv::Mat channels[3] = { black, black, black };
		channels[c] = imgScaled;

		cv::Mat imgTinted;
		cv::merge(channels, 3, imgTinted);

		QImage qimg = DkImage::mat2QImage(imgTinted);
		QPixmap pxm = QPixmap::fromImage(qimg);
		thumbnail->setIcon(pxm);
	}
	void SbChannelWidget::dropEvent(QDropEvent* event)
	{
		qDebug() << "you dropped something there!";
		QList<QUrl> urls = event->mimeData()->urls();
		if (urls.length() < 1)
			return;
		if (urls.length() > 1)
			qDebug() << "you droped more than 1 files. I am taking the first one I find..";
		QUrl url = urls[0];
		QString s = url.toLocalFile();
		loadImage(s);
	}

	void SbChannelWidget::dragEnterEvent(QDragEnterEvent* event)
	{
		if (event->mimeData()->hasUrls()) {
			event->acceptProposedAction();
		}
	}

	void SbChannelWidget::onClickThumbnail()
	{
		const QString FILE_LOC_KEY = "comp_last_file_loc";
		QSettings settings; 


		QString fileName = QFileDialog::getOpenFileName(this, "Open Image File", settings.value(FILE_LOC_KEY).toString());
		if (!fileName.isEmpty()) {
			QFileInfo finf(fileName);
			settings.setValue(FILE_LOC_KEY, QDir().absoluteFilePath(fileName));
			loadImage(fileName);
		}
	}

	void SbChannelWidget::onPushButtonInvert() {
		if (!img.empty()) {
			img = 255 - img;
			setThumbnail(img);
			emit imageChanged(c);
		}
	}
};