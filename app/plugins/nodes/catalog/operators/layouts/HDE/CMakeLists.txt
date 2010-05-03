project(HdeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# HdeLayouter
set( HdeLayouter_h ${HdeLayouter_h}
			HdeLayouterNode.h
			HdeLayouterNodePlugin.h
			)

set( HdeLayouter_moc ${HdeLayouter_moc}	
			HdeLayouterNode.h
			HdeLayouterNodePlugin.h
			)

set( HdeLayouter_src ${HdeLayouter_src}	
			HdeLayouterNode.cpp
			HdeLayouterNodePlugin.cpp
			)

set( HdeLayouter_res ${HdeLayouter_res}	
			HdeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(HdeLayouter_cxx ${HdeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${HdeLayouter_cxx})
source_group("Header Files" FILES ${HdeLayouter_h})
source_group("Resources" FILES ${HdeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(HdeLayouter_src ${HdeLayouter_src} ${HdeLayouter_h} ${HdeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(HdeLayouter SHARED ${HdeLayouter_src} ${HdeLayouter_cxx})

# Add library dependencies
target_link_libraries(HdeLayouter
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
install( FILES ${HdeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS HdeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
