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
//! \file "ParameterPanel.cpp"
//! \brief Implementation file for ParameterPanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       09.07.2009 (last updated)
//!

#include "ParameterPanel.h"
#include "ParameterTabPage.h"
#include "DoubleSlider.h"
#include "NodeFactory.h"
#include "WidgetFactory.h"
#include "WidgetPlugin.h"
#include "ParameterPlugin.h"
#include "Log.h"
#include <QtGui/QTabWidget>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>
#include <QtGui/QMenu>
#include <QtGui/QClipboard>

#include <iostream>

Q_DECLARE_METATYPE(Ogre::Vector3)


///
/// Constructors and Destructors
///


//!
//! Constructor of the ParameterPanel class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
ParameterPanel::ParameterPanel ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    ViewPanel(ViewPanel::T_ParameterEditor, parent, flags),
    m_node(0),
    m_editedWidget(0),
    m_nodeTypeLabel(new QLabel(this)),
    m_nodeTypeLabelAction(0),
    m_nodeNameEdit(new QLineEdit(this)),
    m_nodeNameEditAction(0),
    m_searchEdit(new QLineEdit(this)),
    m_searchTimer(0)
{
    // initialize the node type and name widgets
    m_nodeTypeLabel->setIndent(20);
    m_nodeNameEdit->setStyleSheet(
        "margin-right: 2px;"
        "height: 20px;"
    );

    // initialize the search edit widget
    m_searchEdit->setToolTip(tr("Parameter Search"));
    m_searchEdit->setStatusTip(tr("Enter a name or part of a name of parameters to display"));
    m_searchEdit->setEnabled(false);

    // set object names for QMetaObject::connectSlotsByName to work (called in setupUi function)
    m_nodeNameEdit->setObjectName("m_nodeNameEdit");
    m_searchEdit->setObjectName("m_searchEdit");
	
    setupUi(this);
}



//!
//! Destructor of the NetworkPanel class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ParameterPanel::~ParameterPanel ()
{
}


///
/// Public Slots
///


//!
//! Displays the parameters and values for the given node.
//!
//! \param node The node whose parameters to display in the parameter view.
//!
void ParameterPanel::showParameters ( Node *node )
{
    // remove all widgets from the form layout
    clear();

    // save the node as the currently edited node
    m_node = node;

    // display the type of the selected node in the node type label and the name of the selected node in the node name edit
    if (m_node) {
        m_nodeTypeLabel->setText(m_node->getTypeName() + ":");
        m_nodeNameEdit->setText(m_node->getName());
        QString typeIconName;
        if (m_node->isTypeUnknown()) {
            typeIconName = ":/warningIcon";
            m_nodeTypeLabel->setToolTip(tr("There is no node type plugin available for the type of this node.\nEditing parameters of this node has been deactivated."));
        } else {
            typeIconName = NodeFactory::getCategoryIconName(m_node->getTypeName());
            m_nodeTypeLabel->setToolTip("");
        }
        m_nodeTypeLabel->setStyleSheet(QString(
            "background-image: url(%1);"
            "background-repeat: no-repeat;"
            "background-origin: content;"
            "background-position: left center;"
            "margin-left: 4px;"
            "margin-right: 2px;"
            ).arg(typeIconName)
        );
    } else {
        m_nodeTypeLabel->setText("");
        m_nodeNameEdit->setText("");
    }

    // show or hide the node type and name widgets through their actions
    bool nodeSelected = m_node != 0;
    
	if (m_nodeTypeLabelAction) {
		m_nodeTypeLabelAction->setVisible(nodeSelected);
		m_nodeTypeLabelAction->setEnabled(nodeSelected);
	}
	if (m_nodeNameEditAction) {
		m_nodeNameEditAction->setVisible(nodeSelected);
		m_nodeNameEditAction->setEnabled(nodeSelected);
	}

    // enable or disable the search edit widget
    m_searchEdit->setEnabled(m_node);

    // skip the rest of the function if no node has been set
    if (!m_node)
        return;

    // check if a search text is set for the node that is different from the search text entered in the search text line edit widget
    QString searchText = m_node->getSearchText();
    if (searchText != m_searchEdit->text()) {
        // update the search edit and reset action without triggering signals
        m_searchEdit->blockSignals(true);
		m_searchEdit->setText(searchText);		
        ui_resetSearchAction->setEnabled(!searchText.isEmpty());
        m_searchEdit->blockSignals(false);
    }

    // get a list of parameters that match the search text
	QList<Parameter *> filteredParameters = m_node->getParameterRoot()->filterParameters(searchText.toCaseFolded(), true);

    if (filteredParameters.size() > 0) {
        // build a temporary parameter group for the search results
        ParameterGroup *searchParameterGroup = new ParameterGroup("Search Results");
		searchParameterGroup->setNode(m_node);
        for (int i = 0; i < filteredParameters.size(); ++i)
            searchParameterGroup->addParameter(filteredParameters[i]);

        // add rows for the parameters found
        addRows(searchParameterGroup, ui_formLayout);

        // destroy the temporary parameter group
        searchParameterGroup->clear();
        delete searchParameterGroup;
        searchParameterGroup = 0;
    } else
        // add rows for all parameters
        addRows(m_node->getParameterRoot(), ui_formLayout);
}


//!
//! Slot that is called when the selection in the scene model has changed.
//!
//! \param objectsSelected Flag that states whether objects in the scene are selected.
//!
void ParameterPanel::updateParameters ( bool objectsSelected )
{
    if (objectsSelected)
        return;

    // clear and reset the parameter panel
    showParameters(0);

    // update the search edit and reset action without triggering signals
    m_searchEdit->blockSignals(true);
    m_searchEdit->setText("");
    ui_resetSearchAction->setEnabled(false);
    m_searchEdit->blockSignals(false);
}


///
/// Public Functions
///


//!
//! Fills the given tool bars in a panel frame with actions for the panel.
//!
//! \param mainToolBar The main tool bar to fill with actions.
//! \param panelToolBar The panel tool bar to fill with actions.
//!
void ParameterPanel::fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar )
{
    // add widgets to main tool bar
    m_nodeTypeLabelAction = mainToolBar->addWidget(m_nodeTypeLabel);
    m_nodeNameEditAction = mainToolBar->addWidget(m_nodeNameEdit);

    // make the node type and name edit widgets invisible
    m_nodeTypeLabelAction->setVisible(false);
    m_nodeTypeLabelAction->setEnabled(false);
    m_nodeNameEditAction->setVisible(false);
    m_nodeNameEditAction->setEnabled(false);

    // create the search icon label
    QLabel *searchIconLabel = new QLabel(this);
    searchIconLabel->setFixedWidth(20);
    searchIconLabel->setStyleSheet(
        "background-image: url(:/searchIcon);"
        "background-repeat: no-repeat;"
        "background-origin: content;"
        "background-position: left center;"
        "margin-right: 2px;"
    );
    searchIconLabel->setToolTip(tr("Parameter Search"));

    // add the parameter search controls to the panel tool bar
    panelToolBar->addWidget(searchIconLabel);
    panelToolBar->addWidget(m_searchEdit);
    panelToolBar->addAction(ui_resetSearchAction);
    panelToolBar->addSeparator();
}


///
/// Protected Events
///


//!
//! Event handler that reacts to timer events.
//!
//! \param event The description of the timer event.
//!
void ParameterPanel::timerEvent ( QTimerEvent *event )
{
    if (event->timerId() == m_searchTimer) {
        killTimer(m_searchTimer);
        m_searchTimer = 0;

        filterParameters();
    }
}


///
/// Private Slots
///


//!
//! Requests a name change for the currently edited node and displays the
//! successfully changed name in the node name edit widget.
//!
//! Is called when editing the node's name has finished.
//!
void ParameterPanel::on_m_nodeNameEdit_editingFinished ()
{
    // make sure a node is currently edited
    if (!m_node) {
        Log::warning("No node is currently edited. This function should not have been called.", "ParameterPanel::on_m_nodeNameEdit_editingFinished");
        return;
    }

    QString name (m_nodeNameEdit->text());
    // make sure the name is not empty
    if (!name.isEmpty()) {
        // notify connected objects that the currently edited node should be renamed
        emit objectNameChangeRequested(m_node->getName(), name);

        // check if the node's name has actually changed
        if (m_node->getName() != m_nodeNameEdit->text())
            // apply the successfully changed name to the node name edit
            m_nodeNameEdit->setText(m_node->getName());
    } else
        Log::warning("Please enter a valid name.", "ParameterPanel::on_m_nodeNameEdit_editingFinished");
}


