project(SOMLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SOMLayouter
set( SOMLayouter_h ${SOMLayouter_h}
			SOMLayouterNode.h
			SOMLayouterNodePlugin.h
			)

set( SOMLayouter_moc ${SOMLayouter_moc}	
			SOMLayouterNode.h
			SOMLayouterNodePlugin.h
			)

set( SOMLayouter_src ${SOMLayouter_src}	
			SOMLayouterNode.cpp
			SOMLayouterNodePlugin.cpp
			)

set( SOMLayouter_res ${SOMLayouter_res}	
			SOMLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SOMLayouter_cxx ${SOMLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SOMLayouter_cxx})
source_group("Header Files" FILES ${SOMLayouter_h})
source_group("Resources" FILES ${SOMLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SOMLayouter_src ${SOMLayouter_src} ${SOMLayouter_h} ${SOMLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SOMLayouter SHARED ${SOMLayouter_src} ${SOMLayouter_cxx})

# Add library dependencies
target_link_libraries(SOMLayouter
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
install( FILES ${SOMLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SOMLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
