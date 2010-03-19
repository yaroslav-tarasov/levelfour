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
//! \file "ParameterPanel.h"
//! \brief Header file for ParameterPanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       24.06.2009 (last updated)
//!

#ifndef PARAMETERPANEL_H
#define PARAMETERPANEL_H

#include "FrapperPrerequisites.h"
#include "ViewPanel.h"
#include "ui_ParameterPanel.h"
#include "Node.h"
#include "Parameter.h"
#include "NumberParameter.h"
#include "FilenameParameter.h"
#include "EnumerationParameter.h"
#include <QtGui/QItemSelection>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>


//!
//! Class representing a view for displaying and editing parameters of nodes.
//!
class FRAPPER_GUI_EXPORT ParameterPanel : public ViewPanel, protected Ui::ParameterPanel
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the ParameterPanel class.
    //!
    //! \param parent The parent widget the created instance will be a child of.
    //! \param flags Extra widget options.
    //!
    ParameterPanel ( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

    //!
    //! Destructor of the ParameterPanel class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ParameterPanel ();

public slots: //

    //!
    //! Displays the parameters and values for the given node.
    //!
    //! \param node The node whose parameters to display in the parameter view.
    //!
    void showParameters ( Node *node );

    //!
    //! Slot that is called when the selection in the scene model has changed.
    //!
    //! \param objectsSelected Flag that states whether objects in the scene are selected.
    //!
    void updateParameters ( bool objectsSelected );

public: // functions

    //!
    //! Fills the given tool bars in a panel frame with actions for the panel.
    //!
    //! \param mainToolBar The main tool bar to fill with actions.
    //! \param panelToolBar The panel tool bar to fill with actions.
    //!
    virtual void fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar );

signals: //

    //!
    //! Signal that is emitted when the name of the currently edited node
    //! should change.
    //!
    //! \param oldName The name of the object to rename.
    //! \param newName The new name for the object to rename.
    //!
    void objectNameChangeRequested ( const QString &oldName, const QString &newName );

    //!
    //! Signal that is emitted when the value of the parameter with the given
    //! name of the object with the given name should be changed to the given
    //! value.
    //!
    //! \param objectName The name of the object for which to change the parameter.
    //! \param parameterName The name of the parameter to change.
    //! \param value The new value for the parameter.
    //!
    void parameterChangeRequested ( const QString &objectName, const QString &parameterName, const QVariant &value );

protected: // events

    //!
    //! Event handler that reacts to timer events.
    //!
    //! \param event The description of the timer event.
    //!
    virtual void timerEvent ( QTimerEvent *event );

private slots: //

    //!
    //! Requests a name change for the currently edited node and displays the
    //! successfully changed name in the node name edit widget.
    //!
    //! Is called when editing the node's name has finished.
    //!
    void on_m_nodeNameEdit_editingFinished ();

    //!
    //! Starts the timer that launches the parameter filtering search after a
    //! specific delay.
    //!
    //! Is called when the text in the search edit widget has changed.
    //!
    //! \param text The text that has been entered in the search edit widget.
    //!
    void on_m_searchEdit_textChanged ( const QString &text );

    //!
    //! Cancels a running parameter search and resets the parameter search
    //! settings for the currently edited node.
    //!
    //! Is called when the reset search action has been triggered.
    //!
    //! \param checked The state of the action (unused).
    //!
    void on_ui_resetSearchAction_triggered ( bool checked = false );

    //!
    //! Fills the tab page of the given index with widgets for editing the
    //! parameters that the tab page represents.
    //!
    //! Is called when the index of the current tab in a tab widget has
    //! changed.
    //!
    //! \param index The index of the tab page to fill with editing widgets.
    //!
    void loadTabPage ( int index );

    //!
    //! Opens a context menu at the given position with actions for the label.
    //!
    //! Is called when a context menu is requested for a label widget.
    //!
    //! \param position The position at which to open the context menu.
    //!
    void labelContextMenuRequested ( const QPoint &position );