//!
//! Starts the timer that launches the parameter filtering search after a
//! specific delay.
//!
//! Is called when the text in the search edit widget has changed.
//!
//! \param text The text that has been entered in the search edit widget.
//!
void ParameterPanel::on_m_searchEdit_textChanged ( const QString &text )
{
    // stop a previously started search timer
    if (m_searchTimer != 0)
        killTimer(m_searchTimer);

    m_searchTimer = startTimer(1000);

    ui_resetSearchAction->setEnabled(!text.isEmpty());
}


//!
//! Cancels a running parameter search and resets the parameter search
//! settings for the currently edited node.
//!
//! Is called when the reset search action has been triggered.
//!
//! \param checked The state of the action (unused).
//!
void ParameterPanel::on_ui_resetSearchAction_triggered ( bool /* checked = false */ )
{
    // stop the search timer if it is currently running
    if (m_searchTimer != 0) {
        killTimer(m_searchTimer);
        m_searchTimer = 0;
    }

    // reset the parameter search widgets
    m_searchEdit->setText("");

    // reset the parameter search for the node
    m_node->setSearchText("");

    // display all parameters for the node
    showParameters(m_node);
}


//!
//! Fills the tab page of the given index with widgets for editing the
//! parameters that the tab page represents.
//!
//! Is called when the index of the current tab in a tab widget has
//! changed.
//!
//! \param index The index of the tab page to fill with editing widgets.
//!
void ParameterPanel::loadTabPage ( int index )
{
    QTabWidget *tabWidget = qobject_cast<QTabWidget *>(sender());
    if (!tabWidget)
        return;

    ParameterTabPage *parameterTabPage = dynamic_cast<ParameterTabPage *>(tabWidget->widget(index));
    if (parameterTabPage) {
        // check if the tab page has already been loaded
        if (!parameterTabPage->isLoaded()) {
            // create a new form layout for the tab page and fill it with editing widgets for the page's parameters
            QFormLayout *formLayout = new QFormLayout(parameterTabPage);
            parameterTabPage->setLayout(formLayout);
            addRows(parameterTabPage->getParameterGroup(), formLayout);
            // mark the parameter tab page as being loaded
            parameterTabPage->setLoaded();
        }
    } else
        Log::warning(QString("The tab page with index %1 can not be loaded. The widget of the given index is not a parameter tab page.").arg(index), "ParameterPanel::loadTabPage");
}


//!
//! Opens a context menu at the given position with actions for the label.
//!
//! Is called when a context menu is requested for a label widget.
//!
//! \param position The position at which to open the context menu.
//!
void ParameterPanel::labelContextMenuRequested ( const QPoint &position )
{
    QLabel *label = qobject_cast<QLabel *>(sender());
    if (!label)
        return;

    QMenu contextMenu ("Text Info", this);
    QAction *copyAction = contextMenu.addAction(QIcon(":/copyIcon"), tr("&Copy"));
    if (contextMenu.exec(label->mapToGlobal(position)) == copyAction)
        QApplication::clipboard()->setText(label->text());
}


///
/// Private Slots for updating parameter values when widgets change
///


//!
//! Applies an edited boolean value to the parameter represented by the
//! checkbox widget calling this slot.
//!
//! Is called when a checkbox representing a parameter has been toggled.
//!
//! \param checked The state of the check box.
//!
void ParameterPanel::checkBoxToggled ( bool checked )
{
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());
    if (!checkBox)
        return;

    QString parameterName = checkBox->property("parameterName").toString();
    QVariant parameterValue = QVariant::fromValue(checked);

    requestParameterChange(parameterName, parameterValue);
}


//!
//! Applies an edited numeric value to the parameter represented by the
//! spinbox widget calling this slot.
//!
//! Is called when editing the value of a spin box representing a parameter
//! has finished.
//!
void ParameterPanel::spinBoxEditingFinished ()
{
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(sender());
    if (!spinBox)
        return;

    QString parameterName = spinBox->property("parameterName").toString();
    Parameter::Type parameterType = (Parameter::Type) spinBox->property("parameterType").toInt();
    QVariant parameterValue;
    if (parameterType == Parameter::T_Int)
        parameterValue = QVariant::fromValue(spinBox->value());
    else if (parameterType == Parameter::T_UnsignedInt)
        parameterValue = QVariant::fromValue((unsigned int) spinBox->value());
    else {
        Log::warning(QString("Parameter \"%1\" has an unsupported parameter type (%2) for spin boxes.").arg(parameterName).arg(parameterType), "ParameterPanel::spinBoxEditingFinished");
        return;
    }

    requestParameterChange(parameterName, parameterValue);
}


//!
//! Applies an edited numeric value to the parameter represented by the
//! slider widget calling this slot.
//!
//! Is called when the value of a slider representing a parameter has
//! changed.
//!
void ParameterPanel::sliderValueChanged ()
{
    QSlider *slider = qobject_cast<QSlider *>(sender());
    if (!slider)
        return;

    QString parameterName = slider->property("parameterName").toString();
    Parameter::Type parameterType = (Parameter::Type) slider->property("parameterType").toInt();
    QVariant parameterValue;
    if (parameterType == Parameter::T_Int)
        parameterValue = QVariant::fromValue(slider->value());
    else if (parameterType == Parameter::T_UnsignedInt)
        parameterValue = QVariant::fromValue((unsigned int) slider->value());
    else {
        Log::warning(QString("Parameter \"%1\" has an unsupported parameter type (%2) for sliders.").arg(parameterName).arg(parameterType), "ParameterPanel::sliderValueChanged");
        return;
    }

    requestParameterChange(parameterName, parameterValue);
}


//!
//! Applies an edited floating point numeric value to the parameter
//! represented by the spinbox widget calling this slot.
//!
//! Is called when editing the value of a double spin box representing a
//! parameter has finished.
//!
void ParameterPanel::doubleSpinBoxEditingFinished ()
{
    QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox *>(sender());
    if (!doubleSpinBox)
        return;

    QString parameterName = doubleSpinBox->property("parameterName").toString();
    QVariant parameterValue = QVariant::fromValue(doubleSpinBox->value());

    requestParameterChange(parameterName, parameterValue);
}


//!
//! Applies an edited floating point numeric value to the parameter
//! represented by the slider widget calling this slot.
//!
//! Is called when the value of a double slider representing a parameter
//! has changed.
//!
void ParameterPanel::doubleSliderValueChanged ()
{
    DoubleSlider *slider = qobject_cast<DoubleSlider *>(sender());
    if (!slider)
        return;

    QString parameterName = slider->property("parameterName").toString();
    QVariant parameterValue = QVariant::fromValue(slider->doubleValue());

    requestParameterChange(parameterName, parameterValue);
}


//!
//! Applies an edited string value to the parameter represented by the line
//! edit widget calling this slot.
//!
//! Is called when editing the text of a line edit widget representing a
//! parameter has finished.
//!
void ParameterPanel::lineEditEditingFinished ()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());
    if (!lineEdit)
        return;

    QString parameterName = lineEdit->property("parameterName").toString();
    QVariant parameterValue = QVariant::fromValue(lineEdit->text());

    requestParameterChange(parameterName, parameterValue);
}


//!
//! Displays a dialog for selecting a file for the edited parameter.
//!
//! Is called when the browse button for a filename parameter has been
//! clicked.
//!
void ParameterPanel::browseButtonClicked ()
{
    QPushButton *browseButton = qobject_cast<QPushButton *>(sender());
    if (!browseButton)
        return;

    QString parameterName = browseButton->property("parameterName").toString();
    FilenameParameter *filenameParameter = dynamic_cast<FilenameParameter *>(m_node->getParameter(parameterName));
    if (!filenameParameter) {
        Log::warning(QString("Filename parameter \"%1\" could not be obtained from node \"%2\".").arg(parameterName).arg(m_node->getName()), "ParameterPanel::browseButtonClicked");
        return;
    }

    FilenameParameter::Type filenameType = filenameParameter->getType();
    QString filename = filenameParameter->getValue().toString();
    QString filters = filenameParameter->getFilters();

    // display the file selection dialog
    if (filenameType == FilenameParameter::FT_Save)
        filename = QFileDialog::getSaveFileName(this, QString(tr("Select %1")).arg(parameterName), filename, filters);
    else
        filename = QFileDialog::getOpenFileName(this, QString(tr("Select %1")).arg(parameterName), filename, filters);

    if (!filename.isNull()) {
        emit parameterChangeRequested(m_node->getName(), parameterName, QVariant::fromValue(filename));

        if (m_widgetMap.contains(parameterName)) {
            QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(m_widgetMap.value(parameterName));
            if (lineEdit)
                lineEdit->setText(filename);
        }
    }
}


