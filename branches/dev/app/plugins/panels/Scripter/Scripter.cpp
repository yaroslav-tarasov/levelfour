/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

//!
//! \file "Scripter.cpp"
//! \brief Implementation file for Scripter class.
//!
//! \author     Sebastian Bach <sebastian.bach@filmakademie.de>
//! \version    1.0
//! \date       07.12.2009 (last updated)
//!

#include "Scripter.h"
#include "ParameterTabPage.h"
#include "DoubleSlider.h"
#include "NodeFactory.h"
#include "Log.h"
/*
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
*/


#include <QPushButton>
#include <QMessageBox>



Q_DECLARE_METATYPE(Ogre::Vector3)


///
/// Constructors and Destructors
///

//!
//! Constructor of the Scripter class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
Scripter::Scripter ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    ViewPanel(ViewPanel::T_PluginPanel, parent, flags)
{
	QPushButton *buttonSimpleScene = new QPushButton("&Execute Script", this);
	buttonSimpleScene->setGeometry(20, 250, 100, 25);

	QObject::connect(buttonSimpleScene, SIGNAL(clicked()), this, SLOT(exeScript()));


	m_NodeTypes = NodeFactory::getTypeNames();

	m_Script = new QPlainTextEdit(this);
	m_Script->setGeometry(20,20,400,200);

	m_Script->setPlainText(QString("// Welcome...."));

	m_Console = new QPlainTextEdit(this);
	m_Console->setGeometry(20,300,400,200);
	m_Console->setPlainText(QString("// Console"));	

}


//!
//! Destructor of the NetworkPanel class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
Scripter::~Scripter ()
{

}

///
/// Public Funcitons
///


//!
//! Connects the panel with the scene.
//!
//! \param *nodeModel NodeModel of the scene
//! \param *sceneModel SceneModel of the scene
//!
void Scripter::registerControl(NodeModel *nodeModel, SceneModel *sceneModel)
{
	m_nodeModel = nodeModel;
	m_sceneModel = sceneModel;
	
	/*
	QObject::connect(m_nodeModel, SIGNAL(nodeSelected(Node *)), this, SLOT(updateSelectedNode(Node *)));
	QObject::connect(m_sceneModel, SIGNAL(modified()), this, SLOT(update()));
	QObject::connect(m_sceneModel, SIGNAL(objectCreated(const QString &)), this, SLOT(update()));
	QObject::connect(m_sceneModel, SIGNAL(selectionChanged(bool)), this, SLOT(update()));

	*/

}


//!
//! Fills the given tool bars with actions for the Scripter view.
//!
//! \param mainToolBar The main tool bar to fill with actions.
//! \param panelToolBar The panel tool bar to fill with actions.
//!
void Scripter::fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar )
{
	QAction *ui_descriptionAction;
	ui_descriptionAction = new QAction(this);
    ui_descriptionAction->setObjectName(QString::fromUtf8("ui_descriptionAction"));
    ui_descriptionAction->setCheckable(true);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/infoIcon"), QSize(), QIcon::Normal, QIcon::Off);
    ui_descriptionAction->setIcon(icon1);
	ui_descriptionAction->setToolTip("Show Description");


	 QMessageBox *msgBox = new QMessageBox(this);
	 msgBox->setText("Create Presets Panel");
     msgBox->setInformativeText("By Sebastian Bach 2009");
	 msgBox->setStandardButtons(QMessageBox::Ok);
	 msgBox->setEscapeButton(QMessageBox::Ok);

	connect(ui_descriptionAction, SIGNAL(toggled(bool)), msgBox, SLOT(exec()));

	mainToolBar->addAction(ui_descriptionAction);
}


//!
//! Displays an error message
//!
void Scripter::displayError(QString missing)
{

	 QMessageBox *errorBox = new QMessageBox(this);
	 errorBox->setText("Create Presets Panel");
	 errorBox->setIcon(QMessageBox::Critical);
	
	QString errorMessage;
	errorMessage.append("Could not create scene. Missing Node \"");
	errorMessage.append(missing);
	errorMessage.append("\".");


     errorBox->setInformativeText(errorMessage);
	 errorBox->setStandardButtons(QMessageBox::Ok);
	 errorBox->setEscapeButton(QMessageBox::Ok);

	 errorBox->exec();

	 Log::warning(errorMessage, QString("Scripter::displayError"));

}
///
/// Private Slots
///

