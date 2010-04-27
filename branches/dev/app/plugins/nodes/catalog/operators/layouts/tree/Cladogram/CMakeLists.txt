project(CladogramLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CladogramLayouter
set( CladogramLayouter_h ${CladogramLayouter_h}
			CladogramLayouterNode.h
			CladogramLayouterNodePlugin.h
			)

set( CladogramLayouter_moc ${CladogramLayouter_moc}	
			CladogramLayouterNode.h
			CladogramLayouterNodePlugin.h
			)

set( CladogramLayouter_src ${CladogramLayouter_src}	
			CladogramLayouterNode.cpp
			CladogramLayouterNodePlugin.cpp
			)

set( CladogramLayouter_res ${CladogramLayouter_res}	
			CladogramLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(CladogramLayouter_cxx ${CladogramLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${CladogramLayouter_cxx})
source_group("Header Files" FILES ${CladogramLayouter_h})
source_group("Resources" FILES ${CladogramLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CladogramLayouter_src ${CladogramLayouter_src} ${CladogramLayouter_h} ${CladogramLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(CladogramLayouter SHARED ${CladogramLayouter_src} ${CladogramLayouter_cxx})

# Add library dependencies
target_link_libraries(CladogramLayouter
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
install( FILES ${CladogramLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CladogramLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