//!
//! Reloads the file that the edited parameter represents.
//!
//! Is called when the reload button for a filename parameter has been
//! clicked.
//!
void ParameterPanel::reloadButtonClicked ()
{
    QPushButton *reloadButton = qobject_cast<QPushButton *>(sender());
    if (!reloadButton)
        return;

    QString parameterName = reloadButton->property("parameterName").toString();
    Parameter *parameter = m_node->getParameter(parameterName);
    if (parameter) {
        parameter->executeCommand();
        parameter->propagateDirty(m_node);
        parameter->setDirty(false);
    }
}


//!
//! Displays a dialog for selecting a color for the edited parameter.
//!
//! Is called when the color button representing a color parameter has been
//! clicked.
//!
void ParameterPanel::selectColor ()
{
    QPushButton *colorButton = qobject_cast<QPushButton *>(sender());
    if (!colorButton)
        return;

    QString parameterName = colorButton->property("parameterName").toString();

    // display the color selection dialog with the current color selected
    QColor previousColor = m_node->getColorValue(parameterName);
    QColor selectedColor = QColorDialog::getColor(previousColor);
    if (selectedColor.isValid() && selectedColor != previousColor) {
        requestParameterChange(parameterName, QVariant::fromValue<QColor>(selectedColor));
    }
}


//!
//! Applies a changed combo box item selection to the edited parameter.
//!
//! Is called when the index of the currently selected item of a combo box
//! representing an enumeration parameter has changed.
//!
//! \param index The index of the item currently selected in the combo box.
//!
void ParameterPanel::comboBoxIndexChanged ( int index )
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (!comboBox)
        return;

    QString parameterName = comboBox->property("parameterName").toString();
    QVariant parameterValue = QVariant::fromValue(index);

    requestParameterChange(parameterName, parameterValue);
}


//!
//! Requests the execution of the command represented by the clicked
//! button.
//!
//! Is called when the button representing a command parameter is clicked.
//!
void ParameterPanel::commandButtonClicked ()
{
    QPushButton *commandButton = qobject_cast<QPushButton *>(sender());
    if (!commandButton)
        return;

    QString parameterName = commandButton->property("parameterName").toString();
    Parameter *commandParameter = m_node->getParameter(parameterName);
    if (commandParameter)
        commandParameter->executeCommand();
    else
        Log::warning(QString("Command parameter \"%1\" could not be obtained from node \"%2\".").arg(parameterName).arg(m_node->getName()), "ParameterPanel::commandButtonClicked");
}


///
/// Private Slots for updating widgets when parameter values change
///


//!
//! Updates the widget showing a boolean parameter's value.
//!
//! Is called when the value of a boolean parameter has changed.
//!
void ParameterPanel::updateBooleanWidget ()
{
    Parameter *parameter = dynamic_cast<Parameter *>(sender());
    if (!parameter)
        return;

    QString parameterName = parameter->getName();
	bool enabled = parameter->isEnabled();

    if (m_widgetMap.contains(parameterName)) {
        // get the first widget in the list of widgets (which should be a check box)
        QWidgetList widgets = m_widgetMap.values(parameterName);
        QCheckBox *checkBox = dynamic_cast<QCheckBox *>(widgets[0]);
        if (checkBox) {
            bool parameterValue = parameter->getValue().toBool();
            checkBox->setChecked(parameterValue);
			checkBox->setEnabled(enabled);
        }
    }
}


//!
//! Updates the widgets showing an integer number parameter's value.
//!
//! Is called when the value of an integer number parameter has changed.
//!
void ParameterPanel::updateIntegerWidgets ()
{
    NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(sender());
    if (!numberParameter)
        return;

    Parameter::Size size = numberParameter->getSize();
    if (size == 1)
        updateIntegerWidgets(-1);
    else
        for (Parameter::Size i = 0; i < size; ++i)
            updateIntegerWidgets((int) i);
}


//!
//! Updates the widgets showing the component with the given index of an
//! integer number parameter.
//!
//! Is called when one of the values of an integer number parameter that
//! contains a list of values has changed.
//!
//! \param index The index of the value that has changed in the list of integer values.
//!
void ParameterPanel::updateIntegerWidgets ( int index )
{
    NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(sender());
    if (!numberParameter)
        return;

    // get name and value from parameter
    QString parameterName = numberParameter->getName();
    QVariant parameterValue = numberParameter->getValue();
	bool enabled = numberParameter->isEnabled();

    // adjust parameter name according to given value index
    if (index > -1)
        parameterName = QString("%1[%2]").arg(parameterName).arg(index);

    // check if a widget for the parameter exists
    if (m_widgetMap.contains(parameterName)) {
        // get the unsigned integer value from the parameter value variant
        int value;
        if (index == -1)
            value = parameterValue.toInt();
        else
            value = parameterValue.toList().value(index).toInt();

        // iterate over the widgets for displaying the unsigned integer number parameter
        QWidgetList widgets = m_widgetMap.values(parameterName);
        for (int i = 0; i < widgets.size(); ++i)
            if (widgets[i] != m_editedWidget) {
                // check if the widget is a spin box
                QSpinBox *spinBox = dynamic_cast<QSpinBox *>(widgets[i]);
                if (spinBox) {
                    // update the spin box for the parameter
                    spinBox->blockSignals(true);
                    spinBox->setValue(value);
					spinBox->setEnabled(enabled);
                    spinBox->blockSignals(false);
                } else {
                    // check if the widget is a slider
                    QSlider *slider = dynamic_cast<QSlider *>(widgets[i]);
                    if (slider) {
                        // update the slider for the parameter
                        slider->blockSignals(true);
                        slider->setValue(value);
						slider->setEnabled(enabled);
                        slider->blockSignals(false);
                    }
                }
            }
    }
}


//!
//! Updates the widgets showing an unsigned integer number parameter's
//! value.
//!
//! Is called when the value of an unsigned integer number parameter has
//! changed.
//!
void ParameterPanel::updateUnsignedIntegerWidgets ()
{
    NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(sender());
    if (!numberParameter)
        return;

    Parameter::Size size = numberParameter->getSize();
    if (size == 1)
        updateUnsignedIntegerWidgets(-1);
    else
        for (Parameter::Size i = 0; i < size; ++i)
            updateUnsignedIntegerWidgets((int) i);
}


//!
//! Updates the widgets showing the component with the given index of an
//! unsigned integer number parameter.
//!
//! Is called when one of the values of an unsigned integer number
//! parameter that contains a list of values has changed.
//!
//! \param index The index of the value that has changed in the list of unsigned integer values.
//!
void ParameterPanel::updateUnsignedIntegerWidgets ( int index )
{
    NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(sender());
    if (!numberParameter)
        return;

    // get name and value from parameter
    QString parameterName = numberParameter->getName();
    QVariant parameterValue = numberParameter->getValue();
	bool enabled = numberParameter->isEnabled();

    // adjust parameter name according to given value index
    if (index > -1)
        parameterName = QString("%1[%2]").arg(parameterName).arg(index);

    // check if a widget for the parameter exists
    if (m_widgetMap.contains(parameterName)) {
        // get the unsigned integer value from the parameter value variant
        unsigned int value;
        if (index == -1)
            value = parameterValue.toUInt();
        else
            value = parameterValue.toList().value(index).toUInt();

        // iterate over the widgets for displaying the unsigned integer number parameter
        QWidgetList widgets = m_widgetMap.values(parameterName);
        for (int i = 0; i < widgets.size(); ++i)
            if (widgets[i] != m_editedWidget) {
                // check if the widget is a spin box
                QSpinBox *spinBox = dynamic_cast<QSpinBox *>(widgets[i]);
                if (spinBox) {
                    // update the spin box for the parameter
                    spinBox->blockSignals(true);
                    spinBox->setValue(value);
					spinBox->setEnabled(enabled);
                    spinBox->blockSignals(false);
                } else {
                    // check if the widget is a slider
                    QSlider *slider = dynamic_cast<QSlider *>(widgets[i]);
                    if (slider) {
                        // update the slider for the parameter
                        slider->blockSignals(true);
                        slider->setValue(value);
						slider->setEnabled(enabled);
                        slider->blockSignals(false);
                    }
                }
            }
    }
}


