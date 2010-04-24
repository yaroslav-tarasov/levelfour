/********************************************************************************
** Form generated from reading UI file 'TableViewPanel.ui'
**
** Created: Fri 23. Apr 21:39:22 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLEVIEWPANEL_H
#define UI_TABLEVIEWPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TableViewPanel
{
public:
    QAction *ui_filterAction;
    QAction *ui_geometryAction;
    QAction *ui_materialsAction;
    QAction *ui_camerasAction;
    QAction *ui_lightsAction;
    QAction *ui_imagesAction;
    QAction *ui_resetFilterAction;
    QVBoxLayout *vboxLayout;
    QTableView *tableView;

    void setupUi(QWidget *TableViewPanel)
    {
        if (TableViewPanel->objectName().isEmpty())
            TableViewPanel->setObjectName(QString::fromUtf8("TableViewPanel"));
        TableViewPanel->resize(229, 128);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TableViewPanel->sizePolicy().hasHeightForWidth());
        TableViewPanel->setSizePolicy(sizePolicy);
        ui_filterAction = new QAction(TableViewPanel);
        ui_filterAction->setObjectName(QString::fromUtf8("ui_filterAction"));
        ui_filterAction->setCheckable(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/filterIcon"), QSize(), QIcon::Normal, QIcon::Off);
        ui_filterAction->setIcon(icon);
        ui_geometryAction = new QAction(TableViewPanel);
        ui_geometryAction->setObjectName(QString::fromUtf8("ui_geometryAction"));
        ui_geometryAction->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/geometryIcon"), QSize(), QIcon::Normal, QIcon::Off);
        ui_geometryAction->setIcon(icon1);
        ui_materialsAction = new QAction(TableViewPanel);
        ui_materialsAction->setObjectName(QString::fromUtf8("ui_materialsAction"));
        ui_materialsAction->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/materialsIcon"), QSize(), QIcon::Normal, QIcon::Off);
        ui_materialsAction->setIcon(icon2);
        ui_camerasAction = new QAction(TableViewPanel);
        ui_camerasAction->setObjectName(QString::fromUtf8("ui_camerasAction"));
        ui_camerasAction->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/cameraIcon"), QSize(), QIcon::Normal, QIcon::Off);
        ui_camerasAction->setIcon(icon3);
        ui_lightsAction = new QAction(TableViewPanel);
        ui_lightsAction->setObjectName(QString::fromUtf8("ui_lightsAction"));
        ui_lightsAction->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/lightsIcon"), QSize(), QIcon::Normal, QIcon::Off);
        ui_lightsAction->setIcon(icon4);
        ui_imagesAction = new QAction(TableViewPanel);
        ui_imagesAction->setObjectName(QString::fromUtf8("ui_imagesAction"));
        ui_imagesAction->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/imagesIcon"), QSize(), QIcon::Normal, QIcon::Off);
        ui_imagesAction->setIcon(icon5);
        ui_resetFilterAction = new QAction(TableViewPanel);
        ui_resetFilterAction->setObjectName(QString::fromUtf8("ui_resetFilterAction"));
        ui_resetFilterAction->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resetFilterIcon"), QSize(), QIcon::Normal, QIcon::Off);
        ui_resetFilterAction->setIcon(icon6);
        vboxLayout = new QVBoxLayout(TableViewPanel);
        vboxLayout->setSpacing(3);
        vboxLayout->setContentsMargins(1, 1, 1, 1);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        tableView = new QTableView(TableViewPanel);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        vboxLayout->addWidget(tableView);


        retranslateUi(TableViewPanel);

        QMetaObject::connectSlotsByName(TableViewPanel);
    } // setupUi

    void retranslateUi(QWidget *TableViewPanel)
    {
        TableViewPanel->setWindowTitle(QApplication::translate("TableViewPanel", "Table View", 0, QApplication::UnicodeUTF8));
        ui_filterAction->setText(QApplication::translate("TableViewPanel", "Filter", 0, QApplication::UnicodeUTF8));
        ui_geometryAction->setText(QApplication::translate("TableViewPanel", "Geometry", 0, QApplication::UnicodeUTF8));
        ui_geometryAction->setIconText(QApplication::translate("TableViewPanel", "Geometry", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ui_geometryAction->setToolTip(QApplication::translate("TableViewPanel", "Geometry", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ui_materialsAction->setText(QApplication::translate("TableViewPanel", "Materials", 0, QApplication::UnicodeUTF8));
        ui_camerasAction->setText(QApplication::translate("TableViewPanel", "Cameras", 0, QApplication::UnicodeUTF8));
        ui_lightsAction->setText(QApplication::translate("TableViewPanel", "Lights", 0, QApplication::UnicodeUTF8));
        ui_imagesAction->setText(QApplication::translate("TableViewPanel", "Images", 0, QApplication::UnicodeUTF8));
        ui_imagesAction->setIconText(QApplication::translate("TableViewPanel", "Images", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ui_imagesAction->setToolTip(QApplication::translate("TableViewPanel", "Images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ui_resetFilterAction->setText(QApplication::translate("TableViewPanel", "Reset Filter", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TableViewPanel: public Ui_TableViewPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLEVIEWPANEL_H
