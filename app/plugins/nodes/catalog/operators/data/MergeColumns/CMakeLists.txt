project(MergeColumns)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# MergeColumns
set( MergeColumns_h ${MergeColumns_h}
			MergeColumnsNode.h
			MergeColumnsNodePlugin.h
			)

set( MergeColumns_moc ${MergeColumns_moc}	
			MergeColumnsNode.h
			MergeColumnsNodePlugin.h
			)

set( MergeColumns_src ${MergeColumns_src}	
			MergeColumnsNode.cpp
			MergeColumnsNodePlugin.cpp
			)

set( MergeColumns_res ${MergeColumns_res}	
			MergeColumns.xml
			)

# Create moc files		   
qt4_wrap_cpp(MergeColumns_cxx ${MergeColumns_moc})

# Create source groups
source_group("Moc Files" FILES ${MergeColumns_cxx})
source_group("Header Files" FILES ${MergeColumns_h})
source_group("Resources" FILES ${MergeColumns_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(MergeColumns_src ${MergeColumns_src} ${MergeColumns_h} ${MergeColumns_res})
ENDIF(WIN32)

# Create static library
add_library(MergeColumns SHARED ${MergeColumns_src} ${MergeColumns_cxx})

# Add library dependencies
target_link_libraries(MergeColumns
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			optimized vtkCommon debug vtkCommon
			)

# Install files
install( FILES ${MergeColumns_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS MergeColumns RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
