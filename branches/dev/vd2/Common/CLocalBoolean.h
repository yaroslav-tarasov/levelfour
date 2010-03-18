/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
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

#ifndef C_LOCAL_BOOLEAN_H
#define C_LOCAL_BOOLEAN_H

#include <Qt>

class CLocalBoolean
{
public:
	CLocalBoolean(bool& val) : m_val(&val), m_oldValue(val) { }
	~CLocalBoolean() { *m_val = m_oldValue; }
	CLocalBoolean& operator = (bool val) { *m_val = val; return* this; }

private:
	bool* m_val;
	bool m_oldValue;
};

#endif
