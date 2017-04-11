/*******************************************************************************************************
RulerSettingsDialog.h

nomacs is a fast and small image viewer with the capability of synchronizing multiple instances

Copyright (C) 2017 Pascal Plank

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
#include <qdialog.h>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace nmc {

	class RulerSettingsDialog : public QDialog {

		Q_OBJECT

	public:
		RulerSettingsDialog(QString path, QString tickdist, int refpoints, int threshold, QWidget* parent = 0, Qt::WindowFlags flags = 0);
		~RulerSettingsDialog();

		// Layout items
		QVBoxLayout *verticalLayout;
		QHBoxLayout *horizontalLayout;
		QLabel *chooseImg_label;
		QPushButton *chooseImg_btn;
		QHBoxLayout *horizontalLayout_2;
		QLabel *tickDist_label;
		QComboBox *tickDist_cb;
		QHBoxLayout *horizontalLayout_3;
		QLabel *refPoints_label;
		QComboBox *refPoints_cb;
		QHBoxLayout *horizontalLayout_4;
		QLabel *hessianThres_label;
		QSpinBox *hessianThres_sb;
		QHBoxLayout *horizontalLayout_5;
		QPushButton *help_btn;
		QPushButton *OK_btn;
		QPushButton *cancel_btn;

		bool isOkClicked();
		QString getTemplatePath();
		QString getTickDistance();
		int getReferencePoints();
		int getHessianThreshold();


	protected slots:
		void okPressed();
		void cancelPressed();
		void helpPressed();
		void chooseTemplatePressed();

	protected:
		void init();

		QString tmp_templatePath = "";
		QString tmp_tickDistance;
		int tmp_referencePoints;
		int tmp_hessianThreshold;

		bool isOkPressed;
	};
};