private slots: // for updating parameter values when widgets change

    //!
    //! Applies an edited boolean value to the parameter represented by the
    //! checkbox widget calling this slot.
    //!
    //! Is called when a checkbox representing a parameter has been toggled.
    //!
    //! \param checked The state of the check box.
    //!
    void checkBoxToggled ( bool checked );

    //!
    //! Applies an edited numeric value to the parameter represented by the
    //! spinbox widget calling this slot.
    //!
    //! Is called when editing the value of a spin box representing a parameter
    //! has finished.
    //!
    void spinBoxEditingFinished ();

    //!
    //! Applies an edited numeric value to the parameter represented by the
    //! slider widget calling this slot.
    //!
    //! Is called when the value of a slider representing a parameter has
    //! changed.
    //!
    void sliderValueChanged ();

    //!
    //! Applies an edited floating point numeric value to the parameter
    //! represented by the spinbox widget calling this slot.
    //!
    //! Is called when editing the value of a double spin box representing a
    //! parameter has finished.
    //!
    void doubleSpinBoxEditingFinished ();

    //!
    //! Applies an edited floating point numeric value to the parameter
    //! represented by the slider widget calling this slot.
    //!
    //! Is called when the value of a double slider representing a parameter
    //! has changed.
    //!
    void doubleSliderValueChanged ();

    //!
    //! Applies an edited string value to the parameter represented by the line
    //! edit widget calling this slot.
    //!
    //! Is called when editing the text of a line edit widget representing a
    //! parameter has finished.
    //!
    void lineEditEditingFinished ();

    //!
    //! Displays a dialog for selecting a file for the edited parameter.
    //!
    //! Is called when the browse button for a filename parameter has been
    //! clicked.
    //!
    void browseButtonClicked ();

    //!
    //! Reloads the file that the edited parameter represents.
    //!
    //! Is called when the reload button for a filename parameter has been
    //! clicked.
    //!
    void reloadButtonClicked ();

    //!
    //! Displays a dialog for selecting a color for the edited parameter.
    //!
    //! Is called when the color button representing a color parameter has been
    //! clicked.
    //!
    void selectColor ();

    //!
    //! Applies a changed combo box item selection to the edited parameter.
    //!
    //! Is called when the index of the currently selected item of a combo box
    //! representing an enumeration parameter has changed.
    //!
    //! \param index The index of the item currently selected in the combo box.
    //!
    void comboBoxIndexChanged ( int index );

    //!
    //! Requests the execution of the command represented by the clicked
    //! button.
    //!
    //! Is called when the button representing a command parameter is clicked.
    //!
    void commandButtonClicked ();

private slots: // for updating widgets when parameter values change

    //!
    //! Updates the widget showing a boolean parameter's value.
    //!
    //! Is called when the value of a boolean parameter has changed.
    //!
    void updateBooleanWidget ();

    //!
    //! Updates the widgets showing an integer number parameter's value.
    //!
    //! Is called when the value of an integer number parameter has changed.
    //!
    void updateIntegerWidgets ();

    //!
    //! Updates the widgets showing the component with the given index of an
    //! integer number parameter.
    //!
    //! Is called when one of the values of an integer number parameter that
    //! contains a list of values has changed.
    //!
    //! \param index The index of the value that has changed in the list of integer values.
    //!
    void updateIntegerWidgets ( int index );

    //!
    //! Updates the widgets showing an unsigned integer number parameter's
    //! value.
    //!
    //! Is called when the value of an unsigned integer number parameter has
    //! changed.
    //!
    void updateUnsignedIntegerWidgets ();

    //!
    //! Updates the widgets showing the component with the given index of an
    //! unsigned integer number parameter.
    //!
    //! Is called when one of the values of an unsigned integer number
    //! parameter that contains a list of values has changed.
    //!
    //! \param index The index of the value that has changed in the list of unsigned integer values.
    //!
    void updateUnsignedIntegerWidgets ( int index );

    //!
    //! Updates the widgets showing a floating point number parameter's value.
    //!
    //! Is called when the value of a floating point number parameter has
    //! changed.
    //!
    void updateFloatingPointWidgets ();

    //!
    //! Updates the widgets showing the component with the given index of a
    //! floating point number parameter.
    //!
    //! Is called when one of the values of a floating point number
    //! parameter that contains a list of values has changed.
    //!
    //! \param index The index of the value that has changed in the list of floating point values.
    //!
    void updateFloatingPointWidgets ( int index );

    //!
    //! Updates the widget showing a string parameter's value.
    //!
    //! Is called when the value of a string parameter has changed.
    //!
    void updateStringWidget ();

    //!
    //! Updates the widgets showing a filename parameter's value.
    //!
    //! Is called when the value of a filename parameter has changed.
    //!
    void updateFilenameWidgets ();

    //!
    //! Updates the widget showing a color parameter's value.
    //!
    //! Is called when the value of a color parameter has changed.
    //!
    void updateColorWidget ();

    //!
    //! Updates the widget showing an enumeration parameter's value.
    //!
    //! Is called when the value of an enumeration parameter has changed.
    //!
    void updateEnumerationWidget ();

    //!
    //! Updates the label showing the text info parameter's value.
    //!
    //! Is called when the value of a text info parameter has changed.
    //!
    void updateTextInfoWidget ();

    //!
    //! Updates the label showing info about the image parameter's image.
    //!
    //! Is called when the value of an image parameter has changed.
    //!
    void updateImageWidget ();


