/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "Log.cpp"
//! \brief Implementation file for Log class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "Log.h"
#include <QtCore/QTime>
#include <QtGui/QStandardItem>
#include <QtGui/QHeaderView>
#include <QtGui/QProgressDialog>


///
/// Private Static Data
///


//!
//! Flag that states whether log messages should also be written to stdout
//! and stderr.
//!
bool Log::s_useStandardStreams = false;

//!
//! Flag that states whether the log message handler has been initialized.
//!
bool Log::s_initialized = false;

//!
//! The list of header labels for the standard item models.
//!
QStringList Log::s_headerLabels;

//!
//! The map of standard item models for all combinations of log message
//! types.
//!
Log::MessageModelMap Log::s_messageModels;

//!
//! The map of proxy item models for all combinations of log message
//! types.
//!
Log::ProxyModelMap Log::s_proxyModels;

//!
//! The nested map for storing registered table views and their message
//! filters.
//!
Log::TableViewMap Log::s_tableViews;

//!
//! The list of icons for the different log message types.
//!
QMap<Log::MessageType, QIcon> Log::s_messageIcons;


///
/// Public Static Functions
///


//!
//! Initializes private static data of the log message handler.
//!
//! \param useStandardStreams Flag to control whether to also output log messages to stdout and stderr.
//!
void Log::initialize ( bool useStandardStreams )
{
    // register the custom message handler for Qt messages
    qInstallMsgHandler(Log::addQtLogMessage);

    s_useStandardStreams = useStandardStreams;

    // fill the list of header labels for the standard item models
    s_headerLabels = QStringList() << "" << "Time" << "Function" << "Message";

    // create standard item models for all combinations of log message types
    for (int i = 0; i <= MT_Info + MT_Warning + MT_Error + MT_Debug + MT_Qt + MT_Ogre; ++i) {
		QStandardItemModel *messageModel = new QStandardItemModel();
		QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel();
        s_messageModels[i] = messageModel;
		proxyModel->setSourceModel(messageModel);
		s_proxyModels[i] = proxyModel;
		s_messageModels[i]->setHorizontalHeaderLabels(s_headerLabels);
    }

    s_initialized = true;
}


//!
//! Frees all resources that were used by private static data of the log
//! message handler.
//!
void Log::finalize ()
{
    for (int i = 0; i < s_messageModels.size(); ++i)
        delete s_messageModels[i];

	for (int i = 0; i < s_proxyModels.size(); ++i)
        delete s_proxyModels[i];
}


//!
//! Loads the icons for the different log message types.
//!
void Log::loadIcons ()
{
    s_messageIcons[MT_Info]= QIcon(":/infoIcon");
    s_messageIcons[MT_Warning]= QIcon(":/warningIcon");
    s_messageIcons[MT_Error]= QIcon(":/errorIcon");
    s_messageIcons[MT_Debug]= QIcon(":/debugIcon");
    s_messageIcons[MT_Qt]= QIcon(":/qtIcon");
    s_messageIcons[MT_Ogre]= QIcon(":/ogreIcon");
}


//!
//! Registers the given table view as an output widget for log messages.
//!
//! \param tableView The table view to register as an output widget.
//!
void Log::registerView ( QTableView *tableView )
{
    if (!tableView)
        return;

    // initialize the message filters for the view
    MessageFilters messageFilters;
    messageFilters[MT_Info] = false;
    messageFilters[MT_Warning] = false;
    messageFilters[MT_Error] = false;
    messageFilters[MT_Debug] = false;
    messageFilters[MT_Qt] = false;
    messageFilters[MT_Ogre] = false;
    FilterSettings filterSettings;
    filterSettings.first = false;
    filterSettings.second = messageFilters;

    // add a new entry for the view in the table views map
    s_tableViews.insert(tableView, filterSettings);

    // fill the filtered message model with all messages
    updateModel(tableView);

    // set up headers
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setClickable(false);
    //tableView->horizontalHeader()->setVisible(false);
    tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    tableView->verticalHeader()->setDefaultSectionSize(16);
    tableView->verticalHeader()->setVisible(false);

    // resize the icon, time and function columns according to their size
    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);
    tableView->resizeColumnToContents(2);

    // set up the view's selection behavior
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // disable editing
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // set up additional properties
    //tableView->setAlternatingRowColors(true);
}


//!
//! Unregisters the given table view from the log message handler.
//!
//! \param tableView The table view to unregister from the log message handler.
//!
void Log::unregisterView ( QTableView *tableView )
{
    if (tableView)
        s_tableViews.remove(tableView);
}


