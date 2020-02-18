/********************************************************************************
** Form generated from reading UI file 'formgps.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMGPS_H
#define UI_FORMGPS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormGPS
{
public:
    QAction *actionOpen;
    QAction *actionOptions;
    QAction *actionReset_All;
    QAction *actionColors;
    QAction *actionGrid_On;
    QAction *actionLightBar_On;
    QAction *actionLog_NMEA;
    QAction *actionPursuit_Line;
    QAction *actionSky_On;
    QAction *actionPolygons_On;
    QAction *actionExtra_Guides;
    QAction *actionPorts;
    QAction *actionSettings;
    QAction *actionLoad_Vehicle;
    QAction *actionSave_Vehicle;
    QAction *actionStart_field;
    QAction *actionMetric;
    QAction *actionImperial;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *grnPixels;
    QMenuBar *menuBar;
    QMenu *menuOptions;
    QMenu *menuUnits;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FormGPS)
    {
        if (FormGPS->objectName().isEmpty())
            FormGPS->setObjectName(QStringLiteral("FormGPS"));
        FormGPS->resize(1024, 700);
        actionOpen = new QAction(FormGPS);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOptions = new QAction(FormGPS);
        actionOptions->setObjectName(QStringLiteral("actionOptions"));
        actionReset_All = new QAction(FormGPS);
        actionReset_All->setObjectName(QStringLiteral("actionReset_All"));
        actionColors = new QAction(FormGPS);
        actionColors->setObjectName(QStringLiteral("actionColors"));
        actionGrid_On = new QAction(FormGPS);
        actionGrid_On->setObjectName(QStringLiteral("actionGrid_On"));
        actionGrid_On->setCheckable(true);
        actionGrid_On->setChecked(true);
        actionLightBar_On = new QAction(FormGPS);
        actionLightBar_On->setObjectName(QStringLiteral("actionLightBar_On"));
        actionLightBar_On->setCheckable(true);
        actionLightBar_On->setChecked(true);
        actionLog_NMEA = new QAction(FormGPS);
        actionLog_NMEA->setObjectName(QStringLiteral("actionLog_NMEA"));
        actionLog_NMEA->setCheckable(true);
        actionPursuit_Line = new QAction(FormGPS);
        actionPursuit_Line->setObjectName(QStringLiteral("actionPursuit_Line"));
        actionPursuit_Line->setCheckable(true);
        actionPursuit_Line->setChecked(true);
        actionSky_On = new QAction(FormGPS);
        actionSky_On->setObjectName(QStringLiteral("actionSky_On"));
        actionSky_On->setCheckable(true);
        actionSky_On->setChecked(true);
        actionPolygons_On = new QAction(FormGPS);
        actionPolygons_On->setObjectName(QStringLiteral("actionPolygons_On"));
        actionPolygons_On->setCheckable(true);
        actionPolygons_On->setChecked(true);
        actionExtra_Guides = new QAction(FormGPS);
        actionExtra_Guides->setObjectName(QStringLiteral("actionExtra_Guides"));
        actionExtra_Guides->setCheckable(true);
        actionExtra_Guides->setChecked(true);
        actionPorts = new QAction(FormGPS);
        actionPorts->setObjectName(QStringLiteral("actionPorts"));
        actionSettings = new QAction(FormGPS);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        actionLoad_Vehicle = new QAction(FormGPS);
        actionLoad_Vehicle->setObjectName(QStringLiteral("actionLoad_Vehicle"));
        actionSave_Vehicle = new QAction(FormGPS);
        actionSave_Vehicle->setObjectName(QStringLiteral("actionSave_Vehicle"));
        actionStart_field = new QAction(FormGPS);
        actionStart_field->setObjectName(QStringLiteral("actionStart_field"));
        actionMetric = new QAction(FormGPS);
        actionMetric->setObjectName(QStringLiteral("actionMetric"));
        actionMetric->setCheckable(true);
        actionMetric->setChecked(true);
        actionImperial = new QAction(FormGPS);
        actionImperial->setObjectName(QStringLiteral("actionImperial"));
        actionImperial->setCheckable(true);
        centralWidget = new QWidget(FormGPS);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        grnPixels = new QLabel(centralWidget);
        grnPixels->setObjectName(QStringLiteral("grnPixels"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(grnPixels->sizePolicy().hasHeightForWidth());
        grnPixels->setSizePolicy(sizePolicy);
        grnPixels->setMinimumSize(QSize(400, 200));

        horizontalLayout->addWidget(grnPixels);


        verticalLayout->addLayout(horizontalLayout);

        FormGPS->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FormGPS);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 22));
        menuBar->setNativeMenuBar(true);
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuUnits = new QMenu(menuOptions);
        menuUnits->setObjectName(QStringLiteral("menuUnits"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        FormGPS->setMenuBar(menuBar);
        statusBar = new QStatusBar(FormGPS);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FormGPS->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuOptions->addAction(actionReset_All);
        menuOptions->addSeparator();
        menuOptions->addAction(actionColors);
        menuOptions->addAction(menuUnits->menuAction());
        menuOptions->addAction(actionGrid_On);
        menuOptions->addAction(actionLightBar_On);
        menuOptions->addAction(actionLog_NMEA);
        menuOptions->addAction(actionPursuit_Line);
        menuOptions->addAction(actionSky_On);
        menuOptions->addAction(actionPolygons_On);
        menuOptions->addAction(actionExtra_Guides);
        menuOptions->addSeparator();
        menuOptions->addAction(actionPorts);
        menuOptions->addSeparator();
        menuOptions->addAction(actionSettings);
        menuUnits->addAction(actionMetric);
        menuUnits->addAction(actionImperial);
        menuFile->addAction(actionLoad_Vehicle);
        menuFile->addAction(actionSave_Vehicle);
        menuFile->addSeparator();
        menuFile->addAction(actionStart_field);

        retranslateUi(FormGPS);

        QMetaObject::connectSlotsByName(FormGPS);
    } // setupUi

    void retranslateUi(QMainWindow *FormGPS)
    {
        FormGPS->setWindowTitle(QApplication::translate("FormGPS", "FormGPS", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("FormGPS", "Open", Q_NULLPTR));
        actionOptions->setText(QApplication::translate("FormGPS", "Options", Q_NULLPTR));
        actionReset_All->setText(QApplication::translate("FormGPS", "Reset All", Q_NULLPTR));
        actionColors->setText(QApplication::translate("FormGPS", "Colors", Q_NULLPTR));
        actionGrid_On->setText(QApplication::translate("FormGPS", "Grid On", Q_NULLPTR));
        actionLightBar_On->setText(QApplication::translate("FormGPS", "Lightbar On", Q_NULLPTR));
        actionLog_NMEA->setText(QApplication::translate("FormGPS", "Log NMEA", Q_NULLPTR));
        actionPursuit_Line->setText(QApplication::translate("FormGPS", "Pursuit Line", Q_NULLPTR));
        actionSky_On->setText(QApplication::translate("FormGPS", "Sky On", Q_NULLPTR));
        actionPolygons_On->setText(QApplication::translate("FormGPS", "Polygons On", Q_NULLPTR));
        actionExtra_Guides->setText(QApplication::translate("FormGPS", "Extra Guides", Q_NULLPTR));
        actionPorts->setText(QApplication::translate("FormGPS", "Ports", Q_NULLPTR));
        actionSettings->setText(QApplication::translate("FormGPS", "Settings", Q_NULLPTR));
        actionLoad_Vehicle->setText(QApplication::translate("FormGPS", "Load Vehicle", Q_NULLPTR));
        actionSave_Vehicle->setText(QApplication::translate("FormGPS", "Save Vehicle", Q_NULLPTR));
        actionStart_field->setText(QApplication::translate("FormGPS", "Start field", Q_NULLPTR));
        actionMetric->setText(QApplication::translate("FormGPS", "Metric", Q_NULLPTR));
        actionImperial->setText(QApplication::translate("FormGPS", "Imperial", Q_NULLPTR));
        grnPixels->setText(QString());
        menuOptions->setTitle(QApplication::translate("FormGPS", "Options", Q_NULLPTR));
        menuUnits->setTitle(QApplication::translate("FormGPS", "Units", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("FormGPS", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormGPS: public Ui_FormGPS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMGPS_H