private: // functions

    //!
    //! Activates the custom context menu for the given label widget.
    //!
    //! \param label The label widget to activate a custom context menu for.
    //!
    void activateCustomContextMenu ( QLabel *label );

    //!
    //! Updates the given widget's palette so that it is painted with a Window
    //! color background instead of a Base color background.
    //!
    //! \param widget The widget to gray out.
    //!
    void grayOut ( QWidget *widget );

    //!
    //! Removes all widgets from the layout.
    //!
    void clear ();

    //!
    //! Adds widgets for editing the parameters of the given parameter group to
    //! the given form layout.
    //!
    //! \param parameterGroup The parameter group to create editing widgets for.
    //! \param formLayout The form layout to add editing widgets to.
    //!
    void addRows ( ParameterGroup *parameterGroup, QFormLayout *formLayout );

    //!
    //! Adds widgets for editing the given parameter to the given form layout.
    //!
    //! \param parameter The parameter to create editing widgets for.
    //! \param formLayout The form layout to add editing widgets to.
    //!
    void addRows ( Parameter *parameter, QFormLayout *formLayout );

    //!
    //! Creates a widget for editing a boolean parameter.
    //!
    //! \param parameter The boolean parameter to create an editing widget for.
    //! \return A widget for editing a boolean parameter.
    //!
    QWidget * createBooleanWidget ( Parameter *parameter );

    //!
    //! Creates a layout with widgets for editing editing an integer parameter.
    //!
    //! \param numberParameter The number parameter to create an editing widget for.
    //! \param index The index of the value to edit (for parameters that contain lists of values).
    //! \return A widget for editing an integer parameter.
    //!
    QLayout * createIntegerLayout ( NumberParameter *numberParameter, int index = 0 );

    //!
    //! Creates a layout with widgets for editing an unsigned integer
    //! parameter.
    //!
    //! \param numberParameter The number parameter to create an editing widget for.
    //! \param index The index of the value to edit (for parameters that contain lists of values).
    //! \return A widget for editing an unsigned integer parameter.
    //!
    QLayout * createUnsignedIntegerLayout ( NumberParameter *numberParameter, int index = 0 );

    //!
    //! Creates a layout with widgets for editing a floating point parameter.
    //!
    //! \param numberParameter The number parameter to create an editing widget for.
    //! \param index The index of the value to edit (for parameters that contain lists of values).
    //! \return A layout with widgets for editing a floating point parameter.
    //!
    QLayout * createFloatingPointLayout ( NumberParameter *numberParameter, int index = 0 );

    //!
    //! Creates a widget for editing a string parameter or displaying a read-only
    //! parameter's value.
    //!
    //! \param parameter The string parameter to create an editing widget for.
    //! \return A widget for editing a string parameter.
    //!
    QWidget * createStringWidget ( Parameter *parameter );

    //!
    //! Creates a layout with widgets for editing a filename parameter.
    //!
    //! \param filenameParameter The filename parameter to create an editing widget for.
    //! \return A layout with widgets for editing a filename parameter.
    //!
    QLayout * createFilenameLayout ( FilenameParameter *filenameParameter );

    //!
    //! Creates a widget for editing a color parameter.
    //!
    //! \param parameter The color parameter to create an editing widget for.
    //! \return A widget for editing a color parameter.
    //!
    QWidget * createColorWidget ( Parameter *parameter );

    //!
    //! Creates a widget for editing an enumeration parameter.
    //!
    //! \param enumerationParameter The parameter to create an editing widget for.
    //! \return A widget for editing an enumeration parameter.
    //!
    QWidget * createEnumerationWidget ( EnumerationParameter *enumerationParameter );

    //!
    //! Creates a label widget for displaying the text info contained in the
    //! given parameter.
    //!
    //! \param parameter The text info parameter to create a label widget for.
    //! \return A label widget for displaying the text info contained in the given parameter.
    //!
    QWidget * createTextInfoWidget ( Parameter *parameter );

    //!
    //! Creates a button widget for executing the command represented by the
    //! given parameter.
    //!
    //! \param parameter The command parameter to create a button widget for.
    //! \return A widget for execting the command represented by the given parameter.
    //!
    QWidget * createCommandWidget ( Parameter *parameter );

    //!
    //! Creates a widget displaying information about the image represented by
    //! the given parameter.
    //!
    //! \param parameter The image parameter to create a widget for.
    //! \return A widget displaying information about the image represented by the given parameter, or 0 if not applicable.
    //!
    QWidget * createImageWidget ( Parameter *parameter );

    //!
    //! Creates a layout with widgets for editing a parameter that stores
    //! several values of a specific type.
    //!
    //! \param parameter The vector parameter to create a layout with editing widgets for.
    //! \return A layout with widgets for editing a parameter that stores several values of a specific type.
    //!
    QLayout * createVectorLayout ( Parameter *parameter );

    //!
    //! Notifies connected objects that a parameter of the currently edited
    //! object should be changed.
    //!
    //! \param name The name of the parameter to change.
    //! \param value The new value for the parameter.
    //!
    void requestParameterChange ( const QString &name, const QVariant &value );

    //!
    //! Filters the parameters displayed in the panel by the search text
    //! entered in the search edit widget.
    //!
    void filterParameters ();

	//!
	//! Creates a plugin widget
	//!
	//! \param parameter the parameter of the widget
	//! \return The plugin widget
	//!
	QWidget * createPluginWidget ( Parameter *parameter);