//!
//! Adds the given text to the list of informational log messages.
//!
//! \param message The text to add as an informational log message.
//! \param function The signature of the function that is reporting the message.
//!
void Log::info ( const QString &message, const QString &function /* = "" */ )
{
    addMessage(MT_Info, message, function);

    if (s_useStandardStreams)
        printToStandardOutput("[Info]    ", message, function);

    _RPT0(_CRT_WARN, "[Info]     ");
    if (!function.isEmpty()) {
        _RPT0(_CRT_WARN, "[");
        _RPT0(_CRT_WARN, function.toStdString().c_str());
        _RPT0(_CRT_WARN, "] ");
    }
    _RPT0(_CRT_WARN, QString(message).replace('\n', ' ').toStdString().c_str());
    _RPT0(_CRT_WARN, "\n");
}


//!
//! Adds the given text to the list of warning log messages.
//!
//! \param message The text to add as a warning log message.
//! \param function The signature of the function that is reporting the message.
//!
void Log::warning ( const QString &message, const QString &function /* = "" */ )
{
    addMessage(MT_Warning, message, function);

    if (s_useStandardStreams)
        printToStandardError("[Warning] ", message, function);

    _RPT0(_CRT_WARN, "[Warning]  ");
    if (!function.isEmpty()) {
        _RPT0(_CRT_WARN, "[");
        _RPT0(_CRT_WARN, function.toStdString().c_str());
        _RPT0(_CRT_WARN, "] ");
    }
    _RPT0(_CRT_WARN, QString(message).replace('\n', ' ').toStdString().c_str());
    _RPT0(_CRT_WARN, "\n");
}


//!
//! Adds the given text to the list of error log messages.
//!
//! \param message The text to add as an error log message.
//! \param function The signature of the function that is reporting the message.
//!
void Log::error ( const QString &message, const QString &function /* = "" */ )
{
    addMessage(MT_Error, message, function);

    if (s_useStandardStreams)
        printToStandardError("[ ERROR ] ", message, function);

    _RPT0(_CRT_WARN, "[ ERROR ]  ");
    if (!function.isEmpty()) {
        _RPT0(_CRT_WARN, "[");
        _RPT0(_CRT_WARN, function.toStdString().c_str());
        _RPT0(_CRT_WARN, "] ");
    }
    _RPT0(_CRT_WARN, QString(message).replace('\n', ' ').toStdString().c_str());
    _RPT0(_CRT_WARN, "\n");
}


//!
//! Adds the given text to the list of debugging log messages.
//!
//! \param message The text to add as a debugging log message.
//! \param function The signature of the function that is reporting the message.
//!
void Log::debug ( const QString &message, const QString &function /* = "" */ )
{
    addMessage(MT_Debug, message, function);

    if (s_useStandardStreams)
        printToStandardError("[Debug]   ", message, function);

    _RPT0(_CRT_WARN, "[Debug]    ");
    if (!function.isEmpty()) {
        _RPT0(_CRT_WARN, "[");
        _RPT0(_CRT_WARN, function.toStdString().c_str());
        _RPT0(_CRT_WARN, "] ");
    }
    _RPT0(_CRT_WARN, QString(message).replace('\n', ' ').toStdString().c_str());
    _RPT0(_CRT_WARN, "\n");
}


//!
//! Adds a message to the list of warning log messages that the selected
//! function is not available yet.
//!
//! \param function The signature of the function that is reporting the message.
//!
void Log::na ( const QString &function /* = "" */ )
{
    warning("The selected function has not been implemented yet.", function);
}


//!
//! Adds the given text to the list of Qt log messages.
//!
//! \param qtMessageType The type of the Qt message.
//! \param qtMessage The message text.
//!
void Log::addQtLogMessage ( QtMsgType qtMessageType, const char *qtMessage )
{
    QString message = QString("%1").arg(qtMessage);
    QString messageType;
    QString function ("Qt");

    switch (qtMessageType) {
        case QtDebugMsg:
            messageType = "Debug";
            break;
        case QtWarningMsg:
            messageType = "Warning";
            break;
        case QtCriticalMsg:
            messageType = "Critical";
            break;
        case QtFatalMsg:
            messageType = "Fatal";
            break;
    }
    message = messageType + ": " + message;

    addMessage(MT_Qt, message, function);

    _RPT0(_CRT_WARN, "[Qt]       ");
    _RPT0(_CRT_WARN, QString(message).replace('\n', ' ').toStdString().c_str());
    _RPT0(_CRT_WARN, "\n");
}


