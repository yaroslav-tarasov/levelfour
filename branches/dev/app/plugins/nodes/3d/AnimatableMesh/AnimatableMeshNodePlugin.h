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
//! \file "AnimatableMeshNodePlugin.h"
//! \brief Header file for AnimatableMeshNodePlugin class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       16.03.2009 (last updated)
//!

#ifndef ANIMATABLEMESHNODEPLUGIN_H
#define ANIMATABLEMESHNODEPLUGIN_H

#include "AnimatableMeshNode.h"
#include "NodeTypeInterface.h"
#include "Node.h"

//!
//! Plugin class for creation of AnimatableMeshNode node.
//!
class AnimatableMeshNodePlugin : public QObject, public NodeTypeInterface
{
    Q_OBJECT
    Q_INTERFACES(NodeTypeInterface)

public: /// methods

    virtual Node * createNode ( const QString &name, ParameterGroup *parameterRoot );


};


#endif
