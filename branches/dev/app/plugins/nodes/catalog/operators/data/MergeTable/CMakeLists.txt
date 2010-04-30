project(MergeTable)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# MergeTable
set( MergeTable_h ${MergeTable_h}
			MergeTableNode.h
			MergeTableNodePlugin.h
			)

set( MergeTable_moc ${MergeTable_moc}	
			MergeTableNode.h
			MergeTableNodePlugin.h
			)

set( MergeTable_src ${MergeTable_src}	
			MergeTableNode.cpp
			MergeTableNodePlugin.cpp
			)

set( MergeTable_res ${MergeTable_res}	
			MergeTable.xml
			)

# Create moc files		   
qt4_wrap_cpp(MergeTable_cxx ${MergeTable_moc})

# Create source groups
source_group("Moc Files" FILES ${MergeTable_cxx})
source_group("Header Files" FILES ${MergeTable_h})
source_group("Resources" FILES ${MergeTable_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(MergeTable_src ${MergeTable_src} ${MergeTable_h} ${MergeTable_res})
ENDIF(WIN32)

# Create static library
add_library(MergeTable SHARED ${MergeTable_src} ${MergeTable_cxx})

# Add library dependencies
target_link_libraries(MergeTable
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			)

# Install files
install( FILES ${MergeTable_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS MergeTable RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