//!
//! Updates the widgets showing a floating point number parameter's value.
//!
//! Is called when the value of a floating point number parameter has
//! changed.
//!
void ParameterPanel::updateFloatingPointWidgets ()
{
    NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(sender());
    if (!numberParameter)
        return;

    Parameter::Size size = numberParameter->getSize();
    if (size == 1)
        updateFloatingPointWidgets(-1);
    else
        for (Parameter::Size i = 0; i < size; ++i)
            updateFloatingPointWidgets((int) i);
}


//!
//! Updates the widgets showing the component with the given index of a
//! floating point number parameter.
//!
//! Is called when one of the values of a floating point number
//! parameter that contains a list of values has changed.
//!
//! \param index The index of the value that has changed in the list of floating point values.
//!
void ParameterPanel::updateFloatingPointWidgets ( int index )
{
    NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(sender());
    if (!numberParameter)
        return;

    // get name and value from parameter
    QString parameterName = numberParameter->getName();
    QVariant parameterValue = numberParameter->getValue();
	bool enabled = numberParameter->isEnabled();

    // adjust parameter name according to given value index
    if (index > -1)
        parameterName = QString("%1[%2]").arg(parameterName).arg(index);

    // check if a widget for the parameter exists
    if (m_widgetMap.contains(parameterName)) {
        // get the double value from the parameter value variant
        double value;
        if (index == -1)
            value = parameterValue.toDouble();
        else
            if (parameterValue.canConvert<Ogre::Vector3>())
                value = parameterValue.value<Ogre::Vector3>()[index];
            else
                value = parameterValue.toList().value(index).toDouble();

        // iterate over the widgets for displaying the unsigned integer number parameter
        QWidgetList widgets = m_widgetMap.values(parameterName);
        for (int i = 0; i < widgets.size(); ++i)
            if (widgets[i] != m_editedWidget) {
                // check if the widget is a double spin box
                QDoubleSpinBox *doubleSpinBox = dynamic_cast<QDoubleSpinBox *>(widgets[i]);
                if (doubleSpinBox) {
                    // update the double spin box for the parameter
                    doubleSpinBox->blockSignals(true);
                    doubleSpinBox->setValue(value);
					doubleSpinBox->setEnabled(enabled);
                    doubleSpinBox->blockSignals(false);
                } else {
                    // check if the widget is a double slider
                    DoubleSlider *doubleSlider = dynamic_cast<DoubleSlider *>(widgets[i]);
                    if (doubleSlider) {
                        // update the double slider for the parameter
                        doubleSlider->blockSignals(true);
                        doubleSlider->setDoubleValue(value);
						doubleSlider->setEnabled(enabled);
                        doubleSlider->blockSignals(false);
                    }
                }
            }
    }
}


//!
//! Updates the widget showing a string parameter's value.
//!
//! Is called when the value of a string parameter has changed.
//!
void ParameterPanel::updateStringWidget ()
{
    Parameter *parameter = dynamic_cast<Parameter *>(sender());
    if (!parameter)
        return;
	
    QString parameterName = parameter->getName();
    if (m_widgetMap.contains(parameterName)) {
        // get the first widget in the list of widgets (which should be a line edit)
		bool enabled = parameter->isEnabled();
        QWidgetList widgets = m_widgetMap.values(parameterName);
        QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(widgets[0]);
        if (lineEdit) {
            QString parameterValue = parameter->getValue().toString();
            lineEdit->setText(parameterValue);
			lineEdit->setEnabled(enabled);
        }
    }
}


//!
//! Updates the widgets showing a filename parameter's value.
//!
//! Is called when the value of a filename parameter has changed.
//!
void ParameterPanel::updateFilenameWidgets ()
{
    FilenameParameter *filenameParameter = dynamic_cast<FilenameParameter *>(sender());
    if (!filenameParameter)
        return;

    QString parameterName = filenameParameter->getName();
    if (m_widgetMap.contains(parameterName)) {
        // get the first widget in the list of widgets (which should be a line edit)
        QWidgetList widgets = m_widgetMap.values(parameterName);
        QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(widgets[0]);
        if (lineEdit) {
            QString parameterValue = filenameParameter->getValue().toString();
            lineEdit->setText(parameterValue);
        }
    }
}


//!
//! Updates the widget showing a color parameter's value.
//!
//! Is called when the value of a color parameter has changed.
//!
void ParameterPanel::updateColorWidget ()
{
    Parameter *parameter = dynamic_cast<Parameter *>(sender());
    if (!parameter)
        return;

    QString parameterName = parameter->getName();
    if (m_widgetMap.contains(parameterName)) {
        // get the first widget in the list of widgets (which should be a push button)
        QWidgetList widgets = m_widgetMap.values(parameterName);
        QPushButton *colorButton = dynamic_cast<QPushButton *>(widgets[0]);
        if (colorButton) {
            QColor parameterValue = parameter->getValue().value<QColor>();
            colorButton->setStyleSheet(QString("background: rgb(%1, %2, %3);").arg(parameterValue.red()).arg(parameterValue.green()).arg(parameterValue.blue()));
        }
    }
}


//!
//! Updates the widget showing an enumeration parameter's value.
//!
//! Is called when the value of an enumeration parameter has changed.
//!
void ParameterPanel::updateEnumerationWidget ()
{
    EnumerationParameter *enumerationParameter = dynamic_cast<EnumerationParameter *>(sender());
    if (!enumerationParameter)
        return;

    QString parameterName = enumerationParameter->getName();
    if (m_widgetMap.contains(parameterName)) {
        // get the first widget in the list of widgets (which should be a combo box)
        QWidgetList widgets = m_widgetMap.values(parameterName);
        QComboBox *comboBox = dynamic_cast<QComboBox *>(widgets[0]);
        if (comboBox) {
            comboBox->blockSignals(true);
            comboBox->setCurrentIndex(enumerationParameter->getCurrentIndex());
            comboBox->blockSignals(false);
        }
    }
}


//!
//! Updates the label showing a text info parameter's value.
//!
//! Is called when the value of a text info parameter has changed.
//!
void ParameterPanel::updateTextInfoWidget ()
{
    Parameter *parameter = dynamic_cast<Parameter *>(sender());
    if (!parameter)
        return;

    QString parameterName = parameter->getName();
    if (m_widgetMap.contains(parameterName)) {
        // get the first widget in the list of widgets (which should be a label)
        QWidgetList widgets = m_widgetMap.values(parameterName);
        QLabel *label = dynamic_cast<QLabel *>(widgets[0]);
        if (label) {
            QString parameterValue = parameter->getValue().toString();
            label->setText(parameterValue);
        }
    }
}


//!
//! Updates the label showing info about the image parameter's image.
//!
//! Is called when the value of an image parameter has changed.
//!
void ParameterPanel::updateImageWidget ()
{
    Parameter *imageParameter = dynamic_cast<Parameter *>(sender());
    if (!imageParameter)
        return;

    QString parameterName = imageParameter->getName();
    if (m_widgetMap.contains(parameterName)) {
        // get the first widget in the list of widgets (which should be a label)
        QWidgetList widgets = m_widgetMap.values(parameterName);
        QLabel *label = dynamic_cast<QLabel *>(widgets[0]);
        if (label) {
            if (imageParameter->getPinType() == Parameter::PT_Input)
                if (imageParameter->isConnected()) {
                    // show details of the connected image in a label widget
                    Ogre::TexturePtr inputTexture = m_node->getTextureValue(parameterName);
                    if (!inputTexture.isNull()) {
                        unsigned int width = (unsigned int) inputTexture->getWidth();
                        unsigned int height = (unsigned int) inputTexture->getHeight();
                        QString pixelFormatName = Ogre::PixelUtil::getFormatName(inputTexture->getFormat()).c_str();
                        label->setText(QString("Image dimensions: %1x%2\nPixel format: %3").arg(width).arg(height).arg(pixelFormatName));
                    }
                } else
                    label->setText("No input image connected.\n");
        }
    }
}


///
/// Private Functions
///


//!
//! Activates the custom context menu for the given label widget.
//!
//! \param label The label widget to activate a custom context menu for.
//!
void ParameterPanel::activateCustomContextMenu ( QLabel *label )
{
    if (!label) {
        Log::error("The given label widget is invalid.", "ParameterPanel::activateCustomContextMenu");
        return;
    }

    label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(label, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(labelContextMenuRequested(const QPoint &)));
}