//!
//! Adds the given text to the list of OGRE log messages.
//!
//! \param message The text to add as an OGRE log message.
//! \param logMessageLevel The importance of the logged message.
//!
void Log::addOgreLogMessage ( const QString &message, const QString &logMessageLevel )
{
    QString function ("OGRE");
    if (logMessageLevel != "normal")
        function = QString("OGRE (%1)").arg(logMessageLevel);

    addMessage(MT_Ogre, message, function);

    //_RPT0(_CRT_WARN, "[OGRE]     ");
    //_RPT0(_CRT_WARN, QString(message).replace('\n', ' ').toStdString().c_str());
    //_RPT0(_CRT_WARN, "\n");
}


//!
//! Toggles filtering of log messages for the given table view.
//!
//! \param enabled Flag that controls whether to enable or disable filtering.
//!
void Log::setFilterEnabled ( QTableView *tableView, bool enabled )
{
    if (!tableView)
        return;

    s_tableViews[tableView].first = enabled;

    updateModel(tableView);
}


//!
//! Toggles a specific type of log messages in the given table view by
//! making these messages visible or invisible.
//!
//! \param tableView The table view to filter messages in.
//! \param type The type of log messages to filter.
//! \param visible Flag that controls whether to hide or show the messages.
//!
void Log::filter ( QTableView *tableView, MessageType type, bool visible )
{
    if (!tableView)
        return;

    if (s_tableViews[tableView].second[type] == visible)
        return;

    // update the message filter to the given value
    s_tableViews[tableView].second[type] = visible;

    // activate filtering if it isn't active yet
    if (!s_tableViews[tableView].first)
        s_tableViews[tableView].first = true;

	// test
	s_proxyModels[type]->setFilterRegExp(QRegExp("Freetype", Qt::CaseInsensitive, QRegExp::FixedString));
    s_proxyModels[type]->setFilterKeyColumn(3);

    updateModel(tableView);
}

//!
//! Filters log messages in the given table view by
//! using a reg expression.
//!
//! \param searchText The search text.
//!
void Log::filter ( QTableView *tableView, const QString &searchText)
{
    if (!tableView)
        return;

	for (int i = 0; i <= MT_Info + MT_Warning + MT_Error + MT_Debug + MT_Qt + MT_Ogre; ++i) {
		s_proxyModels[i]->setFilterRegExp(QRegExp(searchText, Qt::CaseInsensitive, QRegExp::FixedString));
		s_proxyModels[i]->setFilterKeyColumn(3);
	}

    updateModel(tableView);
}

//!
//! Clears the history of log messages.
//!
void Log::clear ()
{
    QProgressDialog progressDialog (QObject::tr("Clearing log..."), QString(), 0, s_messageModels.size());
    progressDialog.setWindowTitle(QObject::tr("Clear Log"));

    // iterate over all standard item models and clear all items
    for (int i = 0; i < s_messageModels.size(); ++i) {
        s_messageModels[i]->clear();
        s_messageModels[i]->setHorizontalHeaderLabels(s_headerLabels);

        progressDialog.setValue(i);
    }
}


//!
//! Builds a string representing the message with the given row index in
//! the given item model.
//!
//! \return A string representing the message with the given row index in the given item model.
//!
QString Log::getMessage ( QStandardItemModel *model, int row )
{
    if (!model || row < 0 || row > model->rowCount() - 1)
        return "";

    // get pointers to items contained in the row
    QStandardItem *iconItem = model->item(row);
    QStandardItem *timeItem = model->item(row, 1);
    QStandardItem *functionItem = model->item(row, 2);
    QStandardItem *messageItem = model->item(row, 3);

    // decode log message type to a string
    MessageType logMessageType = (MessageType) iconItem->data().toInt();
    QString messageType;
    switch (logMessageType) {
        case MT_Info:
            messageType = "Info";
            break;
        case MT_Warning:
            messageType = "Warning";
            break;
        case MT_Error:
            messageType = "Error";
            break;
        case MT_Debug:
            messageType = "Debug";
            break;
        case MT_Qt:
            messageType = "Qt";
            break;
        case MT_Ogre:
            messageType = "OGRE";
            break;
    }
    QString test = messageItem->text();
    // build message line
    return QString("%1\t%2\t%3\t%4\n").arg(messageType).arg(timeItem->text()).arg(functionItem->text()).arg(messageItem->text());
}


