project(DigCoLaLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# DigCoLaLayouter
set( DigCoLaLayouter_h ${DigCoLaLayouter_h}
			DigCoLaLayouterNode.h
			DigCoLaLayouterNodePlugin.h
			)

set( DigCoLaLayouter_moc ${DigCoLaLayouter_moc}	
			DigCoLaLayouterNode.h
			DigCoLaLayouterNodePlugin.h
			)

set( DigCoLaLayouter_src ${DigCoLaLayouter_src}	
			DigCoLaLayouterNode.cpp
			DigCoLaLayouterNodePlugin.cpp
			)

set( DigCoLaLayouter_res ${DigCoLaLayouter_res}	
			DigCoLaLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(DigCoLaLayouter_cxx ${DigCoLaLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${DigCoLaLayouter_cxx})
source_group("Header Files" FILES ${DigCoLaLayouter_h})
source_group("Resources" FILES ${DigCoLaLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(DigCoLaLayouter_src ${DigCoLaLayouter_src} ${DigCoLaLayouter_h} ${DigCoLaLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(DigCoLaLayouter SHARED ${DigCoLaLayouter_src} ${DigCoLaLayouter_cxx})

# Add library dependencies
target_link_libraries(DigCoLaLayouter
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
install( FILES ${DigCoLaLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS DigCoLaLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