//!
//! Updates the given widget's palette so that it is painted with a Window
//! color background instead of a Base color background.
//!
//! \param widget The widget to gray out.
//!
void ParameterPanel::grayOut ( QWidget *widget )
{
    if (!widget) {
        Log::error("The given widget is invalid.", "ParameterPanel::grayOut");
        return;
    }

    QPalette widgetPalette = widget->palette();
    widgetPalette.setColor(QPalette::Base, widgetPalette.color(QPalette::Window));
    widget->setPalette(widgetPalette);
}


//!
//! Removes all widgets from the layout.
//!
void ParameterPanel::clear ()
{
    // check if a node's parameters are currently viewed
    if (m_node)
        // disconnect all signal/slot connections from the node's parameters to the parameter panel
        m_node->getParameterRoot()->disconnectParameters(this);

    // delete all layouts from the form layout
    QList<QLayout *> layouts = ui_formLayout->findChildren<QLayout *>();
    for (int i = 0; i < layouts.size(); ++i)
        layouts[i]->deleteLater();
        //delete layouts[0];

    // delete all widgets from the scroll area
    QList<QWidget *> widgets = ui_scrollAreaWidgetContents->findChildren<QWidget *>();
    for (int i = 0; i < widgets.size(); ++i)
        widgets[i]->deleteLater();
        //delete widgets[0];

    foreach (QWidget *widget, m_widgetMap)
        widget->deleteLater();

    m_widgetMap.clear();
}


//!
//! Adds widgets for editing the parameters of the given parameter group to
//! the form layout.
//!
//! \param parameterGroup The parameter group to create editing widgets for.
//! \param formLayout The form layout to add editing widgets to.
//!
void ParameterPanel::addRows ( ParameterGroup *parameterGroup, QFormLayout *formLayout )
{
    const AbstractParameter::List *parameterList = parameterGroup->getParameterList();
    QTabWidget *tabWidget = 0;
    if (parameterGroup == m_node->getParameterRoot()) {
        // TODO
    }
    for (int i = 0; i < parameterList->size(); ++i) {
        AbstractParameter *abstractParameter = parameterList->at(i);
        if (abstractParameter->isGroup()) {
            ParameterGroup *parameterGroup = dynamic_cast<ParameterGroup *>(abstractParameter);
            // create a tab widget to contain a new form layout for the group
            if (!tabWidget) {
                tabWidget = new QTabWidget(this);
                connect(tabWidget, SIGNAL(currentChanged(int)), SLOT(loadTabPage(int)));
                // add the tab widget to the main form layout
                formLayout->addRow(tabWidget);
            }
            ParameterTabPage *page = new ParameterTabPage(tabWidget, parameterGroup);
            tabWidget->addTab(page, parameterGroup->getName());
        } else {
            Parameter *parameter = dynamic_cast<Parameter *>(abstractParameter);
            // check if the parameter is visible and if it's not an output parameter
            if (parameter->isVisible() && (parameter->getPinType() != Parameter::PT_Output || parameter->getType() != Parameter::T_Image))
                addRows(parameter, formLayout);
            tabWidget = 0;
        }
    }
}


//!
//! Adds widgets for editing the given parameter to the form layout.
//!
//! \param parameter The parameter to create editing widgets for.
//! \param formLayout The form layout to add editing widgets to.
//!
void ParameterPanel::addRows ( Parameter *parameter, QFormLayout *formLayout )
{
    // get parameter properties
    QString parameterName = parameter->getName();
    Parameter::Type parameterType = parameter->getType();
    bool enabled = parameter->isEnabled();
    bool readOnly = parameter->isReadOnly();
    bool isConnected = parameter->isConnected();

    // get the parameter's value while also triggering propagation
    QVariant parameterValue = m_node->getValue(parameterName, true);

    //QLabel *label = new QLabel(parameterName + ":", this);
    //label->setEnabled(enabled);
    //if (parameterType != Parameter::T_TextInfo && parameterType != Parameter::T_Image) {
    //    // set the top margin value for the field's label
    //    int topMargin = 1;
    //    if (parameterType == Parameter::T_Filename || parameterType == Parameter::T_Color)
    //        topMargin = 3;
    //    label->setContentsMargins(0, topMargin, 0, 0);
    //}

    // create widgets for the row for editing the parameter's value depending on its type
    if (parameter->getSize() == 1) {
        // depending on the type of parameter either a simple widget or a layout with several widgets is created
        QWidget *widget = 0;
        QLayout *layout = 0;
        switch (parameterType) {
            case Parameter::T_Bool:
                //label->setText(""); // don't show a label for boolean parameters
                widget = createBooleanWidget(parameter);
                break;
            case Parameter::T_Int:
                layout = createIntegerLayout(dynamic_cast<NumberParameter *>(parameter));
                break;
            case Parameter::T_UnsignedInt:
                layout = createUnsignedIntegerLayout(dynamic_cast<NumberParameter *>(parameter));
                break;
            case Parameter::T_Float:
                layout = createFloatingPointLayout(dynamic_cast<NumberParameter *>(parameter));
                break;
            case Parameter::T_String:
                widget = createStringWidget(parameter);
                break;
            case Parameter::T_Filename:
                layout = createFilenameLayout(dynamic_cast<FilenameParameter *>(parameter));
                break;
            case Parameter::T_Color:
                widget = createColorWidget(parameter);
                connect(parameter, SIGNAL(valueChanged()), SLOT(updateColorWidget()));
                break;
            case Parameter::T_Enumeration:
                widget = createEnumerationWidget(dynamic_cast<EnumerationParameter *>(parameter));
                connect(parameter, SIGNAL(valueChanged()), SLOT(updateEnumerationWidget()));
                break;
            case Parameter::T_TextInfo:
                widget = createTextInfoWidget(parameter);
                break;
            case Parameter::T_Command:
                //label->setText(""); // don't show a label for command parameters
                widget = createCommandWidget(parameter);
                break;
            case Parameter::T_Geometry:
                //widget = new QLabel(parameterValue.toString());
                return;
                break;
            case Parameter::T_Light:
                //widget = new QLabel(parameterValue.toString());
                return;
                break;
            case Parameter::T_Camera:
                //widget = new QLabel(parameterValue.toString());
                return;
                break;
            case Parameter::T_Image:
                //label->setText(label->text() + "\n");
                widget = createImageWidget(parameter);
                connect(parameter, SIGNAL(valueChanged()), SLOT(updateImageWidget()));
                break;
            case Parameter::T_VTKTable:
                return;
                break;
			case Parameter::T_VTKTree:
                return;
                break;
			case Parameter::T_VTKGraph:
                return;
                break;
            case Parameter::T_Group:
                {
                    ParameterGroup *parameterGroup = m_node->getGroupValue(parameter->getName());
                    if (parameterGroup) {
                        int numberOfParameters = parameterGroup->getParameterList()->size();
                        widget = new QLabel(QString("Number of contained parameters: %1").arg(numberOfParameters));
                    }
                }
                break;
			case Parameter::T_PlugIn:
					widget = createPluginWidget(parameter);
				break;
            default:
					Log::warning(QString("Could not create editing widgets for parameter \"%1\": Type \"%2\" not supported yet.").arg(parameterName).arg(Parameter::getTypeName(parameterType)), "ParameterPanel::addRows");
        }

        // create the label for the row
        QHBoxLayout *hBoxLayout = new QHBoxLayout;
        QGroupBox *groupBox = new QGroupBox(parameterName, this);
        /*groupBox->setStyleSheet("QGroupBox {background-color: rgb(220, 220, 220); border-radius: 5px; margin-top: 2ex;}\
                                QGroupBox::title {subcontrol-origin: margin; subcontrol-position: top left; padding: 0px 15px;}\
                                QSlider {background-color: rgb(220, 220, 220);}");
		*/
        // add the widget (if it exists) to the form layout
        if (widget)
            //formLayout->addRow(label, widget);
            //groupBox->addWidget(widget);
            //hBoxLayout->addWidget(label);
            hBoxLayout->addWidget(widget);
        // add the layout (if it exists) to the form layout
        if (layout)
            //formLayout->addRow(label, layout);
            //groupBox->addLayout(layout);
            //hBoxLayout->addWidget(label);
            hBoxLayout->addLayout(layout);

        groupBox->setLayout(hBoxLayout);
        formLayout->addRow(groupBox);
    } else {
        // create the label for the row
        QHBoxLayout *hBoxLayout = new QHBoxLayout;
        QGroupBox *groupBox = new QGroupBox(parameterName, this);
        /* groupBox->setStyleSheet("QGroupBox {background-color: rgb(220, 220, 220); border-radius: 5px; margin-top: 2ex;}\
                                QGroupBox::title {subcontrol-origin: margin; subcontrol-position: top left; padding: 0px 15px;}\
                                QSlider {background-color: rgb(220, 220, 220);}");
        */
		//formLayout->addRow(label, createVectorLayout(parameter));
        hBoxLayout->addLayout(createVectorLayout(parameter));
        groupBox->setLayout(hBoxLayout);
        formLayout->addRow(groupBox);
    }
}


