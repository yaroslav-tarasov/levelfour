/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QApplication>
#include <GCF/ComponentFactory>
#include <GCF/ComponentLoaderComponent>
#include <GCF/AppAccessPointComponent>
#include <GCF/UpdaterComponent>
#include "MainWindowComponent.h"
#include "MainWindow.h"


#ifdef Q_WS_WIN
#include <QFile>
#include <QFont>

#endif

#define VTKD_MAJOR      2
#define VTKD_MINOR      8

using namespace GCF;
using namespace Components;

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    a.setApplicationName("Level Four");
    a.setApplicationVersion( QString("%0.%0.1").arg(VTKD_MAJOR).arg(VTKD_MINOR) );

#ifdef Q_WS_WIN
		
		a.setStyle ("plastique");
        // set styleSheet here...
        QFile qss("styles/levelfour.qss");
        qss.open(QFile::ReadOnly);
        a.setStyleSheet(qss.readAll());
        qss.close();
        // set font
        QFont newFont( "Tahoma" );
        newFont.setPointSize( 8 );
        newFont.setWeight( QFont::Normal );
        a.setFont(newFont);

#endif

    // Initialize icon resources
    // Q_INIT_RESOURCE(StdMainWindow);
    Q_INIT_RESOURCE(Main);

    // Initialize the GCF layer
    GCF::initialize();

    // Configure the GUI XML Directory
    QString appDir = a.applicationDirPath();
    QString guiXmlDir = QString("%1/guixml").arg(appDir);
    GCF::AbstractComponent::setGuiXmlFileDirectory(guiXmlDir);
    qDebug("GUI XML Dir: %s", qPrintable(guiXmlDir));

    // Register static components 
    GCF_REGISTER_COMPONENT(MainWindowComponent);

    // Create the host main-window
    GCF::AbstractComponent::setMenuStripEnabled(true);
    GCF::Components::MainWindowComponent::instance().setApplicationLogo(":/LevelFour/vtkdesigner.png");
    GCF::Components::MainWindowComponent::instance().setSplashImage(":/LevelFour/splash.png");
    GCF::Components::MainWindowComponent::instance().initialize();

    // Start the updater component immediately to check for updates..
    QString updatesDir("http://vclserver.com/updates/vtkdesigner/2.8");
    GCF::UpdaterComponent::instance().setPrimaryServerSite(updatesDir);
    GCF::UpdaterComponent::instance().setAllowThirdPartyServerSites(false);
    GCF::UpdaterComponent::instance().setCheckUpdateFrequency(GCF::UpdaterComponent::ApplicationStartupButOnceADay);
    GCF::UpdaterComponent::instance().setUseInbuiltGUIXML(false);
    GCF::UpdaterComponent::instance().setUpdateSelectionStrategy(GCF::UpdaterComponent::SelectByNotifyingUser);
    GCF::UpdaterComponent::instance().initialize();

    // Load the other components.
    GCF::ComponentLoaderComponent::instance().initialize();
    // GCF::AppAccessPointComponent::instance().initialize();

    // Show the main-window
    GCF::Components::MainWindow::instance().show();
    GCF::Components::MainWindow::instance().setToolButtonStyle(Qt::ToolButtonTextOnly);

    return a.exec();
}
