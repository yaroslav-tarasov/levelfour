project(mergeTable)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# mergeTable
set( mergeTable_h ${mergeTable_h}
			mergeTableNode.h
			mergeTableNodePlugin.h
			)

set( mergeTable_moc ${mergeTable_moc}	
			mergeTableNode.h
			mergeTableNodePlugin.h
			)

set( mergeTable_src ${mergeTable_src}	
			mergeTableNode.cpp
			mergeTableNodePlugin.cpp
			)

set( mergeTable_res ${mergeTable_res}	
			mergeTable.xml
			)

# Create moc files		   
qt4_wrap_cpp(mergeTable_cxx ${mergeTable_moc})

# Create source groups
source_group("Moc Files" FILES ${mergeTable_cxx})
source_group("Header Files" FILES ${mergeTable_h})
source_group("Resources" FILES ${mergeTable_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(mergeTable_src ${mergeTable_src} ${mergeTable_h} ${mergeTable_res})
ENDIF(WIN32)

# Create static library
add_library(mergeTable SHARED ${mergeTable_src} ${mergeTable_cxx})

# Add library dependencies
target_link_libraries(mergeTable
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			)

# Install files
install( FILES ${mergeTable_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS mergeTable RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