//!
//! Creates a widget for editing a boolean parameter.
//!
//! \param parameter The parameter to create an editing widget for.
//! \return A widget for editing a boolean parameter.
//!
QWidget * ParameterPanel::createBooleanWidget ( Parameter *parameter )
{
    // get parameter properties
    QString name = parameter->getName();

    // create an editing widget for the parameter
    //QCheckBox *checkBox = new QCheckBox(name, this);
    QCheckBox *checkBox = new QCheckBox(this);
    checkBox->setProperty("parameterName", name);
    checkBox->setChecked(parameter->getValue().toBool());

    m_widgetMap.insertMulti(name, checkBox);

    // set up connections for synching the widgets with the parameter
    connect(parameter, SIGNAL(valueChanged()), SLOT(updateBooleanWidget()));
    connect(checkBox, SIGNAL(toggled(bool)), SLOT(checkBoxToggled(bool)), Qt::QueuedConnection);

    return checkBox;
}


//!
//! Creates a layout with widgets for editing an integer parameter.
//!
//! \param numberParameter The number parameter to create an editing widget for.
//! \param index The index of the value to edit (for parameters that contain lists of values).
//! \return A layout with widgets for editing an integer parameter.
//!
QLayout * ParameterPanel::createIntegerLayout ( NumberParameter *numberParameter, int index /* = 0 */ )
{
    // make sure the given number parameter is valid
    if (!numberParameter) {
        Log::error("The number parameter given is invalid.", "ParameterPanel::createIntegerLayout");
        return 0;
    }

    // get parameter properties
    QString name = numberParameter->getName();
    Parameter::Type parameterType = numberParameter->getType();
    bool enabled = numberParameter->isEnabled();
    bool readOnly = numberParameter->isReadOnly();
    int minValue = numberParameter->getMinValue().toInt();
    int maxValue = numberParameter->getMaxValue().toInt();
    QVariant parameterValue = numberParameter->getValue();
    int stepSize = numberParameter->getStepSize().toInt();
    QString unit = numberParameter->getUnit();
    NumberParameter::InputMethod inputMethod = numberParameter->getInputMethod();
    bool isList = parameterValue.type() == QVariant::List;

    // process parameter value
    int value = 0;
    if (isList)
        value = parameterValue.toList().value(index).toInt();
    else
        value = parameterValue.toInt();

    // process parameter name
    QString parameterName = name;
    if (isList)
        parameterName = QString("%1[%2]").arg(name).arg(index);

    // create the layout and widgets
    QHBoxLayout *layout = new QHBoxLayout();
    QSpinBox *spinBox = 0;
    QSlider *slider = 0;
    if (inputMethod == NumberParameter::IM_SliderPlusSpinBox || inputMethod == NumberParameter::IM_SpinBox) {
        spinBox = new QSpinBox(this);
        spinBox->setProperty("parameterName", parameterName);
        spinBox->setProperty("parameterType", (int) parameterType);
        spinBox->setMinimum(minValue);
        spinBox->setMaximum(maxValue);
        spinBox->setSingleStep(stepSize);
        spinBox->setValue(value);
        if (!unit.isEmpty())
            spinBox->setSuffix(unit);
        spinBox->setEnabled(enabled);
        spinBox->setReadOnly(readOnly);
        if (enabled && readOnly)
            grayOut(spinBox);

        layout->addWidget(spinBox);
        m_widgetMap.insertMulti(parameterName, spinBox);
    }
    if (inputMethod == NumberParameter::IM_SliderPlusSpinBox || inputMethod == NumberParameter::IM_Slider) {
        slider = new QSlider(this);
        slider->setOrientation(Qt::Horizontal);
        slider->setProperty("parameterName", parameterName);
        slider->setProperty("parameterType", (int) parameterType);
        slider->setMinimum(minValue);
        slider->setMaximum(maxValue);
        slider->setSingleStep(stepSize);
        slider->setValue(value);
        slider->setEnabled(enabled && !readOnly);

        layout->addWidget(slider);
        m_widgetMap.insertMulti(parameterName, slider);
    }

    // set up connections for syncing the widgets with the parameter
    if (isList) {
        // only set up one valueChanged() connection (for the parameter's first component)
        if (index == 0)
            connect(numberParameter, SIGNAL(valueChanged(int)), SLOT(updateIntegerWidgets(int)));
    } //else
    // NILZ: Commented out else, otherwise valueChanged() will be omitted, but is needed when
    // complete m_value is changed
    connect(numberParameter, SIGNAL(valueChanged()), SLOT(updateIntegerWidgets()));

    // set up connections for syncing the parameter with the widgets
    if (spinBox)
        connect(spinBox, SIGNAL(editingFinished()), SLOT(spinBoxEditingFinished()), Qt::QueuedConnection);
    if (slider)
        connect(slider, SIGNAL(valueChanged(int)), SLOT(sliderValueChanged()), Qt::QueuedConnection);

    return layout;
}


//!
//! Creates a layout with widgets for editing an unsigned integer
//! parameter.
//!
//! \param numberParameter The number parameter to create an editing widget for.
//! \param index The index of the value to edit (for parameters that contain lists of values).
//! \return A layout with widgets for editing an unsigned integer parameter.
//!
QLayout * ParameterPanel::createUnsignedIntegerLayout ( NumberParameter *numberParameter, int index /* = 0 */ )
{
    // make sure the given number parameter is valid
    if (!numberParameter) {
        Log::error("The number parameter given is invalid.", "ParameterPanel::createUnsignedIntegerLayout");
        return 0;
    }

    // get parameter properties
    QString name = numberParameter->getName();
    Parameter::Type parameterType = numberParameter->getType();
    bool enabled = numberParameter->isEnabled();
    bool readOnly = numberParameter->isReadOnly();
    unsigned int minValue = numberParameter->getMinValue().toUInt();
    unsigned int maxValue = numberParameter->getMaxValue().toUInt();
    QVariant parameterValue = numberParameter->getValue();
    unsigned int stepSize = numberParameter->getStepSize().toUInt();
    QString unit = numberParameter->getUnit();
    NumberParameter::InputMethod inputMethod = numberParameter->getInputMethod();
    bool isList = numberParameter->getValue().type() == QVariant::List;

    // process parameter value
    unsigned int value = 0;
    if (isList)
        value = parameterValue.toList().value(index).toUInt();
    else
        value = parameterValue.toUInt();

    // process parameter name
    QString parameterName = name;
    if (isList)
        parameterName = QString("%1[%2]").arg(name).arg(index);

    // create the layout and widgets
    QHBoxLayout *layout = new QHBoxLayout();
    QSpinBox *spinBox = 0;
    QSlider *slider = 0;
    if (inputMethod == NumberParameter::IM_SliderPlusSpinBox || inputMethod == NumberParameter::IM_SpinBox) {
        spinBox = new QSpinBox(this);
        spinBox->setProperty("parameterName", parameterName);
        spinBox->setProperty("parameterType", (int) parameterType);
        spinBox->setMinimum(minValue);
        spinBox->setMaximum(maxValue);
        spinBox->setSingleStep(stepSize);
        spinBox->setValue(value);
        if (!unit.isEmpty())
            spinBox->setSuffix(unit);
        spinBox->setEnabled(enabled);
        spinBox->setReadOnly(readOnly);
        if (enabled && readOnly)
            grayOut(spinBox);

        layout->addWidget(spinBox);
        m_widgetMap.insertMulti(parameterName, spinBox);
    }
    if (inputMethod == NumberParameter::IM_SliderPlusSpinBox || inputMethod == NumberParameter::IM_Slider) {
        slider = new QSlider(this);
        slider->setOrientation(Qt::Horizontal);
        slider->setProperty("parameterName", parameterName);
        slider->setProperty("parameterType", (int) parameterType);
        slider->setMinimum(minValue);
        slider->setMaximum(maxValue);
        slider->setSingleStep(stepSize);
        slider->setValue(value);
        slider->setEnabled(enabled && !readOnly);

        layout->addWidget(slider);
        m_widgetMap.insertMulti(parameterName, slider);
    }

    // set up connections for syncing the widgets with the parameter
    if (isList) {
        // only set up one valueChanged() connection (for the parameter's first component)
        if (index == 0)
            connect(numberParameter, SIGNAL(valueChanged(int)), SLOT(updateUnsignedIntegerWidgets(int)));
    } //else
    // NILZ: Commented out else, otherwise valueChanged() will be omitted, but is needed when
    // complete m_value is changed
    connect(numberParameter, SIGNAL(valueChanged()), SLOT(updateUnsignedIntegerWidgets()));

    // set up connections for syncing the parameter with the widgets
    if (spinBox)
        connect(spinBox, SIGNAL(editingFinished()), SLOT(spinBoxEditingFinished()), Qt::QueuedConnection);
    if (slider)
        connect(slider, SIGNAL(valueChanged(int)), SLOT(sliderValueChanged()), Qt::QueuedConnection);

    return layout;
}


