project(ConstrainedLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ConstrainedLayouter
set( ConstrainedLayouter_h ${ConstrainedLayouter_h}
			ConstrainedLayouterNode.h
			ConstrainedLayouterNodePlugin.h
			)

set( ConstrainedLayouter_moc ${ConstrainedLayouter_moc}	
			ConstrainedLayouterNode.h
			ConstrainedLayouterNodePlugin.h
			)

set( ConstrainedLayouter_src ${ConstrainedLayouter_src}	
			ConstrainedLayouterNode.cpp
			ConstrainedLayouterNodePlugin.cpp
			)

set( ConstrainedLayouter_res ${ConstrainedLayouter_res}	
			ConstrainedLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(ConstrainedLayouter_cxx ${ConstrainedLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${ConstrainedLayouter_cxx})
source_group("Header Files" FILES ${ConstrainedLayouter_h})
source_group("Resources" FILES ${ConstrainedLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ConstrainedLayouter_src ${ConstrainedLayouter_src} ${ConstrainedLayouter_h} ${ConstrainedLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(ConstrainedLayouter SHARED ${ConstrainedLayouter_src} ${ConstrainedLayouter_cxx})

# Add library dependencies
target_link_libraries(ConstrainedLayouter
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
install( FILES ${ConstrainedLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ConstrainedLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
