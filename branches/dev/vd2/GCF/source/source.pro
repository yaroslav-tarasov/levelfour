include(../config.pri)

TEMPLATE        = lib
win32:CONFIG   += dll 
TARGET          = GCF

MOC_DIR         = $$GCF_TEMP_DIR/gcf
OBJECTS_DIR     = $$GCF_TEMP_DIR/gcf
UI_HEADERS_DIR  = $$GCF_TEMP_DIR/gcf
RCC_DIR         = $$GCF_TEMP_DIR/gcf
UI_DIR          = $$GCF_TEMP_DIR/gcf

DEFINES        += VCL_MAKE_DLL
RESOURCES       = source.qrc

###################### Version Information #########################

VERSION         = 2.2.1

################ Support for ActiveX and KParts ####################

# August 12, 2008. @Akademy 2008, Akonadi/KOffice Den
# Support for ActiveX in GCF
win32 {
    isEmpty(GCF_DISABLE_ACTIVEX) {
        exists( $$(QTDIR)/src/activeqt/container/container.pro ) {
            CONFIG += qaxcontainer
            DEFINES += GCF_ACTIVEQT_AVAILABLE
        #    message("Building GCF with ActiveX support")
        }
    }
}

# Support for KParts in GCF
isEmpty(GCF_DISABLE_KPARTS) {
    include(../kde4.pri)
    !isEmpty(KDE4_INCLUDE_DIR) {
        !isEmpty(KDE4_LIB_DIR) {
            INCLUDEPATH += $$KDE4_INCLUDE_DIR
            LIBS        += -L$$KDE4_LIB_DIR -lkparts
            DEFINES     += GCF_KPARTS_AVAILABLE
            # message("Building GCF with KParts support")
        }
    }
}

INCLUDEPATH    += core ipc menustrip updater

SOURCES         =   \                                        # GCF - Core
                    core/AbstractComponent.cpp \
                    core/ComponentFactory.cpp \
                    core/ComponentGui.cpp \
                    core/ComponentLoaderComponent.cpp \
                    core/ComponentSettings.cpp \
                    core/ConfigurationDialog.cpp \
                    core/ConfigurationDialogComponent.cpp \
                    core/DataStore.cpp \
                    core/Logger.cpp \
                    core/ObjectIterator.cpp \
                    \                                        # GCF - IPC
                    ipc/AppAccessPoint.cpp \
                    ipc/AppAccessPointComponent.cpp \
                    ipc/Message.cpp \
                    ipc/MethodInvocationHelper.cpp \
                    ipc/RemoteAppAccessPoint.cpp \
                    ipc/RemoteAppDiscovery.cpp \
                    ipc/RemoteObject.cpp \
                    \                                        # GCF - MenuStrip
                    menustrip/ActionContainerWidget.cpp \
                    menustrip/ActionDocumentMapper.cpp \
                    menustrip/GenericSignalSpy.cpp \
                    menustrip/Menu.cpp \
                    menustrip/SignalMultiplexer.cpp \
                    \                                        # GCF - Updater
                    updater/FileDownloader.cpp \
                    updater/UpdateDownloaderThread.cpp \
                    updater/UpdateFinderThread.cpp \
                    updater/UpdateInfo.cpp \
                    updater/UpdaterComponent.cpp \
                    updater/UpdateXMLParser.cpp \
                    updater/UpdaterConfigPage.cpp \
                    updater/IconCache.cpp \
                    updater/UpdateSelectionDialog.cpp 
                    
HEADERS         =   \                                        # GCF - Core
                    ../include/GCF/AbstractComponent.h \
                    ../include/GCF/ComponentFactory.h \
                    ../include/GCF/ComponentGui.h \
                    ../include/GCF/ComponentLoaderComponent.h \
                    ../include/GCF/ComponentSettings.h \
                    ../include/GCF/ConfigurationDialog.h \
                    ../include/GCF/ConfigurationDialogComponent.h \
                    ../include/GCF/DataStore.h \
                    core/ComponentFactoryTable \
                    ../include/GCF/Logger.h \
                    ../include/GCF/ObjectIterator.h \
                    \                                        # GCF - IPC
                    ../include/GCF/AppAccessPoint.h \
                    ../include/GCF/AppAccessPointComponent.h \
                    ../include/GCF/Message.h \
                    ../include/GCF/MethodInvocationHelper.h \
                    ../include/GCF/RemoteAppAccessPoint.h \
                    ../include/GCF/RemoteAppDiscovery.h \
                    ../include/GCF/RemoteObject.h \
                    \                                        # GCF - MenuStrip
                    ../include/GCF/ActionContainerWidget.h \
                    ../include/GCF/ActionDocumentMapper.h \
                    ../include/GCF/GenericSignalSpy.h \
                    ../include/GCF/Menu.h \
                    ../include/GCF/SignalMultiplexer.h \
                    \                                        # GCF - Updater
                    updater/FileDownloader.h \
                    updater/UpdateDownloaderThread.h \
                    updater/UpdateFinderThread.h \
                    updater/UpdateInfo.h \
                    ../include/GCF/UpdaterComponent.h \
                    updater/UpdateXMLParser.h \
                    updater/UpdaterConfigPage.h \
                    updater/IconCache.h \
                    updater/UpdateSelectionDialog.h
                  
FORMS           =   \                                        # GCF - Core
                    core/AddPropertyDialog.ui \
                    core/ComponentLoaderConfigPage.ui \
                    core/ComponentPropertyEditorPage.ui \
                    core/ConfigurationDialog.ui \
                    \                                        # GCF - IPC
                    ipc/AddRemoteAppDialog.ui \
                    ipc/AppAccessPointConfigPage.ui \
                    \                                        # GCF - Updater 
                    updater/UpdaterConfigPage.ui \
                    updater/UpdateSelectionDialog.ui