//!
//! Creates a layout with widgets for editing a floating point parameter.
//!
//! \param numberParameter The number parameter to create an editing widget for.
//! \param index The index of the value to edit (for parameters that contain lists of values).
//! \return A layout with widgets for editing a floating point parameter.
//!
QLayout * ParameterPanel::createFloatingPointLayout ( NumberParameter *numberParameter, int index /* = 0 */ )
{
    // make sure the given number parameter is valid
    if (!numberParameter) {
        Log::error("The number parameter given is invalid.", "ParameterPanel::createFloatingPointLayout");
        return 0;
    }

    // get parameter properties
    QString name = numberParameter->getName();
    bool enabled = numberParameter->isEnabled();
    bool readOnly = numberParameter->isReadOnly();
    double minValue = numberParameter->getMinValue().toDouble();
    double maxValue = numberParameter->getMaxValue().toDouble();
    QVariant parameterValue = numberParameter->getValue();
    double stepSize = numberParameter->getStepSize().toDouble();
    QString unit = numberParameter->getUnit();
    NumberParameter::InputMethod inputMethod = numberParameter->getInputMethod();
    bool isList = parameterValue.type() == QVariant::List || parameterValue.canConvert<Ogre::Vector3>();

    // process parameter value
    double value = 0.0;
    if (isList) {
        if (parameterValue.canConvert<Ogre::Vector3>())
            value = parameterValue.value<Ogre::Vector3>()[index];
        else
            value = parameterValue.toList().value(index).toDouble();
    } else
        value = parameterValue.toDouble();

    // process parameter name
    QString parameterName = name;
    if (isList)
        parameterName = QString("%1[%2]").arg(name).arg(index);

    // create the layout and widgets
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(8);

    // NILZ: Add layers for add or mult
    //QPushButton *layerButton = new QPushButton(this);
    //layerButton->setText("+");
    //layerButton->setFixedSize(15, 15);
    //layout->addWidget(layerButton);

    QDoubleSpinBox *doubleSpinBox = 0;
    DoubleSlider *doubleSlider = 0;
    if (inputMethod == NumberParameter::IM_SliderPlusSpinBox || inputMethod == NumberParameter::IM_SpinBox) {
        doubleSpinBox = new QDoubleSpinBox(this);
        doubleSpinBox->setProperty("parameterName", parameterName);
        doubleSpinBox->setMinimum(minValue);
        doubleSpinBox->setMaximum(maxValue);
        doubleSpinBox->setSingleStep(stepSize);
        doubleSpinBox->setValue(value);
        doubleSpinBox->setMinimumWidth(66);
        if (!unit.isEmpty())
            doubleSpinBox->setSuffix(unit);
        doubleSpinBox->setEnabled(enabled);
        doubleSpinBox->setReadOnly(readOnly);
        if (enabled && readOnly)
            grayOut(doubleSpinBox);
        else
            if (numberParameter->isAnimated())
                doubleSpinBox->setStyleSheet("background-color: lightgreen;");

        layout->addWidget(doubleSpinBox);
        m_widgetMap.insertMulti(parameterName, doubleSpinBox);
    }
    if (inputMethod == NumberParameter::IM_SliderPlusSpinBox || inputMethod == NumberParameter::IM_Slider) {
        doubleSlider = new DoubleSlider(this);
        doubleSlider->setOrientation(Qt::Horizontal);
        doubleSlider->setProperty("parameterName", parameterName);
        doubleSlider->setDoubleMinimum(minValue);
        doubleSlider->setDoubleMaximum(maxValue);
        doubleSlider->setDoubleSingleStep(stepSize);
        doubleSlider->setDoubleValue(value);
        doubleSlider->setEnabled(enabled && !readOnly);

        layout->addWidget(doubleSlider);
        m_widgetMap.insertMulti(parameterName, doubleSlider);
    }

    // set up connections for syncing the widgets with the parameter
    if (isList) {
        // only set up one valueChanged() connection (for the parameter's first component)
        if (index == 0)
            connect(numberParameter, SIGNAL(valueChanged(int)), SLOT(updateFloatingPointWidgets(int)));
    } // else
    // NILZ: Commented out else, otherwise valueChanged() will be omitted, but is needed when
    // complete m_value is changed
    connect(numberParameter, SIGNAL(valueChanged()), SLOT(updateFloatingPointWidgets()));

    // set up connections for syncing the parameter with the widgets
    if (doubleSpinBox)
        connect(doubleSpinBox, SIGNAL(editingFinished()), SLOT(doubleSpinBoxEditingFinished()), Qt::QueuedConnection);
    if (doubleSlider)
        connect(doubleSlider, SIGNAL(doubleValueChanged(double)), SLOT(doubleSliderValueChanged()), Qt::QueuedConnection);

    return layout;
}


//!
//! Creates a widget for editing a string parameter or displaying a read-only
//! parameter's value.
//!
//! \param parameter The string parameter to create an editing widget for.
//! \return A widget for editing a string parameter.
//!
QWidget * ParameterPanel::createStringWidget ( Parameter *parameter )
{
    // get parameter properties
    QString parameterName = parameter->getName();
    QString parameterValue = parameter->getValue().toString();
    bool enabled = parameter->isEnabled();
    bool readOnly = parameter->isReadOnly();

    // create an editing widget for the string
    QLineEdit *lineEdit = new QLineEdit(parameterValue, this);
    lineEdit->setProperty("parameterName", parameterName);
    lineEdit->setEnabled(enabled);
    lineEdit->setReadOnly(readOnly);
    if (enabled && readOnly)
        grayOut(lineEdit);

    m_widgetMap.insertMulti(parameterName, lineEdit);

    // set up connections for synching the widgets with the parameter
    connect(parameter, SIGNAL(valueChanged()), SLOT(updateStringWidget()));
    connect(lineEdit, SIGNAL(editingFinished()), SLOT(lineEditEditingFinished()), Qt::QueuedConnection);

    return lineEdit;
}


//!
//! Creates a layout with widgets for editing a filename parameter.
//!
//! \param filenameParameter The filename parameter to create an editing widget for.
//! \return A layout with widgets for editing a filename parameter.
//!
QLayout * ParameterPanel::createFilenameLayout ( FilenameParameter *filenameParameter )
{
    // make sure the given filename parameter is valid
    if (!filenameParameter) {
        Log::error("The filename parameter given is invalid.", "ParameterPanel::createFilenameLayout");
        return 0;
    }

    // get parameter properties
    QString parameterName = filenameParameter->getName();
    QString filename = filenameParameter->getValue().toString();
    bool enabled = filenameParameter->isEnabled();
    bool readOnly = filenameParameter->isReadOnly();

    // create an editing widget for the filename
    QLineEdit *lineEdit = new QLineEdit(filename, this);
    lineEdit->setProperty("parameterName", parameterName);
    lineEdit->setEnabled(enabled);
    lineEdit->setReadOnly(readOnly);
    if (enabled && readOnly)
        grayOut(lineEdit);

    m_widgetMap.insertMulti(parameterName, lineEdit);

    // create a browse button
    QPushButton *browseButton = new QPushButton(tr("Browse..."), this);
    browseButton->setProperty("parameterName", parameterName);
    browseButton->setEnabled(enabled);

    // create a reload button
    QPushButton *reloadButton = new QPushButton(tr("Reload"), this);
    reloadButton->setProperty("parameterName", parameterName);

    // set up connections for synching the widgets with the parameter
    connect(filenameParameter, SIGNAL(valueChanged()), SLOT(updateFilenameWidgets()));
    connect(browseButton, SIGNAL(clicked()), SLOT(browseButtonClicked()), Qt::QueuedConnection);
    connect(reloadButton, SIGNAL(clicked()), SLOT(reloadButtonClicked()), Qt::QueuedConnection);

    // create the layout with the created filename widgets as children
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(lineEdit, 1);
    layout->addWidget(browseButton);
    layout->addWidget(reloadButton);
    return layout;
}


