project(dataTable)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# dataTable
set( dataTable_h ${dataTable_h}
			dataTableNode.h
			dataTableNodePlugin.h
			)

set( dataTable_moc ${dataTable_moc}	
			dataTableNode.h
			dataTableNodePlugin.h
			)

set( dataTable_src ${dataTable_src}	
			dataTableNode.cpp
			dataTableNodePlugin.cpp
			)

set( dataTable_res ${dataTable_res}	
			dataTable.xml
			)

# Create moc files		   
qt4_wrap_cpp(dataTable_cxx ${dataTable_moc})

# Create source groups
source_group("Moc Files" FILES ${dataTable_cxx})
source_group("Header Files" FILES ${dataTable_h})
source_group("Resources" FILES ${dataTable_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(dataTable_src ${dataTable_src} ${dataTable_h} ${dataTable_res})
ENDIF(WIN32)

# Create static library
add_library(dataTable SHARED ${dataTable_src} ${dataTable_cxx})

# Add library dependencies
target_link_libraries(dataTable
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkInfovis debug vtkInfovis
			optimized vtkFiltering debug vtkFiltering
			)

# Install files
install( FILES ${dataTable_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS dataTable RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