///
/// Private Static Functions
///


//!
//! Adds the given text to the list of log messages.
//!
//! \param messageType The type of log message to add.
//! \param message The text to add as a log message.
//! \param function The signature of the function that is reporting the message.
//!
void Log::addMessage ( MessageType messageType, const QString &message, const QString &function /* = "" */ )
{
    if (!s_initialized) {
        fprintf(stderr, "[Debug]    [Log::addMessage] Log message handler has not been initialized yet.");
        return;
    }

    QString modifiedMessage (message);
    modifiedMessage = modifiedMessage.replace('\n', ' ');

    // iterate over all message models
    for (int i = 0; i <= s_messageModels.size(); ++i)
        // check the model's type
        if (i == 0 || (i & messageType)) {
            // create the standard items that make the new row
            QStandardItem *iconItem = new QStandardItem(s_messageIcons[messageType], "");
            iconItem->setData(QVariant::fromValue<int>(messageType));
            QStandardItem *timeItem = new QStandardItem(QTime::currentTime().toString());
            QStandardItem *functionItem = new QStandardItem(function);
            QStandardItem *messageItem = new QStandardItem(modifiedMessage);

            // create a new table row from the items
            QList<QStandardItem *> items;
            items << iconItem << timeItem << functionItem << messageItem;

            // add the row at the top of the model
            QStandardItem *rootItem = s_messageModels[i]->invisibleRootItem();
            rootItem->insertRow(0, items);

            // update all table views that use the model
            for (TableViewMap::iterator it = s_tableViews.begin(); it != s_tableViews.end(); ++it) {
                QTableView *tableView = it.key();
                if (tableView->model() == s_messageModels[i]) {
                    // resize the first three columns to their contents
                    tableView->resizeColumnToContents(0);
                    tableView->resizeColumnToContents(1);
                    tableView->resizeColumnToContents(2);
                }
            }
        }
}


//!
//! Fills the model that is used in the given table view with messages of types
//! according to the active view filters.
//!
//! \param tableView The table view whose model to fill with log messages.
//!
void Log::updateModel ( QTableView *tableView )
{
    if (!tableView)
        return;

    // calculate the type mask according to the table view's filter settings
    int typeMask = 0;
    if (s_tableViews[tableView].first) {
        if (s_tableViews[tableView].second[MT_Info])
            typeMask |= MT_Info;
        if (s_tableViews[tableView].second[MT_Warning])
            typeMask |= MT_Warning;
        if (s_tableViews[tableView].second[MT_Error])
            typeMask |= MT_Error;
        if (s_tableViews[tableView].second[MT_Debug])
            typeMask |= MT_Debug;
        if (s_tableViews[tableView].second[MT_Qt])
            typeMask |= MT_Qt;
        if (s_tableViews[tableView].second[MT_Ogre])
            typeMask |= MT_Ogre;
    }

    // use the message model that of the given type mask
    QSortFilterProxyModel *proxyModel = s_proxyModels[typeMask];
    tableView->setModel(s_proxyModels[typeMask]);

    // resize the first three columns to their contents
    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);
    tableView->resizeColumnToContents(2);
}


//!
//! Writes the given message preceeded by the given header text and the
//! given function name (if available) to the standard output stream.
//!
//! \param header The text to put before the message.
//! \param message The message to write to the standard output stream.
//! \param function The name of a function that reported the message.
//!
void Log::printToStandardOutput ( const QString &header, const QString &message, const QString &function )
{
    if (!function.isEmpty())
        fprintf(stdout, "%s [%s] %s\n", header.toStdString().c_str(), function.toStdString().c_str(), QString(message).replace('\n', ' ').toStdString().c_str());
    else
        fprintf(stdout, "%s %s\n", header.toStdString().c_str(), QString(message).replace('\n', ' ').toStdString().c_str());
}


//!
//! Writes the given message preceeded by the given header text and the
//! given function name (if available) to the standard error stream.
//!
//! \param header The text to put before the message.
//! \param message The message to write to the standard error stream.
//! \param function The name of a function that reported the message.
//!
void Log::printToStandardError ( const QString &header, const QString &message, const QString &function )
{
    if (!function.isEmpty())
        fprintf(stderr, "%s [%s] %s\n", header.toStdString().c_str(), function.toStdString().c_str(), QString(message).replace('\n', ' ').toStdString().c_str());
    else
        fprintf(stderr, "%s %s\n", header.toStdString().c_str(), QString(message).replace('\n', ' ').toStdString().c_str());
}

