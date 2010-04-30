project(DataTable)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# DataTable
set( DataTable_h ${DataTable_h}
			DataTableNode.h
			DataTableNodePlugin.h
			)

set( DataTable_moc ${DataTable_moc}	
			DataTableNode.h
			DataTableNodePlugin.h
			)

set( DataTable_src ${DataTable_src}	
			DataTableNode.cpp
			DataTableNodePlugin.cpp
			)

set( DataTable_res ${DataTable_res}	
			DataTable.xml
			)

# Create moc files		   
qt4_wrap_cpp(DataTable_cxx ${DataTable_moc})

# Create source groups
source_group("Moc Files" FILES ${DataTable_cxx})
source_group("Header Files" FILES ${DataTable_h})
source_group("Resources" FILES ${DataTable_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(DataTable_src ${DataTable_src} ${DataTable_h} ${DataTable_res})
ENDIF(WIN32)

# Create static library
add_library(DataTable SHARED ${DataTable_src} ${DataTable_cxx})

# Add library dependencies
target_link_libraries(DataTable
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkInfovis debug vtkInfovis
			optimized vtkFiltering debug vtkFiltering
			)

# Install files
install( FILES ${DataTable_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS DataTable RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