void Scripter::exeScript()
{

	// Preprearing
	QStringList nodes = m_nodeModel->getNodeNames();

	QMap<QString, QVariant> _variables; // variablename, value

	_variables[QString("_SYSTEM")] = QVariant("Frapper");
	_variables[QString("_AUTHOR")] = QVariant("Sebastian Bach");

	// executin of the script

	QString script = m_Script->toPlainText();

	QStringList _actions = script.split("\n");

	int _lines = _actions.length();

	for(int i = 0; i < _lines; ++i){
		std::cout<<_actions.at(i).toStdString()<<std::endl;
	}

	int counter = 0;
	bool execute = true;

	while(execute){

		QString _line = _actions.at(counter);	

		if(_line.contains(QString("//"))){ // Commentary ??
			QString commentary = QString("Commentary : ");
			_line.remove(QString("//"));
			commentary.append(_line);
			m_Console->appendPlainText(commentary);
		}else{ // Orders
			if(_line.contains("(") && _line.contains(")")){ // Order

				_line.remove(QString(")"));
				QStringList _action = _line.split(QString("("));
				
				if(_action.length() == 2){
					QString _order = _action.at(0);
					QString _arguments = _action.at(1);

					bool knowOrder = false;

					// PRINTS DATA

					if(_order == QString("print")){

						if(_variables.keys().contains(_arguments)){
							QString _variableText = _arguments;
							_variableText.append(" = ");
							_variableText.append(_variables[_arguments].toString());

							m_Console->appendPlainText(_variableText);
						}else{

							if(_arguments == QString("MEMORY")){
								for(int i = 0; i < _variables.keys().length(); ++i)
								{
									QString _variableText = _variables.keys().at(i);
									_variableText.append(" = ");
									_variableText.append(_variables[_variables.keys().at(i)].toString());

									m_Console->appendPlainText(_variableText);
								}
							}else{
								m_Console->appendPlainText(_arguments);
							}
						}
						knowOrder = true;
					} 
					
					// DECLARE VARIABLE


					if(_order == QString("declare")){
						_variables[_arguments] = QVariant();
						knowOrder = true;
					}

					// CREATES A NODE

					if(_order == QString("makeNode")){
							
						QStringList _nodeAttributes = _arguments.split(",");

						if(_nodeAttributes.length() == 2){
							if(m_NodeTypes.contains(_nodeAttributes.at(0))){
								m_sceneModel->createObject(_nodeAttributes.at(0), _nodeAttributes.at(1),QPoint(0,0) );
							}else{
								QString errorMsg = QString("Unknown Node-Type for 'makeNode' at line ");
								errorMsg.append(QString::number(counter));
								m_Console->appendPlainText(errorMsg);
							}
							
						}else{
							
						}

						
						knowOrder = true;
					}

					// SET A VALUE

					if(_order == QString("set")){

						QStringList _settingAttributes = _arguments.split(",");

						if(_settingAttributes.length() == 2){
							if(_settingAttributes.at(0).contains(".")){ // parameter->var
								QStringList nodeAttributes = _settingAttributes.at(0).split(".");

								QVariant value;

								if(nodes.contains(nodeAttributes.at(0))){
									Node *node = m_nodeModel->getNode(nodeAttributes.at(0));
									if(node->hasParameter(nodeAttributes.at(1))){
										value = node->getParameter(nodeAttributes.at(1))->getValue();
									}
								}

								_variables.insert(_settingAttributes.at(1),value);
								//_variables[QString("tadada")] = QVariant("asfjsfk");

							}
							if(_settingAttributes.at(1).contains(".")){ // var->parameter
								QStringList nodeAttributes = _settingAttributes.at(1).split(".");

								if(nodeAttributes.length() == 2){

									const QString variableName = _settingAttributes.at(0);
									const QString nodeName = nodeAttributes.at(0);
									const QString parameterName = nodeAttributes.at(1);
									
									if(_variables.keys().contains(variableName)){
										QVariant value = _variables[variableName];

										m_nodeModel->changeParameter(nodeName, parameterName, value);

									}else{
										QString errorMsg = QString("Variable not found for 'set' at line ");
										errorMsg.append(QString::number(counter));
										m_Console->appendPlainText(errorMsg);
									}
								}
							}

						}else{
							QString errorMsg = QString("Unsufficent attributes for 'set' at line ");
							errorMsg.append(QString::number(counter));
							m_Console->appendPlainText(errorMsg);
						}

						_variables[_arguments] = QVariant();
						knowOrder = true;
					}







					if(!knowOrder){
						QString errorMsg = QString("Unknown command at line ");
						errorMsg.append(QString::number(counter));
						m_Console->appendPlainText(errorMsg);
						execute = false;
					}

				}else{
					QString errorMsg = QString("Invalid formation at line ");
					errorMsg.append(QString::number(counter));
					m_Console->appendPlainText(errorMsg);
					execute = false;
				}
			}else{
				QString errorMsg = QString("Syntax-error at line ");
				errorMsg.append(QString::number(counter));
				m_Console->appendPlainText(errorMsg);
				execute = false;
			}
		}

		/*QString line = ....;
		QString nodename = ....;

		if(nodes.contains("nodename")){
			Node *node = m_nodeModel->getNode("nodename");
			if(node->hasParameter("parametername")){
				QVariant value = node->getParameter("parametername")->getValue();
			}
			

		}

		*/

		counter++;

		if(counter > 1000)
			execute = false;

		if(counter > _lines - 1)
			execute = false;
		
	}

	

	m_nodeModel->beginUpdate();

	m_nodeModel->endUpdate();

}

//!
//! Creates a connection
//!
void Scripter::createConnection(Node *source, Node *target, QString sourceParameter, QString targetParameter)
{
	Parameter *out = source->getParameter(sourceParameter);
	Parameter *in = target->getParameter(targetParameter);
	if(out != NULL && in != NULL){
		m_sceneModel->connectParameters(source, sourceParameter, target, targetParameter);
		source->notifyChange();
		target->notifyChange();
	}else{
		Log::error(QString("Could not create connection!"), "Scripter::createConnection");
	}
}