private: // data

    //!
    //! The node for which parameters are displayed in the view.
    //!
    Node *m_node;

    //!
    //! The widget that is currently being edited which causes the
    //! requestParameterChange() function to be called.
    //!
    //! \see requestParameterChange()
    //!
    QObject *m_editedWidget;

    //!
    //! A map with parameter names as keys and pointers to editing widgets as
    //! values.
    //!
    QMultiMap<QString, QWidget *> m_widgetMap;

    //!
    //! The label displaying the type of the currently edited node.
    //!
    QLabel *m_nodeTypeLabel;

    //!
    //! The action by which the label displaying the type of the currently
    //! edited node in the tool bar can be accessed.
    //!
    QAction *m_nodeTypeLabelAction;

    //!
    //! The line edit for editing the name of the currently edited node.
    //!
    QLineEdit *m_nodeNameEdit;

    //!
    //! The action by which the line edit for editing the name of the currently
    //! edited node in the tool bar can be accessed.
    //!
    QAction *m_nodeNameEditAction;

    //!
    //! The line edit for editing the string for a parameter search.
    //!
    QLineEdit *m_searchEdit;

    //!
    //! The ID of the timer that is (re-)started each time the search text in
    //! the search edit widget is changed.
    //!
    int m_searchTimer;

};


#endif
