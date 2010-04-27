/*
-----------------------------------------------------------------------------
file "VTKAreaLayoutNode.cpp"
brief Implementation file for VTKAreaLayoutNode class.

version    1.0
date       21.04.2010 (last updated)

Description
-----------
This is the base class for all layouts of area structure
such as treemap, treering, icicle, etc. 

Inputs
----------
	In: vtkTree

Outputs
-------
	Out: vtkTable

Result
------

A vtkTable is produced with 4 columns: nodeId, posX, posY, posZ.

Notes
-----
vtkAreaLayout is the core of the class. Nodes 
extending this class are based on vtkAreaLayoutStrategy.

-----------------------------------------------------------------------------
*/

#include "vtkTree.h"