//!
//! Creates a widget for editing a color parameter.
//!
//! \param parameter The color parameter to create an editing widget for.
//! \return A widget for editing a color parameter.
//!
QWidget * ParameterPanel::createColorWidget ( Parameter *parameter )
{
    // get parameter properties
    QString parameterName = parameter->getName();
    QColor color = parameter->getValue().value<QColor>();

    // create an editing widget for the color
    QPushButton *colorButton = new QPushButton();
    colorButton->setProperty("parameterName", parameterName);
    colorButton->setMaximumWidth(75);
    colorButton->setStatusTip(tr("Click to select a color"));
    colorButton->setStyleSheet(QString("background: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    m_widgetMap.insertMulti(parameterName, colorButton);

    // set up connections for synching the widgets with the parameter
    connect(colorButton, SIGNAL(clicked(bool)), SLOT(selectColor()), Qt::QueuedConnection);

    return colorButton;
}


//!
//! Creates a widget for editing an enumeration parameter.
//!
//! \param enumerationParameter The enumeration parameter to create an editing widget for.
//! \return A widget for editing an enumeration parameter.
//!
QWidget * ParameterPanel::createEnumerationWidget ( EnumerationParameter *enumerationParameter )
{
    // make sure the given enumeration parameter is valid
    if (!enumerationParameter) {
        Log::error("The enumeration parameter given is invalid.", "ParameterPanel::createEnumerationWidget");
        return 0;
    }

    // get parameter properties
    QString parameterName = enumerationParameter->getName();
    QStringList literals = enumerationParameter->getLiterals();
    QStringList values = enumerationParameter->getValues();
    int parameterValue = enumerationParameter->getValue().toInt();

    // create an editing widget for the enumeration
    QComboBox *comboBox = new QComboBox(this);
    comboBox->setProperty("parameterName", parameterName);
    for (int i = 0; i < literals.size(); ++i)
        if (literals.at(i) == Parameter::EnumerationSeparator)
            comboBox->insertSeparator(i);
        else {
            if (values.at(i).isEmpty())
                comboBox->insertItem(i, literals.at(i));
            else
                comboBox->insertItem(i, QString("%1 (%2)").arg(literals.at(i)).arg(values.at(i)));
        }
    comboBox->setCurrentIndex(parameterValue);

    m_widgetMap.insertMulti(parameterName, comboBox);

    // set up connections for synching the widgets with the parameter
    connect(comboBox, SIGNAL(currentIndexChanged(int)), SLOT(comboBoxIndexChanged(int)), Qt::QueuedConnection);

    return comboBox;
}


//!
//! Creates a label widget for displaying the text info contained in the
//! given parameter.
//!
//! \param parameter The text info parameter to create a label widget for.
//! \return A label widget for displaying the text info contained in the given parameter.
//!
QWidget * ParameterPanel::createTextInfoWidget ( Parameter *parameter )
{
    // get parameter properties
    QString parameterName = parameter->getName();
    QString parameterValue = parameter->getValue().toString();

    // create a display widget for the text info
    QLabel *label = new QLabel(parameterValue);
    label->setProperty("parameterName", parameterName);
    activateCustomContextMenu(label);

    m_widgetMap.insertMulti(parameterName, label);

    // set up connections for synching the widgets with the parameter
    connect(parameter, SIGNAL(valueChanged()), SLOT(updateTextInfoWidget()));

    return label;
}


//!
//! Creates a button widget for executing the command represented by the
//! given parameter.
//!
//! \param parameter The command parameter to create a button widget for.
//! \return A widget for execting the command represented by the given parameter.
//!
QWidget * ParameterPanel::createCommandWidget ( Parameter *parameter )
{
    // get parameter properties
    QString parameterName = parameter->getName();

    // create an button widget for the command
    QPushButton *button = new QPushButton(parameterName);
    button->setProperty("parameterName", parameterName);

    // set up connection for executing the command
    connect(button, SIGNAL(clicked(bool)), SLOT(commandButtonClicked())); //, Qt::QueuedConnection);

    return button;
}


//!
//! Creates a widget displaying information about the image represented by
//! the given parameter.
//!
//! \param parameter The image parameter to create a widget for.
//! \return A widget displaying information about the image represented by the given parameter, or 0 if not applicable.
//!
QWidget * ParameterPanel::createImageWidget ( Parameter *parameter )
{
    QLabel *label = 0;
    if (parameter->getPinType() == Parameter::PT_Input)
        if (parameter->isConnected()) {
            // show details of the connected image in a label widget
            Ogre::TexturePtr inputTexture = m_node->getTextureValue(parameter->getName());
            if (!inputTexture.isNull()) {
                unsigned int width = (unsigned int) inputTexture->getWidth();
                unsigned int height = (unsigned int) inputTexture->getHeight();
                QString pixelFormatName = Ogre::PixelUtil::getFormatName(inputTexture->getFormat()).c_str();
                label = new QLabel(QString("Image dimensions: %1x%2\nPixel format: %3").arg(width).arg(height).arg(pixelFormatName));
            }
        } else
            label = new QLabel("No input image connected.\n");

    // check if the label exists and if so, activate the custom context menu for the label
    if (label)
        activateCustomContextMenu(label);

    return label;
}


//!
//! Creates a layout with widgets for editing a parameter that stores
//! several values of a specific type.
//!
//! \param parameter The vector parameter to create a layout with editing widgets for.
//! \return A layout with widgets for editing a parameter that stores several values of a specific type.
//!
QLayout * ParameterPanel::createVectorLayout ( Parameter *parameter )
{
    // get parameter properties
    Parameter::Type parameterType = parameter->getType();
    QVariant parameterValue = parameter->getValue();
    QVariantList values = parameterValue.toList();

    // create the layout
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(16);

    // check if the parameter is a numeric parameter
    if (parameterType == Parameter::T_Int || parameterType == Parameter::T_UnsignedInt || parameterType == Parameter::T_Float) {
        NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(parameter);
        // check if the number parameter contains its values in an OGRE vector
        unsigned int numberOfValues = values.size();
        if (parameterValue.canConvert<Ogre::Vector3>())
            numberOfValues = 3;

        // iterate over the number of values
        for (unsigned int i = 0; i < numberOfValues; ++i) {
            QLayout *layout = 0;
            switch (parameterType) {
                case Parameter::T_Int:
                    layout = createIntegerLayout(numberParameter, i);
                    break;
                case Parameter::T_UnsignedInt:
                    layout = createUnsignedIntegerLayout(numberParameter, i);
                    break;
                case Parameter::T_Float:
                    layout = createFloatingPointLayout(numberParameter, i);
                    break;
            }
            if (layout)
                hboxLayout->addLayout(layout);
        }
    } else {
        // add a new label with the custom context menu to the layout
        QLabel *label = new QLabel(QString("Number of values in list: %1").arg(values.size()));
        activateCustomContextMenu(label);
        hboxLayout->addWidget(label);
    }

    return hboxLayout;
}


//!
//! Notifies connected objects that a parameter of the currently edited
//! object should be changed.
//!
//! \param parameterName The name of the parameter to change.
//! \param value The new value for the parameter.
//!
void ParameterPanel::requestParameterChange ( const QString &parameterName, const QVariant &value )
{
    m_editedWidget = sender();
    emit parameterChangeRequested(m_node->getName(), parameterName, value);
    m_editedWidget = 0;
}


//!
//! Filters the parameters displayed in the panel by the search text
//! entered in the search edit widget.
//!
void ParameterPanel::filterParameters ()
{
    if (m_node) {
        m_node->setSearchText(m_searchEdit->text());
        showParameters(m_node);
    }
}


//!
//! Creates a plugin widget
//!
//! \param parameter the parameter of the widget
//! \return The plugin widget
//!
QWidget * ParameterPanel::createPluginWidget ( Parameter *parameter)
{
	ParameterPlugin *parameterPlugin =  dynamic_cast<ParameterPlugin *>(parameter);

	if(!parameterPlugin){
		Log::error(QString("Cannot convert parameter."), "ParameterPanel::createPluginWidget");
		QWidget * w = new QWidget(this);
		return w;
	}

	QString call = parameterPlugin->getCall();
	QString parameterName = parameterPlugin->getName();
	
	WidgetPlugin *widget = WidgetFactory::createWidget(call, parameterPlugin, this);

	m_widgetMap.insertMulti(parameterName, widget);

	return widget;
}
