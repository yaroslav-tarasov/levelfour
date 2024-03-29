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

#ifndef I_CONFIGURABLE_H
#define I_CONFIGURABLE_H

#include <GCF/IContainer>

class IConfigurable : virtual public IContainer
{
public:
    virtual QStringList     configurationOptions() = 0;
    virtual void            showConfigurationDialog(QString option) = 0;
    virtual bool            showDefaultConfigurationDialog() = 0;
};

Q_DECLARE_INTERFACE(IConfigurable, "com.vcreatelogic.IConfigurable/1.0")

#endif

