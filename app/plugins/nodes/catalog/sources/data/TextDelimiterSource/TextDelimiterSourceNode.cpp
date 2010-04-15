#include "TextDelimiterSourceNode.h"
#include "vtkTable.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "SceneNodeParameter.h"
#include "FilenameParameter.h"
#include <QtCore/QFile>
#include <QtCore/QDir>

INIT_INSTANCE_COUNTER(TextDelimiterSourceNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the TextDelimiterSourceNode class.
//!
//! \param name The name to give the new mesh node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
TextDelimiterSourceNode::TextDelimiterSourceNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKTableNode (name, parameterRoot),
		m_dtxt(0)
{
    // set affections and functions
    setChangeFunction("Source File", SLOT(sourceFileChanged()));
    setCommandFunction("Source File", SLOT(sourceFileChanged()));

	setChangeFunction("Field Delimiter Characters", SLOT(fieldDelimiterChanged()));
    setCommandFunction("Field Delimiter Characters", SLOT(fieldDelimiterChanged()));

	setChangeFunction("String Delimiter", SLOT(stringDelimiterChanged()));
    setCommandFunction("String Delimiter", SLOT(stringDelimiterChanged()));

	setChangeFunction("Use String Delimiter", SLOT(useStringDelimiterChanged()));
    setCommandFunction("Use String Delimiter", SLOT(useStringDelimiterChanged()));

	setChangeFunction("Have Header", SLOT(haveHeaderChanged()));
    setCommandFunction("Have Header", SLOT(haveHeaderChanged()));

	setChangeFunction("Merge Consecutive Delimiters", SLOT(mergeConsecutiveDelimitersChanged()));
    setCommandFunction("Merge Consecutive Delimiters", SLOT(mergeConsecutiveDelimitersChanged()));

	setChangeFunction("MaxRecords", SLOT(maxRecordsChanged()));
    setCommandFunction("MaxRecords", SLOT(maxRecordsChanged()));

	setChangeFunction("Detect Numeric Columns", SLOT(detectNumericColumnsChanged()));
    setCommandFunction("Detect Numeric Columns", SLOT(detectNumericColumnsChanged()));

	connect(this, SIGNAL(frameChanged(int)), SLOT(updateAll()));

	setTypeName("TextDelimiterSourceNode");

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the TextDelimiterSourceNode class.
//!
TextDelimiterSourceNode::~TextDelimiterSourceNode ()
{
	emit destroyed();
	m_dtxt->Delete();
    DEC_INSTANCE_COUNTER
    Log::info(QString("TextDelimiterSourceNode destroyed."), "TextDelimiterSourceNode::~TextDelimiterSourceNode");
}


///
/// Public Slots
///

///
/// Private Functions
///

//!
//!
//! \return True, if successful loading of ogre mesh
//!            False, otherwise.
//!
bool TextDelimiterSourceNode::loadFile()
{
    QString filename = getStringValue("Source File");
    if (filename == "") {
        Log::debug(QString("Source file has not been set yet. (\"%1\")").arg(m_name), "TextDelimiterSourceNode::loadFile");
        return false;
    }

    // check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Source file \"%1\" not found.").arg(filename), "TextDelimiterSourceNode::loadFile");
        return false;
    }

    // split the absolute filename to path and base filename
    int lastSlashIndex = filename.lastIndexOf('/');
    QString path = "";
    if (lastSlashIndex > -1) {
        path = filename.mid(0, lastSlashIndex);
        filename = filename.mid(lastSlashIndex + 1);
    }
	if (!filename.endsWith(".csv") && !filename.endsWith(".txt")) {
        Log::error("The source file has to be a csv or txt.", "TextDelimiterSourceNode::loadFile");
        return false;
    }

	// if any, delete previous file
	if (m_dtxt)
		m_dtxt->Delete();

	// create and load new file
	m_dtxt = vtkDelimitedTextReader::New();
	m_dtxt->SetFileName(filename.toLatin1().constData());
	m_dtxt->Update();
	updateTable();

    Log::info(QString("Source file \"%1\" loaded.").arg(filename), "TextDelimiterSourceNode::loadFile");
	Log::info(QString("Number rows \"%1\" loaded: ").arg(m_table->GetNumberOfRows()), "TextDelimiterSourceNode::loadFile");
    return true;
}

///
/// Private Slots
///

//!
//! Change function for the Source File parameter.
//!
void TextDelimiterSourceNode::sourceFileChanged ()
{
    // load new source file
    loadFile();
}

//!
//! Change function for the fieldDelimiter parameter.
//!
void TextDelimiterSourceNode::fieldDelimiterChanged ()
{
    QString fieldDelimiter = getStringValue("Field Delimiter Characters");
	if (!m_dtxt)
		return;

	m_dtxt->SetFieldDelimiterCharacters(fieldDelimiter.toLatin1().constData());
	updateTable();
}

//!
//! Change function for the String Delimiter parameter.
//!
void TextDelimiterSourceNode::stringDelimiterChanged ()
{
    QString stringDelimiter = getStringValue("String Delimiter");
	if (!m_dtxt)
		return;

	if (stringDelimiter.size() <= 0)
		m_dtxt->SetStringDelimiter(char(0));
	else 
	{
		QChar qchar = stringDelimiter.at(0);
		m_dtxt->SetStringDelimiter(qchar.toLatin1());
	}
	updateTable();
}

//!
//! Change function for the Use String Delimiter parameter.
//!
void TextDelimiterSourceNode::useStringDelimiterChanged ()
{
	bool useStringDelimiter = getBoolValue("Use String Delimiter");
	if (!m_dtxt)
		return;

	m_dtxt->SetUseStringDelimiter(useStringDelimiter);
	updateTable();
}

//!
//! Change function for the Have Header parameter.
//!
void TextDelimiterSourceNode::haveHeaderChanged ()
{
	bool haveHeader = getBoolValue("Have Header");
	if (!m_dtxt)
		return;

	m_dtxt->SetHaveHeaders(haveHeader);
	updateTable();
}

//!
//! Change function for the Merge Consecutive Delimiters parameter.
//!
void TextDelimiterSourceNode::mergeConsecutiveDelimitersChanged ()
{
	bool mergeConsecutiveDelimiters = getBoolValue("Merge Consecutive Delimiters");
	if (!m_dtxt)
		return;

	m_dtxt->SetMergeConsecutiveDelimiters(mergeConsecutiveDelimiters);
	updateTable();
}

//!
//! Change function for the MaxRecords parameter.
//!
void TextDelimiterSourceNode::maxRecordsChanged ()
{
	int maxRecords = getIntValue("MaxRecords");
	if (!m_dtxt)
		return;

	m_dtxt->SetMaxRecords(maxRecords);
	updateTable();
}

//!
//! Change function for the Detect Numeric Columns parameter.
//!
void TextDelimiterSourceNode::detectNumericColumnsChanged ()
{
	bool detectNumericColumns = getBoolValue("Detect Numeric Columns");
	if (!m_dtxt)
		return;

	m_dtxt->SetDetectNumericColumns(detectNumericColumns);
	updateTable();
}

void TextDelimiterSourceNode::updateTable()
{
	m_table = m_dtxt->GetOutput();
	emit tableChanged(m_table);
}